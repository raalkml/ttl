#include "t.hpp"
#include "ttl/utility.hpp"
#include "ttl/functional.hpp"
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

struct printer
{
   void operator()(const int &v) { printf(" %d", v); }
};

void test()
{
   assert(ttl::min(1,2) == 1);
   assert(ttl::max(1,2) == 2);
   assert(ttl::minmax(1,2) == ttl::minmax(2,1));
   assert(ttl::minmax(2,1).first == 1);
   assert(ttl::minmax(2,1).second == 2);

   int *p;

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

   p = ttl::remove_copy(a0, a0 + countof(a0), a1, 3);
   print_ints("remove_copy(3):", a1, p);
   assert(ttl::find(a1, p, 3) == p);

   p = ttl::remove_copy_if(a0, a0 + countof(a0), a1, less_than(3));
   print_ints("remove_copy_if(< 3):", a1, p);
   assert(ttl::find(a1, p, 1) == p);
   assert(ttl::find(a1, p, 2) == p);
   assert(ttl::find(a1, p, 3) == a1);

   int a2[] = {1,2,2,3,4,6};
   int *a2end = a2 + countof(a2);
   print_ints("a2:", a2, a2end);
   printf("a2 min: %d\n", *ttl::min_element(a2, a2end));
   assert(ttl::min_element(a2, a2end) == a2);
   printf("a2 max: %d\n", *ttl::max_element(a2, a2end));
   assert(ttl::max_element(a2, a2end) == a2end - 1);
   assert(ttl::minmax_element(a2, a2end).first == ttl::min_element(a2, a2end));
   assert(ttl::minmax_element(a2, a2end).second == ttl::max_element(a2, a2end));

   assert(ttl::count(a2, a2end, 2) == 2);
   assert(ttl::count_if(a2, a2end, less_than(2)) == 1);
   assert(ttl::any_of(a2, a2end, equal_to(4)));
   assert(ttl::none_of(a2, a2end, equal_to(5)));
   assert(*ttl::adjacent_find(a2, a2end) == 2);
   assert(*ttl::adjacent_find(a2, a2end, ttl::equal_to<int>()) == 2);

   fputs("for_each", stdout);
#if __cplusplus >= 201103L // C++11
   fputs("(lambda):", stdout);
   ttl::for_each(a2, a2end, [](const int &v){ printf(" %d", v); } );
#else
   fputs("(class inst):", stdout);
   ttl::for_each(a2, a2end, printer());
#endif
   fputs("\n", stdout);

   p = ttl::lower_bound(a2, a2end, 2);
   printf("a2 dup lower_bound: [%ld\n", (long)(p - a2));
   assert(p - a2 == 1 && *p == 2);
   p = ttl::lower_bound(a2, a2end, 5); // missing, in-range
   assert(p == a2 + 5);
   p = ttl::lower_bound(a2, a2end, 7); // missing, out-of-range
   assert(p == a2end);

   p = ttl::upper_bound(a2, a2end, 2);
   printf("a2 dup upper_bound: %ld)\n", (long)(p - a2));
   assert(p - a2 == 3);
   p = ttl::upper_bound(a2, a2end, 5); // missing, in-range
   assert(p == a2 + 5);
   p = ttl::upper_bound(a2, a2end, 7); // missing, out-of-range;
   assert(p == a2end);

   ttl::pair<int *, int *> r = ttl::equal_range(a2, a2end, 2);
   printf("a2 dup equal_range: [%ld,%ld)\n", (long)(r.first - a2), (long)(r.second - a2));
   assert(ttl::all_of(r.first, r.second, equal_to(2)));
   assert(r.second - r.first == 2);
   assert(*r.first == 2);
   assert(*(r.first + 1) == 2);

   r = ttl::equal_range(a2, a2end, 5);
   assert(r.first == r.second);
   assert(r.first == a2 + 5);

   r = ttl::equal_range(a2, a2end, 7);
   assert(r.first == r.second);
   assert(r.first == a2end);

   p = ttl::remove(a2, a2end, 2);
   print_ints("a2 remove 2:", a2, p);
   assert(ttl::find(a2, p, 2) == p);
   assert(ttl::find(a2, p, 6) + 1 == p);
   assert(a2[1] == 3);
   assert(*ttl::find(a2, p, 6) == 6);
   a2end = p;

   p = ttl::remove_if(a2, a2end, less_than(2));
   print_ints("a2 remove less than 2:", a2, p);
   assert(ttl::find(a2, p, 2) == p);
   assert(ttl::find(a2, p, 6) + 1 == p);
   assert(a2[0] == 3);
   assert(*ttl::find(a2, p, 6) == 6);
   a2end = p;

}
