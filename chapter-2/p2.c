#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  printf("Hello, world (pid: %d)\n", (int) getpid());
  pid_t rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("Hello, I am the child (pid: %d)\n", (int) getpid());
    sleep(10);
    exit(42);
  } else {
    int status;
    pid_t rc_wait = wait(&status);
    printf("Hello, I am the parent of %d (rc_wait: %d) (pid: %d)\n", rc, (int) rc_wait, (int) getpid());
    printf("Child exited with status %d\n", WEXITSTATUS(status));
  }

  return 0;
}
