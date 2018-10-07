#include "common.h"
#define OLI_STRING
#define OLI_HASHMAP
#include <oli.c>
#include <stdio.h>

char data[4096];

void init_data() {
  char* p = data;
  p += sprintf(p, "%s", " \
okay, asking somebody how long they believed in santa claus is so stupid, \
you can't even consider the topic suitable for idle conversation. but if \
you still wanna know how long i believed in some old fat guy who wears a \
funky red suit, i can tell you this: i've never believed in him, ever. \
the santa that showed up at my kindergarten christmas festival, i knew he \
was fake. and i never saw mommy kissing santa or anything. but i have to \
");
  p += sprintf(p, "%s", " \
say, that even as a little kid, i knew better than to believe in some old \
man that only worked one day a year. now, having said that, it wasn't \
until i got older that i realized that aliens, time travelers, ghosts, \
monsters, espers, the evil syndicates and the anime/manga/fantasy flick \
heroes that fight said evil syndicates, were also fake. okay, i guess i \
always knew those things were bogus, i just didn't wanna admit it. all \
");
  p += sprintf(p, "%s", " \
i ever wanted was for an alien, time traveler, ghost, monster, esper, \
evil syndicate, or the hero that fought them to just appear and say \
\"hey\". unfortunately, reality is a hard road indeed. yep, you gotta \
admit, the laws of physics definitely puts a damper on things. i even \
stopped watching those tv shows about aliens and ghosts and stuff. \
aliens, time travelers, espers; of course they don't exist, but a little \
");
  p += sprintf(p, "%s", " \
part of me wishes that they did. i guess i've grown up and realized i can \
think about those things and still accept reality. but by the time i got \
out of junior high, i pretty much outgrew that kind of stuff and i guess \
i got used to the idea of living in an ordinary world. just like that, i \
was in high school... that's when i met her.");
}

string_t s;
hashmap_t hashmap;
char* boundary = " ,.:;?!\"\n\r";

void run(int m) {
  int unique_words;
  hashmap_pair_t* it;
  string_from_c(&s, data);
  for (;;) {
    int len;
    int n;
    s.start += string_spn(&s, boundary);
    len = string_cspn(&s, boundary);
    if (!len) {
      break;
    }
    n = hashmap_geti(&hashmap, s.start, len);
    test_assert(hashmap_seti(&hashmap, s.start, len, n + 1));
    s.start += len;
  }
  unique_words = 0;
  for (it = hashmap.pairs; it; it = it->next) {
    ++unique_words;
  }
  #define t(x, n) test_assert(hashmap_geti(&hashmap, x, strlen(x)) == n)
  t("okay", 2*m);
  t("asking", 1*m);
  t("somebody", 1*m);
  t("how", 2*m);
  t("long", 2*m);
  t("they", 3*m);
  t("believed", 3*m);
  t("in", 6*m);
  t("santa", 3*m);
  t("claus", 1*m);
  t("is", 2*m);
  t("so", 1*m);
  t("stupid", 1*m);
  t("you", 4*m);
  t("can't", 1*m);
  t("even", 3*m);
  t("consider", 1*m);
  t("the", 8*m);
  t("topic", 1*m);
  t("suitable", 1*m);
  t("for", 2*m);
  t("idle", 1*m);
  t("conversation", 1*m);
  t("but", 4*m);
  t("if", 1*m);
  t("you", 4*m);
  t("still", 2*m);
  t("wanna", 2*m);
  t("know", 1*m);
  t("how", 2*m);
  t("long", 2*m);
  t("i", 21*m);
  t("believed", 3*m);
  t("in", 6*m);
  t("some", 2*m);
  t("old", 2*m);
  t("fat", 1*m);
  t("guy", 1*m);
  t("who", 1*m);
  t("wears", 1*m);
  t("a", 6*m);
  t("funky", 1*m);
  t("red", 1*m);
  t("suit", 1*m);
  t("i", 21*m);
  t("can", 2*m);
  t("tell", 1*m);
  t("you", 4*m);
  t("this", 1*m);
  t("i've", 2*m);
  t("never", 2*m);
  t("believed", 3*m);
  t("in", 6*m);
  t("him", 1*m);
  t("ever", 2*m);
  t("the", 8*m);
  t("santa", 3*m);
  t("that", 11*m);
  t("showed", 1*m);
  t("up", 2*m);
  t("at", 1*m);
  t("my", 1*m);
  t("kindergarten", 1*m);
  t("christmas", 1*m);
  t("festival", 1*m);
  t("i", 21*m);
  t("knew", 3*m);
  t("he", 1*m);
  t("was", 3*m);
  t("fake", 2*m);
  t("and", 8*m);
  t("i", 21*m);
  t("never", 2*m);
  t("saw", 1*m);
  t("mommy", 1*m);
  t("kissing", 1*m);
  t("santa", 3*m);
  t("or", 2*m);
  t("anything", 1*m);
  t("but", 4*m);
  t("i", 21*m);
  t("have", 1*m);
  t("to", 4*m);
  t("say", 2*m);
  t("that", 11*m);
  t("even", 3*m);
  t("as", 1*m);
  t("a", 6*m);
  t("little", 2*m);
  t("kid", 1*m);
  t("i", 21*m);
  t("knew", 3*m);
  t("better", 1*m);
  t("than", 1*m);
  t("to", 4*m);
  t("believe", 1*m);
  t("in", 6*m);
  t("some", 2*m);
  t("old", 2*m);
  t("man", 1*m);
  t("that", 11*m);
  t("only", 1*m);
  t("worked", 1*m);
  t("one", 1*m);
  t("day", 1*m);
  t("a", 6*m);
  t("year", 1*m);
  t("now", 1*m);
  t("having", 1*m);
  t("said", 2*m);
  t("that", 11*m);
  t("it", 2*m);
  t("wasn't", 1*m);
  t("until", 1*m);
  t("i", 21*m);
  t("got", 3*m);
  t("older", 1*m);
  t("that", 11*m);
  t("i", 21*m);
  t("realized", 2*m);
  t("that", 11*m);
  t("aliens", 3*m);
  t("time", 4*m);
  t("travelers", 2*m);
  t("ghosts", 2*m);
  t("monsters", 1*m);
  t("espers", 2*m);
  t("the", 8*m);
  t("evil", 3*m);
  t("syndicates", 2*m);
  t("and", 8*m);
  t("the", 8*m);
  t("anime/manga/fantasy", 1*m);
  t("flick", 1*m);
  t("heroes", 1*m);
  t("that", 11*m);
  t("fight", 1*m);
  t("said", 2*m);
  t("evil", 3*m);
  t("syndicates", 2*m);
  t("were", 2*m);
  t("also", 1*m);
  t("fake", 2*m);
  t("okay", 2*m);
  t("i", 21*m);
  t("guess", 3*m);
  t("i", 21*m);
  t("always", 1*m);
  t("knew", 3*m);
  t("those", 3*m);
  t("things", 3*m);
  t("were", 2*m);
  t("bogus", 1*m);
  t("i", 21*m);
  t("just", 3*m);
  t("didn't", 1*m);
  t("wanna", 2*m);
  t("admit", 2*m);
  t("it", 2*m);
  t("all", 1*m);
  t("i", 21*m);
  t("ever", 2*m);
  t("wanted", 1*m);
  t("was", 3*m);
  t("for", 2*m);
  t("an", 2*m);
  t("alien", 1*m);
  t("time", 4*m);
  t("traveler", 1*m);
  t("ghost", 1*m);
  t("monster", 1*m);
  t("esper", 1*m);
  t("evil", 3*m);
  t("syndicate", 1*m);
  t("or", 2*m);
  t("the", 8*m);
  t("hero", 1*m);
  t("that", 11*m);
  t("fought", 1*m);
  t("them", 1*m);
  t("to", 4*m);
  t("just", 3*m);
  t("appear", 1*m);
  t("and", 8*m);
  t("say", 2*m);
  t("hey", 1*m);
  t("unfortunately", 1*m);
  t("reality", 2*m);
  t("is", 2*m);
  t("a", 6*m);
  t("hard", 1*m);
  t("road", 1*m);
  t("indeed", 1*m);
  t("yep", 1*m);
  t("you", 4*m);
  t("gotta", 1*m);
  t("admit", 2*m);
  t("the", 8*m);
  t("laws", 1*m);
  t("of", 6*m);
  t("physics", 1*m);
  t("definitely", 1*m);
  t("puts", 1*m);
  t("a", 6*m);
  t("damper", 1*m);
  t("on", 1*m);
  t("things", 3*m);
  t("i", 21*m);
  t("even", 3*m);
  t("stopped", 1*m);
  t("watching", 1*m);
  t("those", 3*m);
  t("tv", 1*m);
  t("shows", 1*m);
  t("about", 2*m);
  t("aliens", 3*m);
  t("and", 8*m);
  t("ghosts", 2*m);
  t("and", 8*m);
  t("stuff", 2*m);
  t("aliens", 3*m);
  t("time", 4*m);
  t("travelers", 2*m);
  t("espers", 2*m);
  t("of", 6*m);
  t("course", 1*m);
  t("they", 3*m);
  t("don't", 1*m);
  t("exist", 1*m);
  t("but", 4*m);
  t("a", 6*m);
  t("little", 2*m);
  t("part", 1*m);
  t("of", 6*m);
  t("me", 1*m);
  t("wishes", 1*m);
  t("that", 11*m);
  t("they", 3*m);
  t("did", 1*m);
  t("i", 21*m);
  t("guess", 3*m);
  t("i've", 2*m);
  t("grown", 1*m);
  t("up", 2*m);
  t("and", 8*m);
  t("realized", 2*m);
  t("i", 21*m);
  t("can", 2*m);
  t("think", 1*m);
  t("about", 2*m);
  t("those", 3*m);
  t("things", 3*m);
  t("and", 8*m);
  t("still", 2*m);
  t("accept", 1*m);
  t("reality", 2*m);
  t("but", 4*m);
  t("by", 1*m);
  t("the", 8*m);
  t("time", 4*m);
  t("i", 21*m);
  t("got", 3*m);
  t("out", 1*m);
  t("of", 6*m);
  t("junior", 1*m);
  t("high", 2*m);
  t("i", 21*m);
  t("pretty", 1*m);
  t("much", 1*m);
  t("outgrew", 1*m);
  t("that", 11*m);
  t("kind", 1*m);
  t("of", 6*m);
  t("stuff", 2*m);
  t("and", 8*m);
  t("i", 21*m);
  t("guess", 3*m);
  t("i", 21*m);
  t("got", 3*m);
  t("used", 1*m);
  t("to", 4*m);
  t("the", 8*m);
  t("idea", 1*m);
  t("of", 6*m);
  t("living", 1*m);
  t("in", 6*m);
  t("an", 2*m);
  t("ordinary", 1*m);
  t("world", 1*m);
  t("just", 3*m);
  t("like", 1*m);
  t("that", 11*m);
  t("i", 21*m);
  t("was", 3*m);
  t("in", 6*m);
  t("high", 2*m);
  t("school", 1*m);
  t("that's", 1*m);
  t("when", 1*m);
  t("i", 21*m);
  t("met", 1*m);
  t("her", 1*m);
  test_assert(unique_words == 173);
}

int stupid_hash(void* data, int data_len) {
  (void)data;
  (void)data_len;
  return 1337;
}

int stupid_hash2(void* data, int data_len) {
  char* s = data;
  (void)data_len;
  return s[0];
}

int main() {
  int i;
  init_data();

  logs("default hash");
  for (i = 1; i < 10000; ++i) {
    run(i);
  }
  test_assert(hashmap.n_collisions == 0);
  hashmap_free(&hashmap);

  logs("stupid hash");
  memset(&hashmap, 0, sizeof(hashmap));
  hashmap.hash = stupid_hash;
  for (i = 1; i < 100; ++i) {
    run(i);
  }
  test_assert(hashmap.n_collisions == 172);

  logs("stupid hash 2");
  memset(&hashmap, 0, sizeof(hashmap));
  hashmap.hash = stupid_hash2;
  for (i = 1; i < 100; ++i) {
    run(i);
  }
  test_assert(hashmap.n_collisions == 151);

  return 0;
}
