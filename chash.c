#include "chash.h"
#include "rwlock.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "timestamp.h"

static hashRecord *hashRecords[NUM_RECORDS];
int numInserts;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;

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

void initHashRecords(int insertThreads)
{
    for (int i = 0; i < NUM_RECORDS; i++)
    {
        hashRecords[i] = NULL;
    }
    numInserts = insertThreads;
}

hashRecord createHashRecord(char *key, int value)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash((const uint8_t *)key, strlen(key));
    hashRecord newRecord;
    newRecord.hash = hashCode;
    strncpy(newRecord.name, key, NAME_SIZE - 1); // Copy key to name, ensuring no overflow
    newRecord.name[NAME_SIZE - 1] = '\0';        // Null-terminate to avoid overflow issues
    newRecord.salary = value;
    newRecord.next = NULL;
    return newRecord;
}

// Used in insert and delete to search the hash table without reacquiring the write lock
hashRecord *searchHashRecordHelper(char *key)
{
    printf("retVal: %s", key);
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key)) % NUM_RECORDS;
    hashRecord *retVal = hashRecords[hashCode];

    while (retVal != NULL)
    {
        if (strcmp(retVal->name, key) == 0)
        {
            break;
        }
        // not found so go to next
        retVal = retVal->next;
    }

    return retVal;
}

hashRecord *searchHashRecords(char *key)
{
    rwlock_acquire_readlock(&lock);
    hashRecord *retVal = searchHashRecordHelper(key);
    // TODO: Check the result of search and print to file
    if (retVal == NULL)
        fprintf(outputFile, "%lld: SEARCH: NOT FOUND NOT FOUND\n", get_microsecond_timestamp());
    else
        fprintf(outputFile, "%lld: SEARCH,%s\n", get_microsecond_timestamp(), retVal->name);
    rwlock_release_readlock(&lock);
    return retVal;
}

void insertHashRecord(char *key, int value)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key)) % NUM_RECORDS;

    rwlock_acquire_writelock(&lock); // Lock for hash table operations

    hashRecord *newRecord = searchHashRecordHelper(key);

    if (newRecord == NULL)
    {
        newRecord = (hashRecord *)malloc(sizeof(hashRecord));
        if (newRecord == NULL)
        {
            rwlock_release_writelock(&lock);
            return; // allocation failed
        }

        *newRecord = createHashRecord(key, value);
        newRecord->next = hashRecords[hashCode];
        hashRecords[hashCode] = newRecord;

        fprintf(outputFile, "%lld: INSERT,%u,%s,%u\n", get_microsecond_timestamp(), newRecord->hash, newRecord->name, newRecord->salary);

        // Update numInserts and signal within the same lock to ensure atomicity
        pthread_mutex_lock(&cond_mutex);
        numInserts -= 1;
        if (numInserts == 0)
            pthread_cond_broadcast(&cond); // Signal if table populated
        pthread_mutex_unlock(&cond_mutex);
    }
    else
    {
        newRecord->salary = value;
    }

    rwlock_release_writelock(&lock); // Release hash table lock
}

void deleteHashRecord(char *key)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key)) % NUM_RECORDS;

    // TODO: Move this logic to rwlock.c
    pthread_mutex_lock(&cond_mutex);
    while (numInserts > 0)
    {
        fprintf(outputFile, "%lld: WAITING ON INSERTS\n", get_microsecond_timestamp());
        pthread_cond_wait(&cond, &cond_mutex); // Wait for an item to be added
        fprintf(outputFile, "%lld: DELETE AWAKENED\n", get_microsecond_timestamp());
    }
    pthread_mutex_unlock(&cond_mutex);

    rwlock_acquire_writelock(&lock); // Lock for hash table operations

    hashRecord *head = hashRecords[hashCode];
    hashRecord *temp;

    if (head && strcmp(head->name, key) == 0)
    {
        temp = head;
        hashRecords[hashCode] = head->next;

        fprintf(outputFile, "%lld: DELETE,%s\n", get_microsecond_timestamp(), temp->name);

        free(temp);
        head = NULL;
    }

    while (head && head->next)
    {
        if (strcmp(head->next->name, key) == 0)
        {
            temp = head->next;
            head->next = head->next->next;

            fprintf(outputFile, "%lld: DELETE,%s\n", get_microsecond_timestamp(), temp->name);

            free(temp);
            break;
        }
    }

    rwlock_release_writelock(&lock); // Release hash table lock
}

// Function to compare two hashRecord pointers for sorting
int compareHashRecords(const void *a, const void *b)
{
    hashRecord *recordA = *(hashRecord **)a;
    hashRecord *recordB = *(hashRecord **)b;
    return (recordA->hash > recordB->hash) - (recordA->hash < recordB->hash);
}

void printHashRecords()
{
    // Step 1: Collect all hash records in a dynamic array
    hashRecord **recordList = NULL;
    int recordCount = 0;

    // Loop through the hash items and collect all nodes
    for (int i = 0; i < NUM_RECORDS; i++)
    {
        hashRecord *current = hashRecords[i];
        while (current != NULL)
        {
            recordCount++;
            recordList = realloc(recordList, recordCount * sizeof(hashRecord *));
            recordList[recordCount - 1] = current;
            current = current->next;
        }
    }

    // Step 2: Sort the list of records by their hash value
    qsort(recordList, recordCount, sizeof(hashRecord *), compareHashRecords);

    // Step 3: Print the sorted records
    for (int i = 0; i < recordCount; i++)
    {
        fprintf(outputFile, "%u,%s,%u\n", recordList[i]->hash, recordList[i]->name, recordList[i]->salary);
    }

    // Free the temporary list
    free(recordList);
}

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printLockData()
{
    fprintf(outputFile, "Finished all threads.\n");
    fprintf(outputFile, "Number of lock acquisitions: %d\n", lockAcquisitions);
    fprintf(outputFile, "Number of lock releases: %d\n", lockReleases);
}