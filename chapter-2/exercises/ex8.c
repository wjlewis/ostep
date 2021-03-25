#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  int pipefd1[2], pipefd2[2];
  char buf[1];

  if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0) {
    fprintf(stderr, "Unable to create pipe\n");
    exit(1);
  }

  if (fork() == 0) {
    // First child
    close(pipefd2[1]);

    buf[0] = 'A';
    printf("[1] Sending 'A' to [2]\n");
    sleep(1);
    write(pipefd1[1], buf, 1);

    read(pipefd2[0], buf, 1);
    printf("[1] Received %c from [2]\n", buf[0]);

    close(pipefd2[0]);
    close(pipefd1[1]);
    close(pipefd1[0]);
    exit(0);
 } else if (fork() == 0) {
    // Second child
    close(pipefd1[1]);

    read(pipefd1[0], buf, 1);
    printf("[2] Received %c from [1]\n", buf[0]);

    buf[0] = 'B';
    printf("[2] Sending 'B' to [1]\n");
    sleep(1);
    write(pipefd2[1], buf, 1);

    close(pipefd1[0]);
    close(pipefd2[0]);
    close(pipefd2[1]);
    exit(0);
  } else {
    wait(NULL);
    wait(NULL);
  }

  return 0;
}
