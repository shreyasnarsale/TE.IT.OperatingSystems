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

void roundRobin(Process p[], int n, int quantum) {
    int current_time = 0, completed = 0;
    int queue[MAX], front = 0, rear = 0;
    bool inQueue[MAX] = {false};

    queue[rear++] = 0;
    inQueue[0] = true;

    int timeline[10000];
    int tIndex = 0;

    while (completed < n) {
        if (front == rear) {
            int nextArrival = 1e9;
            int nextIdx = -1;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && p[i].remaining_time > 0 && p[i].arrival_time < nextArrival) {
                    nextArrival = p[i].arrival_time;
                    nextIdx = i;
                }
            }
            if (nextIdx == -1)
                break;
            queue[rear++] = nextIdx;
            inQueue[nextIdx] = true;
            if (rear == MAX) rear = 0;
        }

        int idx = queue[front++];
        if (front == MAX) front = 0;

        if (p[idx].arrival_time > current_time) {
            while (current_time < p[idx].arrival_time) {
                timeline[tIndex++] = -1;
                current_time++;
            }
        }

        int exec_time = (p[idx].remaining_time > quantum) ? quantum : p[idx].remaining_time;
        for (int i = 0; i < exec_time; i++) {
            timeline[tIndex++] = p[idx].pid;
        }
        p[idx].remaining_time -= exec_time;
        current_time += exec_time;

        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                queue[rear++] = i;
                if (rear == MAX) rear = 0;
                inQueue[i] = true;
            }
        }

        if (p[idx].remaining_time > 0) {
            queue[rear++] = idx;
            if (rear == MAX) rear = 0;
        } else {
            p[idx].completion_time = current_time;
            completed++;
        }
    }

    printf("\n--- Round Robin Scheduling (Quantum = %d) ---\n", quantum);
    printGanttChart(timeline, tIndex);
    calculateAndDisplay(p, n);
}

int main() {
    int n, quantum;
    Process p[MAX];

    inputProcesses(p, &n);

    printf("Enter Time Quantum (> 0): ");
    scanf("%d", &quantum);
    while (quantum <= 0) {
        printf("Quantum must be > 0. Re-enter: ");
        scanf("%d", &quantum);
    }

    roundRobin(p, n, quantum);

    return 0;
}

/* Compile & Run:
   gcc Round_Robin.c -o rr
   ./rr
*/


/*
Turnaround Time (TAT) = CT − AT
Waiting Time (WT) = TAT − BT


Enter number of processes (1 - 100): 5
Process 1 Arrival Time: 1
Process 1 Burst Time: 2
Process 2 Arrival Time: 3
Process 2 Burst Time: 4
Process 3 Arrival Time: 5
Process 3 Burst Time: 6
Process 4 Arrival Time: 7
Process 4 Burst Time: 3
Process 5 Arrival Time: 7
Process 5 Burst Time: 3
Enter Time Quantum (> 0): 2

--- Round Robin Scheduling (Quantum = 2) ---

Gantt Chart:
 ----------------------------------------------------------------------------
|    | P1 | P1 | P2 | P2 | P3 | P3 | P2 | P2 | P4 | P4 | P5 | P5 | P3 | P3 | P4 | P5 | P3 | P3 |
 ----------------------------------------------------------------------------
0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  

PID     AT      BT      CT      TAT     WT
P1      1       2       3       2       0
P2      3       4       9       6       2
P3      5       6       19      14      8
P4      7       3       16      9       6
P5      7       3       17      10      7

Average Turnaround Time: 8.20
Average Waiting Time: 4.60

*/
