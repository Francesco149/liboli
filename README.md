personal single-header c89 library with misc utilities

no dependencies other than the standard c library

I actually made this just so that I can add ```-loli``` to ldflags

some stuff such as the memory arena and string interning is inspired
by https://github.com/pervognsen/bitwise

windows build scripts coming when I feel like booting up a windows vm

for documentation, read the header portion of ```oli.c```. you could
also take a look at the tests for concrete examples

# building the shared library
just run ```./build``` . it's aware of ```CC```, ```CFLAGS```,
```LDFLAGS``` in case you need to override anything

this will produce liboli.so which you can link with ```-loli```

```
#include <oli.c>
...
```

remember to add oli.c to your project or your include paths as well as
placing liboli.so in your ld paths (such as /usr/lib)

# static build
drop oli.c into your project and
```
#define OLI_IMPLEMENTATION
#include <oli.c>
...
```
no need to mess with linker flags, this will include the implementation
directly into your file

you could also build it as a static library but it's not recommended as
you won't be able to toggle unneeded features off unless you make a
custom build for each project. if you really want to link this like
a normal library just build the shared library

# license
this is free and unencumbered software released into the public domain.
refer to the attached UNLICENSE or http://unlicense.org/
