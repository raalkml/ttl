// vim: sw=3 ts=8 et
#include "t.hpp"
#include <new>

void test()
{
   void *p = ::operator new(10 * sizeof(int));
   printf("::operator new %p\n", p);
   ::operator delete(p);
}
