#include "ctest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct test_node {
  char *name;
  void (*test_fn)(void);
  struct test_node *next;
};

static struct test_node *tests = NULL;

static void report_status(char *test_name, int status);
static void free_tests(void);

void ctest_run_all(void) {
  struct test_node *curr = tests;

  while (curr) {
    pid_t pid;
    if ((pid = fork()) < 0) {
      fprintf(stderr, "Unable to run test \"%s\" (fork failed)\n", curr->name);
      exit(0);
    } else if (pid == 0) {
      (curr->test_fn)();
      exit(0);
    } else {
      // TODO Run tests concurrently.
      int child_status;
      wait(&child_status);
      int status = WEXITSTATUS(child_status);

      report_status(curr->name, status);

      curr = curr->next;
    }
  }

  free_tests();
}

void ctest_add_test(char *test_name, void (*test_fn)(void)) {
  struct test_node *t = malloc(sizeof(struct test_node));
  if (!t) {
    fprintf(stderr, "Unable to allocate space for test\n");
    exit(1);
  }

  t->name = strdup(test_name);
  t->test_fn = test_fn;
  t->next = tests;
  tests = t;
}

// Exit codes
// TODO Summarize test failure in a struct which can be serialized and
// piped back to the process running the tests.
#define SUCCESS    0
#define EQ_FAIL    1
#define NE_FAIL    2
#define GT_FAIL    3
#define LT_FAIL    4

void assert_eq(int x, int y) {
  if (x != y) {
    exit(EQ_FAIL);
  }
}

void assert_ne(int x, int y) {
  if (x == y) {
    exit(EQ_FAIL);
  }
}

void assert_gt(int x, int y) {
  if (!(x > y)) {
    exit(GT_FAIL);
  }
}

void assert_lt(int x, int y) {
  if (!(x < y)) {
    exit(LT_FAIL);
  }
}

#define ANSI_ESC       "\033"
#define RESET_COLORS   ANSI_ESC "[0m"
#define GREEN(s)       "\033[1;32m" s RESET_COLORS
#define RED(s)         "\033[1;31m" s RESET_COLORS

static void report_status(char *test_name, int status) {
  switch (status) {
    case SUCCESS:
      printf(GREEN("[PASSED]"));
      break;
    case EQ_FAIL:
      printf(RED("[FAILED (lhs != rhs)]"));
      break;
    case NE_FAIL:
      printf(RED("[FAILED (lhs == rhs)]"));
      break;
    case GT_FAIL:
      printf(RED("[FAILED (lhs <= rhs)]"));
      break;
    case LT_FAIL:
      printf(RED("[FAILED (lhs >= rhs)]"));
      break;
    default:
      printf(RED("[FAILED (reason unknown)]"));
      break;
  }
  printf(": %s\n", test_name);
}

static void free_tests(void) {
  struct test_node *curr = tests;
  struct test_node *next;

  while (curr) {
    next = curr->next;
    free(curr->name);
    free(curr);
    curr = next;
  }
}
