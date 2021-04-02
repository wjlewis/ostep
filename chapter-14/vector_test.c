#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int main(void) {
  struct vector *v = vector_with_cap(4);

  printf("len: %d; cap: %d\n", vector_len(v), vector_cap(v));

  int *a = malloc(sizeof(int));
  int *b = malloc(sizeof(int));
  int *c = malloc(sizeof(int));
  int *d = malloc(sizeof(int));
  int *e = malloc(sizeof(int));

  *a = 3;
  *b = 10;
  *c = 22;
  *d = 42;
  *e = 4;

  vector_push(v, (void *) a);

  printf("len: %d; cap: %d\n", vector_len(v), vector_cap(v));

  vector_push(v, (void *) b);
  vector_push(v, (void *) c);
  vector_push(v, (void *) d);

  printf("len: %d; cap: %d\n", vector_len(v), vector_cap(v));

  vector_push(v, (void *) e);

  printf("len: %d; cap: %d\n", vector_len(v), vector_cap(v));

  printf("%d\n", *(int *) vector_pop(v));
  printf("%d\n", *(int *) vector_pop(v));
  printf("%d\n", *(int *) vector_pop(v));

  vector_free(v);

  return 0;
}
