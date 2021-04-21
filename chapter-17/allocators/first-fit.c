// A first-fit allocator

#include "alloc.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct free_node {
  int size;
  struct free_node *next;
  struct free_node *prev;
};

struct header {
  int size;
  int magic;
};

#define MEM_SIZE   4096
#define MAGIC_NUM  0x000a110c

static char mem[MEM_SIZE];

static struct free_node *free_list = (struct free_node *) mem;

void *mymalloc(size_t byte_count) {
  struct free_node *curr = free_list;
  int block_size = byte_count + sizeof(struct header);

  while (curr && curr->size < block_size) {
    curr = curr->next;
  }

  if (!curr) {
    return NULL;
  }

  struct header *block_header = (struct header *) curr;

  struct free_node *new_curr = (struct free_node *) ((char *) curr + block_size);
  struct free_node *curr_next = curr->next;
  struct free_node *curr_prev = curr->prev;
  new_curr->size = curr->size - block_size;

  if (curr_prev) {
    new_curr->prev = curr_prev;
    curr_prev->next = new_curr;
  } else {
    free_list = new_curr;
  }

  if (curr_next) {
    new_curr->next = curr_next;
    curr_next->prev = new_curr;
  }

  block_header->magic = MAGIC_NUM;
  block_header->size = byte_count;

  void *block_ptr = (void *) ((char *) block_header + sizeof(struct header*));

  return block_ptr;
}

void myfree(void *ptr) {
  struct header *freed_header = (struct header *) ptr - 1;

  if (freed_header->magic != MAGIC_NUM) {
    fprintf(stderr, "Cannot free invalid block\n");
    exit(1);
  }

  struct free_node *freed = (struct free_node *) freed_header;
  freed->size = freed_header->size + sizeof(struct header);

  // Walk free list, looking for opportunity to coalesce block; if no
  // such location exists, place block at the end.
  struct free_node *curr = free_list;
  bool coalesced = false;
  while (curr) {
    if ((char *) freed + freed->size == (char *) curr) {
      freed->next = curr->next;
      freed->prev = curr->prev;
      freed->size += curr->size;
      curr->prev->next = freed;
      curr->next->prev = freed;
      coalesced = true;
    } else if ((char *) curr + curr->size == (char *) freed) {
      // Freed block belongs after curr
      curr->size += freed->size;
      freed = curr;
      coalesced = true;
    }

    if (!curr->next && !coalesced) {
      // Place freed block at end
      curr->next = freed;
      freed->prev = curr;
      break;
    }

    curr = curr->next;
  }
}

void alloc_init(void) {
  free_list->size = MEM_SIZE;
  free_list->next = NULL;
  free_list->prev = NULL;
}
