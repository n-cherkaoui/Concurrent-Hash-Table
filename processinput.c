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
        char* buffer_copy = strdup(buffer);
        char* command = strtok(buffer, delim); // get the first token (command)

        if (command == NULL) {
            continue; // skip if no command found
        }

        if (strcmp(command, "search") == 0) {
            pthread_create(&threadArray[i], NULL, processSearchThread, buffer_copy);
        }
        // run insert thread
        else if (strcmp(command, "insert") == 0) {
            pthread_create(&threadArray[i], NULL, processInsertThread, buffer_copy);
        }
        // run delete thread
        else if (strcmp(command, "delete") == 0) {
            pthread_create(&threadArray[i], NULL, processDeleteThread, buffer_copy);
        } else {
            free(buffer_copy); // Free memory if no valid command is found
            continue;
        }
        i++;
    }
}

// thread function for search operation
void* processSearchThread(void* buffer)
{
    printf("processing insert thread\n");

    free(buffer);
    return NULL;
}

// thread function for insert operation
void* processInsertThread(void* buffer)
{
    // get the values from the line
    char* command = strtok(buffer, delim);
    char* name = strtok(NULL, delim);
    int salary = atoi(strtok(NULL, delim));
    printf("processing insert thread\n");

    // example of handling the result of the search
    insertHashRecord(name, salary);

    // insert logic goes here
    printf("inserting %s and salary %d\n", name, salary);

    free(buffer);
    return NULL;
}

// thread function for delete operation
void* processDeleteThread(void* buffer)
{
    printf("processing delete thread\n");

    free(buffer);
    return NULL;
    // delete logic goes here
}
