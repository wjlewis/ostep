#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int main(void) {
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Failed to fork\n");
    exit(1);
  } else if (pid == 0) {
    printf("Child waiting...\n");
    int status;
    pid_t wait_pid = wait(&status);
    printf("Child wait completed (pid = %d; status = %d)\n", wait_pid, WEXITSTATUS(status));
    if (wait_pid < 0) {
      switch (errno) {
      case ECHILD:
        fprintf(stderr, "ECHILD\n");
        break;
      default:
        fprintf(stderr, "Some other error occurred\n");
        break;
      }
    }

    printf("Child (pid = %d) exiting with status 42\n", getpid());
    exit(42);
  } else {
    int status;
    pid_t child_pid = wait(&status);
    printf("Child (pid = %d) exited with status %d\n", child_pid, WEXITSTATUS(status));
  }

  return 0;
}
