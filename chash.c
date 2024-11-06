#include "chash.h"
#include "rwlock.h"
#include "main.h"
#include <string.h>
#include "timestamp.h"

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
    hashRecords = NULL;
    numRecords = 0;
}

// hashRecord createHashRecord(char *key, uint32_t value) {
//     uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key));
//     hashRecord newRecord = {hashCode, key, value, NULL};
//     return newRecord;
// }

// hashRecord *insertHashRecord(char *key, uint32_t value) {
//     hashRecord newRecord = createHashRecord(key, value);
// }

// hashRecord *deleteHashRecord(char *key);

hashRecord *searchHashRecords(char *key)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key));
    rwlock_acquire_readlock(&lock);
    hashRecord *retVal = hashRecords;

    while (retVal)
    {
        if (retVal->hash == hashCode)
        {
            break;
        }
    }

    rwlock_release_readlock(&lock);
    return retVal;
}

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
// void printHashTable();