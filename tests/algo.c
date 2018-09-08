#include "common.h"
#define OLI_ALGO
#include <oli.c>

int main() {
    test_assert(al_min(1, 10) ==  1)
    test_assert(al_min(420, 69) == 69)
    test_assert(al_max(1337, 420) == 1337)
    test_assert(al_max(1111, 6666) == 6666)
    logln("passed");
    return 0;
}
