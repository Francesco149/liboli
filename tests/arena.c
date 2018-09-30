#include "common.h"
#define OLI_ARENA
#define OLI_ARRAY
#include <oli.c>

#define LOOPS 1000
typedef int* pint;
def_array(pint);

arena_t arena;
pint_array_t arrays;

int main() {
  int i, j;
  int* p;
  char* first_block;
  char* first_block_end;
  for (i = 0; i < LOOPS; ++i) {
    p = arena_alloc(&arena, sizeof(int) * i);
    if (!i) {
      first_block = (char*)p;
      first_block_end = arena.end_of_block;
    }
    test_assert((long)p % ARENA_ALIGN == 0);
    for (j = 0; j < i; ++j) {
      p[j] = j;
    }
    array_append(&arrays, p);
  }
  for (i = 0; i < LOOPS; ++i) {
    p = arrays.data[i];
    for (j = 0; j < i; ++j) {
      test_assert(p[j] == j)
    }
  }
  test_assert((char*)p < first_block || (char*)p >= first_block_end)
  arena_free(&arena);
  logln("passed");
  return 0;
}
