// vim: sw=3 ts=8 et
#include "t.hpp"

void test()
{
   ttl::array<int, 3> i3 = { {1,2,3} };
   for (ttl::array<int, 3>::const_iterator i = i3.begin(); i != i3.end(); ++i)
      printf("%ld/%d: %d\n", long(i - i3.begin()), i3.size(), *i);
   ttl::array<int, 3> i30;
   i30 = i3;
   printf("equal? %s\n", i3 == i30 ? "yes": "no");
}
