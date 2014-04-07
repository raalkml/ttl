// vim: sw=3 ts=8 et
#include "t.hpp"
#include <set>
#include <utility>
#include "ttl/algorithm.hpp"
#include "ttl/utility.hpp"
#include "ttl/array.hpp"
#include "ttl/set.hpp"

// Explicit template instantiation will instantiate complete template
// template class ttl::set<int, char>;

typedef ttl::set<int> intset;


static void test_iterators(intset &s)
{
   printf("iterator && const_iterator\n");
   // all iterators are different
   intset empty;
   assert(empty.begin()  != intset().begin());
   assert(empty.cbegin() != intset().cbegin());
   assert(empty.end()  != intset().end());
   assert(empty.cend() != intset().cend());

   // iteration through empty containers must not execute the loop body
   for (intset::iterator it = empty.begin(); it != empty.end(); ++it)
      assert(0);
   for (intset::iterator it = empty.end(); it-- != empty.begin();)
      assert(0);
   for (intset::const_iterator it = constify(empty).begin(); it != constify(empty).end(); ++it)
      assert(0);
   for (intset::const_iterator it = constify(empty).end(); it-- != constify(empty).begin();)
      assert(0);
   for (intset::const_iterator it = empty.cbegin(); it != empty.cend(); ++it)
      assert(0);
   for (intset::const_iterator it = empty.cend(); it-- != empty.cbegin();)
      assert(0);

   {
      intset::iterator it1 = s.begin();
      intset::iterator it2 = ++it1;
      assert(it1++ == it2);
      assert(--it1 == it2);
      assert(--it1 == --it2);
      ++it1;
      ++it2;
      assert(it1 == it2--);
      assert(s.begin() == it2);
   }
   {
      intset::const_iterator it1 = s.cbegin();
      intset::const_iterator it2 = ++it1;
      assert(it1++ == it2);
      assert(--it1 == it2);
      assert(--it1 == --it2);
      ++it1;
      ++it2;
      assert(it1 == it2--);
      assert(s.cbegin() == it2);
   }
   int i = 0;
   for (intset::iterator it = s.begin(); it != s.end(); ++it, ++i)
      assert(*it == i);

   for (intset::iterator it = s.end(); it-- != s.begin();)
   {
      --i;
      assert(*it == i);
   }
   i = 0;
   for (intset::const_iterator it = constify(s).begin(); it != constify(s).end(); ++it, ++i)
      assert(*it == i);

   for (intset::const_iterator it = constify(s).end(); it-- != constify(s).begin();)
   {
      --i;
      assert(*it == i);
   }
   i = 0;
   for (intset::const_iterator it = s.cbegin(); it != s.cend(); ++it, ++i)
      assert(*it.operator->() == i);
   for (intset::const_iterator it = s.cend(); it-- != s.cbegin();)
   {
      --i;
      assert(*it.operator->() == i);
   }
   for (intset::const_iterator it = s.cbegin(); it != s.cend(); ++it)
      printf(" {%d}", *it);
   printf("\n");
   for (intset::const_iterator it = s.cend(); it-- != s.cbegin();)
      printf(" {%d}", *it.operator->());
   printf("\n");
}

void test()
{
   intset s;
   printf("sizeof(set of int): %lu\n", (unsigned long)sizeof(s));
   assert(s.empty() == true);

   printf("insert()\n");
   assert(*(intset().insert(1).first) == 1);
   for (int i = 0; i <= 5; ++i)
      assert(s.insert(i).second == true);

   assert(s.insert(5).second == false); // unique keys
   assert(*s.insert(5).first == 5); // the blocking element

   assert(s.empty() == false);
   // assert(s.size() == 10);
   // assert(s.max_size() > s.size());
   assert(s.max_size() > 0);

   test_iterators(s);

   printf("comparing the values with std STL\n");
   {
      std::set<int> sstd;
      for (int i = 0; i <= 5; ++i)
      {
         std::pair<std::set<int>::iterator, bool> re = sstd.insert(i);
         assert(re.second == true);
         // test the behaviour expected from ttl::map
         re = sstd.insert(i);
         assert(*re.first == i);
         assert(re.second == false);
      }

      assert(ttl::equal(sstd.begin(), sstd.end(), s.cbegin()));

      std::set<int>::const_iterator istd = constify(sstd).begin();
      intset::const_iterator ittl = constify(s).begin();
      while (istd != sstd.end())
      {
         assert(*istd == *ittl);
         ++istd;
         ++ittl;
      }
   }

   for (int i = 6; i <= 9; ++i)
      assert(s.insert(i).second == true);

   assert(s.find(9) == constify(s).find(9));
   assert(*s.find(9) == 9);

   printf("erase(key)\n");
   intset().erase(0);
   assert(s.erase(9) == 1);
   assert(s.find(9) == s.end());

   printf("ranges\n");
   intset().lower_bound(0);
   intset().upper_bound(0);
   intset().equal_range(0);
   assert(*s.lower_bound(2) == 2);
   assert(*s.upper_bound(2) == 3);

   ttl::pair<intset::iterator, intset::iterator> range = s.equal_range(3);
   assert(range.first == s.lower_bound(3));
   assert(range.second == s.upper_bound(3));
   assert(*++range.first == 4);

   assert(s.equal_range(0).first == s.begin());
   assert(s.equal_range(8).second == s.end());

   printf("clear()\n");
   intset().clear();
   s.clear();

   printf("copy constructors and assignment operator\n");
   {
      ttl::array<int, 3> arr = { {100, 101, 102} };

      intset sa(arr.begin(), arr.end());
      assert(ttl::equal(sa.cbegin(), sa.cend(), arr.cbegin()));

      intset sb = sa;
      assert(ttl::equal(sb.cbegin(), sb.cend(), arr.cbegin()));

      intset sc;
      sc = sb;
      assert(ttl::equal(sc.cbegin(), sc.cend(), arr.cbegin()));

      for (intset::const_iterator it = sc.cbegin(); it != sc.cend(); ++it)
         printf(" {%d}", *it);
      printf("\n");
   }
   printf("range insert\n");
   {
      const ttl::array<int, 3> arr = { {10, 11, 12} };
      intset sa;
      sa.insert(arr.begin(), arr.end());
      assert(ttl::equal(sa.cbegin(), sa.cend(), arr.cbegin()));
      for (intset::const_iterator it = sa.cbegin(); it != sa.cend(); ++it)
         printf(" {%d}", *it);
      printf("\n");
   }
}
