// vim: sw=3 ts=8 et
#include "ttl/array.hpp"
#include "t.hpp"

void test()
{
   puts("initialization");
   ttl::array<int, 3> i3 = { {1,2,3} };

   for (ttl::array<int, 3>::const_iterator i = i3.begin(); i != i3.end(); ++i)
      printf("%ld/%lu: %d\n", long(i - i3.begin()), (unsigned long)i3.size(), *i);

   puts("iterators");
   {
      ttl::array<int, 3>::iterator j = i3.begin();
      for (ttl::array<int, 3>::const_iterator i = i3.cbegin(); i != i3.cend(); ++i)
         assert(*i == *j++);
      assert(j == i3.end());

      j = i3.begin();
      for (ttl::array<int, 3>::const_iterator i = constify(i3).begin(); i != constify(i3).end(); ++i)
         assert(*i == *j++);
      assert(j == constify(i3).end());
   }

   puts("assignment operator");
   {
      ttl::array<int, 3> i30;
      i30 = i3;
      assert(i3 == i30);

      puts("swap");
      {
         puts("initialization");
         ttl::array<int, 3> i31;
         i31.swap(i3);
         assert(i31 == i30);
      }
   }

   puts("fill");
   {
      ttl::array<testtype, 3> ta;
      ta.fill(testtype(9));
      assert(ta[0].value == 9);
      assert(ta[2] == ta[0]);
      assert(!ta.empty());
      assert(ta.size() == 3);
      assert(ta.max_size() == ta.size());

      puts("assignment and default element ctors");
      {
         ta = ttl::array<testtype, 3>();
         assert(ta[0] == testtype());
         assert(ta[1] == testtype());
         assert(ta[2] == testtype());
      }
   }

   puts("dtors");
}
