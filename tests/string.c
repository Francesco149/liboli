#include "common.h"
#define OLI_STRING
#include <oli.c>
#include <stdlib.h>
#include <string.h>

#define TESTSTR "one,two,three"

int main() {
  string_t s1, s2, s3;
  char test[64];

  strcpy(test, TESTSTR);
  string_from_range(&s1, test, test + strlen(test));
  string_from_c(&s2, test);
  test_assert(!string_cmp(&s1, &s2))
  test_assert(!string_cmp_c(&s1, TESTSTR))
  test_assert(string_len(&s1) == string_len(&s2))
  test_assert(string_len(&s1) == strlen(TESTSTR))
  test_assert(!strcmp(string_dup_c(&s1), TESTSTR))

  string_dup(&s3, &s2);
  test_assert(string_cmp(&s2, &s3) == 0)

  s1.end = s1.start + 3;
  test_assert(!string_cmp_c(&s1, "one"));
  test_assert(string_cmp_c(&s1, "one,"));
  test_assert(string_cmp_c(&s1, TESTSTR));

  s1.start += 4;
  s1.end = s1.start + 3;
  test_assert(!string_cmp_c(&s1, "two"));
  test_assert(string_cmp_c(&s1, "two,"));
  test_assert(string_cmp_c(&s1, TESTSTR + 4));

  s1.start += 4;
  s1.end = s1.start + 5;
  test_assert(!string_cmp_c(&s1, "three"));

  string_from_range(&s1, s2.start, s2.start + string_cspn(&s2, ","));
  test_assert(!string_cmp_c(&s1, "one"));
  test_assert(string_cmp_c(&s1, "one,"));
  test_assert(string_cmp_c(&s1, TESTSTR));

  s2.start += string_cspn(&s2, ",") + 1;
  string_from_range(&s1, s2.start, s2.start + string_cspn(&s2, ","));
  test_assert(!string_cmp_c(&s1, "two"));
  test_assert(string_cmp_c(&s1, "two,"));
  test_assert(string_cmp_c(&s1, TESTSTR + 4));

  s2.start += string_cspn(&s2, ",") + 1;
  string_from_range(&s1, s2.start, s2.start + string_cspn(&s2, ","));
  test_assert(!string_cmp_c(&s1, "three"));

  logln("passed");

  return 0;
}
