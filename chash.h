#include <stdint.h>
#include <stdio.h>

#ifndef CHASH_H_
#define CHASH_H_
#define NAME_SIZE 50
typedef struct hash_struct
{
  uint32_t hash;
  char name[NAME_SIZE];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

uint32_t jenkins_one_at_a_time_hash(const uint8_t *key, size_t length);

void initHashRecords();

hashRecord createHashRecord(char *key, int value);

hashRecord *insertHashRecord(char *key, int value);

hashRecord *deleteHashRecord(char *key);

hashRecord *searchHashRecords(char *key);

void printHashTable();
#endif