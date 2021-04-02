#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <amt>\n\tWhere <amt> is the amount of memory to request, in MiB\n", argv[0]);
    exit(1);
  }

  int amt = atoi(argv[1]);
  if (amt == 0) {
    fprintf(stderr, "<amt> must be a positive number\n");
    exit(1);
  }

  printf("pid = %d\n\n", getpid());

  printf("Requesting %d MiB via `malloc()`\n", amt);
  int total_bytes = 1024 * 1024 * amt;
  char *bytes = malloc(total_bytes * sizeof(char));

  if (bytes == NULL) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(1);
  }

  printf("Continuously iterating through allocated array...\n");
  while (true) {
    for (int i = 0; i < total_bytes; i++) {
      bytes[i] = 0;
    }

    for (int i = 0; i < total_bytes; i++) {
      bytes[i] = 1;
    }
  }

  return 0;
}
