#include "common.h"
#define OLI_ARRAY
#define OLI_BIT
#include <oli.c>

#define ARRAY_LENGTH 1000000

def_array(int)
int_array_t arr;

int main() {
    int i;
    int a, b, c;
    a = 0, b = 1;
    for (i = 0; i < ARRAY_LENGTH; ++i) {
        if (i % 45 == 0) {
            a = 0, b = 1;
        }
        c = a + b;
        a = b;
        b = c;
        append_int(&arr, c);
    }
    test_assert(arr.cap == bit_po2_up(ARRAY_LENGTH))
    test_assert(arr.len == ARRAY_LENGTH)
    a = 0, b = 1;
    for (i = 0; i < ARRAY_LENGTH; ++i) {
        if (i % 45 == 0) {
            a = 0, b = 1;
        }
        c = a + b;
        a = b;
        b = c;
        test_assert(arr.data[i] == c)
    }
    logln("passed");
    return 0;
}
