/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
//                        Tiny Template Library
//
// A library of STL-inspired templates designed to work in conditions
// of constrained resources, but also without RTTI and exceptions.
//
// Some templates are just implementations of STL templates.
//
// There is no allocator support and the O(log N) and similar complexity
// guarantees might be not implemented. Also reverse iterators might be
// missing.
//
// Usually only equality operators are implemented, if any.
//
#ifndef _TINY_TEMPLATE_LIBRARY_HPP_
#define _TINY_TEMPLATE_LIBRARY_HPP_ 1

#include <new>

#include "types.hpp"
#include "functional.hpp"
#include "utility.hpp"
#include "array.hpp"
#include "vector.hpp"
#include "forward_list.hpp"

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

   template<ttl::size_t N> class bitset {};

   template<typename T> class set {}; // collection of unique keys

   template<typename KT, typename T, typename Compare = ttl::less<KT> >
   class map // unique keys to values
   {
   public:
      typedef KT key_type;
      typedef T mapped_type;
      typedef ttl::pair<KT, T> value_type;
      typedef ttl::size_t size_type;
      typedef ttl::ptrdiff_t difference_type;
      typedef Compare key_compare;

      typedef value_type &reference;
      typedef const value_type &const_reference;

      typedef value_type *pointer;
      typedef const value_type *const_pointer;

      typedef value_type *iterator;
      typedef const value_type *const_iterator;

   public:
      explicit map(): elements_(NULL), last_(NULL), end_of_elements_(NULL) {}
      explicit map(const Compare &c): elements_(NULL), last_(NULL), end_of_elements_(NULL), comp_(c) {}
      map(const map& other): comp_(other.comp_) {}
      template<class InputIt>
      map(InputIt first, InputIt last, const Compare & = Compare());
      map &operator=(const map &other);

      T &operator[](const KT &key);
      T &at(const KT &key);
      const T &at(const KT &key) const;

      iterator       begin() { return elements_; }
      const_iterator begin() const { return elements_; }
      iterator       end() { return last_; }
      const_iterator end() const { return last_; }
      const_iterator cbegin() const { return elements_; }
      const_iterator cend() const { return last_; }

      size_type size() const { return last_ - elements_; }
      bool empty() const { return last_ == elements_; }
      size_type max_size() const { return (size_type)-1 / sizeof(value_type); }

      void clear();

      ttl::pair<iterator,bool> insert( const value_type &value );
      iterator insert( const_iterator hint, const value_type& value );
      template< class InputIt >
      void insert( InputIt first, InputIt last );

      iterator erase(const_iterator pos);
      iterator erase(const_iterator first, const_iterator last);
      size_type erase(const key_type &key);

      void swap(map &other);

      size_type count(const KT &key) const;
      iterator find(const KT &key);
      const_iterator find(const KT &key) const;

      ttl::pair<iterator,iterator> equal_range(const KT &key);
      ttl::pair<const_iterator,const_iterator> equal_range(const KT &key) const;

      iterator lower_bound(const KT &key);
      const_iterator lower_bound(const KT &key) const;

      iterator upper_bound(const KT &key);
      const_iterator upper_bound(const KT &key) const;

      key_compare key_comp() const { return comp_; }

      struct value_compare
      {
      protected:
         friend class map<KT, T, Compare>;
         Compare comp;
         value_compare(Compare c): comp(c) {}
      public:
         typedef value_type first_argument_type;
         typedef value_type second_argument_type;
         typedef bool result_type;

         bool operator()(const value_type &a, const value_type &b) const
         {
            return comp(a.first, b.first);
         }
      };
      value_compare value_comp() const;

   private:
      value_type *elements_, *last_, *end_of_elements_;
      Compare comp_;
   };

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
