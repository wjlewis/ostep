#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

char *itoa(int);

int main(void) {
  int pipefd[2];
  char size_buf[1];

  if (pipe(pipefd) < 0) {
    fprintf(stderr, "Unable to create pipe\n");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (pid == 0) {
    close(pipefd[0]);

    printf("Closing STDOUT_FILENO...\n");
    close(STDOUT_FILENO);
    int result = printf("Writing to stdout...\n");

    char *result_str = itoa(result);
    int size = strlen(result_str);
    size_buf[0] = size;

    // Send size, then message (overkill)
    write(pipefd[1], size_buf, 1);
    write(pipefd[1], result_str, size);

    free(result_str);
    close(pipefd[1]);

    exit(0);
  } else {
    close(pipefd[1]);

    // Read result from child
    read(pipefd[0], size_buf, 1);

    char *result_str = malloc(sizeof(char) * (int) size_buf[0]);
    if (result_str == NULL) {
      fprintf(stderr, "Unable to allocate result_str\n");
      exit(1);
    }

    read(pipefd[0], result_str, (int) size_buf[0]);

    printf("`printf` in child returned %s\n", result_str);
    free(result_str);
    wait(NULL);
  }

  return 0;
}

void reverse(char *str) {
  int len = strlen(str);
  char tmp;

  for (int i = 0; i < len / 2; i++) {
    tmp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = tmp;
  }
}

// Allocate space for string here
char *itoa(int n) {
  // We should take the log of n, but we'll just be wasteful
  char *str = malloc(sizeof(char) * 12);
  if (str == NULL) {
    return NULL;
  }

  // Is `n` negative?
  int neg = 0;
  if (n < 0) {
    n *= -1;
    neg = 1;
  }

  char *c;
  for (c = str; n > 0; n /= 10, c++) {
    *c = '0' + (n % 10);
  }
  if (neg) {
    *c++ = '-';
  }
  *c = '\0';

  // Reverse
  reverse(str);

  return str;
}
