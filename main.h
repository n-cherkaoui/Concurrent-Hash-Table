#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#ifndef MAIN_H_
#define MAIN_H_
#define BUFFER_SIZE 10000
pthread_t* readFile();
int countNumThreads(FILE *inputFile, FILE *outputFile, char* delim);
#endif