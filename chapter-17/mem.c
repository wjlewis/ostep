#include <stdio.h>
#include <sys/mman.h>

#define HEAPSIZE (4096)

typedef struct __node_t {
  int              size;
  struct __node_t *next;
} node_t;

void init_heap() {
  node_t *head = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE,
                      MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
  head->size = HEAPSIZE - sizeof(node_t);
  head->next = NULL;
}
