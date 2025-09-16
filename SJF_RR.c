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
    printf("\nGantt Chart:\n");

    // Print top bar
    printf(" ");
    for (int i = 0; i < length; i++) {
        printf("----");
    }
    printf("\n|");

    // Print process ids in gantt chart
    for (int i = 0; i < length; i++) {
        if (timeline[i] == -1)
            printf("    |");  // idle
        else
            printf(" P%d |", timeline[i]);
    }

    printf("\n ");

    // Print bottom bar
    for (int i = 0; i < length; i++) {
        printf("----");
    }
    printf("\n");

    // Print time indices
    for (int i = 0; i <= length; i++) {
        printf("%-4d", i);
    }
    printf("\n");
}

void sjfPreemptive(Process p[], int n) {
    int completed = 0, current_time = 0, min_remaining = 1e9;
    int shortest = -1;
    bool found = false;

    int timeline[1000];  // record running process at each time unit
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
            timeline[tIndex++] = -1; // idle
            current_time++;
            continue;
        }

        // Execute 1 unit of time
        p[shortest].remaining_time--;
        timeline[tIndex++] = p[shortest].pid;

        if (p[shortest].remaining_time == 0) {
            completed++;
            p[shortest].completion_time = current_time + 1;
        }

        current_time++;
    }

    printf("\n--- SJF (Preemptive / SRTF) Scheduling ---\n");
    printGanttChart(timeline, tIndex);   // Print Gantt Chart first
    calculateAndDisplay(p, n);           // Then print table
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
                break; // all done
            queue[rear++] = nextIdx;
            inQueue[nextIdx] = true;
            if (rear == MAX) rear = 0;
        }

        int idx = queue[front++];
        if (front == MAX) front = 0;

        if (p[idx].arrival_time > current_time) {
            while (current_time < p[idx].arrival_time) {
                timeline[tIndex++] = -1; // idle
                current_time++;
            }
        }

        int exec_time = (p[idx].remaining_time > quantum) ? quantum : p[idx].remaining_time;
        for (int i = 0; i < exec_time; i++) {
            timeline[tIndex++] = p[idx].pid;
        }
        p[idx].remaining_time -= exec_time;
        current_time += exec_time;

        // Add newly arrived processes to queue
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                queue[rear++] = i;
                if (rear == MAX) rear = 0;
                inQueue[i] = true;
            }
        }

        // If process still has burst left, add it back to queue
        if (p[idx].remaining_time > 0) {
            queue[rear++] = idx;
            if (rear == MAX) rear = 0;
        } else {
            p[idx].completion_time = current_time;
            completed++;
        }
    }

    printf("\n--- Round Robin Scheduling (Quantum = %d) ---\n", quantum);
    printGanttChart(timeline, tIndex);   // Print Gantt Chart first
    calculateAndDisplay(p, n);           // Then print table
}

int main() {
    int choice, n, quantum;
    Process p[MAX], p_copy[MAX];

    inputProcesses(p, &n);

    printf("\nChoose Scheduling Algorithm:\n");
    printf("1. Shortest Job First (Preemptive)\n");
    printf("2. Round Robin\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    while (choice != 1 && choice != 2) {
        printf("Invalid choice. Choose 1 or 2: ");
        scanf("%d", &choice);
    }

    // Make a copy of original processes for both algorithms
    for (int i = 0; i < n; i++) {
        p_copy[i] = p[i];
    }

    if (choice == 1) {
        sjfPreemptive(p_copy, n);
    } else {
        printf("Enter Time Quantum (> 0): ");
        scanf("%d", &quantum);
        while (quantum <= 0) {
            printf("Quantum must be > 0. Re-enter: ");
            scanf("%d", &quantum);
        }
        roundRobin(p_copy, n, quantum);
    }

    return 0;
}


/*Output:-

     gcc SJF_RR.c -o out
     ./out
*/
