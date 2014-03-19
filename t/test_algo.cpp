#include "t.hpp"
#include "ttl/algorithm.hpp"
#include "ttl/utility.hpp"

struct less_than
{
   int v;
   less_than(int v_): v(v_) {}
   bool operator()(int o) const { return o < v; }
};

static void print_ints(const char *title, const int *first, const int *last)
{
   fputs(title, stdout);
   while (first != last)
      printf(" %d", *first++);
   fputs(".\n", stdout);
}

void test()
{
   int a0[] = {1,2,3,4};
   int a1[countof(a0)] = {0};
   ttl::copy(a0, a0 + countof(a0), a1);
   assert(ttl::equal(a0, a0 + countof(a0), a1));
   print_ints("copy:", a1, a1 + countof(a1));

   ttl::fill_n(a1, countof(a1), 0);
   ttl::copy_n(a0, 2, a1);
   assert(a0[0] == a1[0] && a1[2] == 0);
   print_ints("copy_n:", a1, a1 + countof(a1));

   ttl::fill_n(a1, countof(a1), 0);
   ttl::copy_if(a0, a0 + countof(a0), a1, less_than(3));
   assert(a0[0] == a1[0] && a1[2] == 0);
   print_ints("copy_if:", a1, a1 + countof(a1));

   ttl::fill_n(a1, countof(a1), 0);
   ttl::copy_backward(a0 + 2, a0 + countof(a0), a1 + countof(a1));
   assert(a0[2] == a1[2] && a1[0] == 0);
   print_ints("copy_backward:", a1, a1 + countof(a1));

   int a2[] = {1,2,2,3,4};
   print_ints("a2:", a2, a2 + countof(a2));
   int *p = ttl::lower_bound(a2, a2 + countof(a2), 2);
   printf("a2 dup lower_bound: %ld\n", (long)(p - a2));
   p = ttl::upper_bound(a2, a2 + countof(a2), 2);
   printf("a2 dup upper_bound: %ld\n", (long)(p - a2));

   ttl::pair<int *, int *> r = ttl::equal_range(a2, a2 + countof(a2), 2);
   printf("a2 dup equal_range: [%ld,%ld)\n", (long)(r.first - a2), (long)(r.second - a2));
}
