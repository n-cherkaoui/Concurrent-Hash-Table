#include "rwlock.h"
#include "processinput.h"
#include <pthread.h>
#include "chash.h"
#include <stdbool.h>
#include "timestamp.h"

rwlock_t lock;
pthread_cond_t cond;
int lockAcquisitions;
int lockReleases;

void rwlock_init(rwlock_t *rw)
{
    rw->readers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    sem_init(&rw->readers_lock, 0, 1); // New semaphore for readers synchronization
    pthread_cond_init(&cond, NULL);
    lockAcquisitions = 0;
    lockReleases = 0;
}

void rwlock_acquire_readlock(rwlock_t *rw)
{
    sem_wait(&rw->readers_lock); // Lock the readers count
    rw->readers++;
    if (rw->readers == 1)
    {
        sem_wait(&rw->writelock); // First reader locks the write lock
    }
    sem_post(&rw->readers_lock); // Release the readers count lock

    // Log the acquisition
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: READ LOCK ACQUIRED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);

    sem_wait(&rw->lock);
    lockAcquisitions++;
    sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw)
{
    sem_wait(&rw->readers_lock); // Lock the readers count
    rw->readers--;
    if (rw->readers == 0)
    {
        sem_post(&rw->writelock); // Last reader releases the write lock
    }
    sem_post(&rw->readers_lock); // Release the readers count lock

    // Log the release
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: READ LOCK RELEASED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);

    sem_wait(&rw->lock);
    lockReleases++;
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw)
{
    sem_wait(&rw->writelock); // Acquire write lock

    // Log the acquisition
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: WRITE LOCK ACQUIRED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);

    sem_wait(&rw->lock);
    lockAcquisitions++;
    sem_post(&rw->lock);
}

void rwlock_release_writelock(rwlock_t *rw)
{
    sem_post(&rw->writelock); // Release write lock

    // Log the release
    pthread_mutex_lock(&file_mutex);
    fprintf(outputFile, "%lld: WRITE LOCK RELEASED\n", get_microsecond_timestamp());
    pthread_mutex_unlock(&file_mutex);

    sem_wait(&rw->lock);
    lockReleases++;
    sem_post(&rw->lock);
}

void signal_if_table_populated()
{
    pthread_cond_broadcast(&cond);
}

void check_if_table_populated(rwlock_t *rw, int numRecords)
{
    pthread_mutex_lock(&cond_mutex);
    while (numRecords <= 0)
    {
        // printf("waiting on inserts\n");
        fprintf(outputFile, "%lld: WAITING ON INSERTS\n", get_microsecond_timestamp());
        pthread_cond_wait(&cond, &cond_mutex);
        // printf("delete awakened\n");
        fprintf(outputFile, "%lld: DELETE AWAKENED\n", get_microsecond_timestamp());
    }
    pthread_mutex_unlock(&cond_mutex);
}

// TODO: create method to destroy locks and condition variables
