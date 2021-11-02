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

int is_prime(const int x);
int next_prime(int x);
static ht_item* ht_new_item(const char* k, const char* v);
static ht_hash_table* ht_new_sized(const int base_size);
ht_hash_table* ht_new();
static void ht_delete_item(ht_item* item);
void ht_del_hash_table(ht_hash_table* ht);
static int ht_hash(const char* s, const int a, const int m);
static int ht_get_hash(
    const char* s, const int num_buckets, const int attempt
);

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);
static ht_item HT_DELETED_ITEM = {NULL, NULL};
static void ht_resize(ht_hash_table* ht, const int base_size);
static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);
