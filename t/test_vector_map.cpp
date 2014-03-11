// vim: sw=3 ts=8 et
#include "ttl/vector_map.hpp"
#include "t.hpp"

template class ttl::vector_map<char, int>;

static void print_map(const char *s, const ttl::vector_map<char, int> &m)
{
   fputs(s, stdout);
   for (ttl::vector_map<char, int>::const_iterator i = m.cbegin(); i != m.cend(); ++i)
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
   printf("sizeof vector_map<int,int>: %lu\n", (unsigned long)sizeof(ttl::vector_map<int, int>));
   printf("sizeof vector_map<char,int>: %lu\n", (unsigned long)sizeof(ttl::vector_map<char, int>));
   ttl::vector_map<char, int> m;
   printf("size: %lu\n", (unsigned long)m.size());
   ttl::vector_map<char, int> m1 = m;
   ttl::vector_map<char, int> m2;
   ttl::vector_map<char, int>();
   ttl::vector_map<char, int>();
   m1[(char)'1'] = 1;
   m1[(char)'2'] = 2;
   m1[(char)'3'] = 3;
   m1[(char)'4'] = 4;
   m1[(char)'5'] = 5;
   m1[(char)255] = 255;
   printf("size: %lu\n", (unsigned long)m1.size());
   {
      ttl::vector_map<char,int> m1copy = m1;
      assert(m1copy.size() == m1.size());
      assert(ttl::equal(m1.cbegin(), m1.cend(), m1copy.cbegin()));
      printf("size of a copy: %lu\n", (unsigned long)m1copy.size());
   }
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
   m1.push_back(ttl::vector_map<char,int>::value_type('x','x'));
   m1.push_back(ttl::vector_map<char,int>::value_type('y','y'));
   m1.push_back(ttl::vector_map<char,int>::value_type('z','z'));
   while (m1.size())
   {
      printf("%d:%d ", m1.end()[-1].first, m1.end()[-1].second);
      assert(m1.at(m1.end()[-1].first) == m1.end()[-1].second);
      m1.pop_back();
   }
   printf("\ndtors\n");
}
