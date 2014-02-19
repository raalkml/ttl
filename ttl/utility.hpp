/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of <utility> templates
//
#ifndef _TINY_TEMPLATE_LIBRARY_UTILITY_HPP_
#define _TINY_TEMPLATE_LIBRARY_UTILITY_HPP_

#include "types.hpp"

namespace ttl
{
#if __cplusplus >= 201103L
   template<typename T, unsigned int N> constexpr unsigned int countof(const T (&)[N]) { return N; }
#define countof(a) ttl::countof(a)
#else
   template<typename T, unsigned int N> char (&_char_array_ref(const T (&)[N]))[N];
#define countof(a) sizeof(ttl::_char_array_ref(a))
#endif

   template<typename RandomAccessIterator>
   inline ttl::ptrdiff_t distance(RandomAccessIterator first, RandomAccessIterator last)
   {
      return last - first;
   }
   template<typename RandomAccessIterator, typename Distance>
   inline void advance(RandomAccessIterator &i, Distance d)
   {
      i += d;
   }

   template<class InputIt1, class InputIt2>
   inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
   {
      for (; first1 != last1; ++first1, ++first2)
         if (!(*first1 == *first2))
            return false;
      return true;
   }

   template<typename iterator, typename size_type, typename value_type>
   inline iterator fill_n(iterator first, size_type count, const value_type& value)
   {
      while (count--)
         *first++ = value;
      return first;
   }

   template<typename value_type>
   inline void swap(value_type &a, value_type &b)
   {
      value_type tmp = a;
      a = b;
      b = tmp;
   }

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

   template<typename T1, typename T2>
   struct pair
   {
      typedef T1 first_type;
      typedef T2 second_type;

      T1 first;
      T2 second;

      pair() {}
      pair(T1 _first, T2 _second): first(_first), second(_second) {}
      template<typename U1, typename U2>
      pair(const pair<U1,U2> &other): first(other.first), second(other.second) {}
      template<typename U1, typename U2>
      pair &operator=(const pair<U1,U2> &other)
      {
         first = other.first;
         second = other.second;
         return *this;
      }

      void swap(pair &other)
      {
         ttl::swap(first, other.first);
         ttl::swap(second, other.second);
      }
   };
   template<class T1, class T2>
   inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y)
   {
      return x.first == y.first && x.second == y.second;
   }
   template<class T1, class T2>
   inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y)
   {
      return !(x == y);
   }
   template<class T1, class T2>
   inline pair<T1, T2> make_pair(T1 first, T2 second)
   {
      return pair<T1, T2>(first, second);
   }
}

#endif // _TINY_TEMPLATE_LIBRARY_UTILITY_HPP_
