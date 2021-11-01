#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

// create a new hash table item
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* new_item = malloc(sizeof(ht_item));
    new_item->key = strdup(k);
    new_item->value = strdup(v);
    return new_item;
}

// create a new hash table
ht_hash_table* ht_new() {
    ht_hash_table* new_ht = malloc(sizeof(ht_hash_table));

    new_ht->size = 53;
    new_ht->count = 0;
    new_ht->items = calloc((size_t) new_ht->size, sizeof(ht_item*)); //calloc fills allocated memory with NULL bytes
    return new_ht;
}

// free up allocated memory for hash table items
static void ht_delete_item(ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

// free up allocated memory for hash table
void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (ht_item != NULL) {
            ht_delete_item(ht_item);
        }
    }
    free(ht->items);
    free(ht);
}

/* This hash function has two steps:
Convert the string to a large integer
Reduce the size of the integer to a fixed range by taking its remainder mod m
The variable a should be a prime number larger than the size of the alphabet. 
We're hashing ASCII strings, which has an alphabet size of 128, 
so we should choose a prime larger than that
*/

static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long) pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int) hash;
}