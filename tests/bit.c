#include "common.h"
#define OLI_BIT
#include <oli.c>

int main() {
    test_assert(bit_align_down(180, 64) == 128)
    test_assert(bit_align_up(180, 64) == 192)
    test_assert(bit_po2_up(1) == 1)
    test_assert(bit_po2_up(2) == 2)
    test_assert(bit_po2_up(3) == 4)
    test_assert(bit_po2_up(5) == 8)
    test_assert(bit_po2_up(9) == 16)
    test_assert(bit_po2_up(17) == 32)
    test_assert(bit_po2_up(33) == 64)
    test_assert(bit_po2_up(1000000000) == 0x40000000)
    logln("passed");
    return 0;
}
