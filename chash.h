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

void createHashRecord();

void insert(key, values)

void delete(key)

// Searches hash table
void search(key)

// Prints the number of lock acquisitions and releases
// Prints the hash table sorted by hash value
void printHashTable();
#endif