#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int x = 0;

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (pid == 0) {
    printf("Child assigning `x` to `42`\n");
    x = 42;
  } else {
    printf("Parent assigning `x` to `561`\n");
    x = 561;
  }

  printf("The value of `x` is %d\n", x);
  return 0;
}
