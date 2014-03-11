/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: a vector of key-value pairs.
// Unsorted, keys may be not unique.
//
// Can be used as a small map: the underlying data structure is an array.
// It has much less element overhead than a red-black tree implementation
// and the data cache locality of an array.
//
// Insertions in the middle, deletions in the middle and lookups are, as
// expected, of linear complexity.
//
// A vector_map provides a random iterator and can be sorted with sort<> or
// stable_sort<> and searhed with binary_search, lower_bound, uppper_bound and
// equal_range operations.
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_VECTOR_MAP_HPP_
#define _TINY_TEMPLATE_LIBRARY_VECTOR_MAP_HPP_ 1

#include <new>
#include "utility.hpp"
#include "functional.hpp"
#include "vector.hpp"

namespace ttl
{
   template<typename KT, typename T>
   class vector_map: public vector< pair<KT, T> >
   {
   public:
      typedef KT key_type;
      typedef T mapped_type;
      typedef pair<KT, T> value_type;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef value_type &reference;
      typedef const value_type &const_reference;
      typedef value_type *pointer;
      typedef const value_type *const_pointer;
      typedef typename vector<value_type>::iterator iterator;
      typedef typename vector<value_type>::const_iterator const_iterator;

      explicit vector_map() {}
      vector_map(const vector_map &other): vector<value_type>(other) {}

      iterator       begin() { return vector<value_type>::begin(); }
      const_iterator begin() const { return vector<value_type>::begin(); }
      iterator       end() { return vector<value_type>::end(); }
      const_iterator end() const { return vector<value_type>::end(); }
      const_iterator cbegin() const { return vector<value_type>::cbegin(); }
      const_iterator cend() const { return vector<value_type>::cend(); }

      T &operator[](const KT &key)
      {
         iterator i = find(key);
         if (i != end())
            return i->second;
         return vector<value_type>::insert(i, value_type(key, T()))->second;
      }

      T &at(const KT &key) { return find(key)->second; }
      const T &at(const KT &key) const { return find(key)->second; }

      iterator find(const KT &key) { return iterator(find_key(begin(), key)); }
      const_iterator find(const KT &key) const { return const_iterator(find_key(begin(), key)); }

      size_type count(const KT &key) const
      {
         size_type n = 0;
         for (const_iterator i = begin(); (i = find_key(i, key)) != end();)
            ++n;
         return n;
      }

      pair<iterator,bool> insert(const value_type &value)
      {
         iterator i = find(value.first);
         if (i != end())
            return pair<iterator, bool>(i, false);
         return pair<iterator, bool>(vector<value_type>::insert(i, value), true);
      }

    protected:
      iterator find_key(const_iterator i, const KT &key) const
      {
         for (; i != end(); ++i)
            if (key == i->first)
               break;
         return const_cast<iterator>(i);
      }
   };
#if 0
   template<typename KT, typename T>
   class vector_map
   {
   public:
      typedef KT key_type;
      typedef T mapped_type;
      typedef pair<KT, T> value_type;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;

      typedef value_type &reference;
      typedef const value_type &const_reference;

      typedef value_type *pointer;
      typedef const value_type *const_pointer;

      struct const_iterator;

      struct iterator
      {
      public:
         typedef vector_map<KT,T,Compare>::value_type value_type;
         typedef vector_map<KT,T,Compare>::difference_type difference_type;
         typedef value_type *pointer;
         typedef value_type *reference;

         value_type &operator*() const { return **ptr_; }
         value_type *operator->() const { return *ptr_; }
         iterator &operator++() { ++ptr_; return *this; }
         iterator operator++(int) { iterator tmp(*this); ++ptr_; return tmp; }
         iterator &operator--() { --ptr_; return *this; }
         iterator operator--(int) { iterator tmp(*this); --ptr_; return tmp; }

         bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
         bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }
         bool operator==(const const_iterator &other) const { return other == *this; }
         bool operator!=(const const_iterator &other) const { return other != *this; }
      private:
         value_type **ptr_;
         friend class vector_map<KT,T,Compare>;
         friend class vector_map<KT,T,Compare>::const_iterator;
         iterator(value_type **ptr): ptr_(ptr) {}
      };
      struct const_iterator
      {
      public:
         typedef vector_map<KT,T,Compare>::value_type value_type;
         typedef vector_map<KT,T,Compare>::difference_type difference_type;
         typedef value_type *pointer;
         typedef value_type *reference;

         const value_type &operator*() const { return **ptr_; }
         const value_type *operator->() const { return *ptr_; }
         const_iterator &operator++() { ++ptr_; return *this; }
         const_iterator operator++(int) { const_iterator tmp(*this); ++ptr_; return tmp; }
         const_iterator &operator--() { --ptr_; return *this; }
         const_iterator operator--(int) { const_iterator tmp(*this); --ptr_; return tmp; }

         bool operator==(const const_iterator &other) const { return ptr_ == other.ptr_; }
         bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
         bool operator!=(const const_iterator &other) const { return ptr_ != other.ptr_; }
         bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }

         const_iterator(const iterator &other): ptr_(other.ptr_) {}
      private:
         const value_type * const *ptr_;
         friend class sorted_vector_map<KT,T,Compare>;
         const_iterator(const value_type * const *ptr): ptr_(ptr) {}
      };

   public:
      explicit vector_map():
         elements_(0), last_(0), end_of_elements_(0)
      {}
      vector_map(const vector_map& other);
      explicit vector_map(size_type prealloc)
      {
         elements_ = static_cast<value_type *>(::operator new(prealloc * sizeof(value_type)));
         last_ = elements_;
         end_of_elements_ = elements_ + prealloc;
      }
      template<class InputIt>
      vector_map(InputIt first, InputIt last);

      ~vector_map()
      {
         clear();
         ::operator delete(elements_);
      }

      vector_map &operator=(const vector_map &other);

      T &operator[](const KT &key)
      {
         iterator i = find(key);
         if (i != end())
            return i->second;
         return insert(i, value_type(key, T()))->second;
      }

      T &at(const KT &key) { return find(key)->second; }
      const T &at(const KT &key) const { return find(key)->second; }

      iterator       begin() { return iterator(elements_); }
      const_iterator begin() const { return const_iterator(elements_); }
      iterator       end() { return iterator(last_); }
      const_iterator end() const { return const_iterator(last_); }
      const_iterator cbegin() const { return const_iterator(elements_); }
      const_iterator cend() const { return const_iterator(last_); }

      size_type size() const { return last_ - elements_; }
      bool empty() const { return last_ == elements_; }
      size_type max_size() const { return (size_type)-1 / sizeof(value_type); }
      size_type capacity() const { return end_of_elements_ - elements_; }

      void clear()
      {
         while (last_ > elements_)
            (--last_)->~T();
      }

      void push_back(const value_type &value)
      {
         insert(end(), value);
      }

      void pop_back()
      {
         erase(--cend());
      }

      pair<iterator,bool> insert(const value_type &value)
      {
         iterator i = find(value.first);
         if (i != end())
            return pair<iterator, bool>(i, false);
         return pair<iterator, bool>(insert(i, value), true);
      }
      iterator insert(iterator, const value_type &);

      template<class InputIt>
      void insert(InputIt first, InputIt last);

      iterator erase(const_iterator pos);
      iterator erase(const_iterator first, const_iterator last);
      size_type erase(const key_type &key);

      void swap(vector_map &other);

      iterator find(const KT &key) { return iterator(find_key(elements_, key)); }
      const_iterator find(const KT &key) const { return const_iterator(find_key(elements_, key)); }

      size_type count(const KT &key) const
      {
         size_type n = 0;
         for (const value_type *i = elements_; (i = find_key(i, key)) != last_;)
            ++n;
         return n;
      }

   private:
      value_type *elements_, *last_, *end_of_elements_;

      value_type *find_key(const value_type *i, const KT &k) const
      {
         for (; i != last_; ++i)
            if (key == i->first)
               break;
         return const_cast<value_type *>(i);
      }
   };

   template<typename KT, typename T>
   vector_map<KT,T>::vector_map(const vector_map& other)
   {
      size_type prealloc = other.end_of_elements_ - other.elements_;
      last_ = elements_ = static_cast<value_type *>(::operator new(prealloc * sizeof(value_type)));
      end_of_elements_ = elements_ + prealloc;
      for (const value_type *i = other.elements_; i != other.last_; ++i)
         new(last_++) value_type(*i);
   }

   template<typename KT, typename T>
   typename vector_map<KT,T>::iterator
   vector_map<KT,T>::insert(iterator pos, const value_type& value)
   {
      if (end_of_elements_ - last_ < 1)
      {
         value_type *o;
         value_type *i;
         size_type newcapacity = end_of_elements_ - elements_;
         newcapacity += newcapacity / 2 + 2 * !newcapacity;
         o = static_cast<value_type *>(::operator new(newcapacity * sizeof(value_type)));
         value_type *newelements = o;
         for (i = elements_; i != pos.ptr_;)
            *o++ = *i++;
         new(o) value_type(value);
         pos = iterator(o++);
         for (; i != last_;)
            *o++ = *i++;
         clear();
         ::operator delete(elements_);
         elements_ = newelements;
         end_of_elements_ = elements_ + newcapacity;
         last_ = o;
         return pos;
      }
      value_type **i = last_++;
      value_type **o = last_;
      while (i != pos.ptr_)
         *--o = *--i;
      *i = new value_type(value);
      return iterator(i);
   }
#endif
}
#endif // _TINY_TEMPLATE_LIBRARY_SORTED_VECTOR_MAP_HPP_
