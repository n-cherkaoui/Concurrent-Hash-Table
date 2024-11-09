#include "chash.h"
#include "rwlock.h"
#include "main.h"
#include <string.h>
#include "timestamp.h"

hashRecord* hashRecordsHead; // head of the records
hashRecord* hashRecordsLast;
int numRecords;

uint32_t jenkins_one_at_a_time_hash(const uint8_t *key, size_t length)
{
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length)
    {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

void initHashRecords() {
    hashRecordsHead = NULL;
    numRecords = 0;
}

hashRecord createHashRecord(char *key, int value) {
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key));
    hashRecord newRecord = {hashCode, key, value, NULL};
    return newRecord;
}

hashRecord *searchHashRecords(char *key)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key));
    rwlock_acquire_readlock(&lock);
    hashRecord *retVal = hashRecordsHead;

    while (retVal != NULL)
    {
        if (retVal->hash == hashCode)
        {
            break;
        }
        // not found so go to next
        retVal = retVal->next;
    }

    rwlock_release_readlock(&lock);
    return retVal;
}

hashRecord *insertHashRecord(char *key, int value) {
    // get the lock
    rwlock_acquire_readlock(&lock);
    // make a new node
    hashRecord* newRecord = (hashRecord*)malloc(sizeof(hashRecord));
    // save the data 
    *newRecord = createHashRecord(key, value);
    // add to the end
    hashRecordsLast->next = newRecord;
    // save the new last
    hashRecordsLast = newRecord;
    // release the lock
    rwlock_release_readlock(&lock);
}


hashRecord *deleteHashRecord(char *key) {
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key));
    rwlock_acquire_readlock(&lock);
    hashRecord *retVal = hashRecordsHead;

    // search on ahead so we can have the pointer before the target 
    while (retVal != NULL)
    {
        if (retVal->next->hash == hashCode)
        {
            break;
        }
        // not found so go to next
        retVal = retVal->next;
    }
}



// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printHashTable(){
    fprintf(outputFile, "Number of lock acquisitions: %d\n", lockAcquisitions);
    fprintf(outputFile, "Number of lock releases: %d\n", lockReleases);
}