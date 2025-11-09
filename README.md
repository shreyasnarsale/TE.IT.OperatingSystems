# Operating System Practicals

**Course:** TE BE IT (2019 Pattern) — Savitribai Phule Pune University

## Overview

This repo includes all Operating System practicals implemented in C and shell scripts — covering process control, scheduling, synchronization, memory management, IPC, disk scheduling, and system calls.

## Assignments Summary

**1. Basic Linux Commands & Address Book**
Study basic commands (`echo`, `ls`, `cat`, `grep`, `sed`, etc.) and implement an Address Book program (create, view, insert, delete, modify records).

**2. Process Control System Calls**
Implement `fork`, `execve`, and `wait` — show zombie and orphan processes. Parent/child sorting and reverse array using `execve`.

**3. CPU Scheduling Algorithms**
Implement **SJF (Preemptive)** and **Round Robin** scheduling.

**4. Thread Synchronization**
Use semaphores and mutex to demonstrate **Producer–Consumer** and **Reader–Writer** problems.

**5. Memory Management**
Implement **Page Replacement (FCFS, LRU, Optimal)** and **Banker’s Algorithm** for deadlock avoidance.

**7. IPC – FIFOs**
Full-duplex communication between two processes using named pipes.

**8. Disk Scheduling**
Implement **SSTF**, **SCAN**, and **C-LOOK** algorithms.

**Study Assignment**
Add a new Linux **system call** and demonstrate from user space. Also include **Shared Memory IPC** using System V.

## How to Compile

```bash
gcc program.c -o program
./program
# For threads
gcc program.c -o program -lpthread
```




