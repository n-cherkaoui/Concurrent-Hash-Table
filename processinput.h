#include <pthread.h>
#ifndef createThreads_H_
#define createThreads_H_
#define BUFFER_SIZE 10000
pthread_t *createThreads(int threadCount);
int countNumThreads();
void startThreads(pthread_t *threadArray);
void *processSearchThread(void *key);
char buffer[BUFFER_SIZE];
#endif
