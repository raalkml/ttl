// vim: sw=3 ts=8 et
#include "t.hpp"

template<>
void print_iter(const char *title, ttl::backward_list<int>::const_iterator first, ttl::backward_list<int>::const_iterator last)
{
   fputs(title, stdout);
   for (; first != last; ++first)
   {
      ttl::backward_list<int>::const_iterator::reference r = *first;
      printf('\x21' <= r && r < 127 ? " '%c'": " %d", *first);
   }
   fputs(".\n", stdout);
}
void test()
{
   ttl::backward_list<testtype> fl(10, testtype(9));
   ttl::backward_list<testtype> fl1 = fl;
   int c = 0;
   for (ttl::backward_list<testtype>::iterator i = fl.begin(); i != fl.end(); ++i)
   {
      i->value = c++;
   }
   fl.erase_after(fl.begin(), advanceIt(fl.begin(), 9)); // fl = [0, 9]
   print_iter("after erase_after [1,$-1], fl:", fl.cbegin(), fl.cend());
   fl.push_front(testtype(999));  // fl = [999, 0, 9]
   fl.push_back(testtype(1000));  // fl = [999, 0, 9, 1000]
   fl.push_back(testtype(1001));  // fl = [999, 0, 9, 1000, 1001]
   print_iter("after push_front, push_back, fl:", fl.cbegin(), fl.cend());
   fl.pop_front();                // fl = [999, 0, 9]
   fl.insert_after(fl.before_begin(), testtype(1001));   // fl = [1001, 999, 0, 9]
   fl.insert_after(fl.before_end(), testtype(1002));     // fl = [1001, 999, 0, 9, 1002]
   fl.insert_after(++fl.begin(), testtype(1003));        // fl = [1001, 999, 1003, 0, 9, 1002]
   fl.erase_after(fl.cbegin());                          // fl = [1001, 1003, 0, 9, 1002]
   print_iter("after erase_after, fl:", fl.cbegin(), fl.cend());
   printf("fl.front %d, fl.back %d\n", fl.front().value, fl.back().value);
   fl.reverse();
   print_iter("after reverse, fl    :", fl.cbegin(), fl.cend());
   printf("fl.front %d, fl.back %d\n", fl.front().value, fl.back().value);
   static const int data[] = {'0','1','2','3'};
   fl1.insert_after(fl1.cbefore_begin(), data, data + countof(data));
   print_iter("after insert_after<InputIt>:", fl1.cbegin(), fl1.cend());

   ttl::backward_list<testtype> fl2 = fl1;
   print_iter("after copy ctor:", fl2.cbegin(), fl2.cend());

   ttl::backward_list<testtype> fl3 = fl;
   print_iter("before swap fl1:", fl1.cbegin(), fl1.cend());
   print_iter("before swap fl3:", fl3.cbegin(), fl3.cend());
   fl3.swap(fl1);
   print_iter("after swap fl1:", fl1.cbegin(), fl1.cend());
   print_iter("after swap fl3:", fl3.cbegin(), fl3.cend());
#if TODO
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

   ttl::backward_list<int> flI(data, data + countof(data));
   print_iter("<int> init from data: ", flI.cbegin(), flI.cend());
   ttl::backward_list<int> flI2(data, data + countof(data));
   flI.splice_after(flI.cbefore_begin(), flI2, flI2.cbefore_begin(), flI2.cend());
   print_iter("<int> splice_after: ", flI.cbegin(), flI.cend());
#endif
   printf("dtors\n");
}
