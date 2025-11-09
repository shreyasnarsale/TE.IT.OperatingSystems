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

void CLOOK(int req[], int n, int head) {
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

    printf("\n--- C-LOOK Disk Scheduling ---\nSeek Sequence: ");

    // Move towards higher cylinders
    for (int i = 0; i < r; i++) {
        printf("%d ", right[i]);
        total += abs(head - right[i]);
        head = right[i];
    }

    // Jump back to smallest request
    if (l > 0) {
        total += abs(head - left[0]);
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
    int n, req[100], head;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    printf("Enter disk requests:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    CLOOK(req, n, head);
    return 0;
}

/*
Output:-

$ gcc CLOOK.c -o out
$ ./out

Example:
Input:
8
176 79 34 60 92 11 41 114
50

Output:
--- C-LOOK Disk Scheduling ---
Seek Sequence: 60 79 92 114 176 11 34 41
Total Seek Movement = 322
*/
