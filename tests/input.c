#define OLI_INPUT
#include "common.h"
#include <oli.c>

/* NOTE: don't change the first 2 lines, the test relies on them */

int alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

FILE* f;
char* desc;
input_t i;

int main() {
  input_from_string(&i, "hello world tHiS is a parser!");
  test_assert(input_any(&i, &desc));
  test_assert(!strcmp(desc, "h"));
  test_assert(input_char(&i, 'e', &desc));
  test_assert(!strcmp(desc, "e"));
  test_assert(!input_char(&i, 'e', &desc));
  test_assert(input_one_of(&i, "lo", &desc));
  test_assert(!strcmp(desc, "l"));
  test_assert(input_one_of(&i, "lo", &desc));
  test_assert(!strcmp(desc, "l"));
  test_assert(input_one_of(&i, "lo", &desc));
  test_assert(!strcmp(desc, "o"));
  test_assert(!input_one_of(&i, "lo", &desc));
  test_assert(input_none_of(&i, "world", &desc));
  test_assert(!strcmp(desc, " "));
  test_assert(!input_none_of(&i, "world", &desc));
  test_assert(input_one_of(&i, "world", &desc));
  test_assert(!strcmp(desc, "w"));
  test_assert(input_one_of(&i, "world", &desc));
  test_assert(!strcmp(desc, "o"));
  test_assert(input_one_of(&i, "world", &desc));
  test_assert(!strcmp(desc, "r"));
  test_assert(input_one_of(&i, "world", &desc));
  test_assert(!strcmp(desc, "l"));
  test_assert(input_one_of(&i, "world", &desc));
  test_assert(!strcmp(desc, "d"));
  test_assert(!input_range(&i, 'a', 'z', &desc));
  test_assert(input_char(&i, ' ', &desc));
  test_assert(!strcmp(desc, " "));
  test_assert(input_range(&i, 'a', 'z', &desc));
  test_assert(!strcmp(desc, "t"));
  test_assert(!input_range(&i, 'a', 'z', &desc));
  test_assert(input_satisfy(&i, alpha, &desc));
  test_assert(!strcmp(desc, "H"));
  test_assert(input_satisfy(&i, alpha, &desc));
  test_assert(!strcmp(desc, "i"));
  test_assert(input_satisfy(&i, alpha, &desc));
  test_assert(!strcmp(desc, "S"));
  test_assert(input_any(&i, &desc));
  test_assert(!strcmp(desc, " "));
  test_assert(input_string(&i, "is", &desc));
  test_assert(!strcmp(desc, "is"));
  test_assert(!input_string(&i, "memes", &desc));
  test_assert(input_any(&i, &desc));
  test_assert(!strcmp(desc, " "));
  test_assert(input_any(&i, &desc));
  test_assert(!strcmp(desc, "a"));
  test_assert(input_any(&i, &desc));
  test_assert(!strcmp(desc, " "));
  test_assert(!input_string(&i, "parz", &desc));
  test_assert(!input_string(&i, "pear", &desc));
  test_assert(!input_string(&i, "parzer", &desc));
  test_assert(input_string(&i, "parser!", &desc));
  test_assert(!strcmp(desc, "parser!"));
  test_assert(input_eof(&i));
  input_free(&i);

  test_assert(f = fopen("tests/input.c", "rb"));
  input_from_file(&i, f);
  test_assert(input_string(&i, "#define", &desc));
  test_assert(!strcmp(desc, "#define"));
  test_assert(input_any(&i, &desc));
  test_assert(!strcmp(desc, " "));
  test_assert(!input_string(&i, "memes", &desc));
  test_assert(!input_string(&i, "OLE", &desc));
  test_assert(!input_string(&i, "OLI_INBUTT", &desc));
  test_assert(input_string(&i, "OLI_INPUT", &desc));
  test_assert(!strcmp(desc, "OLI_INPUT"));

  return 0;
}

