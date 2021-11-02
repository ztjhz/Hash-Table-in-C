// hash table items
typedef struct {
    char* key;
    char* value;
} ht_item;

// hash table
typedef struct {
    int size;
    int base_size;
    int count;
    ht_item** items;
} ht_hash_table;

#define HT_PRIME_1 131
#define HT_PRIME_2 137
#define HT_INITIAL_BASE_SIZE 53

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);

static ht_item HT_DELETED_ITEM = {NULL, NULL};