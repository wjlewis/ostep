#include "../ctest.h"

void eq_test(void) {
  assert_eq(3, 3);
}

void eq_test_fail(void) {
  assert_eq(1, 42);
}

void lt_test(void) {
  assert_lt(3, 5);
}

void lt_test_fail(void) {
  assert_lt(34, 1);
}

int main(void) {
  ctest_add_test("3 is equal to 3", eq_test);
  ctest_add_test("1 is equal to 42", eq_test_fail);
  ctest_add_test("3 < 5", lt_test);
  ctest_add_test("34 < 1", lt_test_fail);

  ctest_run_all();

  return 0;
}
