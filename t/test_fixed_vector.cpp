// vim: sw=3 ts=8 et
#include "ttl/utility.hpp"
#include "ttl/algorithm.hpp"
#include "ttl/fixed_vector.hpp"
#include "t.hpp"

void test()
{
   typedef ttl::fixed_vector<testtype, 16> testvector;

   printf("default ctor:\n");
   testvector v0;
   testtype::verbose = false;

   v0.resize(3, testtype(2));
   assert(v0.size() == 3);
   v0.resize(1);
   assert(v0.size() == 1);
   v0.insert(v0.begin(), testtype(0));
   assert(v0.size() == 2 && v0[0] == 0 && v0[1] == 2);
   for (testvector::const_iterator i = v0.cbegin(); i != v0.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");
   testvector vCopy = v0;
   printf("size of a copy: %lu = { %d, %d }\n", (unsigned long)vCopy.size(), vCopy[0].value, vCopy[1].value);
   assert(vCopy == v0);

   printf("\nvalue init ctor:\n");
   testvector v1(10, testtype(-10));
   assert(v1.size() == 10 && ttl::count(v1.cbegin(), v1.cend(), testtype(-10)) == 10);
   assert(testvector().size() == 0);
   assert(testvector(0).size() == 0);
   assert(testvector(1).size() == 1);
   assert(testvector(10).size() == 10);
   assert(testvector(10, testtype(10)).size() == 10);
   assert(testvector(1, testtype(10))[0] == 10);

   printf("\nrw iterator\n");
   for (testvector::iterator i = v1.begin(); i != v1.end(); ++i)
      *i = testtype(i - v1.begin());
   static const int numbers[] = {0,1,2,3,4,5,6,7,8,9};
   assert(ttl::equal(v1.cbegin(), v1.cend(), numbers));

   printf("\ncopy ctor:\n");
   testvector v2(v1);
   v2.resize(12);
   assert(v2.size() == 12);
   v2.erase(v2.begin());
   assert(v2.size() == 11);
   v2.erase(v2.end() - 1);
   assert(v2.size() == 10);
   for (testvector::const_iterator i = v2.cbegin(); i != v2.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");
   assert(ttl::equal(v2.cbegin(), v2.cend() - 1, numbers + 1));

   v0.swap(v2);
   for (testvector::const_iterator i = v2.cbegin(); i != v2.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");

   printf("\noperator=:\n");
   v1 = v0;
   printf("equal? %s\n", v1 == v0 ? "yes": "no");
   for (testvector::const_iterator i = v1.cbegin(); i != v1.cend(); ++i)
      printf(" %d", i->value);
   printf("\n");
   v1 = testvector();

   printf("\nassign:\n");
   v1.assign(5, testtype(100));
   for (testvector::const_iterator i = v1.cbegin(); i != v1.cend(); ++i)
   {
      printf(" %d", i->value);
      assert(i->value == 100);
   }
   for (testvector::const_iterator i = constify(v1).begin(); i != constify(v1).end(); ++i)
      assert(i->value == 100);
   printf("\n");
   static const int in[] = {1,2,3,4,5};
   v1.assign(in, in + countof(in));
   assert(v1[0].value == 1 &&
          v1[1].value == 2 &&
          v1[2].value == 3 &&
          v1[3].value == 4 &&
          v1[4].value == 5 &&
          v1.size() == 5);
   v1.insert(v1.begin() + 1, in, in + countof(in));

   {
      printf("\nctor(first, last):\n");
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
   testtype::verbose = true;
}
