#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Parent waiting for child, CSP-style (using a pipe to synchronize the
// two processes).
int main(void) {
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    fprintf(stderr, "Unable to create pipe\n");
    exit(1);
  }

  char buf[1];

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (pid == 0) {
    close(pipefd[0]);

    buf[0] = 'X';
    printf("Hello (child)\n");
    sleep(3);
    // Send 1 byte to the parent process over the pipe
    write(pipefd[1], buf, 1);
    close(pipefd[1]);
    exit(0);
  } else {
    close(pipefd[1]);

    // Wait for the child to send a byte over the pipe
    read(pipefd[0], buf, 1);
    printf("Goodbye (parent)\n");
  }

  return 0;
}
