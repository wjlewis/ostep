#ifndef __alloc_h__
#define __alloc_h__

#include <stddef.h>

void alloc_init(void);

void *mymalloc(size_t byte_count);

void myfree(void *ptr);

#endif
