#include "rwlock.h"
#include "processinput.h"
#include "main.h"
#include <pthread.h>
#include "timestamp.h"

void rwlock_init(rwlock_t *rw)
{
    rw->readers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    pthread_cond_init(&cond, NULL);
}

void rwlock_acquire_readlock(rwlock_t *rw)
{
    sem_wait(&rw->lock);
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: READ LOCK ACQUIRED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);
    
    rw->readers++;
    rw->lockAcquisitions++;
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
    rw->lockReleases++;
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
