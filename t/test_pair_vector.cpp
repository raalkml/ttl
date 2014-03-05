// vim: sw=3 ts=8 et
#include "ttl/pair_vector.hpp"
#include "t.hpp"

template class ttl::pair_vector<char, int>;

static void print_map(const char *s, const ttl::pair_vector<char, int> &m)
{
   fputs(s, stdout);
   for (ttl::pair_vector<char, int>::const_iterator i = m.cbegin(); i != m.cend(); ++i)
      if (i->first < 0)
         printf("--- = %3d ", i->second);
      else if (i->first < 32)
         printf("... = %3d ", i->second);
      else
         printf("'%c' = %3d ", i->first, i->second);
   fputs(".\n", stdout);
}

void test()
{
   ttl::pair_vector<char, int> m;
   ttl::pair_vector<char, int> m1 = m;
   ttl::pair_vector<char, int> m2;
   ttl::pair_vector<char, int>();
   ttl::pair_vector<char, int>();
   m1[(char)'1'] = 1;
   m1[(char)'2'] = 2;
   m1[(char)'3'] = 3;
   m1[(char)'4'] = 4;
   m1[(char)'5'] = 5;
   m1[(char)255] = 255;
   print_map("[]\n", m1);
   m1.insert(ttl::make_pair((char)'a', (int)0xa));
   m1.insert(ttl::make_pair((char)'b', (int)0xb));
   m1.insert(ttl::make_pair((char)'c', (int)0xc));
   m1.insert(ttl::make_pair((char)'d', (int)0xd));
   m1.insert(ttl::make_pair((char)'e', (int)0xe));
   m1.insert(ttl::make_pair((char)'f', (int)0xf));
   m1.insert(ttl::make_pair((char)255, 255));
   print_map("insert\n", m1);
   //for (char c = 0; (unsigned)c <= 127u; ++c)
   //   m1.insert(m1.end(), ttl::make_pair(c, (int)c));
   //print_map("insert(iterator)\n", m1);
   m1.clear();
   print_map("clear: ", m1);
}
