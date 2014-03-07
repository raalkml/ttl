// vim: sw=3 ts=8 et
#include "t.hpp"
#include "ttl/map.hpp"

typedef ttl::map<int, char> i2cmap;

void test()
{
   i2cmap m;
   printf("sizeof(int-char map): %lu\n", (unsigned long)sizeof(m));
}
