/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of <functional> templates
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_FUNCTIONAL_HPP_
#define _TINY_TEMPLATE_LIBRARY_FUNCTIONAL_HPP_ 1

namespace ttl
{
   template<typename T>
   struct less
   {
      typedef T first_argument_type;
      typedef T second_argument_type;
      typedef bool result_type;
      bool operator()(const T &a, const T &b) const { return a < b; }
   };

   template<typename T>
   struct greater
   {
      typedef T first_argument_type;
      typedef T second_argument_type;
      typedef bool result_type;
      bool operator()(const T &a, const T &b) const { return a > b; }
   };

   template<typename T>
   struct equal_to
   {
      typedef T first_argument_type;
      typedef T second_argument_type;
      typedef bool result_type;
      bool operator()(const T &a, const T &b) const { return a == b; }
   };

   template<typename T>
   struct not_equal_to
   {
      typedef T first_argument_type;
      typedef T second_argument_type;
      typedef bool result_type;
      bool operator()(const T &a, const T &b) const { return a != b; }
   };
}

#endif // _TINY_TEMPLATE_LIBRARY_FUNCTIONAL_HPP_
