// vim: sw=3 ts=8 et
#include "ttl/algorithm.hpp"
#include "ttl/functional.hpp"
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

static
void print_iter_ptr(const char *title, ttl::list<testtype>::const_iterator first, ttl::list<testtype>::const_iterator last)
{
   fputs(title, stdout);
   intptr_t base = first != last ? (intptr_t)&first->value: 0;
   for (; first != last; ++first)
   {
      ttl::list<testtype>::const_iterator::reference r = *first;
      printf('\x21' <= r.value && r.value < 127 ? " %ld:'%c'": " %ld:%d", (long)((intptr_t)&r.value - base), r.value);
   }
   fputs(".\n", stdout);
}

namespace ttl
{
   bool operator<(const testtype &a, const testtype &b)
   {
      return a.value < b.value;
   }
}

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

   assert(dl.size() < dl.max_size());
   assert(dl.front() == constify(dl).front() && dl.front() == nine);
   assert(dl.back() == constify(dl).back() && dl.back() == nine);
   assert(ttl::count(dl.begin(), dl.end(), dl.front()) == 10 &&
          ttl::count(dl.begin(), dl.end(), dl.back()) == 10 &&
          ttl::count(dl.begin(), dl.end(), nine) == 10 &&
          ttl::count(dl.begin(), dl.end(), nine) == (int)dl.size());
   assert(ttl::count(dl.cbegin(), dl.cend(), nine) == 10);
   assert(ttl::count(constify(dl).begin(), constify(dl).end(), nine) == 10);

   {
      ttl::list<testtype> dl1 = dl;
      ttl::list<testtype> dl2 = dl;
      assert(dl1 == dl);
      assert(!(dl2 != dl1));
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
   assert(dl.size() == 3);

   dl.push_front(testtype(999));  // dl = [999, 0, 8, 9]
   assert(dl.front() == testtype(999));
   assert(*++dl.cbegin() == testtype(0));
   assert(dl.back() == testtype(9));
   assert(dl.size() == 4);

   dl.push_front(testtype(1000)); // dl = [1000, 999, 0, 8, 9]
   assert(dl.front() == testtype(1000));
   assert(dl.size() == 5);
   dl.pop_front();                // dl = [999, 0, 9]
   assert(dl.front() == testtype(999));
   assert(dl.size() == 4);

   dl.push_back(testtype(1000)); // dl = [999, 0, 8, 9, 1000]
   assert(dl.back() == testtype(1000));
   assert(dl.size() == 5);
   dl.pop_front();               // dl = [0, 8, 9, 1000]
   assert(dl.front() == testtype(0));
   assert(dl.size() == 4);
   dl.pop_back();                // dl = [0, 8, 9]
   assert(dl.back() == testtype(9));
   dl.pop_back();                // dl = [0, 8]
   assert(dl.back() == testtype(8));
   assert(dl.size() == 2);

   dl.insert(dl.begin(), testtype(1001));   // dl = [1001, 0, 8]
   assert(dl.front() == testtype(1001));
   assert(dl.size() == 3);

   dl.insert(++dl.begin(), testtype(1002));    // dl = [1001, 1002, 0, 8]
   dl.insert(++ ++dl.begin(), testtype(1003)); // dl = [1001, 1002, 1003, 0, 8]
   assert(dl.size() == 5);
   dl.erase(dl.cbegin());                      // dl = [1002, 1003, 0, 8]
   assert(dl.front() == testtype(1002));
   assert(dl.size() == 4);
   print_iter("after erase(at):", dl.cbegin(), dl.cend());

   static const int data[] = {'0','1','2','3'};
   static const int ascii[] = {'A','B','C','D'};

   dl.insert(dl.begin(), data, data + countof(data));
   print_iter("after insert<>(at, from, to):", dl.cbegin(), dl.cend());
   assert(dl.size() == 8);

   {
      ttl::list<testtype> dl1, dl2;
      dl1.assign(data, data + countof(data));
      dl2.assign(data, data + countof(data));
      print_iter("after assign dl1:", dl1.cbegin(), dl1.cend());
      print_iter("after assign dl2:", dl2.cbegin(), dl2.cend());
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), data));
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), dl2.begin()));
      assert(dl1.size() == dl2.size());
   }

   {
      ttl::list<testtype> dl1, dl2;
      dl1.assign(data, data + countof(data));
      dl2.assign(data, data + countof(data));
      dl1.splice(dl1.begin(), dl2);
      print_iter("after splice dl1:", dl1.cbegin(), dl1.cend());

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

   dl.remove(testtype(1003));
   print_iter("after remove, dl :", dl.cbegin(), dl.cend());
   assert(ttl::count_if(dl.begin(), dl.end(), t::equal_to<testtype>(1003)) == 0);

   dl.remove_if(t::equal_to<testtype>(testtype(1002)));
   print_iter("after remove, dl :", dl.cbegin(), dl.cend());
   assert(ttl::count_if(dl.begin(), dl.end(), t::equal_to<testtype>(1002)) == 0);

   dl.insert(dl.end(), 10, nine);
   print_iter_ptr("before unique, dl :", dl.cbegin(), dl.cend());
   dl.unique();
   print_iter_ptr("after unique, dl :", dl.cbegin(), dl.cend());
   assert(ttl::count_if(dl.begin(), dl.end(), t::equal_to<testtype>(nine)) == 1);

   dl.insert(dl.end(), 10, nine);
   print_iter_ptr("before unique(pred), dl :", dl.cbegin(), dl.cend());
   dl.unique(ttl::equal_to<testtype>()); // binary predicate
   print_iter_ptr("after unique(pred), dl :", dl.cbegin(), dl.cend());
   assert(ttl::count_if(dl.begin(), dl.end(), t::equal_to<testtype>(nine)) == 1);

   {
      ttl::list<testtype> dl1, dl2;
      dl1.assign(data, data + countof(data));
      dl1.push_back(testtype('a'));
      dl2.assign(ascii, ascii + countof(ascii));
      dl2.push_back(testtype('b'));

      print_iter("dl1:", dl1.cbegin(), dl1.cend());
      print_iter("dl2:", dl2.cbegin(), dl2.cend());

      ttl::list<testtype> dl1copy = dl1;
      ttl::list<testtype> dl2copy = dl2;

      dl1.merge(dl2copy);
      print_iter("after merge, dl1:", dl1.cbegin(), dl1.cend());

      dl2.merge(dl1copy, ttl::less<testtype>());
      print_iter("after merge, dl2:", dl1.cbegin(), dl1.cend());

      assert(dl1copy.empty());
      assert(dl2copy.empty());
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), dl2.cbegin()));

      dl2copy.merge(dl2); // merge into empty list
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), dl2copy.cbegin()));

      dl2copy.merge(dl1copy); // merge an empty list
      assert(ttl::equal(dl1.cbegin(), dl1.cend(), dl2copy.cbegin()));
   }

   dl.resize(40);
   assert(dl.size() == 40);
   dl.resize(4);
   assert(dl.size() == 4);

   dl.resize(30, nine);
   assert(dl.size() == 30);
   assert(ttl::count(dl.begin(), dl.end(), nine) == 26);

   printf("dtors\n");
}
