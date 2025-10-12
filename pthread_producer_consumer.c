#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;           // Counts empty slots
sem_t full;            // Counts filled slots
pthread_mutex_t mutex; // For mutual exclusion on buffer

void* producer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        int item = rand() % 100;  // Produce a random item

        sem_wait(&empty);          // Wait if no empty slot
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer %d produced %d\n", id, item);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);           // Signal item available

        sleep(rand() % 2);         // Simulate variable production time
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        sem_wait(&full);           // Wait if no filled slot
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumer %d consumed %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);          // Signal empty slot available

        sleep(rand() % 2);         // Simulate variable consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;
    int prod_id = 0;
    int cons_id = 1;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod_thread, NULL, producer, &prod_id);
    pthread_create(&cons_thread, NULL, consumer, &cons_id);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}



/* Output:- 

$ gcc pthread_producer_consumer.c -o out
$ ./out

*/


