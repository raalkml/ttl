// vim: sw=3 ts=8 et
#include <unistd.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/time.h>

#include <bitset>
#include "ttl/ttl.hpp"

#define countof(a) (sizeof(a)/sizeof(*a))


struct testtype
{
   int value;
   testtype();
   testtype(int);
   testtype(const testtype &);
   testtype &operator=(const testtype &);
   ~testtype();
};
testtype::testtype():value(-1) { printf("%p %s\n", this, __func__); }
testtype::testtype(int v):value(v) { printf("%p %s(int v) %d\n", this, __func__, value); }
testtype::testtype(const testtype &o): value(o.value) { printf("%p %s(const testtype &) %d\n", this, __func__, value); }
testtype &testtype::operator=(const testtype &o) { value = o.value; printf("%p %s %d\n", this, __func__, value); return *this; }
testtype::~testtype() { printf("%p %s %d\n", this, __func__, value); }
inline bool operator==(const testtype &a, const testtype &b)
{
   return a.value == b.value;
}

void test_array()
{
   printf("\nXXX %s\n", __func__);

   ttl::array<int, 3> i3 = { {1,2,3} };
   for (ttl::array<int, 3>::const_iterator i = i3.begin(); i != i3.end(); ++i)
      printf("%ld/%d: %d\n", long(i - i3.begin()), i3.size(), *i);
   ttl::array<int, 3> i30;
   i30 = i3;
   printf("equal? %s\n", i3 == i30 ? "yes": "no");
}

void test_pair()
{
   printf("\nXXX %s\n", __func__);

   ttl::pair<int, char> pp1(-1, 'a');
   ttl::pair<int, char> pp2 = ttl::make_pair(-2, 'b');
   printf("%d, '%c'\n", pp1.first, pp1.second);
   pp1.swap(pp2);
   printf("%d, '%c'\n", pp1.first, pp1.second);
}

template class ttl::vector<testtype>;

void test_vector()
{
   printf("\nXXX %s\n", __func__);

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

   printf("\nctor(first, last):\n");
   ttl::vector<testtype>(in, in + countof(in));

   testvector(in, in + countof(in));
   testvector(in, in + countof(in));
   testvector(in, in + countof(in));
   testvector(in, in + countof(in));
   testvector(in, in + countof(in));
   printf("\ndestructors:\n");
}

template class ttl::map<char, int>;

void test_map()
{
   printf("\nXXX %s\n", __func__);

   ttl::map<char, int> m;
   ttl::map<char, int> m1 = m;
   ttl::map<char, int> m2;
   ttl::map<char, int>();
   ttl::map<char, int>();
   //m1['a'] = 'a';
}

template class ttl::forward_list<testtype>;

void test_forward_list()
{
   printf("\nXXX %s\n", __func__);

   ttl::forward_list<testtype> fl(10, testtype(9));
   ttl::forward_list<testtype> fl1 = fl;
   ttl::forward_list<testtype> fl2 = fl;
   ttl::forward_list<testtype> fl3 = fl;
//   for (auto i = fl.begin(); i != fl.end(); ++i)
//      ;
   fl.reverse();
}

template class ttl::bitset<128>;

void test_bitset()
{
   printf("\nXXX %s\n", __func__);

   ttl::bitset<128> bs0(0xfefeffUL);
   ttl::bitset<128> bs064(0xfefe00000000ULL);
   ttl::bitset<128> bs1("1010101010_");
   ttl::bitset<256> bs2("0101010101");
   ttl::bitset<0>   bs3("0101010101");
   ttl::bitset<0>   bs3_ = bs3;
   ttl::bitset<16>  bs4;
   bs4 = bs2;
   for (unsigned i = bs0.size(); i--;)
      printf("%d",(bool)bs0[i]);
   printf("\n");
   for (unsigned i = bs064.size(); i--;)
      printf("%d",(bool)bs064[i]);
   printf("\n");
   for (unsigned i = bs4.size(); i--;)
      printf("%d",(bool)bs4[i]);
   printf("\n");
   bs0 = bs1;
   for (unsigned i = bs0.size(); i--;)
      printf("%d",(bool)bs0[i]);
   printf("\n");
   bs1 = bs3;
   for (unsigned i = bs1.size(); i--;)
      printf("%d",(bool)bs1[i]);
   printf("\n");
   printf("sizeof bs0(128) %u vs %u, size: %u vs %u\n", sizeof(bs0), sizeof(std::bitset<128>), bs0.size(), std::bitset<128>().size());
   printf("sizeof bs2(256) %u vs %u\n", sizeof(bs2), sizeof(std::bitset<256>));
   printf("sizeof bs3(0) %u vs %u, size: %u vs %u\n", sizeof(bs3), sizeof(std::bitset<0>), bs3.size(), std::bitset<0>().size());
}

int main(int argc, char* argv[], char* envp[])
{
   void *p = ::operator new(10 * sizeof(int));
   printf("::operator new %p\n", p);
   ::operator delete(p);

   test_array();
   test_pair();
   test_vector();
   test_map();
   test_forward_list();
   test_bitset();
   return 0;
}


