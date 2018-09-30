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

#define OLI_MAJOR 3
#define OLI_MINOR 1
#define OLI_PATCH 1

#define pp_stringify1(x) #x
#define pp_stringify(x) pp_stringify1(x)

#define OLI_VERSION_STR \
  "liboli-" \
  pp_stringify(OLI_MAJOR) "." \
  pp_stringify(OLI_MINOR) "." \
  pp_stringify(OLI_PATCH)

#if defined(OLI_INTERN) || defined(OLI_INPUT)
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

#if defined(OLI_INPUT)
#define OLI_STRING
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
 * def_array(mytype) will define a type-safe resizable array mytype_array_t
 * you can use array_* macros to operate on it
 *
 * in case of out-of-memory, operations that can grow the array don't do
 * anything
 *
 * the new design of this module is inspired by https://github.com/rxi/vec
 */

#define def_array2(type, name) \
  typedef struct { \
    int cap; \
    int len; \
    type* data; \
  } name##_array_t

#define def_array(type) def_array2(type, type)
#define def_array_t(type) def_array2(type##_t, type)

#define array_reserve(arr, n) \
  array_reserve_i(n, array_unpack(arr))

#define array_free(arr) \
  array_free_i(array_unpack(arr))

#define array_append(arr, x) \
  (array_reserve((arr), (arr)->len + 1) \
    ? ((arr)->data[(arr)->len++] = (x), 1) \
    : 0)

#define array_cat(arr, x, n) do { \
  if (array_reserve(arr, (arr)->len + (n))) { \
    int _i = 0; \
    for (_i = 0; _i < (n); ++_i) { \
      (arr)->data[(arr)->len++] = (x)[_i]; \
    } \
  } \
} while (0)

/* internal helpers, not to be used directly */
#define array_unpack(arr) \
  &(arr)->cap, \
  &(arr)->len, \
  (void**)&(arr)->data, \
  (int)sizeof((arr)->data[0])

int array_reserve_i(int n, int* cap, int* len, void** data, int esize);
void array_free_i(int* cap, int* len, void** data, int esize);

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

/* aligns x down to a power-of-two value a */
#define bit_align_down(x, a) \
  ((x) & ~((a) - 1))

/* aligns x up to a power-of-two value a */
#define bit_align_up(x, a) \
  bit_align_down((x) + (a) - 1, a)

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
def_array_t(pchar);

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
 *   // ...
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
#if defined(OLI_INPUT) || defined(OLI_ALL)

/*
 * reads from either a file or a string, with parsing in mind
 *
 * we can push and pop positions up to pos_stack's size. the total range
 * of the stored positions cannot exceed the backtrack buffer size
 *
 * when we pop a position, we can decide to rewind to it by passing
 * rewind = 1 or just discard it (rewind = 0)
 *
 * functions that take a desc parameter will allocate a copy
 * of the matched string in *desc using an internal memory arena which
 * is valid until input_free is called.
 * if desc is 0, no allocation will happen.
 */

#include <stdio.h>

enum { I_STRING, I_FILE };

typedef struct {
  int type;
  union {
    string_t string;
    FILE* file;
  } data;
  arena_t arena;
  int col;
  int row;
  char last;
  int pos_top;
  int pos_stack[64];
  int backtrack_size;
  char backtrack[4096];
} input_t;

void input_free(input_t* i);
void input_push(input_t* i);
void input_pop(input_t* i, int rewind);
void input_from_string(input_t* i, char* s);
void input_from_range(input_t* i, char* start, char* end);
void input_from_file(input_t* i, FILE* f);
char input_getc(input_t* i);
int input_eof(input_t* i);
int input_any(input_t* i, char** desc);
int input_char(input_t* i, char expected, char** desc);
int input_range(input_t* i, char start, char end, char** desc);
int input_one_of(input_t* i, char* charset, char** desc);
int input_none_of(input_t* i, char* charset, char** desc);
int input_satisfy(input_t* i, int(*condition)(char), char** desc);
int input_string(input_t* i, char* str, char** desc);

#endif /* OLI_INPUT */
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
#if defined(OLI_ARRAY) || defined(OLI_ALL)

/*
 * these don't always use all params but we always pass all of them to
 * ensure that we get a compiler error on things that don't have the same
 * fields as an array struct
 */

int array_reserve_i(int n, int* cap, int* len, void** data, int esize) {
  (void)len;
  if (*cap <= n) {
    void* newdata;
    int newcap = *cap ? *cap * 2 : 16;
    newdata = realloc(*data, esize * newcap);
    if (!newdata) {
      return 0;
    }
    *data = newdata;
    *cap = newcap;
  }
  return 1;
}

void array_free_i(int* cap, int* len, void** data, int esize) {
  (void)esize;
  free(*data);
  *cap = 0;
  *len = 0;
  *data = 0;
}

#endif /* OLI_ARRAY */
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
  char* res = malloc(len + 1);
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
  new_map.keys = calloc(new_cap, sizeof(int));
  new_map.values = malloc(new_cap * sizeof(void*));
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
  array_append(&arena->blocks, arena->block);
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
  array_free(&arena->blocks);
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
#if defined(OLI_INPUT) || defined(OLI_ALL)

#include <string.h>

void input_free(input_t* i) {
  arena_free(&i->arena);
}

void input_push(input_t* i) {
  if (i->pos_top >= countof(i->pos_stack) - 1) {
    return;
  }
  ++i->pos_top;
  i->pos_stack[i->pos_top] = i->pos_stack[i->pos_top - 1];
}

void input_pop(input_t* i, int rewind) {
  if (i->pos_top <= 0) {
    return;
  }
  if (!rewind) {
    i->pos_stack[i->pos_top - 1] = i->pos_stack[i->pos_top];
  }
  --i->pos_top;
}

void input_from_string(input_t* i, char* s) {
  memset(i, 0, sizeof(input_t));
  i->type = I_STRING;
  string_from_c(&i->data.string, s);
}

void input_from_range(input_t* i, char* start, char* end) {
  memset(i, 0, sizeof(input_t));
  i->type = I_STRING;
  string_from_range(&i->data.string, start, end);
}

void input_from_file(input_t* i, FILE* f) {
  memset(i, 0, sizeof(input_t));
  i->type = I_FILE;
  i->data.file = f;
}

char input_getc(input_t* i) {
  int pos = i->pos_stack[i->pos_top];
  if (pos < i->backtrack_size) {
    return i->backtrack[pos];
  }
  if (i->pos_stack[0] >= i->backtrack_size) {
    int j;
    for (j = 0; j <= i->pos_top; ++j) {
      i->pos_stack[j] -= i->backtrack_size;
    }
    i->backtrack_size = 0;
  }
  switch (i->type) {
    case I_STRING: return *i->data.string.start;
    case I_FILE: return getc(i->data.file);
  }
  return 0;
}

int input_eof(input_t* i) {
  switch (i->type) {
    case I_STRING: return string_len(&i->data.string) <= 0;
    case I_FILE: return feof(i->data.file);
  }
  return 1;
}

int input_success(input_t* i, char c, char** desc) {
  if (i->pos_stack[i->pos_top] >= i->backtrack_size) {
    if (i->backtrack_size < countof(i->backtrack)) {
      i->backtrack[i->backtrack_size++] = c;
    }
    if (c == '\n') {
      ++i->row;
      i->col = 0;
    } else {
      ++i->col;
    }
    if (i->type == I_STRING) {
      ++i->data.string.start;
    }
    i->last = c;
  }
  ++i->pos_stack[i->pos_top];
  if (desc) {
    *desc = arena_alloc(&i->arena, 2);
    if (*desc) {
      (*desc)[0] = c;
      (*desc)[1] = 0;
    }
  }
  return 1;
}

int input_failure(input_t* i, char c) {
  if (i->type == I_FILE) {
    if (i->pos_stack[i->pos_top] >= i->backtrack_size) {
      ungetc(c, i->data.file);
    }
  }
  return 0;
}

int input_any(input_t* i, char** desc) {
  char c = input_getc(i);
  if (input_eof(i)) {
    return 0;
  }
  return input_success(i, c, desc);
}

int input_char(input_t* i, char expected, char** desc) {
  char c = input_getc(i);
  if (input_eof(i)) {
    return 0;
  }
  return c == expected
    ? input_success(i, c, desc)
    : input_failure(i, c);
}

int input_range(input_t* i, char start, char end, char** desc) {
  char c = input_getc(i);
  if (input_eof(i)) {
    return 0;
  }
  return c >= start && c <= end
    ? input_success(i, c, desc)
    : input_failure(i, c);
}

int input_one_of(input_t* i, char* charset, char** desc) {
  char c = input_getc(i);
  if (input_eof(i)) {
    return 0;
  }
  return strchr(charset, c)
    ? input_success(i, c, desc)
    : input_failure(i, c);
}

int input_none_of(input_t* i, char* charset, char** desc) {
  char c = input_getc(i);
  if (input_eof(i)) {
    return 0;
  }
  return !strchr(charset, c)
    ? input_success(i, c, desc)
    : input_failure(i, c);
}

int input_satisfy(input_t* i, int(*condition)(char), char** desc) {
  char c = input_getc(i);
  if (input_eof(i)) {
    return 0;
  }
  return condition(c)
    ? input_success(i, c, desc)
    : input_failure(i, c);
}

int input_string(input_t* i, char* str, char** desc) {
  char* s;
  input_push(i);
  for (s = str; *s; ++s) {
    if (!input_char(i, *s, 0)) {
      input_pop(i, 1);
      return 0;
    }
  }
  input_pop(i, 0);
  if (desc) {
    *desc = arena_alloc(&i->arena, strlen(str) + 1);
    if (*desc) {
      strcpy(*desc, str);
    }
  }
  return 1;
}

#endif /* OLI_INPUT */
/* --------------------------------------------------------------------- */
#endif /* OLI_IMPLEMENTATION */
