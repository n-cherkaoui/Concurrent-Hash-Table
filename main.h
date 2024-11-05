#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#ifndef MAIN_H_
#define MAIN_H_
#define BUFFER_SIZE 10000
#define NAME_SIZE 50
pthread_t* readFile();
int countNumThreads(FILE *inputFile, FILE *outputFile, char *delim, char *buffer);
void createThreads(FILE *inputFile, FILE *outputFile, int numThreads, pthread_t *threadArray, char *buffer, char *delim);
#endif