#include "processinput.h"
#include "main.h"
#include "chash.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "timestamp.h"

const char delim[2] = ",";

pthread_t *createThreads(int threadCount)
{
    pthread_t *threads = NULL;

    if (inputFile != NULL)
    {
        if (threadCount == 0)
        {
            return NULL;
        }
        threads = calloc(threadCount, sizeof(pthread_t));
        startThreads(threads);
    }
    else
    {
        perror("commands.txt");
        return NULL;
    }

    return threads;
}

int countNumThreads()
{
    int threads = 0;

    if (fgets(buffer, BUFFER_SIZE, inputFile) != NULL)
    {
        strtok(buffer, delim);
        threads = atoi(strtok(NULL, delim));
    }

    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "Running %i threads\n", threads);
    pthread_mutex_unlock(&file_mutex);
    
    return threads;
}

void startThreads(pthread_t *threadArray)
{
    int i = 0;

    while (fgets(buffer, BUFFER_SIZE, inputFile) != NULL)
    {
        if (strcmp(strtok(buffer, delim), "search") == 0)
        {
            char *arg = strdup(strtok(NULL, delim));
            pthread_create(&threadArray[i], NULL, processSearchThread, arg);
        }
        i += 1;
    }
}

void *processSearchThread(void *key) {
    printf("processing search thread");
    hashRecord *record = searchHashRecords(key);
    // if (record == NULL) {
    //     fprintf(outputFile, "%lld: SEARCH: NOT FOUND NOT FOUND", get_microsecond_timestamp());
        
    // }
    free(key);

}