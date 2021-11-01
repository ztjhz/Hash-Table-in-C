// hash table items
typedef struct {
    char* key;
    char* value;
} ht_item;

// hash table
typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

