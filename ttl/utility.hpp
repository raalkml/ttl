/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of <utility> templates
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_UTILITY_HPP_
#define _TINY_TEMPLATE_LIBRARY_UTILITY_HPP_

#include "types.hpp"

namespace ttl
{
#if __cplusplus >= 201103L
   template<typename T, unsigned int N> constexpr unsigned int countof(const T (&)[N]) { return N; }
#ifndef countof
#define countof(a) ttl::countof(a)
#endif
#define ttl_countof(a) ttl::countof(a)
#else
   template<typename T, unsigned int N> char (&_char_array_ref(const T (&)[N]))[N];
#ifndef countof
#define countof(a) sizeof(ttl::_char_array_ref(a))
#endif
#define ttl_countof(a) sizeof(ttl::_char_array_ref(a))
#endif

   template<typename RandomAccessIterator>
   inline int distance(RandomAccessIterator first, RandomAccessIterator last)
   {
      return last - first;
   }
   template<typename RandomAccessIterator, typename Distance>
   inline void advance(RandomAccessIterator &i, Distance d)
   {
      i += d;
   }

   template<typename T>
   inline void swap(T &a, T &b)
   {
      T tmp = a;
      a = b;
      b = tmp;
   }

   template<typename T1, typename T2>
   struct pair
   {
      typedef T1 first_type;
      typedef T2 second_type;

      T1 first;
      T2 second;

      pair(): first(), second() {}
      pair(T1 _first, T2 _second): first(_first), second(_second) {}
      template<typename U1, typename U2>
      pair(const pair<U1,U2> &other): first(other.first), second(other.second) {}
      pair &operator=(const pair &other)
      {
         first = other.first;
         second = other.second;
         return *this;
      }
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
   inline pair<T1, T2> make_pair(const T1 &first, const T2 &second)
   {
      return pair<T1, T2>(first, second);
   }

   //
   // pair<> type selection:
   //
   template<typename Pair>
   struct select_first
   {
      typename Pair::first_type &operator()(Pair &r) const { return r.first; }
      const typename Pair::first_type &operator()(const Pair &r) const { return r.first; }
   };
   template<typename Pair>
   struct select_second
   {
      typename Pair::second_type &operator()(Pair &r) const { return r.second; }
      const typename Pair::second_type &operator()(const Pair &r) const { return r.second; }
   };

}

#endif // _TINY_TEMPLATE_LIBRARY_UTILITY_HPP_
