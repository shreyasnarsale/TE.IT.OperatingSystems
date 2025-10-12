#include <stdio.h>
#include <stdlib.h>

#define MAX 30

void printFrames(int frames[], int f) {
    for (int i = 0; i < f; i++) {
        if (frames[i] == -1)
            printf("- ");
        else
            printf("%d ", frames[i]);
    }
}

int findOptimal(int pages[], int frames[], int f, int n, int index) {
    int farthest = index, pos = -1;
    for (int i = 0; i < f; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == n)
            return i; // Not used again
    }
    return (pos == -1) ? 0 : pos;
}

void FCFS(int pages[], int n, int f) {
    int frames[MAX], front = 0, pageFaults = 0;
    for (int i = 0; i < f; i++)
        frames[i] = -1;

    printf("\n==========================");
    printf("\n--- FCFS Page Replacement ---");
    printf("\n==========================\n");
    printf("Page\tFrames\t\tStatus\n");

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                hit = 1;
                break;
            }
        }

        if (!hit) {
            frames[front] = pages[i];
            front = (front + 1) % f;
            pageFaults++;
        }

        printf("%d\t", pages[i]);
        printFrames(frames, f);
        printf("\t%s\n", hit ? "Hit" : "Page Fault");
    }

    float faultRatio = ((float)pageFaults / n) * 100;
    printf("==========================\n");
    printf("Total Page Faults = %d\n", pageFaults);
    printf("Page Fault Ratio  = %.2f%%\n", faultRatio);
    printf("==========================\n");
}

void LRU(int pages[], int n, int f) {
    int frames[MAX], counter = 0, time[MAX], pageFaults = 0;
    for (int i = 0; i < f; i++)
        frames[i] = -1;

    printf("\n==========================");
    printf("\n--- LRU Page Replacement ---");
    printf("\n==========================\n");
    printf("Page\tFrames\t\tStatus\n");

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                hit = 1;
                time[j] = ++counter;
                break;
            }
        }

        if (!hit) {
            int min = 0;
            for (int j = 1; j < f; j++) {
                if (frames[j] == -1) {
                    min = j;
                    break;
                }
                if (time[j] < time[min])
                    min = j;
            }
            frames[min] = pages[i];
            time[min] = ++counter;
            pageFaults++;
        }

        printf("%d\t", pages[i]);
        printFrames(frames, f);
        printf("\t%s\n", hit ? "Hit" : "Page Fault");
    }

    float faultRatio = ((float)pageFaults / n) * 100;
    printf("==========================\n");
    printf("Total Page Faults = %d\n", pageFaults);
    printf("Page Fault Ratio  = %.2f%%\n", faultRatio);
    printf("==========================\n");
}

void Optimal(int pages[], int n, int f) {
    int frames[MAX], pageFaults = 0;
    for (int i = 0; i < f; i++)
        frames[i] = -1;

    printf("\n==========================");
    printf("\n--- Optimal Page Replacement ---");
    printf("\n==========================\n");
    printf("Page\tFrames\t\tStatus\n");

    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                hit = 1;
                break;
            }
        }

        if (!hit) {
            int empty = -1;
            for (int j = 0; j < f; j++) {
                if (frames[j] == -1) {
                    empty = j;
                    break;
                }
            }

            if (empty != -1)
                frames[empty] = pages[i];
            else {
                int pos = findOptimal(pages, frames, f, n, i + 1);
                frames[pos] = pages[i];
            }
            pageFaults++;
        }

        printf("%d\t", pages[i]);
        printFrames(frames, f);
        printf("\t%s\n", hit ? "Hit" : "Page Fault");
    }

    float faultRatio = ((float)pageFaults / n) * 100;
    printf("==========================\n");
    printf("Total Page Faults = %d\n", pageFaults);
    printf("Page Fault Ratio  = %.2f%%\n", faultRatio);
    printf("==========================\n");
}

int main() {
    int n, pages[MAX], f, choice;

    printf("\n==========================");
    printf("\n  Page Replacement System");
    printf("\n==========================\n");

    printf("Enter number of pages: ");
    scanf("%d", &n);
    printf("Enter page reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);
    printf("Enter number of frames (minimum 3): ");
    scanf("%d", &f);

    while (1) {
        printf("\n==========================");
        printf("\n---- Page Replacement Menu ----");
        printf("\n==========================");
        printf("\n1. FCFS");
        printf("\n2. LRU");
        printf("\n3. Optimal");
        printf("\n4. Exit");
        printf("\n==========================");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                FCFS(pages, n, f);
                break;
            case 2:
                LRU(pages, n, f);
                break;
            case 3:
                Optimal(pages, n, f);
                break;
            case 4:
                printf("\n==========================");
                printf("\nProgram exited successfully ");
                printf("\n==========================\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
/* Output:-

$ gcc page_replacement.c -o out 
$ ./out

*/






