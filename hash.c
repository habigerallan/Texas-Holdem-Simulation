#include "hash.h"
#include <stdlib.h>
#include <stdio.h>

static int hash(int key, int size) {
    return key % size;
}

HashTable *createHashTable(int size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->size = size;
    table->buckets = malloc(sizeof(HashEntry *) * size);
    for (int i = 0; i < size; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

void freeHashTable(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        HashEntry *entry = table->buckets[i];
        while (entry != NULL) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

void insert(HashTable *table, int key, int value) {
    int bucket = hash(key, table->size);
    HashEntry *newEntry = malloc(sizeof(HashEntry));
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = table->buckets[bucket];
    table->buckets[bucket] = newEntry;
}

HashEntry *search(HashTable *table, int key) {
    int bucket = hash(key, table->size);
    HashEntry *entry = table->buckets[bucket];
    while (entry != NULL) {
        if (entry->key == key) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

void delete(HashTable *table, int key) {
    int bucket = hash(key, table->size);
    HashEntry *entry = table->buckets[bucket];
    HashEntry *prev = NULL;
    while (entry != NULL) {
        if (entry->key == key) {
            if (prev == NULL) {
                table->buckets[bucket] = entry->next;
            } else {
                prev->next = entry->next;
            }
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}
