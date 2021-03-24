#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int fib(int);

// What happens when a parent and child write to a file descriptor
// concurrently?
int main(void) {
  int fd = open("ex2.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

  if (fd < 0) {
    fprintf(stderr, "Failed to open file\n");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Failed to fork\n");
    exit(1);
  } else if (pid == 0) {
    dprintf(fd, "This is the first line from the child\n");
    dprintf(fd, "This is another line from the child\n");
  } else {
    dprintf(fd, "This is the first line from the parent\n");
    // Kill some time...
    printf("%d\n", fib(40));
    dprintf(fd, "This is another line from the parent\n");

    wait(NULL);
    close(fd);
  }

  return 0;
}

int fib(int n) {
  if (n < 2) {
    return n;
  }

  return fib(n-1) + fib(n-2);
}
