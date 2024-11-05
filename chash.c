#include <stdint.h>
#include "chash.h"

hashRecord createHashRecord(char *name, int salary);

hashRecord *insertHashRecord(char *key, int value);

hashRecord *deleteHashRecord(char *key);

int searchHashRecords(char *key);

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printHashTable();