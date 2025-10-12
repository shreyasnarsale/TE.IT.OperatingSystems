#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5    
#define MAX_ITEMS 20     

int buffer[BUFFER_SIZE];
int in = 0;   // Index where producer inserts next item
int out = 0;  // Index where consumer removes next item


// Synchronization primitives
sem_t empty;          
sem_t full;           
pthread_mutex_t mutex;


int produced_count = 0;
int consumed_count = 0;

// Producer thread function
void* producer(void* arg) {
    int id = *((int*)arg);  

    while (1) {
       
        pthread_mutex_lock(&mutex);
        if (produced_count >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        int item = rand() % 100;  

        sem_wait(&empty);         // Wait for at least one empty slot in buffer
        pthread_mutex_lock(&mutex); // Enter critical section to access buffer

        // Add item to buffer at 'in' index
        buffer[in] = item;
        printf("Producer %d produced %d at %d\n", id, item, in);

        // Move 'in' index forward circularly
        in = (in + 1) % BUFFER_SIZE;

        produced_count++;        

        pthread_mutex_unlock(&mutex); // Leave critical section
        sem_post(&full);             // Signal that there is a new full slot

        sleep(rand() % 2);           // Simulate time taken to produce
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    int id = *((int*)arg);  // Extract consumer ID from argument

    while (1) {
        // Check if we have consumed enough items; exit if yes
        pthread_mutex_lock(&mutex);
        if (consumed_count >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        sem_wait(&full);          // Wait for at least one full slot in buffer
        pthread_mutex_lock(&mutex); // Enter critical section to access buffer

        // Remove item from buffer at 'out' index
        int item = buffer[out];
        printf("Consumer %d consumed %d at %d\n", id, item, out);

        // Move 'out' index forward circularly
        out = (out + 1) % BUFFER_SIZE;

        consumed_count++;         

        pthread_mutex_unlock(&mutex); 
        sem_post(&empty);             

        sleep(rand() % 2);            
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;
    int prod_id = 0;  
    int cons_id = 1;  

    // Initialize semaphores:
    // empty semaphore starts with BUFFER_SIZE (all slots empty initially)
    sem_init(&empty, 0, BUFFER_SIZE);

    // full semaphore starts with 0 (no items in buffer initially)
    sem_init(&full, 0, 0);

    // Initialize the mutex for protecting buffer access
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads, passing their IDs as arguments
    pthread_create(&prod_thread, NULL, producer, &prod_id);
    pthread_create(&cons_thread, NULL, consumer, &cons_id);

    
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("All items produced and consumed. Exiting.\n");
    return 0;
}


/* Output:- 

$ gcc pthread_producer_consumer_loop.c -o out
$ ./out

*/



