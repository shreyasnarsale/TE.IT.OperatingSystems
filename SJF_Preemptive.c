#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void inputProcesses(Process p[], int *n) {
    printf("Enter number of processes (1 - %d): ", MAX);
    scanf("%d", n);

    while (*n <= 0 || *n > MAX) {
        printf("Invalid. Enter number between 1 and %d: ", MAX);
        scanf("%d", n);
    }

    for (int i = 0; i < *n; i++) {
        p[i].pid = i + 1;

        printf("Process %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival_time);
        while (p[i].arrival_time < 0) {
            printf("Arrival time can't be negative. Re-enter: ");
            scanf("%d", &p[i].arrival_time);
        }

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst_time);
        while (p[i].burst_time <= 0) {
            printf("Burst time must be > 0. Re-enter: ");
            scanf("%d", &p[i].burst_time);
        }

        p[i].remaining_time = p[i].burst_time;
    }
}

void calculateAndDisplay(Process p[], int n) {
    float total_tat = 0, total_wt = 0;

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        total_tat += p[i].turnaround_time;
        total_wt += p[i].waiting_time;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

void printGanttChart(int timeline[], int length) {
    printf("\nGantt Chart:\n ");

    for (int i = 0; i < length; i++)
        printf("----");
    printf("\n|");

    for (int i = 0; i < length; i++) {
        if (timeline[i] == -1)
            printf("    |");
        else
            printf(" P%d |", timeline[i]);
    }

    printf("\n ");
    for (int i = 0; i < length; i++)
        printf("----");
    printf("\n");

    for (int i = 0; i <= length; i++)
        printf("%-4d", i);
    printf("\n");
}

void sjfPreemptive(Process p[], int n) {
    int completed = 0, current_time = 0, min_remaining = 1e9;
    int shortest = -1;
    bool found = false;
    int timeline[1000];
    int tIndex = 0;

    while (completed != n) {
        found = false;
        min_remaining = 1e9;
        shortest = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time &&
                p[i].remaining_time > 0 &&
                p[i].remaining_time < min_remaining) {
                min_remaining = p[i].remaining_time;
                shortest = i;
                found = true;
            }
        }

        if (!found) {
            timeline[tIndex++] = -1;
            current_time++;
            continue;
        }

        p[shortest].remaining_time--;
        timeline[tIndex++] = p[shortest].pid;

        if (p[shortest].remaining_time == 0) {
            completed++;
            p[shortest].completion_time = current_time + 1;
        }

        current_time++;
    }

    printf("\n--- Shortest Job First (Preemptive / SRTF) ---\n");
    printGanttChart(timeline, tIndex);
    calculateAndDisplay(p, n);
}

int main() {
    int n;
    Process p[MAX];

    inputProcesses(p, &n);
    sjfPreemptive(p, n);

    return 0;
}

/* Compile & Run:
   gcc SJF_Preemptive.c -o sjf
   ./sjf
*/


/*

Turnaround Time (TAT) = CT − AT
Waiting Time (WT) = TAT − BT

Enter number of processes (1 - 100): 5
Process 1 Arrival Time: 0
Process 1 Burst Time: 1
Process 2 Arrival Time: 1
Process 2 Burst Time: 5
Process 3 Arrival Time: 2
Process 3 Burst Time: 4
Process 4 Arrival Time: 3
Process 4 Burst Time: 7
Process 5 Arrival Time: 4
Process 5 Burst Time: 9

--- Shortest Job First (Preemptive / SRTF) ---

Gantt Chart:
 --------------------------------------------------------------------------------------------------------
| P1 | P2 | P2 | P2 | P2 | P2 | P3 | P3 | P3 | P3 | P4 | P4 | P4 | P4 | P4 | P4 | P4 | P5 | P5 | P5 | P5 | P5 | P5 | P5 | P5 | P5 |
 --------------------------------------------------------------------------------------------------------
0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  

PID     AT      BT      CT      TAT     WT
P1      0       1       1       1       0
P2      1       5       6       5       0
P3      2       4       10      8       4
P4      3       7       17      14      7
P5      4       9       26      22      13

Average Turnaround Time: 10.00
Average Waiting Time: 4.80

*/
