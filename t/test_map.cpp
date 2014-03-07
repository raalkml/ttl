// vim: sw=3 ts=8 et
#include "t.hpp"
#include <map>
#include "ttl/map.hpp"

// Explicit template instantiation will instantiate complete template
// template class ttl::map<int, char>;
// template class std::map<int, char>;

typedef ttl::map<int, char> i2cmap;

template <class C> inline const C &constify(C &c) { return c; }

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

   assert(m.empty() == false);
   // assert(m.size() == 10);
   assert(m.max_size() > m.size());

   printf("iterator && const_iterator\n");
   {
      int i = 0;
      for (i2cmap::iterator it = m.begin(); it != m.end(); ++it, ++i)
      {
         assert(it->first == i);
         assert(it->second == (char)i + '0');
      }
      i = 0;
      for (i2cmap::const_iterator it = constify(m).begin(); it != constify(m).end(); ++it, ++i)
      {
         assert(it->first == i);
         assert(it->second == (char)i + '0');
      }
   }
   for (i2cmap::const_iterator it = m.cbegin(); it != m.cend(); ++it)
      printf(" {%d: 0x%02x}", it->first, (unsigned char)it->second);
   printf("\n");

   printf("comparing the values with std STL\n");
   {
      std::map<int,char> mstd;
      for (int i = 0; i < 5; ++i)
         assert(mstd.insert(std::map<int,char>::value_type(i, (char)i + '0')).second == true);
      for (int i = 5; i < 10; ++i)
         mstd[i] = (char)i + '0';
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

}
