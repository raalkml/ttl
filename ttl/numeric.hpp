/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of numeric operations
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_NUMERIC_HPP_
#define _TINY_TEMPLATE_LIBRARY_NUMERIC_HPP_

namespace ttl
{
   template<class ForwardIterator, class T>
   void iota(ForwardIterator first, ForwardIterator last, T value)
   {
      for (; first != last; ++first, ++value)
         *first = value;
   }

   template<class InputIt, class T>
   T accumulate(InputIt first, InputIt last, T value)
   {
      for (; first != last; ++first)
         value = value + *first;
      return value;
   }

}
#endif // _TINY_TEMPLATE_LIBRARY_NUMERIC_HPP_

