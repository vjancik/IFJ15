#include "htable.h"

typedef enum {UP, DOWN} resize_t;
const size_t HTABLE_START_SIZE = 2;
// Prime numbers
//2, 5, 11, 23, 53, 97
size_t HTABLE_SIZES[30] = {2, 5, 11, 23, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
                           49157, 98317, 196613, 393241, 786433, 1572869,
                           3145739, 6291469, 12582917, 25165843, 50331653,
                           100663319, 201326611, 402653189, 805306457,
                           1610612741};

void htable_resize(htable_t* htable);
static unsigned int hash(str_t* key, size_t cap_index);

htable_t* htable_init() {
    htable_t* htable = _ifj15_calloc(HTABLE, sizeof(htable_t), true);
    htable->array = _ifj15_calloc(ARRAY, sizeof(ulist_t)*HTABLE_START_SIZE,
                                  false);
    htable->cap_index = 0;
    htable->size = 0;
    return htable;
}

void _htable_free(htable_t* htable) {
    size_t capacity = HTABLE_SIZES[htable->cap_index];
    for (size_t i = 0; i < capacity; ++i) {
        if (htable->array[i] != NULL)
            _ulist_str_free(htable->array[i]);
    }
    free(htable->array);
    free(htable);
}

void htable_set(htable_t* htable, str_t* key, symbol_t item) {
    ++(htable->size);
    if (htable->size > 5*HTABLE_SIZES[htable->cap_index])
        htable_resize(htable);
    size_t hash_code = hash(key, htable->cap_index);
    if (htable->array[hash_code] == NULL)
        htable->array[hash_code] = _ulist_str_init(false);
    ulist_set(htable->array[hash_code], key, item);
}

symbol_t* htable_get(htable_t* htable, str_t* key) {
    size_t hash_code = hash(key, htable->cap_index);
    if (htable->array[hash_code] == NULL)
        return NULL;
    return ulist_get(htable->array[hash_code], key);
}

symbol_t htable_pop(htable_t* htable, str_t* key) {
    --(htable->size);
    //if (htable->size < 2*HTABLE_SIZES[htable->cap_index] && htable->cap_index > 0)
    //    htable_resize(htable, DOWN);
    size_t hash_code = hash(key, htable->cap_index);
    if (htable->array[hash_code] == NULL)
        return INVALID_SYM;
    return ulist_pop(htable->array[hash_code], key);
}

void htable_resize(htable_t* htable) {
    size_t old_capacity = HTABLE_SIZES[htable->cap_index];
    ++htable->cap_index;

    ulist_str_t** old_array = htable->array;
    size_t capacity = HTABLE_SIZES[htable->cap_index];
    htable->array = _ifj15_calloc(ARRAY, sizeof(ulist_str_t*)*capacity, false);
    for (size_t i = 0; i < old_capacity; ++i) {
        if (old_array[i] != NULL) {
            unode_str_t* unode = old_array[i]->front;
            while (unode != NULL) {
                htable_set(htable, unode->key, unode->item);
                unode = unode->next;
            }
            _ulist_str_free(old_array[i]);
        }
    }
    free(old_array);
}

static unsigned int hash(str_t* key, size_t cap_index) {
    size_t i = 0;
    int hash = 0;
    while (key->c_str[i] != 0) {
        hash = key->c_str[i] + (31 * hash);
        ++i;
    }
    return (hash & 0x7fffffff) % HTABLE_SIZES[cap_index];
}
