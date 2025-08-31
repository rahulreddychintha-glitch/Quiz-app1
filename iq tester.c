#include <stdio.h>
#include<conio.h>
#include <time.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"

int main() {
    int choice, score = 0, menu, qcount = 0;
    time_t start, end;

    printf(BLUE "==============================\n" RESET);
    printf(BLUE "       QUIZ APPLICATION       \n" RESET);
    printf(BLUE "==============================\n" RESET);

    printf(WHITE "1. Start Quiz\n" RESET);
    printf(WHITE "2. Exit\n" RESET);
    printf(YELLOW "Enter your choice: " RESET);
    scanf("%d", &menu);

    if (menu == 2) {
        printf(RED "Exiting... Goodbye!\n" RESET);
        return 0;
    }
    if (menu != 1) {
        printf(RED "Invalid choice! Exiting.\n" RESET);
        return 0;
    }

    time(&start);
    printf(BLUE "\n--- Welcome to the Quiz ---\n\n" RESET);

    /* Q1 */
    printf(WHITE "Q1. Capital of Kazakhstan?\n" RESET);
    printf("1. Almaty\n2. Astana\n3. Tashkent\n4. Bishkek\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q2 */
    printf(WHITE "Q2. Largest desert by area?\n" RESET);
    printf("1. Sahara\n2. Gobi\n3. Antarctica\n4. Arabian\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q3 */
    printf(WHITE "Q3. Prime number after 97?\n" RESET);
    printf("1. 99\n2. 101\n3. 103\n4. 97\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q4 */
    printf(WHITE "Q4. Average-case time of Quick Sort?\n" RESET);
    printf("1. O(n)\n2. O(n log n)\n3. O(n^2)\n4. O(log n)\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q5 */
    printf(WHITE "Q5. First Nobel Prizes awarded in which year?\n" RESET);
    printf("1. 1895\n2. 1901\n3. 1914\n4. 1921\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    if (qcount % 5 == 0 && qcount < 30) {
        printf(BLUE "----- Menu -----\n" RESET);
        printf("1. Continue Quiz\n2. Exit\n");
        printf(YELLOW "Enter your choice: " RESET); scanf("%d",&menu);
        if (menu == 2) {
            time(&end);
            double t = difftime(end,start);
            printf(GREEN "\nYou answered %d questions.\n" RESET, qcount);
            printf("Your score = %d\n", score);
            printf("Time taken = %.0f seconds\n", t);
            return 0;
        }
        printf("\n");
    }

    /* Q6 */
    printf(WHITE "Q6. Element with highest melting point?\n" RESET);
    printf("1. Carbon\n2. Tungsten\n3. Osmium\n4. Tantalum\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q7 */
    printf(WHITE "Q7. IPv6 address length?\n" RESET);
    printf("1. 32 bits\n2. 64 bits\n3. 128 bits\n4. 256 bits\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q8 */
    printf(WHITE "Q8. Author of \"Gödel, Escher, Bach\"?\n" RESET);
    printf("1. Douglas Hofstadter\n2. Kurt Gödel\n3. Martin Gardner\n4. Richard Feynman\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 1: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q9 */
    printf(WHITE "Q9. Determinant of [[2,1],[5,3]]?\n" RESET);
    printf("1. 1\n2. 11\n3. -1\n4. 7\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 1: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q10 */
    printf(WHITE "Q10. Limestone's main chemical formula?\n" RESET);
    printf("1. CaCO3\n2. CaO\n3. Ca(OH)2\n4. CaSO4\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 1: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    if (qcount % 5 == 0 && qcount < 30) {
        printf(BLUE "----- Menu -----\n" RESET);
        printf("1. Continue Quiz\n2. Exit\n");
        printf(YELLOW "Enter your choice: " RESET); scanf("%d",&menu);
        if (menu == 2) {
            time(&end);
            double t = difftime(end,start);
            printf(GREEN "\nYou answered %d questions.\n" RESET, qcount);
            printf("Your score = %d\n", score);
            printf("Time taken = %.0f seconds\n", t);
            return 0;
        }
        printf("\n");
    }

    /* Q11 */
    printf(WHITE "Q11. Indian Constitution came into effect in?\n" RESET);
    printf("1. 1947\n2. 1949\n3. 1950\n4. 1952\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q12 */
    printf(WHITE "Q12. Densest element at STP?\n" RESET);
    printf("1. Iridium\n2. Osmium\n3. Platinum\n4. Uranium\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q13 */
    printf(WHITE "Q13. Speed of light in vacuum (~)?\n" RESET);
    printf("1. 3.0×10^6 m/s\n2. 3.0×10^7 m/s\n3. 3.0×10^8 m/s\n4. 3.0×10^9 m/s\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q14 */
    printf(WHITE "Q14. Founder of the Mughal Empire?\n" RESET);
    printf("1. Akbar\n2. Babur\n3. Humayun\n4. Sher Shah Suri\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q15 */
    printf(WHITE "Q15. RSA security is based primarily on?\n" RESET);
    printf("1. Discrete log over elliptic curves\n2. Integer factorization\n3. Lattice reduction\n4. Hash collisions\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    if (qcount % 5 == 0 && qcount < 30) {
        printf(BLUE "----- Menu -----\n" RESET);
        printf("1. Continue Quiz\n2. Exit\n");
        printf(YELLOW "Enter your choice: " RESET); scanf("%d",&menu);
        if (menu == 2) {
            time(&end);
            double t = difftime(end,start);
            printf(GREEN "\nYou answered %d questions.\n" RESET, qcount);
            printf("Your score = %d\n", score);
            printf("Time taken = %.0f seconds\n", t);
            return 0;
        }
        printf("\n");
    }

    /* Q16 */
    printf(WHITE "Q16. Which sorting algorithm is stable by default?\n" RESET);
    printf("1. Quick sort\n2. Heap sort\n3. Merge sort\n4. Selection sort\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q17 */
    printf(WHITE "Q17. Planet with the shortest day (fastest rotation)?\n" RESET);
    printf("1. Saturn\n2. Jupiter\n3. Mars\n4. Neptune\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q18 */
    printf(WHITE "Q18. Sum of elements in the nth row of Pascal's triangle (row 0-based)?\n" RESET);
    printf("1. n!\n2. 2^n\n3. n^2\n4. Fibonacci(n)\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q19 */
    printf(WHITE "Q19. A tree with n vertices has how many edges?\n" RESET);
    printf("1. n\n2. n-1\n3. n+1\n4. 2n-1\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q20 */
    printf(WHITE "Q20. UNIX command to count lines in a file?\n" RESET);
    printf("1. cat file | count\n2. nl file\n3. wc -l file\n4. grep -c . file\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    if (qcount % 5 == 0 && qcount < 30) {
        printf(BLUE "----- Menu -----\n" RESET);
        printf("1. Continue Quiz\n2. Exit\n");
        printf(YELLOW "Enter your choice: " RESET); scanf("%d",&menu);
        if (menu == 2) {
            time(&end);
            double t = difftime(end,start);
            printf(GREEN "\nYou answered %d questions.\n" RESET, qcount);
            printf("Your score = %d\n", score);
            printf("Time taken = %.0f seconds\n", t);
            return 0;
        }
        printf("\n");
    }

    /* Q21 */
    printf(WHITE "Q21. ASCII code for uppercase 'A'?\n" RESET);
    printf("1. 65\n2. 66\n3. 64\n4. 97\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 1: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q22 */
    printf(WHITE "Q22. Primary fusion in the Sun converts?\n" RESET);
    printf("1. Helium to carbon\n2. Hydrogen to helium\n3. Oxygen to neon\n4. Iron to nickel\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q23 */
    printf(WHITE "Q23. Trivial zeros of Riemann zeta ζ(s) occur at?\n" RESET);
    printf("1. Negative odd integers\n2. Negative even integers\n3. Positive even integers\n4. s=1 only\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q24 */
    printf(WHITE "Q24. Algorithm commonly used to find a Minimum Spanning Tree?\n" RESET);
    printf("1. Dijkstra's\n2. Bellman-Ford\n3. Kruskal's\n4. Floyd–Warshall\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q25 */
    printf(WHITE "Q25. Many of the largest known primes are of which form?\n" RESET);
    printf("1. Fermat primes (2^(2^n)+1)\n2. Mersenne primes (2^p-1)\n3. Sophie Germain primes\n4. Twin primes\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    if (qcount % 5 == 0 && qcount < 30) {
        printf(BLUE "----- Menu -----\n" RESET);
        printf("1. Continue Quiz\n2. Exit\n");
        printf(YELLOW "Enter your choice: " RESET); scanf("%d",&menu);
        if (menu == 2) {
            time(&end);
            double t = difftime(end,start);
            printf(GREEN "\nYou answered %d questions.\n" RESET, qcount);
            printf("Your score = %d\n", score);
            printf("Time taken = %.0f seconds\n", t);
            return 0;
        }
        printf("\n");
    }

    /* Q26 */
    printf(WHITE "Q26. Which Indian missile is an ICBM-class system?\n" RESET);
    printf("1. Prithvi-II\n2. Agni-V\n3. Akash\n4. BrahMos\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q27 */
    printf(WHITE "Q27. CPI primarily measures?\n" RESET);
    printf("1. Economic growth rate\n2. Consumer price inflation\n3. Interest rate level\n4. Trade deficit\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 2: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q28 */
    printf(WHITE "Q28. SQL join that returns rows common to both tables?\n" RESET);
    printf("1. LEFT JOIN\n2. RIGHT JOIN\n3. FULL OUTER JOIN\n4. INNER JOIN\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 4: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q29 */
    printf(WHITE "Q29. Scheduling best suited for time-sharing systems?\n" RESET);
    printf("1. First-Come, First-Served\n2. Priority Scheduling\n3. Round Robin\n4. Shortest Job First\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    /* Q30 */
    printf(WHITE "Q30. Deficiency of which vitamin causes scurvy?\n" RESET);
    printf("1. Vitamin A\n2. Vitamin B12\n3. Vitamin C\n4. Vitamin D\n");
    printf(YELLOW "Enter your choice: " RESET); scanf("%d",&choice);
    switch(choice){ case 3: printf(GREEN "Correct!\n" RESET); score++; break; default: printf(RED "Wrong!\n" RESET); }
    qcount++; printf("\n");

    time(&end);
    double t = difftime(end, start);

    printf(BLUE "==============================\n" RESET);
    printf("Your Total Score = %d out of 30\n", score);
    printf("Time taken = %.0f seconds\n", t);
    if (score == 30)       printf(GREEN "Outstanding! Perfect 30/30!\n" RESET);
    else if (score >= 24)  printf(GREEN "Excellent performance!\n" RESET);
    else if (score >= 18)  printf(YELLOW "Great Job!\n" RESET);
    else if (score >= 12)  printf(YELLOW "Good Try!\n" RESET);
    else                   printf(RED "Keep practicing!\n" RESET);
    printf(BLUE "==============================\n" RESET);

    getch();
}