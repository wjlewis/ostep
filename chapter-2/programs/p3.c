#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  printf("Hello, world (pid: %d)\n", (int) getpid());
  pid_t rc = fork();

  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("Hello, I'm the child (pid: %d)\n", (int) getpid());
    char *myargs[3];
    myargs[0] = strdup("wc");
    myargs[1] = strdup("p3.c");
    myargs[2] = NULL;
    execvp(myargs[0], myargs);
    printf("This shouldn't be printed\n");
  } else {
    wait(NULL);
    printf("Hello, I'm the parent of %d (pid: %d)\n", (int) rc, (int) getpid());
  }

  return 0;
}
