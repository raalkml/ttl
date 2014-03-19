/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of STL algorithms
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_
#define _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_

#include "types.hpp"
#include "utility.hpp"

namespace ttl
{
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
      for (; count > 0; --count)
      {
         *result = *first;
         ++result;
         ++first;
      }
      return result;
   }

   template<class BidirIt1, class BidirIt2>
   BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
   {
      while (first != last)
         *--d_last = *--last;
      return d_last;
   }

}
#endif // _TINY_TEMPLATE_LIBRARY_ALGORITHM_HPP_

