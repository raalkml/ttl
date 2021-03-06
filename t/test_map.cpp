// vim: sw=3 ts=8 et
#include "t.hpp"
#include <map>
#include <utility>
#include "ttl/algorithm.hpp"
#include "ttl/utility.hpp"
#include "ttl/array.hpp"
#include "ttl/map.hpp"

// Explicit template instantiation will instantiate complete template
// template class ttl::map<int, char>;
// template class std::map<int, char>;

typedef ttl::map<int, char> i2cmap;


namespace ttl
{
   inline bool operator==(const std::map<int,char>::value_type &a,
                          const map<int,char>::value_type &b)
   {
      return a.first == b.first && a.second == b.second;
   }

   inline bool operator==(const pair<const int,char> &a, const pair<int,char> &b)
   {
      return a.first == b.first && a.second == b.second;
   }
}

static void test_iterators(i2cmap &m)
{
   printf("iterator && const_iterator\n");
   // all iterators are different
   i2cmap empty;
   assert(empty.begin()  != i2cmap().begin());
   assert(empty.cbegin() != i2cmap().cbegin());
   assert(empty.end()  != i2cmap().end());
   assert(empty.cend() != i2cmap().cend());

   // iteration through empty containers must not execute the loop body
   for (i2cmap::iterator it = empty.begin(); it != empty.end(); ++it)
      assert(0);
   for (i2cmap::iterator it = empty.end(); it-- != empty.begin();)
      assert(0);
   for (i2cmap::const_iterator it = constify(empty).begin(); it != constify(empty).end(); ++it)
      assert(0);
   for (i2cmap::const_iterator it = constify(empty).end(); it-- != constify(empty).begin();)
      assert(0);
   for (i2cmap::const_iterator it = empty.cbegin(); it != empty.cend(); ++it)
      assert(0);
   for (i2cmap::const_iterator it = empty.cend(); it-- != empty.cbegin();)
      assert(0);

   {
      i2cmap::iterator it1 = m.begin();
      i2cmap::iterator it2 = ++it1;
      assert(it1++ == it2);
      assert(--it1 == it2);
      assert(--it1 == --it2);
      ++it1;
      ++it2;
      assert(it1 == it2--);
      assert(m.begin() == it2);
   }
   {
      i2cmap::const_iterator it1 = m.cbegin();
      i2cmap::const_iterator it2 = ++it1;
      assert(it1++ == it2);
      assert(--it1 == it2);
      assert(--it1 == --it2);
      ++it1;
      ++it2;
      assert(it1 == it2--);
      assert(m.cbegin() == it2);
   }
   int i = 0;
   for (i2cmap::iterator it = m.begin(); it != m.end(); ++it, ++i)
   {
      assert(it->first == i);
      assert(it->second == (char)i + '0');
   }
   for (i2cmap::iterator it = m.end(); it-- != m.begin();)
   {
      --i;
      assert(it->first == i);
      assert(it->second == (char)i + '0');
   }
   i = 0;
   for (i2cmap::const_iterator it = constify(m).begin(); it != constify(m).end(); ++it, ++i)
   {
      assert(it->first == i);
      assert(it->second == (char)i + '0');
   }
   for (i2cmap::const_iterator it = constify(m).end(); it-- != constify(m).begin();)
   {
      --i;
      assert(it->first == i);
      assert(it->second == (char)i + '0');
   }
   i = 0;
   for (i2cmap::const_iterator it = m.cbegin(); it != m.cend(); ++it, ++i)
   {
      assert(it->first == i);
      assert(it->second == (char)i + '0');
   }
   for (i2cmap::const_iterator it = m.cend(); it-- != m.cbegin();)
   {
      --i;
      assert(it->first == i);
      assert(it->second == (char)i + '0');
   }
   for (i2cmap::const_iterator it = m.cbegin(); it != m.cend(); ++it)
      printf(" {%d: 0x%02x}", it->first, (unsigned char)it->second);
   printf("\n");
   for (i2cmap::const_iterator it = m.cend(); it-- != m.cbegin();)
      printf(" {%d: 0x%02x}", it->first, (unsigned char)it->second);
   printf("\n");
}

void test()
{
   i2cmap m;
   printf("sizeof(int-char map): %lu\n", (unsigned long)sizeof(m));
   assert(m.empty() == true);

   printf("insert(value_type)\n");
   assert(i2cmap().insert(i2cmap::value_type(1, '1')).first->second == '1');
   for (int i = 0; i < 5; ++i)
      assert(m.insert(i2cmap::value_type(i, (char)i + '0')).second == true);
   printf("operator[](key) = value\n");
   for (int i = 5; i < 10; ++i)
      m[i] = (char)i + '0';

   assert(m.insert(i2cmap::value_type(5, (char)5 + '0')).second == false); // unique keys
   assert(m.insert(i2cmap::value_type(5, (char)5 + '0')).first->first == 5); // the blocking element

   assert(m.empty() == false);
   // assert(m.size() == 10);
   // assert(m.max_size() > m.size());
   assert(m.max_size() > 0);

   test_iterators(m);

   printf("comparing the values with std STL\n");
   {
      std::map<int,char> mstd;
      for (int i = 0; i < 5; ++i)
      {
         std::map<int,char>::value_type tmp(i, (char)i + '0');
         std::pair<std::map<int,char>::iterator, bool> re = mstd.insert(tmp);
         assert(re.second == true);
         // test the behaviour expected from ttl::map
         re = mstd.insert(tmp);
         assert(re.first->first == i);
         assert(re.second == false);
      }
      for (int i = 5; i < 10; ++i)
         mstd[i] = (char)i + '0';

      assert(ttl::equal(mstd.begin(), mstd.end(), m.cbegin()));

      std::map<int,char>::const_iterator istd = constify(mstd).begin();
      i2cmap::const_iterator ittl = constify(m).begin();
      while (istd != mstd.end())
      {
         assert(istd->first == ittl->first);
         assert(istd->second == ittl->second);
         ++istd;
         ++ittl;
      }
   }

   printf("at(key)\n");
   assert(m.at(0) == '0');
   assert(constify(m).at(1) == '1');

   assert(m.find(9) == constify(m).find(9));

   printf("erase(key)\n");
   i2cmap().erase(0);
   assert(m.erase(9) == 1);
   assert(m.find(9) == m.end());

   printf("ranges\n");
   i2cmap().lower_bound(0);
   i2cmap().upper_bound(0);
   i2cmap().equal_range(0);
   assert(m.lower_bound(2)->second == '2');
   assert(m.upper_bound(2)->first == 3);
   assert(m.equal_range(3).first == m.lower_bound(3));
   assert(m.equal_range(3).second == m.upper_bound(3));
   assert(*++m.equal_range(3).first == i2cmap::value_type(4,'4'));
   assert(m.equal_range(0).first == m.begin());
   assert(m.equal_range(8).second == m.end());

   printf("clear()\n");
   i2cmap().clear();
   m.clear();

   printf("copy constructors and assignment operator\n");
   {
      ttl::array<ttl::pair<int, char>, 3> arr;
      arr[0] = ttl::pair<int, char>(100,'\x64');
      arr[1] = ttl::pair<int, char>(101,'\x65');
      arr[2] = ttl::pair<int, char>(102,'\x66');

      i2cmap ma(arr.begin(), arr.end());
      assert(ttl::equal(ma.cbegin(), ma.cend(), arr.cbegin()));

      i2cmap mb = ma;
      assert(ttl::equal(mb.cbegin(), mb.cend(), arr.cbegin()));
      assert(ma == mb);

      i2cmap mc;
      mc = mb;
      assert(ttl::equal(mc.cbegin(), mc.cend(), arr.cbegin()));
      assert(!(mb != mc));

      for (i2cmap::const_iterator it = mc.cbegin(); it != mc.cend(); ++it)
         printf(" {%d: 0x%02x}", it->first, (unsigned char)it->second);
      printf("\n");
   }
   printf("range insert\n");
   {
      ttl::array<ttl::pair<int, char>, 3> arr;
      arr[0] = ttl::pair<int, char>(10,-'\x0a');
      arr[1] = ttl::pair<int, char>(11,-'\x0b');
      arr[2] = ttl::pair<int, char>(12,-'\x0c');
      i2cmap ma;
      ma.insert(arr.begin(), arr.end());
      assert(ttl::equal(ma.cbegin(), ma.cend(), arr.cbegin()));
      for (i2cmap::const_iterator it = ma.cbegin(); it != ma.cend(); ++it)
         printf(" {%d: 0x%02x}", it->first, (unsigned char)it->second);
      printf("\n");
   }
}
