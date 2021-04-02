#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *x = malloc(1);

  printf("Allocated byte at %p\n", x);

  return 0;
}
