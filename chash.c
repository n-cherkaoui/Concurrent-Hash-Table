// Locks Required

// Condition Variables
// all inserts must release their write locks before a delete starts.
// all writes must release their locks before a read can occur.

// Writing and reading cannot occur simultaneously.
// 1 read semaphore - since simultaneous reads are allowed.
// 1 write lock - only one write allowed at once due to collisions.

#include <stdint.h>

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

createHashRecord()

insert(key, values)

delete(key)

search(key)

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
printHashTable();