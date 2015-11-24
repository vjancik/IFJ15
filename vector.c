#include "vector.h"

const size_t VECTOR_START_CAP = 8;

void* _vector_init(vector_item_t vi, size_t start_cap, bool ptable_insert) {
    switch(vi) {
        case VI_CHAR:
        {
            vector_char_t* vct = _ifj15_malloc(VECTOR, sizeof(vector_char_t), ptable_insert);
            vct->array = _ifj15_malloc(ARRAY, sizeof(char)*start_cap, false);
            vct->capacity = start_cap;
            vct->size = 0;
            return vct;
        }
        case VI_INT:
        {
            vector_int_t* vct = _ifj15_malloc(VECTOR, sizeof(vector_int_t), ptable_insert);
            vct->array = _ifj15_malloc(ARRAY, sizeof(int)*start_cap, false);
            vct->capacity = start_cap;
            vct->size = 0;
            return vct;
        }
        case VI_HTABLE:
        {
            vector_htable_t* vct = _ifj15_malloc(VECTOR, sizeof(vector_htable_t), ptable_insert);
            vct->array = _ifj15_malloc(ARRAY, sizeof(htable_t*)*start_cap, false);
            vct->capacity = start_cap;
            vct->size = 0;
            return vct;
        }
        case VI_TOKEN:
        {
            vector_token_t* vct = _ifj15_malloc(VECTOR, sizeof(vector_token_t), ptable_insert);
            vct->array = _ifj15_malloc(ARRAY, sizeof(token_t)*start_cap, false);
            vct->capacity = start_cap;
            vct->size = 0;
            return vct;
        }
        default:
            error("Unknown vector type", ERROR_INTERNAL);
            return NULL;
    }
}

void _vector_free(void* vector) {
    free(((vector_char_t*)vector)->array);
    free(vector);
}

#define vector_resize(type, vct, new_capacity) do {                         \
    vct->array = _ifj15_realloc(vct->array, sizeof(type)*(new_capacity),    \
                                false);                                     \
    vct->capacity = (new_capacity);                                         \
    } while(0)

void _vector_push_char(vector_char_t* vct, char c) {
    if (vct->size == vct->capacity)
        vector_resize(char, vct, vct->capacity*2);

    vct->array[(vct->size)++] = c;
}
void _vector_push_int(vector_int_t* vct, int i) {
    if (vct->size == vct->capacity)
        vector_resize(int, vct, vct->capacity*2);

    vct->array[(vct->size)++] = i;
}
void _vector_push_htable(vector_htable_t* vct, htable_t* h) {
    if (vct->size == vct->capacity)
        vector_resize(htable_t*, vct, vct->capacity*2);

    vct->array[(vct->size)++] = h;
}
void _vector_push_token(vector_token_t* vct, token_t t) {
    if (vct->size == vct->capacity)
        vector_resize(token_t, vct, vct->capacity*2);

    vct->array[(vct->size)++] = t;
}

int _vector_top_char(vector_char_t* vct, bool remove_top) {
    if (vct->size == 0) {
        //warning("vector_pop/vector_top: Tried to pop/get item from an empty vector");
        return -1;
    }
    char top_char = vct->array[vct->size - 1];
    if (remove_top) --(vct->size);
    return top_char;
}
int _vector_top_int(vector_int_t* vct, bool remove_top) {
    if (vct->size == 0) {
        warning("vector_pop/vector_top: Tried to pop/get item from an empty vector");
        return -1;
    }
    int top_int = vct->array[vct->size - 1];
    if (remove_top) --(vct->size);
    return top_int;
}
htable_t* _vector_top_htable(vector_htable_t* vct, bool remove_top) {
    if (vct->size == 0) {
        //warning("vector_pop/vector_top: Tried to pop/get item from an empty vector");
        return NULL;
    }
    htable_t* top_htable = vct->array[vct->size - 1];
    if (remove_top) --(vct->size);
    return top_htable;
}
token_t _vector_top_token(vector_token_t* vct, bool remove_top) {
    if (vct->size == 0) {
        //warning("vector_pop/vector_top: Tried to pop/get item from an empty vector");
        token_t ret = {.type = TT_NONE};
        return ret;
    }
    token_t top_token = vct->array[vct->size - 1];
    if (remove_top) --(vct->size);
    return top_token;
}

int _vector_at_char(vector_char_t* vct, size_t pos) {
    if (pos < vct->size)
        return vct->array[pos];
    warning("vector_at: Tried to access item out of range");
    return -1;
}
int _vector_at_int(vector_int_t* vct, size_t pos) {
    if (pos < vct->size)
        return vct->array[pos];
    warning("vector_at: Tried to access item out of range");
    return -1;
}
htable_t* _vector_at_htable(vector_htable_t* vct, size_t pos) {
    if (pos < vct->size)
        return vct->array[pos];
    warning("vector_at: Tried to access item out of range");
    return NULL;
}
token_t _vector_at_token(vector_token_t* vct, size_t pos) {
    if (pos < vct->size)
        return vct->array[pos];
    warning("vector_at: Tried to access item out of range");
    token_t ret = {.type = TT_NONE};
    return ret;
}

int _vector_find_char(vector_char_t* vct, vector_compare cmp) {
    int found = -1;
    for (int i = vct->size - 1; i >= 0; --i) {
        if (cmp(vct->array[i])) {
            found = vct->array[i];
            break;
        }
    }
    return found;
}

symbol_t* _vector_find_symbol(vector_htable_t* vct, str_t* name) {
    symbol_t* found = NULL;
    for (int i = vct->size - 1; i >= 0; --i) {
        found = htable_get(vct->array[i], name);
        if (found != NULL) {
            return found;
        }
    }
    return found;
}

// for pos: pos < vct->size
void _vector_insert(vector_char_t* vct, char c, size_t pos) {
    _vector_push_char(vct, vct->array[vct->size - 1]);
    for (int i = vct->size - 2; i > pos; --i)
        vct->array[i] = vct->array[i - 1];
    vct->array[pos] = c;
}

void _vector_insert_after(vector_char_t* vct, char c, vector_compare cmp) {

    for (int i = vct->size - 1; i >= 0; --i) {
        if (cmp(vct->array[i])) {
            (i == vct->size - 1) ? _vector_push_char(vct, c) : _vector_insert(vct, c, i + 1);
            break;
        }
    }
}
