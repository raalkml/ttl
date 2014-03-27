#include "t.hpp"
#include "ttl/utility.hpp"
#include "ttl/functional.hpp"
#include "ttl/array.hpp"
#include "ttl/algorithm.hpp"

struct less_than
{
   int v;
   less_than(int v_): v(v_) {}
   bool operator()(int o) const { return o < v; }
};

struct equal_to
{
   int v;
   equal_to(int v_): v(v_) {}
   bool operator()(int o) const { return o == v; }
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
   int *p;

   static const int a0[] = {1,3,5,7};
   static const int a1[] = {2,4,6,8};
   int result[countof(a0) + countof(a1)];
   p = ttl::merge(a0, a0 + countof(a0), a1, a1 + countof(a1), result);
   print_ints("a0:", a0, a0 + countof(a0));
   print_ints("a1:", a1, a1 + countof(a1));
   print_ints("merge:", result, result + countof(result));
   assert(p == result + countof(result));
   assert(ttl::is_sorted(result, result + countof(result)));

   static const ttl::array<int, countof(a0)> a2 = {{8,10,12,14}};
   p = ttl::merge(a0, a0 + countof(a0), a2.begin(), a2.end(), result);
   assert(ttl::is_sorted(result, result + countof(result)));

   static const ttl::array<int, countof(a0)> a3 = {{1,2,3,4}};
   p = ttl::merge(a3.cbegin(), a3.cend(), a2.cbegin(), a2.cend(), result);
   assert(ttl::is_sorted(result, result + countof(result)));
}
