// vim: sw=3 ts=8 et
#include <stddef.h>
#include "ttl/ttl.hpp"
#include "t.hpp"

void test()
{
   printf("ttl::size_t %lu\n", (unsigned long)sizeof(ttl::size_t));
   printf("ttl::ptrdiff_t %lu\n", (unsigned long)sizeof(ttl::ptrdiff_t));
   assert(sizeof(ttl::size_t) == sizeof(::size_t));
   assert(sizeof(ttl::ptrdiff_t) == sizeof(::ptrdiff_t));
}
