#include <pthread.h>
#ifndef createThreads_H_

#define createThreads_H_
#define BUFFER_SIZE 256
pthread_t *createThreads(int threadCount);
int countNumThreads();
void startThreads(pthread_t *threadArray, int threadCount);
void *processSearchThread(void *key);
void *processInsertThread(void *key);
void *processDeleteThread(void *key);
#endif
