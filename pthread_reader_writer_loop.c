#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 20   // Maximum items to produce/consume

int buffer[BUFFER_SIZE];
int in = 0;   // Next insertion index
int out = 0;  // Next removal index

sem_t empty;          // Semaphore counting empty slots
sem_t full;           // Semaphore counting filled slots
pthread_mutex_t mutex; // Mutex for mutual exclusion

int produced_count = 0;
int consumed_count = 0;

void* producer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (produced_count >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;  // Produced enough items, exit loop
        }
        pthread_mutex_unlock(&mutex);

        int item = rand() % 100;

        sem_wait(&empty);         // Wait for empty slot
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer %d produced %d at %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        produced_count++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);          // Signal new full slot

        sleep(rand() % 2);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (consumed_count >= MAX_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;  // Consumed enough items, exit loop
        }
        pthread_mutex_unlock(&mutex);

        sem_wait(&full);          // Wait for full slot
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumer %d consumed %d at %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        consumed_count++;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);         // Signal new empty slot

        sleep(rand() % 2);
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

    printf("All items produced and consumed. Exiting.\n");
    return 0;
}




/* Output: 

$ gcc pthread_reader_writer_loop.c -o out
$ ./out

*/


