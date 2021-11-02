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
static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;

    ht->size = next_prime(ht->base_size);

    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}


ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
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
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }

    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* curr_item = ht->items[0];
    int i = 1;
    while (curr_item != NULL && curr_item != &HT_DELETED_ITEM) {
        // for same key
        if (strcmp(curr_item->key, key) == 0) {
            ht_delete_item(curr_item);
            ht->items[index] = item;
            return;

        }
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
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

/* 
Deleting from an open addressed hash table is more complicated than inserting or searching. 
The item we wish to delete may be part of a collision chain. 
Removing it from the table will break that chain, and will make finding items in the tail of the chain impossible. 
To solve this, instead of deleting the item, we simply mark it as deleted.

We mark an item as deleted by replacing it with a pointer to a global sentinel item which represents 
that a bucket contains a deleted item. */

void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }

    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 0;

    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_delete_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}


static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}