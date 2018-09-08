#include "common.h"
#define OLI_MAP
#include <oli.c>

int values[] = { 300, 300, 400, 300, 123, 1, 2000000000, 5, 5, 5, 5,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 15, 14, 15, 16,
    17, 17, 11, 11, 11 };
map_t map;

int main() {
    int i;
    for (i = 0; i < countof(values); ++i) {
        int cur = map_geti(&map, values[i]);
        test_assert(map_seti(&map, values[i], cur + 1) != 0)
    }
    test_assert(map.cap == 64)
    test_assert(map.len == 21)
    test_assert(map_geti(&map, 300) == 3)
    test_assert(map_geti(&map, 400) == 1)
    test_assert(map_geti(&map, 123) == 1)
    test_assert(map_geti(&map, 1) == 2)
    test_assert(map_geti(&map, 2000000000) == 1)
    test_assert(map_geti(&map, 5) == 5)
    test_assert(map_geti(&map, 2) == 1)
    test_assert(map_geti(&map, 3) == 1)
    test_assert(map_geti(&map, 4) == 1)
    test_assert(map_geti(&map, 6) == 1)
    test_assert(map_geti(&map, 7) == 1)
    test_assert(map_geti(&map, 8) == 1)
    test_assert(map_geti(&map, 9) == 1)
    test_assert(map_geti(&map, 10) == 1)
    test_assert(map_geti(&map, 11) == 4)
    test_assert(map_geti(&map, 12) == 1)
    test_assert(map_geti(&map, 13) == 1)
    test_assert(map_geti(&map, 14) == 1)
    test_assert(map_geti(&map, 15) == 3)
    test_assert(map_geti(&map, 16) == 1)
    test_assert(map_geti(&map, 17) == 2)
    test_assert(!map_geti(&map, 123123123))
    test_assert(!map_geti(&map, 123123123))
    logln("passed");
    return 0;
}
