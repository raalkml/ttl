// vim: sw=3 ts=8 et
#include "ttl/algorithm.hpp"
#include "ttl/utility.hpp"
#include "ttl/list.hpp"
#include "t.hpp"

template<>
void print_iter(const char *title, ttl::list<int>::const_iterator first, ttl::list<int>::const_iterator last)
{
   fputs(title, stdout);
   for (; first != last; ++first)
   {
      ttl::list<int>::const_iterator::reference r = *first;
      printf('\x21' <= r && r < 127 ? " '%c'": " %d", *first);
   }
   fputs(".\n", stdout);
}

static bool ANY(const testtype &) { return true; }

void test()
{
   testtype::verbose = false;

   printf("sizeof(int) %lu, sizeof(void *) %lu, sizeof list<int> %lu, sizeof(iterator) %lu, sizeof(*iterator) %lu\n",
          (unsigned long)sizeof(int),
          (unsigned long)sizeof(void *),
          (unsigned long)sizeof(ttl::list<int>),
          (unsigned long)sizeof(ttl::list<int>().begin()),
          (unsigned long)sizeof(*ttl::list<int>().begin()));

   const testtype nine(9);
   ttl::list<testtype> dl(10, nine);

   assert(dl.front() == constify(dl).front() && dl.front() == nine);
   assert(dl.back() == constify(dl).back() && dl.back() == nine);
   assert(ttl::count(dl.begin(), dl.end(), dl.front()) == 10 &&
          ttl::count(dl.begin(), dl.end(), dl.back()) == 10 &&
          ttl::count(dl.begin(), dl.end(), nine) == 10);
   assert(ttl::count(dl.cbegin(), dl.cend(), nine) == 10);
   assert(ttl::count(constify(dl).begin(), constify(dl).end(), nine) == 10);

   {
      ttl::list<testtype> dl1 = dl;
      ttl::list<testtype> dl2 = dl;
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), dl.cbegin()));
      assert(ttl::equal(dl2.cbegin(), dl2.cend(), dl1.begin()));
      dl1.clear();
      assert(dl1.empty());

      dl1.swap(dl2); // case 1
      assert(dl2.empty());
      assert(ttl::count(dl1.cbegin(), dl1.cend(), nine) == 10);

      dl1.swap(dl2); // case 2
      assert(dl1.empty());
      assert(ttl::count(dl2.cbegin(), dl2.cend(), nine) == 10);

      dl1.push_back(testtype(0xfeed));
      dl1.swap(dl2); // case 3
      assert(ttl::count(dl1.cbegin(), dl1.cend(), nine) == 10);
      assert(dl2.front() == testtype(0xfeed) &&
             dl2.front() == dl2.back() &&
             ++dl2.begin() == dl2.end());
   }

   int c = 0;
   for (ttl::list<testtype>::iterator i = dl.begin(); i != dl.end(); ++i)
   {
      assert(*i == nine);
      i->value = c++;
   }
   for (ttl::list<testtype>::iterator i = dl.end(); i-- != dl.begin();)
   {
      assert(i->value == --c);
      *i = testtype(c);
   }

   dl.erase(++dl.begin(), advanceIt(dl.begin(), 9)); // dl = [0, 8, 9]
   print_iter("after erase(from, to), dl:", dl.cbegin(), dl.cend());
   assert(dl.front() == testtype(0));
   assert(*++dl.cbegin() == testtype(8));
   assert(dl.back() == testtype(9));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 3);

   dl.push_front(testtype(999));  // dl = [999, 0, 8, 9]
   assert(dl.front() == testtype(999));
   assert(*++dl.cbegin() == testtype(0));
   assert(dl.back() == testtype(9));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 4);

   dl.push_front(testtype(1000)); // dl = [1000, 999, 0, 8, 9]
   assert(dl.front() == testtype(1000));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 5);
   dl.pop_front();                // dl = [999, 0, 9]
   assert(dl.front() == testtype(999));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 4);

   dl.push_back(testtype(1000)); // dl = [999, 0, 8, 9, 1000]
   assert(dl.back() == testtype(1000));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 5);
   dl.pop_front();               // dl = [0, 8, 9, 1000]
   assert(dl.front() == testtype(0));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 4);
   dl.pop_back();                // dl = [0, 8, 9]
   assert(dl.back() == testtype(9));
   dl.pop_back();                // dl = [0, 8]
   assert(dl.back() == testtype(8));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 2);

   dl.insert(dl.begin(), testtype(1001));   // dl = [1001, 0, 8]
   assert(dl.front() == testtype(1001));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 3);

   dl.insert(++dl.begin(), testtype(1002));    // dl = [1001, 1002, 0, 8]
   dl.insert(++ ++dl.begin(), testtype(1003)); // dl = [1001, 1002, 1003, 0, 8]
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 5);
   dl.erase(dl.cbegin());                      // dl = [1002, 1003, 0, 8]
   assert(dl.front() == testtype(1002));
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 4);
   print_iter("after erase(at):", dl.cbegin(), dl.cend());

   static const int data[] = {'0','1','2','3'};
   dl.insert(dl.begin(), data, data + countof(data));
   print_iter("after insert<>(at, from, to):", dl.cbegin(), dl.cend());
   assert(ttl::count_if(dl.begin(), dl.end(), ANY) == 8);

   {
      ttl::list<testtype> dl1, dl2;
      dl1.assign(data, data + countof(data));
      dl2.assign(data, data + countof(data));
      print_iter("after assign dl1:", dl1.cbegin(), dl1.cend());
      print_iter("after assign dl2:", dl2.cbegin(), dl2.cend());
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), data));
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), dl2.begin()));
      assert(ttl::count_if(dl1.begin(), dl1.end(), ANY) == ttl::count_if(dl2.begin(), dl2.end(), ANY));
   }

   {
      ttl::list<testtype> dl1, dl2;
      dl1.assign(data, data + countof(data));
      dl2.assign(data, data + countof(data));
      dl1.splice(dl1.begin(), dl2);
      print_iter("after splice dl1:", dl1.cbegin(), dl1.cend());

      static const int ascii[] = {'A','B','C','D'};
      dl2.assign(ascii, ascii + countof(ascii));
      dl1.splice(dl1.begin(), dl2, dl2.begin());
      print_iter("after splice(from) dl1:", dl1.cbegin(), dl1.cend());
      print_iter("                   dl2:", dl2.cbegin(), dl2.cend());

      dl1.splice(dl1.begin(), dl2, dl2.begin(), ++ ++dl2.begin());
      print_iter("after splice(from,to) dl1:", dl1.cbegin(), dl1.cend());
      print_iter("                      dl2:", dl2.cbegin(), dl2.cend());
   }

   print_iter("before reverse, dl:", dl.cbegin(), dl.cend());
   dl.reverse();
   print_iter("after reverse, dl :", dl.cbegin(), dl.cend());

#if 0
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

   ttl::list<int> flI(data, data + countof(data));
   print_iter("<int> init from data: ", flI.cbegin(), flI.cend());
   ttl::list<int> flI2(data, data + countof(data));
   flI.splice_after(flI.cbefore_begin(), flI2, flI2.cbefore_begin(), flI2.cend());
   print_iter("<int> splice_after: ", flI.cbegin(), flI.cend());
#endif
   printf("dtors\n");
}
