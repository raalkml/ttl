/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of STL algorithms
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_
#define _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_

#include "types.hpp"
#include "type_traits.hpp"

namespace ttl
{
#if __cplusplus >= 201103L // C++11
#define ttl_constexpr constexpr
#else
#define ttl_constexpr
#endif
   template<class T1, class T2> struct pair;
   template<class T1, class T2> pair<T1,T2> make_pair(T1, T2);
#if __cplusplus >= 201103L // C++11
   template<class T> struct remove_reference;
   template<class T> constexpr typename remove_reference<T>::type &&move(T &&);
#else
   template<class T> struct remove_reference;
   template<class T> typename remove_reference<T>::type &move(T &);
#endif

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

   template<class ForwardIt1, class ForwardIt2>
   ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                     ForwardIt2 subfirst, ForwardIt2 sublast)
   {
      for (;;)
      {
         ForwardIt1 i = first;
         ForwardIt2 s = subfirst;
         for (;;)
         {
            if (s == sublast)
               return first;
            if (i == last)
               return last;
            if (!(*i == *s))
               break;
            ++i, ++s;
         }
         ++first;
      }
   }

   template<class ForwardIt1, class ForwardIt2, class BinaryPredicate>
   ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                     ForwardIt2 subfirst, ForwardIt2 sublast, BinaryPredicate pred)
   {
      for (;;)
      {
         ForwardIt1 i = first;
         ForwardIt2 s = subfirst;
         for (;;)
         {
            if (s == sublast)
               return first;
            if (i == last)
               return last;
            if (!pred(*i, *s))
               break;
            ++i, ++s;
         }
         ++first;
      }
   }

   template<class ForwardIt, class Size, class T>
   ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, const T &value)
   {
      if (count <= 0)
         return first;

      for(; first != last; ++first)
      {
         if (*first == value)
         {
            ForwardIt candidate = first;
            for (Size c = 1;; ++c)
            {
               if (c == count)
                  return candidate;
               if (++first == last)
                  return last;
               if (!(*first == value))
                  break;
            }
         }
      }
      return last;
   }

   template<class ForwardIt, class Size, class T, class BinaryPredicate>
   ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, const T &value, BinaryPredicate pred)
   {
      if (count <= 0)
         return first;

      for(; first != last; ++first)
      {
         if (*first == value)
         {
            ForwardIt candidate = first;
            for (Size c = 1;; ++c)
            {
               if (c == count)
                  return candidate;
               if (++first == last)
                  return last;
               if (!pred(*first, value))
                  break;
            }
         }
      }
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

   // find duplicates
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

   template<class InputIt1, class InputIt2, class BinaryPredicate>
   inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate pred)
   {
      for (; first1 != last1; ++first1, ++first2)
         if (!pred(*first1, *first2))
            return false;
      return true;
   }

   template<class InputIt1, class InputIt2>
   inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
   {
      for (; first1 != last1 && first2 != last2; ++first1, ++first2)
         if (!(*first1 == *first2))
            return false;
      return first1 == last1 && first2 == last2;
   }

   template<class InputIt1, class InputIt2, class BinaryPredicate>
   inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate pred)
   {
      for (; first1 != last1 && first2 != last2; ++first1, ++first2)
         if (!pred(*first1, *first2))
            return false;
      return first1 == last1 && first2 == last2;
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

   template<class ForwardIt, class T>
   void replace(ForwardIt first, ForwardIt last, const T &oldval, const T &newval)
   {
      for (; first != last; ++first)
         if (*first == oldval)
            *first = newval;
   }

   template<class ForwardIt, class UnaryPredicate, class T>
   void replace_if(ForwardIt first, ForwardIt last, UnaryPredicate p, const T &newval)
   {
      for (; first != last; ++first)
         if (p(*first))
            *first = newval;
   }

   template<class InputIt, class OutputIt, class T>
   OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first, const T &oldval, const T &newval)
   {
      for (; first != last; ++first, ++d_first)
         *d_first = *first == oldval ? newval: *first;
      return d_first;
   }

   template<class InputIt, class OutputIt, class UnaryPredicate, class T>
   OutputIt replace_copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate p, const T& newval)
   {
      for (; first != last; ++first, ++d_first)
         *d_first = p(*first) ? newval: *first;
      return d_first;
   }

   template<class InputIt, class OutputIt, class T>
   OutputIt remove_copy(InputIt first, InputIt last, OutputIt d_first, const T &value)
   {
      for (; first != last; ++first)
         if (!(*first == value))
         {
            *d_first = *first;
            ++d_first;
         }
      return d_first;
   }

   template<class InputIt, class OutputIt, class UnaryPredicate>
   OutputIt remove_copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate p)
   {
      for (; first != last; ++first)
         if (!p(*first))
         {
            *d_first = *first;
            ++d_first;
         }
      return d_first;
   }

   template<class ForwardIt, class T>
   ForwardIt remove(ForwardIt first, ForwardIt last, const T &value)
   {
      first = find(first, last, value);
      if (first != last)
         for (ForwardIt i = first; ++i != last; )
            if (!(*i == value))
            {
               *first = ttl::move(*i);
               ++first;
            }
      return first;
   }

   template<class ForwardIt, class UnaryPredicate>
   ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
   {
      first = find_if(first, last, p);
      if (first != last)
         for (ForwardIt i = first; ++i != last; )
            if (!p(*i))
            {
               *first = ttl::move(*i);
               ++first;
            }
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
   inline bool binary_search(ForwardIt first, ForwardIt last, const T &value)
   {
      first = lower_bound(first, last, value);
      return !(first == last) && !(value < *first);
   }

   template<class ForwardIt, class T, class Compare>
   inline bool binary_search(ForwardIt first, ForwardIt last, const T &value, Compare comp)
   {
       first = lower_bound(first, last, value, comp);
       return !(first == last) && !comp(value, *first);
   }

   template<class ForwardIt, class T>
   pair<ForwardIt,ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T &value)
   {
      ForwardIt low = lower_bound(first, last, value);
      return make_pair(low, upper_bound(low, last, value));
   }

   template<class ForwardIt, class T, class Compare>
   pair<ForwardIt,ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T &value, Compare comp)
   {
      ForwardIt low = lower_bound(first, last, value, comp);
      return make_pair(low, upper_bound(low, last, value, comp));
   }

   //
   // Sorting operations
   //

   template<class ForwardIt>
   ForwardIt is_sorted_until(ForwardIt first, ForwardIt last)
   {
      if (first != last)
      {
         ForwardIt next = first;
         while (++next != last)
         {
            if (*next < *first)
               return next;
            first = next;
         }
      }
      return last;
   }

   template<class ForwardIt>
   bool is_sorted(ForwardIt first, ForwardIt last)
   {
      return is_sorted_until(first, last) == last;
   }

   //
   // Set operations on sorted ranges
   //

   template<class InputIt1, class InputIt2, class OutputIt>
   OutputIt merge(InputIt1 first1, InputIt1 last1,
                  InputIt2 first2, InputIt2 last2,
                  OutputIt output)
   {
      for (; first1 != last1; ++output)
         if (first2 == last2)
            return ttl::copy(first1, last1, output);
         else if (*first2 < *first1)
         {
            *output = *first2;
            ++first2;
         }
         else
         {
            *output = *first1;
            ++first1;
         }
      return ttl::copy(first2, last2, output);
   }

   //
   // Minimum/maximum operations
   //
   template<class T>
   inline ttl_constexpr const T &min(const T &a, const T &b)
   {
      return a < b ? a: b;
   }

   template<class T, class Compare>
   inline ttl_constexpr const T &min(const T &a, const T &b, Compare comp)
   {
      return comp(a, b) ? a: b;
   }

   template<class T>
   inline ttl_constexpr const T &max(const T &a, const T &b)
   {
      return a < b ? b: a;
   }

   template<class T, class Compare>
   inline ttl_constexpr const T &max(const T &a, const T &b, Compare comp)
   {
      return comp(a, b) ? b: a;
   }

   template<class T>
   inline pair<const T &, const T &> minmax(const T &a, const T &b)
   {
      return b < a ? pair<const T &, const T &>(b, a): pair<const T &, const T &>(a, b);
   }

   template<class T, class Compare>
   inline pair<const T &, const T &> minmax(const T &a, const T &b, Compare comp)
   {
      return comp(b, a) ? pair<const T &, const T &>(b, a): pair<const T &, const T &>(a, b);
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

   template<class ForwardIt>
   ttl::pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first, ForwardIt last)
   {
      // TODO: optimize
      return ttl::pair<ForwardIt, ForwardIt>(min_element(first, last), max_element(first, last));
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_

