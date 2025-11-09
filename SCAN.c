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

void SCAN(int req[], int n, int head, int disk_size) {
    int total = 0, left[100], right[100];
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

    // Move towards higher cylinders
    for (int i = 0; i < r; i++) {
        printf("%d ", right[i]);
        total += abs(head - right[i]);
        head = right[i];
    }

    // Move back to smaller cylinders
    if (l > 0) {
        total += abs(disk_size - 1 - head);
        head = disk_size - 1;

        for (int i = l - 1; i >= 0; i--) {
            printf("%d ", left[i]);
            total += abs(head - left[i]);
            head = left[i];
        }
    }

    printf("\nTotal Seek Movement = %d\n", total);
}

int main() {
    int n, req[100], head, disk_size;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    printf("Enter disk requests:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter total disk size (number of cylinders): ");
    scanf("%d", &disk_size);

    SCAN(req, n, head, disk_size);
    return 0;
}

/*

Output:-

$ gcc SCAN.c -o out
$ ./out


Example:
Input:
8
176 79 34 60 92 11 41 114
50
200

Output:
--- SCAN Disk Scheduling ---
Seek Sequence: 60 79 92 114 176 199 41 34 11
Total Seek Movement = 389
*/
