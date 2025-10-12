// File1: shm_server.c

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#define SHM_KEY 1234   // Shared memory key
#define SIZE 1024       // Size of shared memory

int main() {
    int shmid;
    char *shmaddr;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the segment to the address space
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Enter message to write in shared memory: ");
    fgets(shmaddr, SIZE, stdin);     // write directly to shared memory
    shmaddr[strcspn(shmaddr, "\n")] = '\0'; // remove newline

    printf("Server: Message written to shared memory.\n");

    // Detach shared memory
    shmdt(shmaddr);

    printf("Server: Waiting for client to read... (Run client now)\n");

    return 0;
}


//File2: shm_client.c

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define SHM_KEY 1234
#define SIZE 1024

int main() {
    int shmid;
    char *shmaddr;

    // Locate the shared memory segment created by server
    shmid = shmget(SHM_KEY, SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach to it
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Client: Message read from shared memory:\n");
    printf("%s\n", shmaddr);

    // Detach and destroy shared memory
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);   // remove shared memory segment

    printf("Client: Shared memory detached and destroyed.\n");

    return 0;
}






/* Output:-

$ gcc shm_server.c -o out (Run at 1st terminal)
$ gcc shm_client.c -o out (Run at 2nd terminal)
$ ./out

*/


