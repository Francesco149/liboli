#include <stdlib.h>
#define OLI_LOG

#define test_assert(x) \
  if (!(x)) { \
    logs(#x ": test failed"); \
    exit(1); \
  }
