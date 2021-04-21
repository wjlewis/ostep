#ifndef __ctest_h__
#define __ctest_h__

void ctest_add_test(char *test_name, void (*test_fn)(void));

void ctest_run_all(void);

void assert_eq(int x, int y);

void assert_ne(int x, int y);

void assert_gt(int x, int y);

void assert_lt(int x, int y);

#endif
