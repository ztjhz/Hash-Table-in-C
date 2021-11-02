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

// double hashing makes use of two hash functions to calculate the index of an item to be stored after a collision
static int ht_get_hash(
    const char* s, const int num_buckets, const int attempt
) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* curr_item = ht->items[0];
    int i = 1;
    while (curr_item != NULL) {
        index = ht_get_hash(item->key, ht->size, i);
        curr_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

