#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BUFFER 10

int buffer[MAX_BUFFER];
int in = 0;   // Index for next produced item
int out = 0;  // Index for next consumed item
int count = 0; // Optional: count of items in buffer

sem_t empty;   // Counts empty slots
sem_t full;    // Counts full slots
pthread_mutex_t mutex;

int total_produce, total_consume;

void *producer(void *arg) {
    int produced = 0;
    while (produced < total_produce) {
        int item = rand() % 1000;  // Produce a random item

        sem_wait(&empty);          // Wait for empty slot
        pthread_mutex_lock(&mutex);

        // Add item to buffer
        buffer[in] = item;
        in = (in + 1) % MAX_BUFFER;
        count++;
        printf("Produced: %d (buffer count: %d)\n", item, count);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);           // Signal full slot available

        produced++;
        usleep(100000); // Sleep for 0.1 sec to slow down for demonstration
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int consumed = 0;
    while (consumed < total_consume) {
        sem_wait(&full);           // Wait for full slot
        pthread_mutex_lock(&mutex);

        // Remove item from buffer
        int item = buffer[out];
        out = (out + 1) % MAX_BUFFER;
        count--;
        printf("\tConsumed: %d (buffer count: %d)\n", item, count);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);          // Signal empty slot available

        consumed++;
        usleep(150000); // Sleep for 0.15 sec to slow down
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Input validation for production and consumption counts
    printf("Enter total number of items to produce: ");
    while (scanf("%d", &total_produce) != 1 || total_produce <= 0) {
        printf("Invalid input. Enter a positive integer: ");
        while (getchar() != '\n'); // clear buffer
    }

    printf("Enter total number of items to consume: ");
    while (scanf("%d", &total_consume) != 1 || total_consume <= 0 || total_consume > total_produce) {
        printf("Invalid input. Enter a positive integer less than or equal to produced count: ");
        while (getchar() != '\n'); // clear buffer
    }

    // Initialize semaphores and mutex
    if (sem_init(&empty, 0, MAX_BUFFER) != 0) {
        perror("sem_init empty failed");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&full, 0, 0) != 0) {
        perror("sem_init full failed");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("mutex init failed");
        exit(EXIT_FAILURE);
    }

    // Create producer and consumer threads
    if (pthread_create(&prod_thread, NULL, producer, NULL) != 0) {
        perror("Failed to create producer thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&cons_thread, NULL, consumer, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("All items produced and consumed successfully.\n");
    return 0;
}

