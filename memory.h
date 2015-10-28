#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"
#include "error.h"
#include "ulist.h"
#include "vector.h"
#include "ptable.h"
#include "htable.h"
#include "str.h"

enum _ptr_t {PTABLE, HTABLE, ULIST, VECTOR, ARRAY, SIMPLE, STRING};

// Global program pointer table
ptable_t* ifj15_ptable;

#define ifj15_malloc1(size) _ifj15_malloc(SIMPLE, size, true)
#define ifj15_malloc2(type, size) _ifj15_malloc(type, size, true)

#define ifj15_malloc(...) VFUNC(ifj15_malloc, __VAR_ARGS__)

#define ifj15_calloc1(size) _ifj15_calloc(SIMPLE, size, true)
#define ifj15_calloc2(type, size) _ifj15_calloc(type, size, true)

#define ifj15_calloc(...) VFUNC(ifj15_calloc, __VAR_ARGS__)

#define ifj15_realloc(ptr, size) _ifj15_realloc(ptr, size, true)

void ifj15_memory_init();
void* _ifj15_malloc(ptr_t type, size_t size, bool ptable_insert_b);
void* _ifj15_realloc(void* ptr, size_t size, bool ptable_insert_b);
void* _ifj15_calloc(ptr_t type, size_t size, bool ptable_insert_b);
void ifj15_free(void* ptr);
void ifj15_free_all();
#endif
