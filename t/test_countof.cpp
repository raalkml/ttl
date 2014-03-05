// vim: sw=3 ts=8 et
#include "ttl/utility.hpp"
#include "t.hpp"

void test()
{
   const static int a[] = {1,2,23,4};
   typedef int arr[10];
   struct { int a[10]; } s = { {1,2,2,3} };
   printf("countof %lu\n", (unsigned long)countof(a));
   printf("countof %lu\n", (unsigned long)countof(s.a));
#ifdef TEST_COUNTOF_FAIL
   //printf("countof %u\n", countof(arr));
   printf("countof %u\n", countof(s.a[0]));
#endif
}
