/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: implementation of STL array
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_ARRAY_HPP_
#define _TINY_TEMPLATE_LIBRARY_ARRAY_HPP_

#include "types.hpp"
#include "utility.hpp"

namespace ttl
{
   template<typename T, const ttl::size_t N>
   struct array
   {
      typedef T                                     value_type;
      typedef value_type &                          reference;
      typedef const value_type &                    const_reference;
      typedef value_type *                          iterator;
      typedef const value_type *                    const_iterator;
      typedef ttl::size_t                           size_type;
      typedef ttl::ptrdiff_t                        difference_type;

      value_type elements_[N ? N : 1];

      void fill(const value_type &v)
      {
         ttl::fill_n(begin(), size(), v);
      }

      void swap(array &other)
      {
         ttl::swap_ranges(begin(), end(), other.begin());
      }

      iterator       begin() { return iterator(&elements_[0]); }
      const_iterator begin() const { return const_iterator(&elements_[0]); }
      iterator       end() { return iterator(&elements_[N]); }
      const_iterator end() const { return const_iterator(&elements_[N]); }
      const_iterator cbegin() const { return const_iterator(&elements_[0]); }
      const_iterator cend() const { return const_iterator(&elements_[N]); }

      size_type size() const { return N; }
      size_type max_size() const { return N; }
      bool empty() const { return size() == 0; }

      reference operator[](size_type i) { return elements_[i]; }
      const_reference operator[](size_type i) const { return elements_[i]; }
      reference at(size_type i) { return elements_[i]; }
      const_reference at(size_type i) const { return elements_[i]; }
      reference front() { return *begin(); }
      const_reference front() const { return *begin(); }
      reference back() { return N ? *(end() - 1) : *end(); }
      const_reference back() const { return N ? *(end() - 1) : *end(); }

      T* data() { return &elements_[0]; }
      const T* data() const { return &elements_[0]; }
   };

   template<typename T, const ttl::size_t N>
   inline bool operator==(const array<T, N> &a, const array<T, N> &b)
   {
      return a.size() == b.size() && ttl::equal(a.begin(), a.end(), b.begin());
   }
   template<typename T, const ttl::size_t N>
   inline bool operator!=(const array<T, N> &a, const array<T, N> &b)
   {
      return !(a == b);
   }

}
#endif // _TINY_TEMPLATE_LIBRARY_ARRAY_HPP_
