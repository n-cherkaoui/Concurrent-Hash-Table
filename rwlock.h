#include <semaphore.h>

#ifndef RWLOCK_H_
#define RWLOCK_H_
typedef struct _rwlock_t
{
    sem_t lock;     
    sem_t writelock;
    sem_t readers_lock;
    int readers;   
    
} rwlock_t;

// locks can be called by any file
extern rwlock_t lock;
extern int lockAcquisitions;
extern int lockReleases;
extern pthread_cond_t cond;

void rwlock_init(rwlock_t *rw);
void rwlock_acquire_readlock(rwlock_t *rw);
void rwlock_release_readlock(rwlock_t *rw);
void rwlock_acquire_writelock(rwlock_t *rw);
void rwlock_release_writelock(rwlock_t *rw);
void signal_if_table_populated();
void check_if_table_populated(rwlock_t *rw, int numRecords);
#endif