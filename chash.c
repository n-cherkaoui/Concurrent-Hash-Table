#include <stdint.h>

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

hash_struct createHashRecord(char *name, int salary) {}

hash_struct *insert(char *key, int value) {}

hash_struct *delete(key)

int search(key)

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printHashTable();