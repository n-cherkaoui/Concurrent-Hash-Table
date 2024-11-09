#include "processinput.h"
#include "chash.h"
#include "main.h"
#include "timestamp.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char buffer[BUFFER_SIZE];
const char delim[2] = ",";

pthread_t* createThreads(int threadCount)
{
    // dynamic array of threads
    pthread_t* threads = NULL;

    if (inputFile != NULL) {
        if (threadCount == 0) {
            return NULL;
        }
        threads = calloc(threadCount, sizeof(pthread_t));
        startThreads(threads, threadCount);
    } else {
        perror("commands.txt");
        return NULL;
    }

    return threads;
}

int countNumThreads()
{
    int threads = 0;

    if (fgets(buffer, BUFFER_SIZE, inputFile) != NULL) {
        strtok(buffer, delim);
        threads = atoi(strtok(NULL, delim));
    }

    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "Running %i threads\n", threads);
    pthread_mutex_unlock(&file_mutex);

    return threads;
}

void startThreads(pthread_t* threadArray, int threadCount)
{
    int i = 0;
    char buffer[BUFFER_SIZE]; // buffer to hold each line from the input file

    // convert lines from input file into threads
    while (fgets(buffer, BUFFER_SIZE, inputFile) != NULL && i < threadCount) {
        char* command = strtok(buffer, delim); // get the first token (command)

        if (command == NULL) {
            continue; // skip if no command found
        }

        // run search thread
        if (strcmp(command, "search") == 0) {
            char* arg = strdup(strtok(NULL, delim)); // duplicate argument for thread
            if (arg != NULL) {
                pthread_create(&threadArray[i], NULL, processSearchThread, arg);
                i++;
            }
        }
        // run insert thread
        else if (strcmp(command, "insert") == 0) {
            char* arg = strdup(strtok(NULL, delim)); // duplicate argument for thread
            if (arg != NULL) {
                pthread_create(&threadArray[i], NULL, processInsertThread, arg);
                i++;
            }
        }
        // run delete thread
        else if (strcmp(command, "delete") == 0) {
            char* arg = strdup(strtok(NULL, delim)); // duplicate argument for thread
            if (arg != NULL) {
                pthread_create(&threadArray[i], NULL, processDeleteThread, arg);
                i++;
            }
        }
    }
}

// thread function for search operation
void* processSearchThread(void* key)
{
    printf("processing search thread\n");
    hashRecord* record = searchHashRecords((char*)key);

    // example of handling the result of the search
    if (record == NULL) {
        printf("record not found for key: %s\n", (char*)key);
    } else {
        printf("record found for key: %s\n", (char*)key);
    }

    free(key);
    return NULL;
}

// thread function for insert operation
void* processInsertThread(void* key)
{
    printf("processing insert thread\n");
    // search to see if key exists already
    hashRecord* record = searchHashRecords((char*)key);

    // example of handling the result of the search
    if (record == NULL) {
        // TODO: implement insert

    } else {
        // key found so update struct
        
    }

    // insert logic goes here
    printf("inserting record with key: %s\n", (char*)key);

    free(key);
    return NULL;
}

// thread function for delete operation
void* processDeleteThread(void* key)
{
    printf("processing delete thread\n");

    // delete logic goes here
    printf("deleting record with key: %s\n", (char*)key);

    free(key);
    return NULL;
}