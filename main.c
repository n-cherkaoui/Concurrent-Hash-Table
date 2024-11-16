#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "main.h"

// sem_t mutex;
// volatile int counter = 0;

// Function to read file.
pthread_t *readFile(FILE *outputFile)
{
    FILE *inputFile = fopen("commands.txt", "r");
    const char s[1] = ",";
    int i;
    pthread_t *threads = NULL;
    char buffer[BUFFER_SIZE];

    if (inputFile != NULL)
    {
        int threadCount = countNumThreads(inputFile, outputFile, s, buffer);
        if (threadCount == 0)
        {
            return NULL;
        }
        createThreads(inputFile, threadCount, threads, buffer);

        fclose(inputFile);
    }
    else
    {
        perror("commands.txt");
        return NULL;
    }
}

// Test23
int countNumThreads(FILE *inputFile, FILE *outputFile, char *delim, char *buffer)
{
    int threads = 0;

    if (fgets(buffer, BUFFER_SIZE, inputFile) != NULL)
    {
        strtok(buffer, delim);
        threads = atoi(strtok(NULL, delim));
    }

    sprintf(buffer, "Running %i threads\n", threads);
    fputs(buffer, outputFile);
    return threads;
}
void createThreads(FILE *inputFile, int numThreads, pthread_t *threadArray, char *buffer)
{
    while (fgets(buffer, BUFFER_SIZE, inputFile) != NULL)
    {

    }
}

int main(void)
{
    FILE *outputFile = fopen("output2.txt", "w");
    pthread_t *threads = readFile(outputFile);
    fclose(outputFile);
    return 0;
}