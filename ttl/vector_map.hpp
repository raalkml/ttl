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
// stable_sort<> and searched with binary_search, lower_bound, uppper_bound and
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
   private:
      vector_map<KT,T> &operator=(const vector_map &other); /* none */
   };
}
#endif // _TINY_TEMPLATE_LIBRARY_SORTED_VECTOR_MAP_HPP_
