plugins {
    id 'com.android.application'
    id 'org.jetbrains.kotlin.android'
    id 'kotlin-kapt'
}

android {
    namespace 'com.example.bi_offline'
    compileSdk 34

    defaultConfig {
        applicationId "com.example.bi_offline"
        minSdk 21
        targetSdk 34
        versionCode 1
        versionName "1.0"
    }
    buildFeatures {
        viewBinding true
    }
    compileOptions {
        sourceCompatibility JavaVersion.VERSION_17
        targetCompatibility JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = '17'
    }
}

dependencies {
    implementation "org.jetbrains.kotlin:kotlin-stdlib:1.9.0"
    implementation 'androidx.core:core-ktx:1.11.0'
    implementation 'androidx.appcompat:appcompat:1.6.1'
    implementation 'com.google.android.material:material:1.9.0'
    implementation 'androidx.constraintlayout:constraintlayout:2.1.4'
    implementation "androidx.lifecycle:lifecycle-runtime-ktx:2.6.1"
    implementation "androidx.room:room-runtime:2.5.2"
    kapt "androidx.room:room-compiler:2.5.2"
    implementation "androidx.room:room-ktx:2.5.2"
    implementation "org.jetbrains.kotlinx:kotlinx-coroutines-android:1.7.1"
    implementation 'androidx.recyclerview:recyclerview:1.3.0'
}
<manifest package="com.example.bi_offline"
    xmlns:android="http://schemas.android.com/apk/res/android">

    <uses-permission android:name="android.permission.SEND_SMS"/>
    <uses-permission android:name="android.permission.RECEIVE_SMS"/>
    <uses-permission android:name="android.permission.READ_SMS"/>
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.INTERNET"/>
    <uses-permission android:name="android.permission.FOREGROUND_SERVICE"/>

    <application
        android:allowBackup="true"
        android:label="BI"
        android:icon="@mipmap/ic_launcher"
        android:roundIcon="@mipmap/ic_launcher_round">

        <activity android:name=".ui.ChatActivity"/>
        <activity android:name=".ui.RoomListActivity"/>
        <activity android:name=".ui.OtpActivity"/>
        <activity android:name=".ui.LoginActivity">
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>
                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
        </activity>

        <receiver android:name=".sms.SmsReceiver" android:exported="true">
            <intent-filter android:priority="999">
                <action android:name="android.provider.Telephony.SMS_RECEIVED"/>
            </intent-filter>
        </receiver>

    </application>
</manifest>
package com.example.bi_offline.data

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "messages")
data class Message(
    @PrimaryKey(autoGenerate = true) val id: Int = 0,
    val roomId: String,
    val sender: String,
    val content: String?,     // text content (or "[PHOTO]<path>")
    val photoPath: String?,   // local path saved on receiver device (optional)
    val timestamp: Long,
    val replyTo: Int?         // message id replied to
)
package com.example.bi_offline.data

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import kotlinx.coroutines.flow.Flow

@Dao
interface MessageDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(msg: Message): Long

    @Query("SELECT * FROM messages WHERE roomId = :room ORDER BY timestamp ASC")
    fun getChat(room: String): Flow<List<Message>>

    @Query("SELECT DISTINCT roomId FROM messages ORDER BY MAX(timestamp) DESC")
    fun getRooms(): Flow<List<String>>

    @Query("SELECT * FROM messages WHERE id = :id LIMIT 1")
    suspend fun getById(id: Int): Message?
}
package com.example.bi_offline.data

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase

@Database(entities = [Message::class], version = 1, exportSchema = false)
abstract class AppDatabase : RoomDatabase() {
    abstract fun messageDao(): MessageDao

    companion object {
        @Volatile private var INSTANCE: AppDatabase? = null
        fun getInstance(context: Context): AppDatabase =
            INSTANCE ?: synchronized(this) {
                INSTANCE ?: Room.databaseBuilder(
                    context.applicationContext,
                    AppDatabase::class.java, "bi_db"
                ).fallbackToDestructiveMigration().build().also { INSTANCE = it }
            }
    }
}
package com.example.bi_offline.sms

import android.telephony.SmsManager

object SmsSender {
    /**
     * Sends a text message that includes a room prefix.
     * Example body: [ROOMID] Hello everyone
     */
    fun sendSms(roomId: String, toNumber: String, message: String) {
        val full = "[$roomId] $message"
        val sms = SmsManager.getDefault()
        val parts = sms.divideMessage(full)
        sms.sendMultipartTextMessage(toNumber, null, parts, null, null)
    }

    /**
     * Fallback for sending photo: we send a small tag + remote URL/path via SMS.
     * True MMS sending is carrier-dependent and often requires custom APIs.
     */
    fun sendPhotoTag(roomId: String, toNumber: String, remotePathTag: String) {
        val full = "[$roomId] [PHOTO] $remotePathTag"
        val sms = SmsManager.getDefault()
        val parts = sms.divideMessage(full)
        sms.sendMultipartTextMessage(toNumber, null, parts, null, null)
    }
}
package com.example.bi_offline.sms

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.telephony.SmsMessage
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import com.example.bi_offline.data.AppDatabase
import com.example.bi_offline.data.Message

class SmsReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        val bundle: Bundle? = intent.extras ?: return
        val pdus = bundle["pdus"] as? Array<*> ?: return

        for (pdu in pdus) {
            val format = bundle.getString("format")
            val sms = SmsMessage.createFromPdu(pdu as ByteArray, format)
            val from = sms.displayOriginatingAddress ?: "unknown"
            val body = sms.messageBody ?: ""
            // parse room id
            val regex = "\\[(.*?)\\]".toRegex()
            val roomId = regex.find(body)?.groupValues?.get(1) ?: "DEFAULT"
            // remove the first tag
            val content = body.replaceFirst("\\[$roomId\\]".toRegex(), "").trim()
            val isPhoto = content.startsWith("[PHOTO]")

            CoroutineScope(Dispatchers.IO).launch {
                val db = AppDatabase.getInstance(context)
                db.messageDao().insert(
                    Message(
                        roomId = roomId,
                        sender = from,
                        content = if (isPhoto) null else content,
                        photoPath = if (isPhoto) content.removePrefix("[PHOTO]").trim() else null,
                        timestamp = System.currentTimeMillis(),
                        replyTo = null
                    )
                )
            }
        }
    }
}
package com.example.bi_offline.ui

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.bi_offline.databinding.ActivityLoginBinding

class LoginActivity : AppCompatActivity() {
    private lateinit var binding: ActivityLoginBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnContinue.setOnClickListener {
            val mobile = binding.etMobile.text.toString().trim()
            if (mobile.isNotEmpty()) {
                val i = Intent(this, OtpActivity::class.java)
                i.putExtra("mobile", mobile)
                startActivity(i)
            }
        }
    }
}
package com.example.bi_offline.ui

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.bi_offline.databinding.ActivityOtpBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class OtpActivity : AppCompatActivity() {
    private lateinit var binding: ActivityOtpBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityOtpBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val mobile = intent.getStringExtra("mobile") ?: ""

        binding.btnVerify.setOnClickListener {
            val otp = binding.etOtp.text.toString().trim()
            if (otp.isNotEmpty()) {
                // simulate success
                CoroutineScope(Dispatchers.IO).launch {
                    getSharedPreferences("bi_prefs", MODE_PRIVATE)
                        .edit().putString("my_number", mobile).apply()
                }
                startActivity(Intent(this, RoomListActivity::class.java))
                finish()
            }
        }
    }
} 
package com.example.bi_offline.ui

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.bi_offline.data.AppDatabase
import com.example.bi_offline.databinding.ActivityRoomListBinding
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch

class RoomListActivity : AppCompatActivity() {
    private lateinit var binding: ActivityRoomListBinding
    private lateinit var adapter: RoomAdapter

    private val requestPermissions =
        registerForActivityResult(ActivityResultContracts.RequestMultiplePermissions()) { perms ->
            // ignore – we try to request at runtime
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityRoomListBinding.inflate(layoutInflater)
        setContentView(binding.root)

        adapter = RoomAdapter { roomId ->
            val i = Intent(this, ChatActivity::class.java)
            i.putExtra("room_id", roomId)
            startActivity(i)
        }
        binding.recycler.layoutManager = LinearLayoutManager(this)
        binding.recycler.adapter = adapter

        binding.btnCreateRoom.setOnClickListener {
            val id = binding.etRoomId.text.toString().trim()
            if (id.isNotEmpty()) {
                val i = Intent(this, ChatActivity::class.java)
                i.putExtra("room_id", id)
                startActivity(i)
            }
        }

        // request SMS permissions if not granted
        val needed = mutableListOf<String>()
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.SEND_SMS) != PackageManager.PERMISSION_GRANTED)
            needed.add(Manifest.permission.SEND_SMS)
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.RECEIVE_SMS) != PackageManager.PERMISSION_GRANTED)
            needed.add(Manifest.permission.RECEIVE_SMS)
        if (needed.isNotEmpty()) requestPermissions.launch(needed.toTypedArray())

        val db = AppDatabase.getInstance(this)
        lifecycleScope.launch {
            db.messageDao().getRooms().collectLatest { list ->
                adapter.submit(list)
            }
        }
    }
}
package com.example.bi_offline.ui

import android.Manifest
import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Bundle
import android.provider.OpenableColumns
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.bi_offline.data.AppDatabase
import com.example.bi_offline.data.Message
import com.example.bi_offline.databinding.ActivityChatBinding
import com.example.bi_offline.sms.SmsSender
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch
import java.io.File
import java.io.FileOutputStream

class ChatActivity : AppCompatActivity() {
    private lateinit var binding: ActivityChatBinding
    private lateinit var adapter: MessageAdapter
    private lateinit var roomId: String
    private lateinit var myNumber: String
    private val PICK_IMAGE = 101

    private val pickImageLauncher = registerForActivityResult(ActivityResultContracts.GetContent()) { uri: Uri? ->
        uri?.let { sendPhoto(uri) }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityChatBinding.inflate(layoutInflater)
        setContentView(binding.root)

        roomId = intent.getStringExtra("room_id") ?: "DEFAULT"
        myNumber = getSharedPreferences("bi_prefs", MODE_PRIVATE).getString("my_number", "me") ?: "me"
        binding.tvRoomTitle.text = "Room: $roomId"

        adapter = MessageAdapter(myNumber)
        binding.recycler.layoutManager = LinearLayoutManager(this)
        binding.recycler.adapter = adapter

        val db = AppDatabase.getInstance(this)
        lifecycleScope.launch {
            db.messageDao().getChat(roomId).collectLatest { list ->
                adapter.submitList(list)
                binding.recycler.scrollToPosition(list.size - 1.coerceAtLeast(0))
            }
        }

        binding.btnSend.setOnClickListener {
            val text = binding.etMessage.text.toString().trim()
            if (text.isNotEmpty()) {
                if (ActivityCompat.checkSelfPermission(this, Manifest.permission.SEND_SMS) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.SEND_SMS), 2001)
                    return@setOnClickListener
                }
                // for demo: send same SMS to each contact they want — here we will only send to one peer entered
                val to = binding.etPeerNumber.text.toString().trim()
                if (to.isNotEmpty()) {
                    SmsSender.sendSms(roomId, to, text)
                }
                // save locally
                lifecycleScope.launch(Dispatchers.IO) {
                    db.messageDao().insert(
                        Message(
                            roomId = roomId,
                            sender = myNumber,
                            content = text,
                            photoPath = null,
                            timestamp = System.currentTimeMillis(),
                            replyTo = null
                        )
                    )
                }
                binding.etMessage.text?.clear()
            }
        }

        binding.btnAttach.setOnClickListener {
            // pick image
            pickImageLauncher.launch("image/*")
        }
    }

    private fun sendPhoto(uri: Uri) {
        // copy to app private file and send a "photo tag" via SMS with path
        lifecycleScope.launch(Dispatchers.IO) {
            val filename = saveUriToInternal(uri)
            val to = binding.etPeerNumber.text.toString().trim()
            if (to.isNotEmpty()) {
                SmsSender.sendPhotoTag(roomId, to, filename)
            }
            val db = AppDatabase.getInstance(this@ChatActivity)
            db.messageDao().insert(
                Message(
                    roomId = roomId,
                    sender = myNumber,
                    content = null,
                    photoPath = filename,
                    timestamp = System.currentTimeMillis(),
                    replyTo = null
                )
            )
        }
    }

    private fun saveUriToInternal(uri: Uri): String {
        // Save file to app files dir and return absolute path
        val resolver = contentResolver
        val name = queryName(uri) ?: "img_${System.currentTimeMillis()}.jpg"
        val file = File(filesDir, name)
        resolver.openInputStream(uri).use { input ->
            FileOutputStream(file).use { out ->
                input?.copyTo(out)
            }
        }
        return file.absolutePath
    }

    private fun queryName(uri: Uri): String? {
        var name: String? = null
        contentResolver.query(uri, null, null, null, null)?.use { cursor ->
            val idx = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME)
            if (idx >= 0 && cursor.moveToFirst()) {
                name = cursor.getString(idx)
            }
        }
        return name
    }
}
package com.example.bi_offline.ui

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.example.bi_offline.databinding.ItemRoomBinding

class RoomAdapter(private val onClick: (String) -> Unit) : RecyclerView.Adapter<RoomAdapter.VH>() {
    private val list = mutableListOf<String>()
    fun submit(data: List<String>) {
        list.clear(); list.addAll(data); notifyDataSetChanged()
    }
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) =
        VH(ItemRoomBinding.inflate(LayoutInflater.from(parent.context), parent, false))
    override fun getItemCount() = list.size
    override fun onBindViewHolder(holder: VH, position: Int) {
        val room = list[position]
        holder.binding.tvRoom.text = room
        holder.binding.root.setOnClickListener { onClick(room) }
    }
    class VH(val binding: ItemRoomBinding) : RecyclerView.ViewHolder(binding.root)
}
package com.example.bi_offline.ui

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.example.bi_offline.data.Message
import com.example.bi_offline.databinding.ItemMessageReceivedBinding
import com.example.bi_offline.databinding.ItemMessageSentBinding
import java.text.SimpleDateFormat
import java.util.*

class MessageAdapter(private val myNumber: String) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {
    private val list = mutableListOf<Message>()
    fun submitList(data: List<Message>) {
        list.clear(); list.addAll(data); notifyDataSetChanged()
    }
    override fun getItemViewType(position: Int): Int {
        return if (list[position].sender == myNumber) 1 else 0
    }
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        val li = LayoutInflater.from(parent.context)
        return if (viewType == 1) {
            SentVH(ItemMessageSentBinding.inflate(li, parent, false))
        } else {
            RecVH(ItemMessageReceivedBinding.inflate(li, parent, false))
        }
    }
    override fun getItemCount() = list.size
    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        val m = list[position]
        val time = SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date(m.timestamp))
        if (holder is SentVH) {
            holder.binding.tvText.text = m.content ?: "[PHOTO]"
            holder.binding.tvTime.text = time
        } else if (holder is RecVH) {
            holder.binding.tvText.text = m.content ?: "[PHOTO]"
            holder.binding.tvFrom.text = m.sender
            holder.binding.tvTime.text = time
        }
    }
    class SentVH(val binding: ItemMessageSentBinding) : RecyclerView.ViewHolder(binding.root)
    class RecVH(val binding: ItemMessageReceivedBinding) : RecyclerView.ViewHolder(binding.root)
}
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical" android:padding="16dp"
    android:layout_width="match_parent" android:layout_height="match_parent">
    <EditText android:id="@+id/etMobile" android:hint="Enter mobile" android:inputType="phone"
        android:layout_width="match_parent" android:layout_height="wrap_content"/>
    <Button android:id="@+id/btnContinue" android:text="Continue" android:layout_width="wrap_content"
        android:layout_height="wrap_content" />
</LinearLayout>
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical" android:padding="16dp"
    android:layout_width="match_parent" android:layout_height="match_parent">
    <EditText android:id="@+id/etOtp" android:hint="Enter OTP" android:inputType="number"
        android:layout_width="match_parent" android:layout_height="wrap_content"/>
    <Button android:id="@+id/btnVerify" android:text="Verify" android:layout_width="wrap_content"
        android:layout_height="wrap_content" />
</LinearLayout>
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical" android:padding="12dp"
    android:layout_width="match_parent" android:layout_height="match_parent">
    <EditText android:id="@+id/etRoomId" android:hint="Room ID (e.g. FRIENDS123)"
        android:layout_width="match_parent" android:layout_height="wrap_content"/>
    <Button android:id="@+id/btnCreateRoom" android:text="Create / Join" android:layout_width="wrap_content"
        android:layout_height="wrap_content"/>
    <androidx.recyclerview.widget.RecyclerView android:id="@+id/recycler"
        android:layout_width="match_parent" android:layout_height="0dp" android:layout_weight="1"/>
</LinearLayout>
<?xml version="1.0" encoding="utf-8"?>
<androidx.cardview.widget.CardView xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_margin="6dp" android:layout_width="match_parent" android:layout_height="wrap_content">
    <TextView android:id="@+id/tvRoom" android:padding="12dp" android:textSize="16sp"
        android:layout_width="match_parent" android:layout_height="wrap_content"/>
</androidx.cardview.widget.CardView>
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical" android:padding="8dp"
    android:layout_width="match_parent" android:layout_height="match_parent">
    <TextView android:id="@+id/tvRoomTitle" android:layout_width="match_parent" android:layout_height="wrap_content"
        android:text="Room" android:textStyle="bold" android:padding="4dp"/>
    <EditText android:id="@+id/etPeerNumber" android:hint="Peer number (to send SMS)" android:layout_width="match_parent"
        android:layout_height="wrap_content" />
    <androidx.recyclerview.widget.RecyclerView android:id="@+id/recycler" android:layout_width="match_parent"
        android:layout_height="0dp" android:layout_weight="1" />
    <LinearLayout android:layout_width="match_parent" android:layout_height="wrap_content" android:orientation="horizontal">
        <ImageButton android:id="@+id/btnAttach" android:layout_width="48dp" android:layout_height="48dp" android:src="@android:drawable/ic_menu_camera"/>
        <EditText android:id="@+id/etMessage" android:layout_weight="1" android:layout_width="0dp"
            android:layout_height="wrap_content" android:hint="Message"/>
        <Button android:id="@+id/btnSend" android:text="Send" android:layout_width="wrap_content" android:layout_height="wrap_content"/>
    </LinearLayout>
</LinearLayout>
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical" android:padding="6dp" android:layout_width="match_parent" android:layout_height="wrap_content">
    <TextView android:id="@+id/tvText" android:layout_width="wrap_content" android:layout_height="wrap_content" android:layout_gravity="end"/>
    <TextView android:id="@+id/tvTime" android:layout_width="wrap_content" android:layout_height="wrap_content" android:textSize="10sp" android:layout_gravity="end"/>
</LinearLayout>
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical" android:padding="6dp" android:layout_width="match_parent" android:layout_height="wrap_content">
    <TextView android:id="@+id/tvFrom" android:layout_width="wrap_content" android:layout_height="wrap_content" android:textStyle="bold"/>
    <TextView android:id="@+id/tvText" android:layout_width="wrap_content" android:layout_height="wrap_content"/>
    <TextView android:id="@+id/tvTime" android:layout_width="wrap_content" android:layout_height="wrap_content" android:textSize="10sp"/>
</LinearLayout>

