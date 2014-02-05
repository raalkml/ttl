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
   ttl::array<int, 3> i3 = { {1,2,3} };
   for (ttl::array<int, 3>::const_iterator i = i3.begin(); i != i3.end(); ++i)
      printf("%ld/%d: %d\n", long(i - i3.begin()), i3.size(), *i);
   ttl::array<int, 3> i30;
   i30 = i3;
   printf("equal? %s\n", i3 == i30 ? "yes": "no");
}

void test_pair()
{
   ttl::pair<int, char> pp1(-1, 'a');
   ttl::pair<int, char> pp2 = ttl::make_pair(-2, 'b');
   printf("%d, '%c'\n", pp1.first, pp1.second);
   pp1.swap(pp2);
   printf("%d, '%c'\n", pp1.first, pp1.second);
}

template class ttl::vector<testtype>;

void test_vector()
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

void print_map(const char *s, const ttl::map<char, int> &m)
{
   fputs(s, stdout);
   for (ttl::map<char, int>::const_iterator i = m.cbegin(); i != m.cend(); ++i)
      if (i->first < 0)
         printf("--- = %3d ", i->second);
      else if (i->first < 32)
         printf("... = %3d ", i->second);
      else
         printf("'%c' = %3d ", i->first, i->second);
   fputs(".\n", stdout);
}

void test_map()
{
   ttl::map<char, int> m;
   ttl::map<char, int> m1 = m;
   ttl::map<char, int> m2;
   ttl::map<char, int>();
   ttl::map<char, int>();
   for (unsigned char c = 0; c < 255u; ++c)
      m1[(char)c] = c;
   m1[(char)255] = 255;
   print_map("[]\n", m1);
   for (unsigned char c = 0; c < 255u; ++c)
      m1.insert(ttl::make_pair((char)c, (int)c));
   m1.insert(ttl::make_pair((char)255, 255));
   print_map("insert\n", m1);
   //for (char c = 0; (unsigned)c <= 127u; ++c)
   //   m1.insert(m1.end(), ttl::make_pair(c, (int)c));
   //print_map("insert(iterator)\n", m1);
   m1.clear();
   print_map("clear: ", m1);
}

template class ttl::forward_list<testtype>;


template<class Iterator, typename Distance>
inline Iterator advanceIt(Iterator i, Distance d)
{
   while (d--)
      ++i;
   return i;
}

template<class Iterator>
void print_iter(const char *title, Iterator first, Iterator last)
{
   fputs(title, stdout);
   for (; first != last; ++first)
   {
      typename Iterator::reference r = *first;
      printf('\x21' <= r.value && r.value < 127 ? " '%c'": " %d", first->value);
   }
   fputs(".\n", stdout);
}
template<>
void print_iter(const char *title, ttl::forward_list<int>::const_iterator first, ttl::forward_list<int>::const_iterator last)
{
   fputs(title, stdout);
   for (; first != last; ++first)
   {
      ttl::forward_list<int>::const_iterator::reference r = *first;
      printf('\x21' <= r && r < 127 ? " '%c'": " %d", *first);
   }
   fputs(".\n", stdout);
}

void test_forward_list()
{
   ttl::forward_list<testtype> fl(10, testtype(9));
   ttl::forward_list<testtype> fl1 = fl;
   int c = 0;
   for (ttl::forward_list<testtype>::iterator i = fl.begin(); i != fl.end(); ++i)
   {
      i->value = c++;
   }
   fl.erase_after(fl.begin(), advanceIt(fl.begin(), 9)); // fl = [0, 9]
   fl.push_front(testtype(999));  // fl = [999, 0, 9]
   fl.push_front(testtype(1000)); // fl = [1000, 999, 0, 9]
   fl.pop_front();                // fl = [999, 0, 9]
   fl.insert_after(fl.before_begin(), testtype(1001));   // fl = [1001, 999, 0, 9]
   fl.insert_after(++fl.before_begin(), testtype(1002)); // fl = [1001, 1002, 999, 0, 9]
   fl.insert_after(++fl.begin(), testtype(1003));        // fl = [1001, 1002, 1003, 999, 0, 9]
   fl.erase_after(fl.cbegin());                          // fl = [1001, 1003, 999, 0, 9]
   print_iter("after erase_after, fl:", fl.cbegin(), fl.cend());

   fl.reverse();
   print_iter("after reverse, fl:", fl.cbegin(), fl.cend());

   static const int data[] = {'0','1','2','3'};
   fl1.insert_after(fl1.cbefore_begin(), data, data + countof(data));
   print_iter("after insert_after<InputIt>:", fl1.cbegin(), fl1.cend());

   ttl::forward_list<testtype> fl2 = fl1;
   print_iter("after copy ctor:", fl2.cbegin(), fl2.cend());

   ttl::forward_list<testtype> fl3 = fl;
   print_iter("before swap fl1:", fl1.cbegin(), fl1.cend());
   print_iter("before swap fl3:", fl3.cbegin(), fl3.cend());
   fl3.swap(fl1);
   print_iter("after swap fl1:", fl1.cbegin(), fl1.cend());
   print_iter("after swap fl3:", fl3.cbegin(), fl3.cend());

   fl3.erase_after(advanceIt(fl3.cbegin(), 3), fl3.cend());
   fl1.splice_after(advanceIt(fl1.cbefore_begin(), 3), fl3);
   print_iter("after splice_after fl1<-fl3: fl1:", fl1.cbegin(), fl1.cend());
   print_iter("after splice_after fl1<-fl3: fl3:", fl3.cbegin(), fl3.cend());

   fl1.assign(data, data + countof(data));
   fl2.assign(data, data + countof(data));
   print_iter("after assign fl1:", fl1.cbegin(), fl1.cend());
   print_iter("after assign fl2:", fl2.cbegin(), fl2.cend());

   fl1.splice_after(fl1.cbefore_begin(), fl2, fl2.cbefore_begin());
   print_iter("after splice_after (one) fl1:", fl1.cbegin(), fl1.cend());
   print_iter("after splice_after (one) fl2:", fl2.cbegin(), fl2.cend());

   fl2.splice_after(fl2.cbefore_begin(), fl1, fl1.cbefore_begin(), advanceIt(fl1.cbegin(),2));
   print_iter("after splice_after (range) fl1:", fl1.cbegin(), fl1.cend());
   print_iter("after splice_after (range) fl2:", fl2.cbegin(), fl2.cend());

   fl1.splice_after(fl1.cbefore_begin(), fl2, fl2.cend(), fl2.cend());
   print_iter("after splice_after (end,end) fl1:", fl1.cbegin(), fl1.cend());

   fl1.splice_after(fl1.cbefore_begin(), fl2, fl2.cbefore_begin(), fl2.cend());
   print_iter("after splice_after (before_begin,end) fl1:", fl1.cbegin(), fl1.cend());

   // Undefined behaviour:
   // splice past end should crash:
   // fl1.splice_after(fl1.cend(), fl2, fl2.cbefore_begin(), fl2.cend());
   // splice inside the spliced range loops:
   // fl1.splice_after(fl1.cbegin(), fl1, fl1.cbefore_begin(), fl1.cend());

   ttl::forward_list<int> flI(data, data + countof(data));
   print_iter("<int> init from data: ", flI.cbegin(), flI.cend());
   printf("dtors\n");
}

template class ttl::bitset<128>;

template<typename T> void print_bitset(const char *s, const T &bs)
{
   fputs(s, stdout);
   for (unsigned i = bs.size(); i--;)
   {
      fputc("01"[bs[i]], stdout);
      if (i && !((i) % 8))
         fputc('_', stdout);
   }
   fputs(".\n", stdout);
}

void test_bitset()
{
   ttl::bitset<128> bs0(0xfefeffUL);
   ttl::bitset<128> bs064(0xfefe00000000ULL);
   ttl::bitset<128> bs1("1010101010_");
   ttl::bitset<256> bs2("0101010101");
   ttl::bitset<0>   bs3("0101010101");
   ttl::bitset<0>   bs3_ = bs3;

   print_bitset("<128>:UL  ", bs0);
   print_bitset("<128>:ULL ", bs064);
   print_bitset("<256>:    ", bs2);

   ttl::bitset<16> bs4 = bs2;
   print_bitset("(<16>)<256> ", bs4);
   printf("u32:    0x%08lx\n", bs4.to_ulong());
   printf("u64:    0x%016llx\n", bs4.to_ullong());
   bs4.reset();
   print_bitset("bs4:      ", bs4);
   bs4 = ttl::bitset<64>().set();
   print_bitset("<16>=<64> ", bs4);
   bs4[1].flip();
   print_bitset("flip      ", bs4);
   bs4.set(1);
   print_bitset(" set      ", bs4);
   bs4.reset(1);
   print_bitset("rset      ", bs4);
   bs4.flip();
   print_bitset("flip      ", bs4);
   bs4[4] = 1;
   print_bitset("refset    ", bs4);

   print_bitset("<0>       ", bs3);

   bs0 = bs1;
   print_bitset("<128>=<128> ", bs0);

   bs1 = bs3;
   print_bitset("<128>=<0>   ", bs1);

   printf("sizeof bs2(256) %lu vs %lu\n",
          (unsigned long)sizeof(bs2),
          (unsigned long)sizeof(std::bitset<256>));
   printf("sizeof bs0(128) %lu vs %lu, cap: %lu, size: %lu vs %lu\n",
          (unsigned long)sizeof(bs0),
          (unsigned long)sizeof(std::bitset<128>),
          (unsigned long)bs0.capacity(),
          (unsigned long)bs0.size(),
          (unsigned long)std::bitset<128>().size());
   printf("sizeof bs3(0) %lu vs %lu, cap: %lu, size: %lu vs %lu\n",
          (unsigned long)sizeof(bs3),
          (unsigned long)sizeof(std::bitset<0>),
          (unsigned long)bs3.capacity(),
          (unsigned long)bs3.size(),
          (unsigned long)std::bitset<0>().size());
   bs4.set();
   print_bitset("1: ", bs4);
   print_bitset("2: ", bs2);
   print_bitset("3: ", bs3);
   printf("all : %d %d %d\n", bs4.all(), bs2.all(), bs3.all());
   printf("any : %d %d %d\n", bs4.any(), bs2.any(), bs3.any());
   printf("none: %d %d %d\n", bs4.none(), bs2.none(), bs3.none());

   // should cause no code to be produced if optimizing
   for (unsigned i = 0; i < 256; ++i)
   {
      bs3[i].flip();
      bs3[i] = bs3[i];
      bs3[i] = false;
      bs3[i] = bs3.test(i);
   }
   bs3.set();
   bs3.reset();
   bs3.flip();
   bs3.to_ulong();
   bs3.to_ullong();
}

static void test_new()
{
   void *p = ::operator new(10 * sizeof(int));
   printf("::operator new %p\n", p);
   ::operator delete(p);
}

static void test_types()
{
   printf("int:      is_integral: %d\n", ttl::is_integral<int>::value);
   printf("float:    is_integral: %d\n", ttl::is_integral<float>::value);
   printf("testtype: is_integral: %d\n", ttl::is_integral<testtype>::value);
   printf("int:  is_void: %d\n", ttl::is_void<int>::value);
   printf("void: is_void: %d\n", ttl::is_void<void>::value);
   printf("int:    is_float: %d\n", ttl::is_floating_point<int>::value);
   printf("double: is_float: %d\n", ttl::is_floating_point<double>::value);
   printf("float:  is_float: %d\n", ttl::is_floating_point<float>::value);
   printf("int:  is_signed: %d\n", ttl::is_signed<int>::value);
   printf("uint: is_signed: %d\n", ttl::is_signed<unsigned int>::value);
   printf("int:  is_unsigned: %d\n", ttl::is_unsigned<int>::value);
   printf("uint: is_unsigned: %d\n", ttl::is_unsigned<unsigned int>::value);
}

static const struct
{
   const char *name;
   void (* const fn)();
} tests[] = {
   { "new",    &test_new },
   { "array",  &test_array },
   { "pair",   &test_pair },
   { "vector", &test_vector },
   { "map",    &test_map },
   { "list",   &test_forward_list },
   { "bitset", &test_bitset },
   { "types",  &test_types },
};

int main(int argc, char* argv[], char* envp[])
{
   printf("unsigned long: %lu\n", (unsigned long)sizeof(unsigned long));
   printf("unsigned long long: %lu\n", (unsigned long)sizeof(unsigned long long));
   if (argc <= 1)
   {
      printf("%s", argv[0]);
      for (unsigned i = 0; i < sizeof(tests)/sizeof(*tests); ++i)
         printf(" %s", tests[i].name);
      printf("\n");
   }
   for (int arg = 1; arg < argc; ++arg)
      for (unsigned i = 0; i < sizeof(tests)/sizeof(*tests); ++i)
         if (strcasecmp(argv[arg], tests[i].name) == 0)
         {
            printf("*** %s ****\n", tests[i].name);
            (tests[i].fn)();
            printf("*** %s finished ****\n", tests[i].name);
         }
   return 0;
}


