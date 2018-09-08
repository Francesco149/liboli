#include "common.h"
#define OLI_INTERN
#define OLI_STRING
#include <oli.c>

interns_t intr;
string_t str;
int n;

int main() {
    char* kw_memes = intern_str(&intr, "memes");
    test_assert(kw_memes)
    string_from_c(&str, "foo bar blah 123 memes sdfsd sdfas as asdf a "
        "memes mmm sdfn sfjn wi ksamndka memes memes memes jdfn memes");
    for (; str.start < str.end; ) {
        int len;
        str.start += string_cspn(&str, " ") + 1;
        len = string_cspn(&str, " ");
        if (intern_range(&intr, str.start, str.start + len) == kw_memes) {
            ++n;
        }
    }
    test_assert(n == 6)
    logln("passed");
    return 0;
}
