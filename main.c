#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "main.h"
#include "rwlock.h"
#include "chash.h"
#include "processinput.h"

FILE *outputFile;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
    outputFile = fopen("output2.txt", "w");
    inputFile = fopen("commands.txt", "r");
    pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
    rwlock_init(&lock);
    initHashRecords();

    int numThreads = countNumThreads();
    pthread_t *threads = createThreads(numThreads);
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL); // Wait for each thread to finish
    }

    printHashTable();

    free(threads);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}