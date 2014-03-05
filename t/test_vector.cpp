// vim: sw=3 ts=8 et
#include "t.hpp"

template class ttl::vector<testtype>;

void test()
{
   typedef ttl::vector<testtype> testvector;

   printf("default ctor:\n");
   ttl::vector<testtype> v0;
   v0.resize(3, testtype(2));
   v0.resize(1);
   v0.insert(v0.begin(), testtype(0));
   for (ttl::vector<testtype>::const_iterator i = v0.cbegin(); i != v0.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");

   printf("\nvalue init ctor:\n");
   testvector v1(10, testtype(-10));

   printf("\nrw iterator\n");
   for (ttl::vector<testtype>::iterator i = v1.begin(); i != v1.end(); ++i)
      *i = testtype(i - v1.begin());

   printf("\ncopy ctor:\n");
   ttl::vector<testtype> v2(v1);
   v2.resize(12);
   v2.erase(v2.begin());
   v2.erase(v2.end() - 1);
   for (ttl::vector<testtype>::const_iterator i = v2.cbegin(); i != v2.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");

   v0.swap(v2);
   for (ttl::vector<testtype>::const_iterator i = v2.cbegin(); i != v2.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");

   printf("\noperator=:\n");
   v1 = v0;
   printf("equal? %s\n", v1 == v0 ? "yes": "no");
   for (ttl::vector<testtype>::const_iterator i = v1.cbegin(); i != v1.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");
   v1 = ttl::vector<testtype>();

   printf("\nassign:\n");
   v1.assign(5, testtype(100));
   for (ttl::vector<testtype>::const_iterator i = v1.cbegin(); i != v1.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");
   static const int in[] = {1,2,3,4,5};
   v1.assign(in, in + countof(in));
   v1.insert(v1.begin() + 1, in, in + countof(in));

   {
      printf("\nctor(first, last):\n");
      ttl::vector<testtype>(in, in + countof(in));
      testvector v(in, in + countof(in));
      assert(v[0].value == 1 &&
             v[1].value == 2 &&
             v[2].value == 3 &&
             v[3].value == 4 &&
             v[4].value == 5 &&
             v.size() == 5);
   }

   {
      testvector v;
      v.push_back(999);
      assert(v.size() == 1 && v[0].value == 999);
      v.pop_back();
      assert(v.empty());
   }
   printf("\ndestructors:\n");
}
