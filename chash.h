#include <stdint.h>

#ifndef CHASH_H_
#define CHASH_H_
typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

// Instantiates hash table
void createHashRecord();

// Inserts a value into the hash table
void insert(key, values)

// Deletes entry from hash table
void delete(key)

// Searches hash table
void search(key)

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printHashTable();
#endif