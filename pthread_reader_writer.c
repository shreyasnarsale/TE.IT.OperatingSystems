#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int produced_count = 0;


int shared_data = 0;       // Shared resource
int read_count = 0;        // Number of active readers

pthread_mutex_t mutex;      // Mutex to protect read_count
pthread_mutex_t write_mutex; // Mutex to allow writers exclusive access

void* reader(void* arg) {
    int id = *((int*)arg);

    while (1) {
        // Reader entry section
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1) {
            // First reader locks the write_mutex to block writers
            pthread_mutex_lock(&write_mutex);
        }
        pthread_mutex_unlock(&mutex);

        // Critical section: Reading shared data
        printf("Reader %d is reading data = %d\n", id, shared_data);
        sleep(rand() % 3);  // Simulate reading time

        // Reader exit section
        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            // Last reader unlocks write_mutex to allow writers
            pthread_mutex_unlock(&write_mutex);
        }
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3);  // Wait before next read
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&write_mutex);  // Lock for exclusive writing

        // Critical section: Writing to shared data
        shared_data = rand() % 1000;
        printf("Writer %d is writing data = %d\n", id, shared_data);
        sleep(rand() % 3);  // Simulate writing time

        pthread_mutex_unlock(&write_mutex);  // Unlock to allow others

        sleep(rand() % 3);  // Wait before next write
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join threads (program runs infinitely, so this is optional)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);

    return 0;
}


/* Output:-

$ gcc pthread_reader_writer.c -o out
$ ./out

*/


