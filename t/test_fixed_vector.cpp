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
   print_iter("after resize v0: ", v0.cbegin(), v0.cend());
   assert(v0.size() == 3);
   v0.resize(1);
   assert(v0.size() == 1);
   v0.insert(v0.begin(), testtype(0));
   assert(v0.size() == 2 && v0[0] == 0 && v0[1] == 2);
   print_iter("after insert v0: ", v0.cbegin(), v0.cend());

   {
      testvector vCopy = v0;
      printf("size of a copy: %lu = { %d, %d }\n", (unsigned long)vCopy.size(), vCopy[0].value, vCopy[1].value);
      assert(vCopy == v0);
   }

   printf("value init (generator) ctor\n");
   testvector v1(10, testtype(-10));
   print_iter("after insert v1: ", v1.cbegin(), v1.cend());
   assert(v1.size() == 10 && ttl::count(v1.cbegin(), v1.cend(), testtype(-10)) == 10);
   assert(testvector().size() == 0);
   assert(testvector(0).size() == 0);
   assert(testvector(1).size() == 1);
   assert(testvector(10).size() == 10);
   assert(testvector(10, testtype(10)).size() == 10);
   assert(testvector(1, testtype(10))[0] == 10);

   printf("rw iterator\n");
   for (testvector::iterator i = v1.begin(); i != v1.end(); ++i)
      *i = testtype(i - v1.begin());
   print_iter("v1: ", v1.cbegin(), v1.cend());
   static const int numbers[] = {0,1,2,3,4,5,6,7,8,9};
   assert(ttl::equal(v1.cbegin(), v1.cend(), numbers));

   printf("copy ctor\n");
   testvector v2(v1);
   v2.resize(12);
   assert(v2.size() == 12);

   v2.erase(v2.begin());
   assert(v2.size() == 11);
   v2.erase(v2.end() - 1);
   assert(v2.size() == 10);
   print_iter("after erase v2: ", v2.cbegin(), v2.cend());
   assert(ttl::equal(v2.cbegin(), v2.cend() - 1, numbers + 1));
   {
      testvector t = v2;
      t.erase(t.begin(), t.end());
      assert(t.empty() && t.size() == 0);

      t = v2;
      t.erase(t.begin() + 1, t.end() - 1);
      assert(t[0] == v2.front() && t[1] == v2.back() && t.size() == 2);

      t = v2;
      t.erase(t.begin(), t.begin() + t.size() / 2);
      assert(ttl::equal(t.begin(), t.end(), v2.begin() + t.size(), v2.end()));

      t = v2;
      t.erase(t.begin() + t.size() / 2, t.end());
      assert(ttl::equal(t.begin(), t.end(), v2.begin(), v2.begin() + t.size()));
   }

   {
      testvector t0 = v0, t2 = v2;
      v0.swap(v2);
      print_iter("after swap v0: ", v0.cbegin(), v0.cend());
      print_iter("after swap v2: ", v2.cbegin(), v2.cend());
      assert(v2 == t0 && v0 == t2);
   }

   v1 = v0;
   print_iter("assignment v1: ", v1.cbegin(), v1.cend());
   assert(v1 == v0);
   v1 = testvector();
   assert(v1.size() == 0);

   v1.assign(5, testtype('x'));
   print_iter("after assign(generator) v1: ", v1.cbegin(), v1.cend());
   assert(ttl::count(v1.cbegin(), v1.cend(), testtype('x')) == 5);

   // test const iterators
   for (testvector::const_iterator i = v1.cbegin(); i != v1.cend(); ++i)
      assert(i->value == 'x');
   for (testvector::const_iterator i = constify(v1).begin(); i != constify(v1).end(); ++i)
      assert(i->value == 'x');

   static const int ascii[] = {'A','B','C','D','E','F','G'};
   static const int in[] = {1,2,3,4,5};

   printf("insert(range)\n");
   {
      testvector t(ascii, ascii + countof(ascii));
      t.insert(t.begin(), in, in + countof(in));
      assert(ttl::equal(t.begin(), t.begin() + countof(in), in, in + countof(in)));
      assert(ttl::equal(t.begin() + countof(in), t.end(), ascii, ascii + countof(ascii)));
   }
   {
      testvector t(ascii, ascii + countof(ascii));
      t.insert(t.end(), in, in + countof(in));
      assert(ttl::equal(t.begin(), t.begin() + countof(ascii), ascii, ascii + countof(ascii)));
      assert(ttl::equal(t.begin() + countof(ascii), t.end(), in, in + countof(in)));
   }

   v1.assign(ascii, ascii + countof(ascii));
   assert(ttl::equal(v1.cbegin(), v1.cend(), ascii, ascii + countof(ascii)));

   print_iter("before insert v1: ", v1.cbegin(), v1.cend());
   v1.insert(v1.begin() + 1, in, in + countof(in));
   print_iter(" after insert v1: ", v1.cbegin(), v1.cend());
   assert(v1[0] == ascii[0]);
   assert(ttl::equal(v1.cbegin() + 1, v1.cbegin() + 1 + countof(in), in, in + countof(in)));
   assert(ttl::equal(v1.cbegin() + 1 + countof(in), v1.cend(), ascii + 1, ascii + countof(ascii)));

   {
      testvector t(ascii, ascii + countof(ascii));
      testvector big(16, testtype('X'));
      t.insert(t.begin() + 4, big.begin(), big.end());
      print_iter(" after insert big:", t.cbegin(), t.cend());
      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin(), big.begin(), big.end());
      print_iter(" after insert big:", t.cbegin(), t.cend());
      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.end(), big.begin(), big.end());
      print_iter(" after insert big:", t.cbegin(), t.cend());
   }

   {
      testvector t(ascii, ascii + countof(ascii));
      t.insert(t.begin() + 4, 100, testtype('O'));
      print_iter(" after insert big:", t.cbegin(), t.cend());

      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin() + 4, t.size() - 3, testtype('O'));

      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin() + 4, t.size() - 4, testtype('O'));

      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin() + 4, t.size() - 5, testtype('O'));

      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin(), 100, testtype('O'));
      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin(), t.size()-1, testtype('O'));
      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin(), t.size(), testtype('O'));
      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.begin(), t.size()+1, testtype('O'));

      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.end(), 0, testtype('O'));
      t.assign(ascii, ascii + countof(ascii));
      t.insert(t.end(), 1, testtype('O'));
   }

   {
      printf("ctor(range)\n");
      testvector t(in, in + countof(in));
      assert(ttl::equal(t.begin(), t.begin() + countof(in), in, in + countof(in)));
   }

   {
      printf("{push,pop}_back and element accessors\n");
      testvector v;
      v.push_back(999);
      assert(v.size() == 1 && v[0].value == 999);
      v.pop_back();
      assert(v.empty());
      v.assign(ascii, ascii + countof(ascii));
      assert(v.front() == *ascii);
      for (const int *i = ascii; i < ascii + countof(ascii); ++i)
      {
         assert(v[i - ascii] == *i);
         assert(v.at(i - ascii) == *i);
      }
      for (const int *i = ascii + countof(ascii); v.size();)
      {
         assert(v.back() == *--i);
         v.pop_back();
      }
      assert(v.empty());
      for (const int *i = ascii; i < ascii + countof(ascii); ++i)
         v.push_back(*i);
      assert(ttl::equal(v.begin(), v.end(), ascii, ascii + countof(ascii)));
   }

   printf("destructors:\n");
   testtype::verbose = true;
}
