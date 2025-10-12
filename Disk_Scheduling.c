#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
}

void SSTF(int req[], int n, int head) {
    int visited[100] = {0}, count = 0, cur = head, total = 0;
    printf("\n--- SSTF Disk Scheduling ---\nSeek Sequence: ");
    while (count < n) {
        int min = 9999, idx = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int diff = abs(cur - req[i]);
                if (diff < min) {
                    min = diff;
                    idx = i;
                }
            }
        }
        visited[idx] = 1;
        total += abs(cur - req[idx]);
        cur = req[idx];
        printf("%d ", cur);
        count++;
    }
    printf("\nTotal Seek Movement = %d\n", total);
}

void SCAN(int req[], int n, int head, int disk_size) {
    int total = 0;
    int left[100], right[100];
    int l = 0, r = 0;

    for (int i = 0; i < n; i++) {
        if (req[i] < head)
            left[l++] = req[i];
        else
            right[r++] = req[i];
    }
    sort(left, l);
    sort(right, r);

    printf("\n--- SCAN Disk Scheduling ---\nSeek Sequence: ");

    // Moving away from spindle (towards higher cylinders)
    for (int i = 0; i < r; i++) {
        printf("%d ", right[i]);
        total += abs(head - right[i]);
        head = right[i];
    }

    // Then reverse direction
    if (l > 0) {
        total += abs(disk_size - 1 - head); // go to end
        head = disk_size - 1;

        for (int i = l - 1; i >= 0; i--) {
            printf("%d ", left[i]);
            total += abs(head - left[i]);
            head = left[i];
        }
    }

    printf("\nTotal Seek Movement = %d\n", total);
}

void CLOOK(int req[], int n, int head) {
    int total = 0;
    int left[100], right[100];
    int l = 0, r = 0;

    for (int i = 0; i < n; i++) {
        if (req[i] < head)
            left[l++] = req[i];
        else
            right[r++] = req[i];
    }

    sort(left, l);
    sort(right, r);

    printf("\n--- C-LOOK Disk Scheduling ---\nSeek Sequence: ");

    // Moving away from spindle (towards higher cylinders)
    for (int i = 0; i < r; i++) {
        printf("%d ", right[i]);
        total += abs(head - right[i]);
        head = right[i];
    }

    // Jump back to the lowest request
    if (l > 0) {
        total += abs(head - left[0]); // jump back
        head = left[0];
        for (int i = 0; i < l; i++) {
            printf("%d ", left[i]);
            total += abs(head - left[i]);
            head = left[i];
        }
    }

    printf("\nTotal Seek Movement = %d\n", total);
}

int main() {
    int n, req[100], head, choice, disk_size;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);
    printf("Enter disk requests:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter total disk size (number of cylinders): ");
    scanf("%d", &disk_size);

    while (1) {
        printf("\n---- Disk Scheduling Menu ----\n");
        printf("1. SSTF\n2. SCAN\n3. C-LOOK\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                SSTF(req, n, head);
                break;
            case 2:
                SCAN(req, n, head, disk_size);
                break;
            case 3:
                CLOOK(req, n, head);
                break;
            case 4:
                printf("\nProgram exited successfully... \n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}





/* Output:-

$ gcc Disk_Scheduling.c -o out
$ ./out

*/