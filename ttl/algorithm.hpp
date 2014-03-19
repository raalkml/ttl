/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of STL algorithms
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_
#define _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_

#include "types.hpp"

namespace ttl
{
   //
   // Non-modifying sequence operations
   //

   template<class InputIt, class T>
   InputIt find(InputIt first, InputIt last, const T &value)
   {
      for (; first != last; ++first)
         if (*first == value)
            return first;
      return last;
   }

   template<class InputIt, class UnaryPredicate>
   InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
   {
      for (; first != last; ++first)
         if (p(*first))
            return first;
      return last;
   }

   template<class InputIt, class UnaryPredicate>
   InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate p)
   {
      for (; first != last; ++first)
         if (!p(*first))
            return first;
      return last;
   }

   template<class InputIt, class T>
   /* InputIt::difference_type */ int count(InputIt first, InputIt last, const T &value)
   {
      int c = 0;
      for (; first != last; ++first)
         if (*first == value)
            ++c;
      return c;
   }

   template<class InputIt, class UnaryPredicate>
   /* InputIt::difference_type */ int count_if(InputIt first, InputIt last, UnaryPredicate p)
   {
      int c = 0;
      for (; first != last; ++first)
         if (p(*first))
            ++c;
      return c;
   }

   template<class InputIt, class UnaryPredicate>
   bool all_of(InputIt first, InputIt last, UnaryPredicate p)
   {
      return find_if_not(first, last, p) == last;
   }

   template<class InputIt, class UnaryPredicate>
   bool any_of(InputIt first, InputIt last, UnaryPredicate p)
   {
       return find_if(first, last, p) != last;
   }

   template<class InputIt, class UnaryPredicate>
   bool none_of(InputIt first, InputIt last, UnaryPredicate p)
   {
       return find_if(first, last, p) == last;
   }

   template<class ForwardIt>
   ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
   {
      if (first == last)
         return last;
      ForwardIt next = first;
      ++next;
      for (; next != last; ++next, ++first)
         if (*first == *next)
            return first;
      return last;
   }

   template<class ForwardIt, class BinaryPredicate>
   ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPredicate p)
   {
      if (first == last)
         return last;
      ForwardIt next = first;
      ++next;
      for (; next != last; ++next, ++first)
         if (p(*first, *next))
            return first;
      return last;
   }

   template<class InputIt1, class InputIt2>
   inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
   {
      for (; first1 != last1; ++first1, ++first2)
         if (!(*first1 == *first2))
            return false;
      return true;
   }

   template<class InputIt, class UnaryFunction>
   UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
   {
      for (; first != last; ++first)
         f(*first);
      return f;
   }

   //
   // Modifying sequence operations
   //

   template<class iterator1, class iterator2>
   inline void iter_swap(iterator1 a, iterator2 b)
   {
      swap(*a, *b);
   }

   template<class iterator1, class iterator2>
   inline iterator1 swap_ranges(iterator1 first1, iterator1 last1, iterator2 first2)
   {
      for (; first1 != last1; ++first1, ++first2)
         iter_swap(first1, first2);
      return first2;
   }

   template<class ForwardIt, class T>
   void fill(ForwardIt first, ForwardIt last, const T &value)
   {
      for (; first != last; ++first)
         *first = value;
   }

   template<typename iterator, typename size_type, typename value_type>
   inline iterator fill_n(iterator first, size_type count, const value_type& value)
   {
      for (; count > 0; --count, ++first)
         *first = value;
      return first;
   }

   template<class InputIt, class OutputIt>
   OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
   {
      for (; first != last; ++first)
      {
         *d_first = *first;
         ++d_first;
      }
      return d_first;
   }

   template<class InputIt, class OutputIt, class UnaryPredicate>
   OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred)
   {
      for (; first != last; ++first)
         if (pred(*first))
         {
            *d_first = *first;
            ++d_first;
         }
      return d_first;
   }

   template<class InputIt, class Size, class OutputIt>
   OutputIt copy_n(InputIt first, Size count, OutputIt result)
   {
      for (; count > 0; --count, ++first, ++result)
         *result = *first;
      return result;
   }

   template<class BidirIt1, class BidirIt2>
   BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
   {
      while (first != last)
         *--d_last = *--last;
      return d_last;
   }

   template<class ForwardIt, class Generator>
   void generate(ForwardIt first, ForwardIt last, Generator g)
   {
      for (; first != last; ++first)
         *first = g();
   }

   template<class OutputIt, class Size, class Generator>
   OutputIt generate_n(OutputIt first, Size count, Generator g)
   {
      for (; count > 0; --count, ++first)
         *first = g();
      return first;
   }

   //
   // Binary search operations on sorted ranges
   //

   template<class ForwardIt, class T>
   ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value)
   {
      for (int count = last - first; count > 0;)
      {
         int step = count / 2;
         ForwardIt it = first;
         it = first + step;
         if (*it < value)
         {
            first = ++it;
            count -= step + 1;
         }
         else
            count = step;
      }
      return first;
   }

   template<class ForwardIt, class T, class Compare>
   ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value, Compare comp)
   {
      for (int count = last - first; count > 0;)
      {
         int step = count / 2;
         ForwardIt it = first;
         it += step;
         if (comp(*it, value))
         {
            first = ++it;
            count -= step + 1;
         }
         else
            count = step;
      }
      return first;
   }

   template<class ForwardIt, class T>
   ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T &value)
   {
      for (int count = last - first; count > 0;)
      {
         int step = count / 2;
         ForwardIt it = first;
         it += step;
         if (!(value < *it))
         {
            first = ++it;
            count -= step + 1;
         }
         else
            count = step;
      }
      return first;
   }

   template<class ForwardIt, class T, class Compare>
   ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T &value, Compare comp)
   {
      for (int count = last - first; count > 0;)
      {
         int step = count / 2;
         ForwardIt it = first;
         it += step;
         if (!comp(value, *it))
         {
            first = ++it;
            count -= step + 1;
         }
         else
            count = step;
      }
      return first;
   }

   template<class ForwardIt, class T>
   bool binary_search(ForwardIt first, ForwardIt last, const T &value)
   {
      first = lower_bound(first, last, value);
      return !(first == last) && !(value < *first);
   }

   template<class ForwardIt, class T, class Compare>
   bool binary_search(ForwardIt first, ForwardIt last, const T &value, Compare comp)
   {
       first = lower_bound(first, last, value, comp);
       return !(first == last) && !comp(value, *first);
   }

   template<class T1, class T2> struct pair;

   template<class ForwardIt, class T>
   pair<ForwardIt,ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T &value)
   {
      return make_pair(lower_bound(first, last, value),
                       upper_bound(first, last, value));
   }

   template<class ForwardIt, class T, class Compare>
   pair<ForwardIt,ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T &value, Compare comp)
   {
      return make_pair(lower_bound(first, last, value, comp),
                       upper_bound(first, last, value, comp));
   }

   //
   // Minimum/maximum operations
   //
   template<class T>
   const T &min(const T &a, const T &b)
   {
      return a < b ? a: b;
   }

   template<class T, class Compare>
   const T &min(const T &a, const T &b, Compare comp)
   {
      return comp(a, b) ? a: b;
   }

   template<class T>
   const T &max(const T &a, const T &b)
   {
      return a < b ? b: a;
   }

   template<class T, class Compare>
   const T &max(const T &a, const T &b, Compare comp)
   {
      return comp(a, b) ? b: a;
   }

   template<class T>
   pair<const T &, const T &> minmax(const T &a, const T &b)
   {
      return b < a ? make_pair(b, a): make_pair(a, b);
   }

   template<class T, class Compare>
   pair<const T &, const T &> minmax(const T &a, const T &b, Compare comp)
   {
      return comp(b, a) ? make_pair(b, a): make_pair(a, b);
   }

   template<class ForwardIt>
   ForwardIt min_element(ForwardIt first, ForwardIt last)
   {
      if (first == last)
         return last;
      ForwardIt smallest = first;
      for (++first; first != last; ++first)
         if (*first < *smallest)
            smallest = first;
      return smallest;
   }

   template<class ForwardIt>
   ForwardIt max_element(ForwardIt first, ForwardIt last)
   {
      if (first == last)
         return last;
      ForwardIt largest = first;
      for (++first; first != last; ++first)
         if (*largest < *first)
            largest = first;
      return largest;
   }

}
#endif // _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_

