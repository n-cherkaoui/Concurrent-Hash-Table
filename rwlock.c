#include "rwlock.h"
#include "processinput.h"
#include "main.h"
#include <pthread.h>
#include "timestamp.h"

rwlock_t lock;
pthread_cond_t cond;
int lockAcquisitions;
int lockReleases;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;

void rwlock_init(rwlock_t *rw)
{
    rw->readers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    pthread_cond_init(&cond, NULL);
    lockAcquisitions = 0;
    lockReleases = 0;
}

void rwlock_acquire_readlock(rwlock_t *rw)
{
    sem_wait(&rw->lock);
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: READ LOCK ACQUIRED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);
    
    rw->readers++;
    lockAcquisitions++;
    if (rw->readers == 1) // first reader gets writelock
        sem_wait(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw)
{
    sem_wait(&rw->lock);
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: READ LOCK RELEASED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);
    
    rw->readers--;
    lockReleases++;
    if (rw->readers == 0) // last reader lets it go
        sem_post(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw)
{
    sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw)
{
    sem_post(&rw->writelock);
}

void signal_table_populated() {
    pthread_cond_signal(&cond);
}

void check_if_table_populated(rwlock_t *rw) {
    pthread_mutex_lock(&cond_mutex);
    pthread_cond_wait(&cond, &cond_mutex);
    pthread_mutex_unlock(&cond_mutex);
}