// vim: sw=3 ts=8 et
#include <bitset>
#include "ttl/bitset.hpp"
#include "t.hpp"

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

ttl::bitset<16> &negate(ttl::bitset<16> &bs)
{
   return bs = ~bs;
}

void test()
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
   assert(bs4.all());
   assert(bs4.any());
   assert(!bs4.none());
   assert(bs2.any());
   assert(bs3.none());

   // should cause no code to be produced if optimizing
   for (unsigned i = 0; i < 256; ++i)
   {
      bs3[i].flip();
      bs3[i] = bs3[i];
      bs3[i] = false;
      bs3[i] = bs3.test(i);
   }
   for (unsigned i = 0; i < 256; ++i)
      bs3[i].flip();
   for (unsigned i = 0; i < 256; ++i)
      bs3[i] = bs3[i];
   for (unsigned i = 0; i < 256; ++i)
      bs3[i] = false;
   for (unsigned i = 0; i < 256; ++i)
      bs3[i] = bs3.test(i);
   bs3.set();
   bs3.reset();
   bs3.flip();
   bs3.to_ulong();
   bs3.to_ullong();
   assert(0 == ttl::bitset<128>("0").count());
   assert(1 == ttl::bitset<128>("1").count());
   bs0.reset();
   assert(0 == bs0.count());
   bs0[65] = 1;
   printf("%lu\n", (unsigned long)bs0.count());
   assert(1 == bs0.count());
   bs0[10] = 1;
   assert(2 == bs0.count());
   bs0[16] = 1;
   assert(3 == bs0.count());
   bs0[32] = 1;
   assert(4 == bs0.count());
   bs0[64] = 1;
   assert(5 == bs0.count());
   bs0[64] = 0;
   assert(4 == bs0.count());
   assert(0 == ttl::bitset<0>(1ul).count());
}
