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

#include "templates.hpp"

typedef int8_t Int8;
typedef uint8_t UInt8;
typedef int16_t Int16;
typedef uint16_t UInt16;
typedef int32_t Int32;
typedef uint32_t UInt32;

namespace HBMOSTTypes
{
   typedef UInt8 FBlockID;
   typedef UInt8 InstID;
};

typedef int ValueType;
#define VALUE_MAX INT_MAX
#define VALUE_MIN INT_MIN

#define HB_ARRAYLEN(a) (sizeof(a)/sizeof(*a))
#define DBG_MSG(a) do { printf a; fputc('\n', stdout); } while(0)
#define DBG_WARNING(a) do { fputs("warning: ", stdout); printf a; fputc('\n', stdout); } while(0)
#define DBG_ERROR(a) do { fputs("error: ", stdout); printf a; fputc('\n', stdout); } while(0)
#define DBG_FLUSH fputc('\n', stdout)
#define TRC_SCOPE(a,b,c) ((void)0)


struct testtype
{
   int value;
   testtype():value(-1) { printf("%p %s\n", this, __func__); }
   testtype(int v):value(v) { printf("%p %s(int v) %d\n", this, __func__, value); }
   testtype(const testtype &o): value(o.value) { printf("%p %s(const testtype &) %d\n", this, __func__, value); }
   testtype &operator=(const testtype &o) { value = o.value; printf("%p %s %d\n", this, __func__, value); return *this; }
   ~testtype() { printf("%p %s %d\n", this, __func__, value); }
};
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
   v1.assign(in, in + HB_ARRAYLEN(in));
   v1.insert(v1.begin() + 1, in, in + HB_ARRAYLEN(in));

   printf("\nctor(first, last):\n");
   ttl::vector<testtype>(in, in + HB_ARRAYLEN(in));

   testvector(in, in + HB_ARRAYLEN(in));
   testvector(in, in + HB_ARRAYLEN(in));
   testvector(in, in + HB_ARRAYLEN(in));
   testvector(in, in + HB_ARRAYLEN(in));
   testvector(in, in + HB_ARRAYLEN(in));
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

void test_forward_list()
{
   printf("\nXXX %s\n", __func__);

   ttl::forward_list<testtype> fl(10, testtype(9));
//   for (auto i = fl.begin(); i != fl.end(); ++i)
//      ;
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
   return 0;
}


