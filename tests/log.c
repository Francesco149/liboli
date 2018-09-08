#define OLI_LOG
#include <oli.c>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define ARRAY_SIZE 1000000

int compar_int(void const* p1, void const* p2) {
    int left = *(int*)p1;
    int right = *(int*)p2;
    return ((left > right) - (left < right));
}

int main() {
    int i;
    int* arr;
    logs("hello world");
    logln("allocating %d bytes", (int)sizeof(int) * ARRAY_SIZE);
    arr = malloc(sizeof(int) * ARRAY_SIZE);
    if (!arr) {
        log_errno("malloc");
        return 0;
    }
    srand(time(0));
    arr[0] = 0x80000000;
    for (i = 1; i < ARRAY_SIZE - 1; ++i) {
        arr[i] = rand();
    }
    arr[ARRAY_SIZE - 1] = 0x7FFFFFFF;
    qsort(arr, ARRAY_SIZE, sizeof(arr[0]), compar_int);
    logs("qsort done");
    log_dump("%d", arr[0]);
    log_dump("%d", arr[ARRAY_SIZE - 1]);
    errno = ENOMEM;
    log_errno("test errno");
    return 0;
}
