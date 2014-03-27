/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: types
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_TYPE_TRAITS_HPP_
#define _TINY_TEMPLATE_LIBRARY_TYPE_TRAITS_HPP_

#include <limits.h>
#include "types.hpp"

namespace ttl
{
   template<class T> struct remove_const          { typedef T type; };
   template<class T> struct remove_const<const T> { typedef T type; };
   template<class T> struct remove_volatile             { typedef T type; };
   template<class T> struct remove_volatile<volatile T> { typedef T type; };
   template<class T>
   struct remove_cv
   {
      typedef typename ttl::remove_volatile<typename ttl::remove_const<T>::type>::type type;
   };
   template<class T> struct remove_reference { typedef T type; };
   template<class T> struct remove_reference<T&> { typedef T type; };
#if __cplusplus >= 201103L // C++11
   template<class T> struct remove_reference<T&&> { typedef T type; };
#endif

   template<typename T, T v>
   struct integral_constant
   {
      static const T value = v;
      typedef T value_type;
      typedef integral_constant<T, v> type;
   };

   struct true_type: integral_constant<bool, true> {};
   struct false_type: integral_constant<bool, false> {};

   template<typename T, typename U> struct is_same: false_type {};
   template<typename T> struct is_same<T, T>: true_type {};

   // is_const<T>::value == true if and only if T has const-qualification.
   template<typename T> struct is_const_value: false_type {};
   template<typename T> struct is_const_value<const T*>: true_type {};
   template<typename T> struct is_const_value<const volatile T*>: true_type {};
   template<typename T> struct is_const: is_const_value<T*> {};
   // is_const is false because T is const, not the reference to T.
   template<typename T> struct is_const<T&>: false_type{};

   // is_volatile<T>::value == true if and only if T has volatile-qualification.
   template<typename T> struct is_volatile_value: false_type {};
   template<typename T> struct is_volatile_value<volatile T*>: true_type {};
   template<typename T> struct is_volatile_value<const volatile T*>: true_type {};
   template<typename T> struct is_volatile: is_volatile_value<T*> {};
   // is_const is false because T is const, not the reference to T.
   template <typename T> struct is_volatile<T&>: false_type {};

    // is_signed<T>::value == true if and only if T is one of the following types:
    template<class T> struct is_signed: integral_constant<bool,
#if (CHAR_MAX == SCHAR_MAX)
       is_same<char, typename remove_cv<T>::type>::value ||
#endif
#if defined(__WCHAR_MAX__) && ((__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 32767))
       is_same<wchar_t, typename remove_cv<T>::type>::value ||
#endif
       is_same<signed char, typename remove_cv<T>::type>::value ||
       is_same<signed short, typename remove_cv<T>::type>::value ||
       is_same<signed int, typename remove_cv<T>::type>::value ||
       is_same<signed long, typename remove_cv<T>::type>::value ||
       is_same<signed long long, typename remove_cv<T>::type>::value > {};

    // is_unsigned<T>::value == true if and only if T is one of the following types:
    template<class T> struct is_unsigned: integral_constant<bool,
#if (CHAR_MAX == UCHAR_MAX)
       is_same<char, typename remove_cv<T>::type>::value ||
#endif
#if (defined(__WCHAR_MAX__) && ((__WCHAR_MAX__ == 4294967295U) || (__WCHAR_MAX__ == 65535))) || defined(_MSC_VER)
       is_same<wchar_t, typename remove_cv<T>::type>::value ||
#endif
       is_same<unsigned char, typename remove_cv<T>::type>::value ||
       is_same<unsigned short, typename remove_cv<T>::type>::value ||
       is_same<unsigned int, typename remove_cv<T>::type>::value ||
       is_same<unsigned long, typename remove_cv<T>::type>::value ||
       is_same<unsigned long long, typename remove_cv<T>::type>::value > {};

    // is_void<T>::value == true if and only if T is a void
    template<class T> struct is_void:
       integral_constant<bool, is_same<void, typename remove_cv<T>::type>::value> {};

    // is_integral<T>::value == true if and only if T is one of the following types:
    // bool, char, signed char, unsigned char, wchar_t, short, int, long,
    // long long, unsigned short, unsigned int, unsigned long, unsigned long long
    template<class T> struct is_integral: integral_constant<bool,
       // is_same<char16_t, typename remove_cv<T>::type>::value ||
       // is_same<char32_t, typename remove_cv<T>::type>::value ||
       is_same<unsigned char, typename remove_cv<T>::type>::value ||
       is_same<unsigned short, typename remove_cv<T>::type>::value ||
       is_same<unsigned int, typename remove_cv<T>::type>::value ||
       is_same<unsigned long, typename remove_cv<T>::type>::value ||
       is_same<unsigned long long, typename remove_cv<T>::type>::value ||
       is_same<signed char, typename remove_cv<T>::type>::value ||
       is_same<signed short, typename remove_cv<T>::type>::value ||
       is_same<signed int, typename remove_cv<T>::type>::value ||
       is_same<signed long, typename remove_cv<T>::type>::value ||
       is_same<signed long long, typename remove_cv<T>::type>::value ||
       is_same<bool, typename remove_cv<T>::type>::value ||
       is_same<char, typename remove_cv<T>::type>::value ||
       is_same<wchar_t, typename remove_cv<T>::type>::value > {};

    // is_floating_point<T>::value == true if and only if T is one of the types:
    template<class T> struct is_floating_point: integral_constant<bool,
       is_same<float, typename remove_cv<T>::type>::value ||
       is_same<double, typename remove_cv<T>::type>::value ||
       is_same<long double, typename remove_cv<T>::type>::value> {};

    // is_arithmetic<T>::value == true if and only if:
    // is_floating_point<T>::value == true, or
    // is_integral<T>::value == true
    template <typename T>
    struct is_arithmetic: integral_constant<bool,
       is_integral<T>::value ||
       is_floating_point<T>::value >{};


    // is_fundamental<T>::value == true if and only if:
    // is_floating_point<T>::value == true, or
    // is_integral<T>::value == true, or
    // is_void<T>::value == true
    template <typename T>
    struct is_fundamental: integral_constant<bool,
       is_void<T>::value ||
       is_integral<T>::value ||
       is_floating_point<T>::value >{};

   template<class T> struct is_pointer_helper    : false_type {};
   template<class T> struct is_pointer_helper<T*>: true_type {};
   template<class T> struct is_pointer: is_pointer_helper<typename remove_cv<T>::type> {};

   // is_scalar<T>::value == true if and only if:
   // is_arithmetic<T>::value == true, or
   // is_enum<T>::value == true, or -- not implemented: enum detection needs compiler support
   // is_pointer<T>::value == true, or
   // is_member_pointer<T>::value == true
   template<typename T>
   struct is_scalar: integral_constant<bool,
      is_arithmetic<T>::value ||
      is_pointer<T>::value
      /* || is_enum<T>::value
         || is_member_pointer<T>::value
         || is_same<std::nullptr_t, typename remove_cv<T>::type>::value */ > {};

   template<class T> struct is_array: false_type {};
   template<class T> struct is_array<T[]>: true_type {};
   template<class T, ttl::size_t N> struct is_array<T[N]>: true_type {};

}
#endif // _TINY_TEMPLATE_LIBRARY_TYPE_TRAITS_HPP_
