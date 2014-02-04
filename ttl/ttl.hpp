/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
//                        Tiny Template Library
//
// A library of STL-inspired templates designed to work in conditions
// of constrained resources, but also without RTTI and exceptions.
//
// Some templates are implementations of STL, some are not.
//
// There is no allocator support and the O(log N) and similar complexity
// guarantees might be not implemented. Also reverse iterators might be
// missing.
//
// Usually only equality operators are implemented, if any.
//
// There is generally no streaming operator support.
//
#ifndef _TINY_TEMPLATE_LIBRARY_HPP_
#define _TINY_TEMPLATE_LIBRARY_HPP_ 1

#include <new>

#include "types.hpp"
#include "type_traits.hpp"
#include "functional.hpp"
#include "utility.hpp"
#include "array.hpp"
#include "vector.hpp"
#include "forward_list.hpp"
#include "map.hpp"
#include "bitset.hpp"

namespace ttl
{

   // a template for single-linked list with O(1) insertion in the back
   // (keeps tail pointer)
   template<typename T> class backward_list {};

   template<typename T> class list {};

   // Lists with a fixed predefined storage
   template<typename T, const ttl::size_t N> class fixed_list {};
   template<typename T, const ttl::size_t N> class fixed_forward_list {};
   template<typename T, const ttl::size_t N> class fixed_backward_list {};

   template<typename T> class set {}; // collection of unique keys

#if 0
   template<typename Key, typename Compare> class multiset
   {
   public:
      typedef Key     key_type;
      typedef Key     value_type;
      typedef Compare key_compare;
      typedef Compare value_compare;
      typedef typename _Key_alloc_type::pointer             pointer;
      typedef typename _Key_alloc_type::const_pointer       const_pointer;
      typedef typename _Key_alloc_type::reference           reference;
      typedef typename _Key_alloc_type::const_reference     const_reference;
      typedef typename _Rep_type::const_iterator            iterator;
      typedef typename _Rep_type::const_iterator            const_iterator;
      typedef typename _Rep_type::size_type                 size_type;
      typedef typename _Rep_type::difference_type           difference_type;

      multiset();
      explicit multiset(const _Compare &__comp);
      template<typename _InputIterator>
      multiset(_InputIterator __first, _InputIterator __last);
      template<typename _InputIterator>
      multiset(_InputIterator __first, _InputIterator __last, const _Compare& __comp);
      multiset(const multiset &);

      multiset &operator=(const multiset &);

      key_compare key_comp() const;

      iterator begin() const;
      iterator end() const;
      const_iterator cbegin() const;
      const_iterator cend() const;

      bool empty() const;
      size_type size() const;
      size_type max_size() const;

      swap(multiset &);

      iterator insert(const value_type& __x);
      iterator insert(iterator __position, const value_type& __x);
      template<typename _InputIterator>
      void insert(_InputIterator __first, _InputIterator __last);

      void erase(iterator __position);
      size_type erase(const key_type& __x);
      void erase(iterator __first, iterator __last);
      void clear();

      size_type count(const key_type& __x) const;
      iterator find(const key_type& __x);
      const_iterator find(const key_type& __x) const;
      iterator lower_bound(const key_type& __x);
      const_iterator lower_bound(const key_type& __x) const;
      iterator upper_bound(const key_type& __x);
      const_iterator upper_bound(const key_type& __x) const;
      ttl::pair<iterator, iterator> equal_range(const key_type& __x);
      ttl::pair<const_iterator, const_iterator> equal_range(const key_type& __x) const;

   };

   template<typename T> class multimap { };
#endif // NO
}

#endif // _TINY_TEMPLATE_LIBRARY_HPP_
