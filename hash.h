#ifndef HASH_H
#define HASH_H

typedef struct HashEntry {
    int key;
    int value;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry **buckets;
    int size;
} HashTable;

HashTable *createHashTable(int size);
void freeHashTable(HashTable *table);
void insert(HashTable *table, int key, int value);
HashEntry *search(HashTable *table, int key);
void delete(HashTable *table, int key);

#endif