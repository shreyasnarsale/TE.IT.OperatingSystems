//parent.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n -1; i++)
        for (int j = 0; j < n - i -1; j++)
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
}

int main() {
    int n;

    printf("Enter the number of elements in the array: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size\n");
        return 1;
    }

    int arr[n];
    printf("Enter %d integers separated by spaces:\n", n);

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input\n");
            return 1;
        }
    }

    bubbleSort(arr, n);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process exec

        char *child_program = "./child";
        static char str_arr[100][12];
        char *child_argv[n + 2];

        child_argv[0] = child_program;

        for (int i = 0; i < n; i++) {
            snprintf(str_arr[i], sizeof(str_arr[i]), "%d", arr[i]);
            child_argv[i + 1] = str_arr[i];
        }
        child_argv[n + 1] = NULL;

        execve(child_program, child_argv, NULL);

        perror("execve failed");
        exit(1);
    } else {
        wait(NULL);
        printf("Parent process: Child finished execution.\n");
    }

    return 0;
}





//child.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No array passed to child process.\n");
        return 1;
    }

    int n = argc - 1;
    int arr[n];

    // Convert arguments to integers
    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    printf("Child process: Array in reverse order:\n");
    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

