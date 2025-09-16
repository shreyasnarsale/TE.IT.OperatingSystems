#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100

// Bubble sort (used by parent)
void bubbleSort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}

// Selection sort (used by child)
void selectionSort(int arr[], int n) {
    int min_idx, temp;
    for (int i = 0; i < n-1; i++) {
        min_idx = i;
        for (int j = i+1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

void printArray(const char* who, int arr[], int n) {
    printf("%s sorted array: ", who);
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n;
    int arr[MAX];

    printf("Enter number of integers to sort: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) {
        printf("Invalid input!\n");
        exit(1);
    }

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input!\n");
            exit(1);
        }
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // Child process
        printf("\n[Child] PID: %d, Parent PID: %d\n", getpid(), getppid());

        // Simulate longer work to create zombie/orphan demonstration
        printf("[Child] Sorting with selection sort...\n");
        selectionSort(arr, n);

        // Sleep to allow parent to exit early (or not call wait)
        sleep(5);

        printArray("[Child]", arr, n);
        printf("[Child] Finished.\n");
        exit(0);
    } 
    else {
        // Parent process
        printf("\n[Parent] PID: %d, Child PID: %d\n", getpid(), pid);

        printf("[Parent] Sorting with bubble sort...\n");
        bubbleSort(arr, n);
        printArray("[Parent]", arr, n);

        // To demonstrate zombie process:
        // Don't call wait immediately, sleep for 6 seconds
        printf("[Parent] Sleeping for 6 seconds to demonstrate zombie state...\n");
        sleep(6);

        // Now reap child process
        int status;
        wait(&status);
        printf("[Parent] Child process reaped, exit status: %d\n", WEXITSTATUS(status));

        // To demonstrate orphan process:
        // Uncomment following lines and comment out wait & sleep above
        /*
        printf("[Parent] Exiting immediately to orphan the child process.\n");
        exit(0);
        */

        printf("[Parent] Finished.\n");
    }

    return 0;
}


/*Output:-
   gcc fortsortstates.c -o out
   ./out
*/
