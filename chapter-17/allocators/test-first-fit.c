#include "alloc.h"
#include "../../testing/ctest.h"
#include <stdio.h>

void test_distinct(void) {
  void *p1 = mymalloc(1);
  void *p2 = mymalloc(1);
  assert_ne((long) p1, (long) p2);
}

void test_memlimit(void) {
  void *p = mymalloc(4096);
  assert_eq((long) p, (long) NULL);
}

void test_coalesce(void) {
  void *p1 = mymalloc(1000);
  void *p2 = mymalloc(1000);
  void *p3 = mymalloc(1000);
  void *p4 = mymalloc(1000);
  void *p5 = mymalloc(4000);

  assert_ne((long) p1, (long) NULL);
  assert_ne((long) p2, (long) NULL);
  assert_ne((long) p3, (long) NULL);
  assert_ne((long) p4, (long) NULL);
  assert_eq((long) p5, (long) NULL);

  myfree(p1);
  myfree(p3);
  myfree(p2);
  myfree(p4);

  p5 = mymalloc(4000);
  assert_ne((long) p5, (long) NULL);
}

void test_misc(void) {
  void *p1 = mymalloc(20);
  void *p2 = mymalloc(5);
  void *p3 = mymalloc(20);
  void *p4 = mymalloc(4000);

  assert_eq((long) mymalloc(40), (long) NULL);
  
  myfree(p1);
  myfree(p2);

  // There's enough total memory, but not contiguously
  assert_eq((long) mymalloc(40), (long) NULL);

  p1 = mymalloc(20);
  p2 = mymalloc(20);
  assert_ne((long) p1, (long) NULL);
  assert_ne((long) p2, (long) NULL);

  myfree(p1);
  myfree(p2);
  myfree(p3);

  // Now there should be enough space to allocate 40 bytes
  p1 = mymalloc(40);
  assert_ne((long) p1, (long) NULL);

  myfree(p3);
  myfree(p4);
}

int main(void) {
  alloc_init();

  ctest_add_test("Allocation returns distinct pointers", test_distinct);
  ctest_add_test("Attempting to allocate more than mem limit returns a NULL pointer", test_memlimit);
  ctest_add_test("Freed blocks are coalesced", test_coalesce);
  ctest_add_test("Miscellaneous interactions", test_misc);

  ctest_run_all();

  return 0;
}
