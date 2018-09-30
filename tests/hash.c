#include "common.h"
#define OLI_HASH
#include <oli.c>

#define S1 "hello world"
#define S2 "ayy lmao"

int main() {
  char s[64];

  strcpy(s, S1);
  test_assert(hash_fnv32(S1, strlen(S1)) == hash_fnv32(S1, strlen(S1)))
  test_assert(hash_fnv32(s, strlen(s)) == hash_fnv32(S1, strlen(S1)))
  test_assert(hash_fnv32(s, strlen(s)) != hash_fnv32(S2, strlen(S2)))

  test_assert(hash_int32(1337) == hash_int32(1337))
  test_assert(hash_int32(69) != hash_int32(420))

  logln("passed");
  return 0;
}
