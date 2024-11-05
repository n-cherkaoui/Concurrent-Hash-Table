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

hashRecord createHashRecord(char *name, int salary);

hashRecord *insertHashRecord(char *key, int value);

hashRecord *deleteHashRecord(char *key);

int searchHashRecords(char *key);

void printHashTable();
#endif