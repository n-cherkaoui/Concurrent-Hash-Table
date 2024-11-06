#include <stdio.h>

#ifndef MAIN_H_
#define MAIN_H_
#define INPUT_FILE "commands.txt"
#define OUTPUT_FILE "output2.txt"
FILE *inputFile;
extern FILE *outputFile;
extern pthread_mutex_t file_mutex;
#endif