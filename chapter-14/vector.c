#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

struct vector {
  int len;
  int cap;
  void **data;
};

struct vector *vector_new(void) {
  struct vector *v = malloc(sizeof(v));

  if (v == NULL) {
    fprintf(stderr, "vector_new: Unable to allocate vector\n");
    exit(1);
  }

  v->len = 0;
  v->cap = 0;

  return v;
}

struct vector *vector_with_cap(int cap) {
  struct vector *v = vector_new();

  if (v == NULL) {
    fprintf(stderr, "vector_with_cap: Unable to allocate vector\n");
    exit(1);
  }

  void **data = malloc(cap * sizeof(void *));
  if (data == NULL) {
    vector_free(v);
    fprintf(stderr, "vector_with_cap: Unable to allocate space for data\n");
    exit(1);
  }

  v->data = data;
  v->cap = cap;

  return v;
}

// Add `datum` to the end of `v`. Grows the vector's `data` array if
// necessary.
void vector_push(struct vector *v, void *datum) {
  if (v->cap <= v->len) {
    // Increase capacity by a factor of 2
    void **new_data = realloc(v->data, v->cap * 2 * sizeof(void *));
    if (new_data == NULL) {
      vector_free(v);
      fprintf(stderr, "vector_push: Unable to grow vector's capacity\n");
      exit(1);
    }
    v->data = new_data;
    v->cap = 2 * v->cap;
  }

  v->data[v->len] = datum;
  v->len++;
}

void *vector_pop(struct vector *v) {
  if (v->len == 0) {
    fprintf(stderr, "vector_pop: Attempt to pop empty vector\n");
    vector_free(v);
    exit(0);
  }

  void *out = v->data[v->len-1];
  v->len--;

  return out;
}

int vector_len(struct vector *v) {
  return v->len;
}

int vector_cap(struct vector *v) {
  return v->cap;
}

// A vector's data is "moved" into it, so the vector is responsible for
// freeing it.
void vector_free(struct vector *v) {
  for (int i = 0; i < v->len; i++) {
    free(v->data[i]);
  }

  free(v);
}
