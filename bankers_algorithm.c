#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m, i, j, k;

    // n = number of processes, m = number of resources
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    int alloc[n][m];   // Allocation matrix
    int max[n][m];     // Maximum demand matrix
    int avail[m];      // Available resources
    int need[n][m];    // Need matrix

    // Input allocation matrix
    printf("\nEnter Allocation Matrix (rows = processes, columns = resources):\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    // Input maximum demand matrix
    printf("\nEnter Maximum Matrix (rows = processes, columns = resources):\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    // Input available resources
    printf("\nEnter Available Resources:\n");
    for (i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    // Calculate Need matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Display Allocation, Maximum, Need Matrices
    printf("\nProcess\tAllocation\tMaximum\t\tNeed\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t", i);
        for (j = 0; j < m; j++)
            printf("%d ", alloc[i][j]);
        printf("\t\t");
        for (j = 0; j < m; j++)
            printf("%d ", max[i][j]);
        printf("\t\t");
        for (j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    // Banker's Algorithm
    bool finish[n];
    for (i = 0; i < n; i++)
        finish[i] = false;

    int safeSeq[n];
    int count = 0;

    while (count < n) {
        bool found = false;

        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canAllocate = true;

                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (k = 0; k < m; k++)
                        avail[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("\nSystem is NOT in a safe state.\n");
            return 0;
        }
    }

    // Print safe sequence
    printf("\nSystem is in a SAFE state.\nSafe sequence is: ");
    for (i = 0; i < n; i++) {
        printf("P%d", safeSeq[i]);
        if (i != n - 1)
            printf(" -> ");
    }
    printf("\n");

    return 0;
}


/* --------------------------
   Example Compilation & Run:
   gcc bankers_algorithm.c -o out
   ./out
-----------------------------*/
