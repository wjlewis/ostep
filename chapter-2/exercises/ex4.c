#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int execl_ls(void);
int execlp_ls(void);
int execle_ls(void);

int main(void) {
  if (execl_ls() != 0) {
    printf("execl_ls failed\n");
  }

  if (execlp_ls() != 0) {
    printf("execlp_ls failed\n");
  }

  if (execle_ls() != 0) {
    printf("execle_ls failed\n");
  }

  return 0;
}

void panic(char *message);

int execl_ls(void) {
  pid_t pid = fork();

  if (pid < 0) {
    panic("Fork failed");
  } else if (pid == 0) {
    printf("excl(\"/bin/ls\", \".\", (char *) 0);\n");
    if (execl("/bin/ls", ".", (char *) 0) < 0) {
      exit(1);
    }
  } else {
    int status;
    wait(&status);
    return WEXITSTATUS(status);
  }

  return 0;
}

int execlp_ls(void) {
  pid_t pid = fork();

  if (pid < 0) {
    panic("Fork failed");
  } else if (pid == 0) {
    printf("exclp(\"ls\", \".\", (char *) 0);\n");
    if (execlp("ls", ".", (char *) 0) != 0) {
      exit(1);
    }
  } else {
    int status;
    wait(&status);
    return WEXITSTATUS(status);
  }

  return 0;
}

int execle_ls(void) {
  pid_t pid = fork();

  if (pid < 0) {
    panic("Fork failed");
  } else if (pid == 0) {
    char *env[] = { "NAME=value", 0 };
    printf("excle(\"/bin/ls\", \"/bin/ls\", \".\", (char *) 0, env);\n");
    if (execle("/bin/ls", "/bin/ls", ".", (char *) 0, env) != 0) {
      exit(1);
    }
  } else {
    int status;
    wait(&status);
    return WEXITSTATUS(status);
  }

  return 0;
}

void panic(char *message) {
  fprintf(stderr, "%s", message);
  exit(1);
}
