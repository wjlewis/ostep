#ifndef __vector_h__
#define __vector_h__

struct vector;

struct vector *vector_new(void);

struct vector *vector_with_cap(int cap);

void vector_free(struct vector *);

void vector_push(struct vector *, void *x);

void *vector_pop(struct vector *);

int vector_len(struct vector *);

int vector_cap(struct vector *);

#endif
