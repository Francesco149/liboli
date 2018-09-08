/*
 * personal single-header c89 library with misc utilities
 *
 * no dependencies other than the standard c library
 *
 * I actually made this just so that I can add ```-loli``` to ldflags
 *
 * some stuff such as the memory arena and string interning is inspired
 * by https://github.com/pervognsen/bitwise
 *
 * windows build scripts coming when I feel like booting up a windows vm
 *
 * for documentation, read the header portion of ```oli.c```. you could
 * also take a look at the tests for concrete examples
 *
 * # building the shared library
 * just run ```./build``` . it's aware of ```CC```, ```CFLAGS```,
 * ```LDFLAGS``` in case you need to override anything
 *
 * this will produce liboli.so which you can link with ```-loli```
 *
 * ```
 * #include <oli.c>
 * ...
 * ```
 *
 * remember to add oli.c to your project or your include paths as well as
 * placing liboli.so in your ld paths (such as /usr/lib)
 *
 * # static build
 * drop oli.c into your project and
 * ```
 * #define OLI_IMPLEMENTATION
 * #include <oli.c>
 * ...
 * ```
 * no need to mess with linker flags, this will include the implementation
 * directly into your file
 *
 * you could also build it as a static library but it's not recommended as
 * you won't be able to toggle unneeded features off unless you make a
 * custom build for each project. if you really want to link this like
 * a normal library just build the shared library
 *
 * # license
 * this is free and unencumbered software released into the public domain.
 * refer to the attached UNLICENSE or http://unlicense.org/
 */

#ifndef OLI_H
#define OLI_H

#define OLI_MAJOR 2
#define OLI_MINOR 0
#define OLI_PATCH 0

#define pp_stringify1(x) #x
#define pp_stringify(x) pp_stringify1(x)

#define OLI_VERSION_STR \
    "liboli-" \
    pp_stringify(OLI_MAJOR) "." \
    pp_stringify(OLI_MINOR) "." \
    pp_stringify(OLI_PATCH)

#ifdef OLI_INTERN
#define OLI_MAP
#define OLI_ARENA
#endif

#ifdef OLI_ARENA
#define OLI_BIT
#define OLI_ARRAY
#endif

#ifdef OLI_MAP
#define OLI_HASH
#endif

#if defined(OLI_ARENA) || defined(OLI_MAP)
#define OLI_ALGO
#endif

/* --------------------------------------------------------------------- */

/* misc utils that I don't know how to categorize */
#define countof(x) (int)(sizeof(x) / sizeof((x)[0]))

/* --------------------------------------------------------------------- */
#if defined(OLI_LOG) || defined(OLI_ALL)

#include <errno.h>
#include <string.h>

/* all log functions print to stderr */

/* prints "milliseconds.microseconds [file:line] message\n" */
#define logln \
    log_print_header(__FILE__, __LINE__), \
    log_println

/* calls log("x=fmt", x) */
#define log_dump(fmt, x) \
    logln(#x "=" fmt, x)

#define logs(str) \
    logln("%s", str)

/* not thread safe */
#define log_errno(msg) \
    logln("%s: %s", msg, strerror(errno))

void log_print_header(char const* file, int line);
void log_printf(char* fmt, ...);
void log_println(char* fmt, ...);

#endif /* OLI_LOG */
/* --------------------------------------------------------------------- */
#if defined(OLI_ARRAY) || defined(OLI_ALL)
#include <stdlib.h>

/*
 * def_array(mytype) will define a resizable array for the given type.
 *
 * here is exactly what's generated:
 * - the type mytype_array_t (must be initialized to zero)
 * - int reserve_mytype(mytype_array_t* arr, int n)
 *   returns 1 on success and 0 on failure (out of memory)
 * - int append_mytype(mytype_array_t* arr, mytype x)
 *   returns 1 on success and 0 on failure (out of memory)
 * - void free_mytype_array(mytype_array_t* arr)
 */

#define def_array2(type, name) \
typedef struct { \
    type* data; \
    int cap; \
    int len; \
} name##_array_t; \
\
int reserve_##name(name##_array_t* arr, int n) { \
    if (arr->cap <= n) { \
        void* new_data; \
        arr->cap = arr->cap ? arr->cap * 2 : 16; \
        new_data = realloc(arr->data, sizeof(type) * arr->cap); \
        if (!new_data) { \
            return 0; \
        } \
        arr->data = (type*)new_data; \
    } \
    return 1; \
} \
\
int append_##name(name##_array_t* arr, type x) { \
    if (!reserve_##name(arr, arr->len + 1)) { \
        return 0; \
    } \
    arr->data[arr->len++] = x; \
    return 1; \
} \
\
void free_##name##_array(name##_array_t* arr) { \
    free(arr->data); \
    arr->data = 0; \
    arr->cap = 0; \
    arr->len = 0; \
}

#define def_array(type) def_array2(type, type)
#define def_array_t(type) def_array2(type##_t, type)

#endif /* OLI_ARRAY */
/* --------------------------------------------------------------------- */
#if defined(OLI_ALGO) || defined(OLI_ALL)

#define al_min(x, y) ((x) < (y) ? (x) : (y))
#define al_max(x, y) ((x) > (y) ? (x) : (y))

#endif /* OLI_ALGO */
/* --------------------------------------------------------------------- */
#if defined(OLI_STRING) || defined(OLI_ALL)

/* points to a range within a string, doesn't have to be zero-terminated */

typedef struct {
    char* start;
    char* end;
} string_t;

void string_from_range(string_t* s, char* start, char* end);
void string_from_c(string_t* s, char* str);
int string_len(string_t* s);
char* string_dup_c(string_t* s);
void string_dup(string_t* dst, string_t* src);
int string_cmp(string_t* s1, string_t* s2);
int string_cmp_c(string_t* s, char* str);
int string_spn(string_t* s, char* accept);
int string_cspn(string_t* s, char* reject);

#endif /* OLI_STRING */
/* --------------------------------------------------------------------- */
#if defined(OLI_HASH) || defined(OLI_ALL)

int hash_fnv32(void* data, int len);
int hash_int32(int x);

#endif /* OLI_HASH */
/* --------------------------------------------------------------------- */
#if defined(OLI_MAP) || defined(OLI_ALL)

/*
 * hashmap with 32-bit integer keys
 * - zero is reserved for empty values. setting value to zero will remove
 *   the key/value pair from the hashmap
 * - map_t must be initialized to zero
 * - uses hash_int32 to hash keys
 * - map_grow and map_set can fail if we're out of memory. they return 0
 *   on failure. map_set will still attempt to insert the value even if
 *   the grow fails. if map_grow keeps failing and no space is left in the
 *   map, map_set will also fail.
 */

typedef struct {
    int* keys;
    void** values;
    int cap;
    int len;
} map_t;

void map_free(map_t* map);
int map_reserve(map_t* map, int new_cap);
int map_set(map_t* map, int key, void* value);
int map_seti(map_t* map, int key, int value);
void* map_get(map_t* map, int key);
int map_geti(map_t* map, int key);

#endif /* OLI_MAP */
/* --------------------------------------------------------------------- */
#if defined(OLI_BIT) || defined(OLI_ALL)

/* various bitwise utilities */

/* aligns x to a power-of-two value a */
#define bit_align_down(x, a) ((x) & ~((a) - 1))
#define bit_align_up(x, a) bit_align_down((x) + (a) - 1, a)

/* closest higher power of two for a 32-bit int. returns 0 for x=0 */
int bit_po2_up(int x);

#endif
/* --------------------------------------------------------------------- */
#if defined(OLI_ARENA) || defined(OLI_ALL)

/*
 * very simple allocator for when you want to allocate a bunch of stuff
 * and free it all at once. reduces malloc overhead by pre-allocating big
 * contiguous chunks of memory
 *
 * arena_t must be initialized to zero
 * arena_reserve and arena_alloc will return 0 on failure (out of memory)
 */

#define ARENA_ALIGN sizeof(void*)
#define ARENA_BLOCK_SIZE (1024 * 1024)

typedef char* pchar_t;
def_array_t(pchar)

typedef struct {
    char* block;
    char* end_of_block;
    pchar_array_t blocks;
} arena_t;

/* ensures that there are at least min_size bytes reserved */
int arena_reserve(arena_t* arena, int min_size);
void* arena_alloc(arena_t* arena, int size);
void arena_free(arena_t* arena);

#endif
/* --------------------------------------------------------------------- */
#if defined(OLI_INTERN) || defined(OLI_ALL)

/*
 * string interning creates a unique pointer for unique strings which
 * allows quick string comparisons by simply comparing interned char ptrs.
 * there's some overhead the first time you intern a new string, but
 * as long as it's strings that are compared against often it should be
 * faster than doing strcmp every time.
 *
 * interns_t must be initialized to zero
 *
 * example:
 *
 * interns_t interns = {0};
 * char* keyword_while = intern_str(&interns, "while");
 *
 * // ...
 *
 * // hopefully you are comparing against keyword_while many times
 * if (intern_str(&interns, str) == keyword_while) {
 *     // ...
 * }
 */

typedef struct {
    arena_t arena;
    map_t map;
} interns_t;

char* intern_range(interns_t* interns, char* start, char* end);
char* intern_str(interns_t* interns, char* str);

#endif /* OLI_INTERN */
/* --------------------------------------------------------------------- */
#endif /* OLI_H */

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

#ifdef OLI_IMPLEMENTATION

/* --------------------------------------------------------------------- */
#if defined(OLI_LOG) || defined(OLI_ALL)

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void log_print_header(char const* file, int line) {
    int olderr = errno;
    unsigned t = (unsigned)clock();
    unsigned ms = t / (CLOCKS_PER_SEC / 1000);
    unsigned us = t / (CLOCKS_PER_SEC / 1000000);
    log_printf("%07u.%07u [%s:%d] ", ms, us, file, line);
    errno = olderr; /* allows log_errno to work */
}

void log_printf(char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
}

void log_println(char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
    fputc('\n', stderr);
}

#endif /* OLI_LOG */
/* --------------------------------------------------------------------- */
#if defined(OLI_STRING) || defined(OLI_ALL)
#include <stdlib.h>
#include <string.h>

void string_from_range(string_t* s, char* start, char* end) {
    s->start = start;
    s->end = end;
}

void string_from_c(string_t* s, char* str) {
    s->start = str;
    s->end = s->start + (s->start ? strlen(s->start) : 0);
}

int string_len(string_t* s) {
    return s->end - s->start;
}

char* string_dup_c(string_t* s) {
    int len = string_len(s);
    char* res = (char*)malloc(len + 1);
    if (res) {
        memcpy(res, s->start, len);
        res[len] = 0;
    }
    return res;
}

void string_dup(string_t* dst, string_t* src) {
    string_from_range(dst, src->start, src->end);
}

int string_cmp(string_t* s1, string_t* s2) {
    int len1 = string_len(s1);
    int len2 = string_len(s2);
    if (len1 > len2) {
        return 1;
    } else if (len1 < len2) {
        return -1;
    }
    return strncmp(s1->start, s2->start, len1);
}

int string_cmp_c(string_t* s, char* str) {
    string_t s2;
    string_from_c(&s2, str);
    return string_cmp(s, &s2);
}

int string_spn(string_t* s, char* accept) {
    char* p;
    for (p = s->start; p < s->end; ++p) {
        if (!strchr(accept, *p)) {
            break;
        }
    }
    return p - s->start;
}

int string_cspn(string_t* s, char* reject) {
    char* p;
    for (p = s->start; p < s->end; ++p) {
        if (strchr(reject, *p)) {
            break;
        }
    }
    return p - s->start;
}

#endif /* OLI_STRING */
/* --------------------------------------------------------------------- */
#if defined(OLI_HASH) || defined(OLI_ALL)

int hash_fnv32(void* data, int len) {
    int i;
    char* p = (char*)data;
    int x = 0x811c9dc5;
    for (i = 0; i < len; ++i) {
        x ^= p[i];
        x *= 0x1000193;
        x ^= x >> 16;
    }
    return x;
}

int hash_int32(int x) {
    x *= 0x85ebca6b;
    x ^= x >> 16;
    return x;
}

#endif /* OLI_HASH */
/* --------------------------------------------------------------------- */
#if defined(OLI_MAP) || defined(OLI_ALL)
#include <stdlib.h>

void map_free(map_t* map) {
    free(map->keys);
    free(map->values);
    map->keys = 0;
    map->values = 0;
    map->cap = 0;
    map->len = 0;
}

int map_reserve(map_t* map, int new_cap) {
    int i;
    map_t new_map;
    new_cap = al_max(16, new_cap);
    new_map.len = 0;
    new_map.cap = new_cap;
    new_map.keys = (int*)calloc(new_cap, sizeof(int));
    new_map.values = (void**)malloc(new_cap * sizeof(void*));
    if (!new_map.keys || !new_map.values) {
        free(new_map.keys);
        free(new_map.values);
        return 0;
    }
    for (i = 0; i < map->cap; ++i) {
        if (map->keys[i]) {
            map_set(&new_map, map->keys[i], map->values[i]);
        }
    }
    free(map->keys);
    free(map->values);
    *map = new_map;
    return 1;
}

int map_set(map_t* map, int key, void* value) {
    int i, j;
    int hash = hash_int32(key ? key : 1);
    if (2*map->len >= map->cap) {
        map_reserve(map, 2*map->cap);
    }
    if (map->len >= map->cap - 1) {
        return 0;
    }
    i = hash;
    for (j = 0; j < map->cap; ++j, ++i) {
        i &= map->cap - 1;
        if (!map->keys[i]) {
            map->keys[i] = key;
            map->values[i] = value;
            ++map->len;
            return 1;
        } else if (map->keys[i] == key) {
            if (value) {
                map->values[i] = value;
            } else {
                map->keys[i] = 0;
                --map->len;
            }
            return 1;
        }
    }
    return 0;
}

int map_seti(map_t* map, int key, int value) {
    return map_set(map, key, (void*)(long)value);
}

void* map_get(map_t* map, int key) {
    int i;
    int hash = hash_int32(key);
    if (!map->cap) {
        return 0;
    }
    for (i = hash; ; ++i) {
        i &= map->cap - 1;
        if (map->keys[i] == key) {
            return map->values[i];
        } else if (!map->keys[i]) {
            /* assuming the map always has at least 1 free slot */
            break;
        }
    }
    return 0;
}

int map_geti(map_t* map, int key) {
    return (int)(long)map_get(map, key);
}

#endif /* OLI_MAP */
/* --------------------------------------------------------------------- */
#if defined(OLI_BIT) || defined(OLI_ALL)

int bit_po2_up(int x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

#endif /* OLI_BIT */
/* --------------------------------------------------------------------- */
#if defined(OLI_ARENA) || defined(OLI_ALL)
#include <stdlib.h>

int arena_reserve(arena_t* arena, int min_size) {
    int size;
    char* new_block;
    if (arena->end_of_block - arena->block >= min_size) {
        return 1;
    }
    size = bit_align_up(al_max(min_size, ARENA_BLOCK_SIZE), ARENA_ALIGN);
    new_block = malloc(size);
    if (!new_block) {
        return 0;
    }
    arena->block = new_block;
    arena->end_of_block = new_block + size;
    append_pchar(&arena->blocks, arena->block);
    return 1;
}

void* arena_alloc(arena_t* arena, int size) {
    void* res;
    if (!arena_reserve(arena, size)) {
        return 0;
    }
    size = bit_align_up(size, ARENA_ALIGN);
    res = arena->block;
    arena->block += size;
    return res;
}

void arena_free(arena_t* arena) {
    int i;
    for (i = 0; i < arena->blocks.len; ++i) {
        free(arena->blocks.data[i]);
    }
    free_pchar_array(&arena->blocks);
    arena->block = 0;
    arena->end_of_block = 0;
}
#endif
/* --------------------------------------------------------------------- */
#if defined(OLI_INTERN) || defined(OLI_ALL)
#include <string.h>

typedef struct intern {
    int len;
    struct intern* next;
    char str[1]; /* actually len bytes */
} intern_t;

char* intern_range(interns_t* interns, char* start, char* end) {
    intern_t* new_intern;
    intern_t* it;
    int alloc_size;
    int len = end - start;
    int key = hash_fnv32(start, len);
    intern_t* first_intern = (intern_t*)map_get(&interns->map, key);
    for (it = first_intern; it; it = it->next) {
        if (it->len == len && !strncmp(it->str, start, len)) {
            return it->str;
        }
    }
    alloc_size = sizeof(intern_t) - sizeof(new_intern->str) + len;
    new_intern = (intern_t*)arena_alloc(&interns->arena, alloc_size);
    if (!new_intern) {
        return 0;
    }
    new_intern->len = len;
    new_intern->next = first_intern;
    memcpy(new_intern->str, start, len);
    if (!map_set(&interns->map, key, new_intern)) {
        return 0;
    }
    return new_intern->str;
}

char* intern_str(interns_t* interns, char* str) {
    return intern_range(interns, str, str + strlen(str));
}

#endif /* OLI_INTERN */
/* --------------------------------------------------------------------- */
#endif /* OLI_IMPLEMENTATION */
