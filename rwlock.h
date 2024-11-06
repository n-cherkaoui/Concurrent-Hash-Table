#include <semaphore.h>

#ifndef RWLOCK_H_
#define RWLOCK_H_
typedef struct _rwlock_t
{
    sem_t lock;     
    sem_t writelock;
    int readers;   
    int lockAcquisitions;
    int lockReleases;
} rwlock_t;

rwlock_t lock;
pthread_cond_t cond;

void rwlock_init(rwlock_t *rw);
void rwlock_acquire_readlock(rwlock_t *rw);
void rwlock_release_readlock(rwlock_t *rw);
void rwlock_acquire_writelock(rwlock_t *rw);
void rwlock_release_writelock(rwlock_t *rw);
#endif