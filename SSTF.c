#include <stdio.h>
#include <stdlib.h>

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

int main() {
    int n, req[100], head;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    printf("Enter disk requests:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    SSTF(req, n, head);
    return 0;
}

/*


/* Output:-

$ gcc SSTF.c -o out
$ ./out


Example:
Input:
8
176 79 34 60 92 11 41 114
50

Output:
--- SSTF Disk Scheduling ---
Seek Sequence: 41 34 11 60 79 92 114 176
Total Seek Movement = 204
*/
