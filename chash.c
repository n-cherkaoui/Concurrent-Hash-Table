#include "chash.h"
#include "rwlock.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include "timestamp.h"

static hashRecord* hashRecords[NUM_RECORDS];
int numRecords;

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length)
{
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

void initHashRecords()
{
    for (int i = 0; i < NUM_RECORDS; i++) {
        hashRecords[i] = NULL;
    }
    numRecords = 0;
}

hashRecord createHashRecord(char* key, int value)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash((const uint8_t*)key, strlen(key));
    hashRecord newRecord;
    newRecord.hash = hashCode;
    strncpy(newRecord.name, key, NAME_SIZE - 1); // Copy key to name, ensuring no overflow
    newRecord.name[NAME_SIZE - 1] = '\0'; // Null-terminate to avoid overflow issues
    newRecord.salary = value;
    newRecord.next = NULL;
    return newRecord;
}

hashRecord* searchHashRecords(char* key)
{
    printf("retVal: %s", key);
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key)) % NUM_RECORDS;
    hashRecord* retVal = hashRecords[hashCode];

    while (retVal != NULL) {
        if (strcmp(retVal->name, key) == 0) {
            break;
        }
        // not found so go to next
        retVal = retVal->next;
    }

    return retVal;
}

void insertHashRecord(char* key, int value)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key)) % NUM_RECORDS;
    // get the lock
    // check if record exists
    hashRecord* newRecord = searchHashRecords(key);

    // if record does not exist
    if (newRecord == NULL) {
        // make a new node
        newRecord = (hashRecord*)malloc(sizeof(hashRecord));
        if (newRecord == NULL) {
            return; // allocation failed
        }
        // save the data
        *newRecord = createHashRecord(key, value);

        // add record to head of list
        newRecord->next = hashRecords[hashCode];
        hashRecords[hashCode] = newRecord;

        // awaken delete threads
        numRecords += 1;
        if (numRecords > 0) {
            signal_table_populated();
        }
    } else {
        // otherwise update the node
        newRecord->salary = value;
    }
    // release the lock
}

void deleteHashRecord(char* key)
{
    uint32_t hashCode = jenkins_one_at_a_time_hash(key, strlen(key)) % NUM_RECORDS;

    if (numRecords <= 0) {
        check_if_table_populated(&lock);
    }

    hashRecord* head = hashRecords[hashCode];
    hashRecord* temp;

    // If node is at start of list
    if (head && strcmp(head->name, key) == 0) {
        temp = head;
        hashRecords[hashCode] = head->next;
        free(temp);
        // prevents below loop from starting
        head = NULL;
    }

    // search on ahead so we can have the pointer before the target
    while (head && head->next) {
        if (strcmp(head->next->name, key) == 0) {
            temp = head->next;
            head->next = head->next->next;
            free(temp);
            break;
        }
    }
}

// prints the actual hash table items
void printHashRecords()
{
    // loop through the hash items
    for (int i = 0; i < NUM_RECORDS; i++) {
        printf("- ");
        // check for collision
        hashRecord* print = hashRecords[i];
        while (print != NULL) {
            printf("%s - ", print->name);
            print = print->next;
        }
        printf("\n");
    }
}

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printHashTable()
{
    fprintf(outputFile, "Number of lock acquisitions: %d\n", lockAcquisitions);
    fprintf(outputFile, "Number of lock releases: %d\n", lockReleases);
}