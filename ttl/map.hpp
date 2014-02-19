/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: the map template implementation
//
#ifndef _TINY_TEMPLATE_LIBRARY_MAP_HPP_
#define _TINY_TEMPLATE_LIBRARY_MAP_HPP_ 1

#include "utility.hpp"

namespace ttl
{
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

      struct const_iterator;

      struct iterator
      {
      public:
         typedef map<KT,T,Compare>::value_type value_type;
         typedef ttl::ptrdiff_t difference_type;
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
         friend class map<KT,T,Compare>;
         friend class map<KT,T,Compare>::const_iterator;
         iterator(value_type **ptr): ptr_(ptr) {}
      };
      struct const_iterator
      {
      public:
         typedef map<KT,T,Compare>::value_type value_type;
         typedef ttl::ptrdiff_t difference_type;
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
         friend class map<KT,T,Compare>;
         const_iterator(const value_type * const *ptr): ptr_(ptr) {}
      };

   public:
      explicit map(): elements_(NULL), last_(NULL), end_of_elements_(NULL) {}
      map(const map& other);
      explicit map(size_type prealloc)
      {
         elements_ = last_ = new value_type*[prealloc];
         end_of_elements_ = elements_ + prealloc;
      }
      explicit map(const Compare &c): elements_(NULL), last_(NULL), end_of_elements_(NULL), comp_(c) {}

      template<class InputIt>
      map(InputIt first, InputIt last, const Compare & = Compare());
      map &operator=(const map &other);

      ~map()
      {
         for (value_type **i = elements_; i != last_; ++i)
            delete *i;
         delete [] elements_;
      }

      T &operator[](const KT &key)
      {
         iterator i = find(key);
         if (i == end())
            i = insert_and_sort(ttl::make_pair(key, T()));
         return i->second;
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

      void clear()
      {
         while (last_ > elements_)
            delete *--last_;
      }

      ttl::pair<iterator,bool> insert(const value_type &value)
      {
         iterator i = find(value.first);
         if (i != end())
            return ttl::pair<iterator, bool>(i, false);
         return ttl::pair<iterator, bool>(insert_and_sort(value), true);
      }
      iterator insert(iterator, const value_type &);

      template<class InputIt>
      void insert(InputIt first, InputIt last);

      iterator erase(const_iterator pos);
      iterator erase(const_iterator first, const_iterator last);
      size_type erase(const key_type &key);

      void swap(map &other);

      iterator find(const KT &key);
      const_iterator find(const KT &key) const;

      size_type count(const KT &key) const { return find(key) == cend(); }

      ttl::pair<iterator,iterator> equal_range(const KT &key)
      {
         iterator i1 = find(key);
         iterator i2 = i1;
         if (i2 != end())
            ++i2;
         return ttl::make_pair(i1, i2);
      }
      ttl::pair<const_iterator,const_iterator> equal_range(const KT &key) const
      {
         const_iterator i1 = find(key);
         const_iterator i2 = i1;
         if (i2 != cend())
            ++i2;
         return ttl::make_pair(i1, i2);
      }

      iterator lower_bound(const KT &key) { return find(key); }
      const_iterator lower_bound(const KT &key) const { return find(key); }

      iterator upper_bound(const KT &key)
      {
         iterator i = find(key);
         if (i != end())
            ++i;
         return i;
      }
      const_iterator upper_bound(const KT &key) const
      {
         const_iterator i = find(key);
         if (i != cend())
            ++i;
         return i;
      }

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
      value_compare value_comp() const { return value_compare(comp_); }

   private:
      value_type **elements_, **last_, **end_of_elements_;
      Compare comp_;
      iterator insert_and_sort(const value_type &);
   };

   template<typename KT, typename T, typename Compare>
   map<KT,T,Compare>::map(const map& other): comp_(other.comp_)
   {
      size_type prealloc = other.end_of_elements_ - other.elements_;
      elements_ = last_ = new value_type*[prealloc];
      end_of_elements_ = elements_ + prealloc;
      for (const value_type * const *i = other.elements_; i != other.last_; ++i)
         *last_++ = new value_type(**i);
   }

   template<typename KT, typename T, typename Compare>
   typename map<KT,T,Compare>::iterator map<KT,T,Compare>::find(const KT &key)
   {
      unsigned L = 0, H = size();
      while (L < H) {
         unsigned m = L + (H - L) / 2;
         if (elements_[m]->first == key)
            return iterator(elements_ + m);
         if (comp_(elements_[m]->first, key))
            L = m + 1;
         else
            H = m;
      }
      return end();
   }
   template<typename KT, typename T, typename Compare>
   typename map<KT,T,Compare>::const_iterator map<KT,T,Compare>::find(const KT &key) const
   {
      unsigned L = 0, H = size();
      while (L < H) {
         unsigned m = L + (H - L) / 2;
         if (elements_[m]->first == key)
            return const_iterator(elements_ + m);
         if (comp_(elements_[m]->first, key))
            L = m + 1;
         else
            H = m;
      }
      return cend();
   }
   template<typename KT, typename T, typename Compare>
   typename map<KT,T,Compare>::iterator map<KT,T,Compare>::insert_and_sort(const value_type& value)
   {
      if (end_of_elements_ - last_ < 1)
      {
         value_type **o;
         value_type **i;
         size_type newcapacity = end_of_elements_ - elements_;
         newcapacity += newcapacity/2; // temporarily funny...
         if (!newcapacity)
            newcapacity = 2;
         value_type **newelements = o = new value_type *[newcapacity];
         for (i = elements_; i != last_;)
            *o++ = *i++;
         delete [] elements_;
         elements_ = newelements;
         end_of_elements_ = elements_ + newcapacity;
         last_ = o;
         while (o < end_of_elements_)
            *o++ = NULL;
      }
      value_type *newvalue = new value_type(value);
      *last_++ = newvalue;
      for (size_type i = 1, n = last_ - elements_; i < n; ++i)
      {
         value_type *k = elements_[i];
         size_type j = i - 1;
         while (comp_(k->first, elements_[j]->first))
         {
            elements_[j + 1] = elements_[j];
            if (j-- == 0)
               break;
         }
         elements_[j + 1] = k;
      }
      value_type **i = elements_;
      for (; i != last_; ++i)
         if (*i == newvalue)
            break;
      return iterator(i);
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_MAP_HPP_
