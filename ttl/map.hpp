/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: the map template implementation
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_MAP_HPP_
#define _TINY_TEMPLATE_LIBRARY_MAP_HPP_ 1

#include "types.hpp"
#include "functional.hpp"
#include "utility.hpp"
#include "rbtree.hpp"

namespace ttl
{
   template<typename KT, typename T, typename Compare = less<KT> >
   class map // unique keys to values
   {
   public:
      typedef KT key_type;
      typedef T mapped_type;
      typedef pair<const KT, T> value_type;
      typedef ttl::size_t size_type;
      typedef ttl::ptrdiff_t difference_type;
      typedef Compare key_compare;
      typedef value_type &reference;
      typedef const value_type &const_reference;
      typedef value_type *pointer;
      typedef const value_type *const_pointer;

      struct value_compare
      {
         typedef value_type first_argument_type;
         typedef value_type second_argument_type;
         typedef bool result_type;

         bool operator()(const value_type &a, const value_type &b) const
         {
            return Compare()(a.first, b.first);
         }
      };

   private:
      typedef rbtree<KT, pair<const KT, T>, select_first< pair<const KT,T> >, Compare> tree_type;
      typedef typename tree_type::node node_type;

      tree_type rbtree_;

   public:
      struct const_iterator;

      struct iterator
      {
         typedef typename map<KT,T,Compare>::node_type node_type;
      public:
         typedef map<KT,T,Compare>::value_type value_type;
         typedef ttl::ptrdiff_t difference_type;
         typedef value_type *pointer;
         typedef value_type *reference;

         value_type &operator*() const { return ptr_->data; }
         value_type *operator->() const { return &ptr_->data; }
         iterator &operator++()
         {
            ptr_ = static_cast<node_type *>(rbtree_base::next_node(ptr_));
            return *this;
         }
         iterator operator++(int)
         {
            iterator tmp(*this);
            ptr_ = static_cast<node_type *>(rbtree_base::next_node(ptr_));
            return tmp;
         }
         iterator &operator--() { ptr_ = prev(ptr_); return *this; }
         iterator operator--(int)
         {
            iterator tmp(*this);
            ptr_ = prev(ptr_);
            return tmp;
         }

         bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
         bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }
         bool operator==(const const_iterator &other) const { return other == *this; }
         bool operator!=(const const_iterator &other) const { return other != *this; }
      private:
         node_type *ptr_;
         friend class map<KT,T,Compare>;
         friend class map<KT,T,Compare>::const_iterator;
         iterator(node_type *ptr): ptr_(ptr) {}
         static node_type *prev(const node_type *);
      };
      struct const_iterator
      {
         typedef typename map<KT,T,Compare>::iterator::node_type node_type;
      public:
         typedef map<KT,T,Compare>::value_type value_type;
         typedef ttl::ptrdiff_t difference_type;
         typedef value_type *pointer;
         typedef value_type *reference;

         const value_type &operator*() const { return ptr_->data; }
         const value_type *operator->() const { return &ptr_->data; }
         const_iterator &operator++()
         {
            ptr_ = static_cast<const node_type *>(rbtree_base::next_node(ptr_));
            return *this;
         }
         const_iterator operator++(int)
         {
            const_iterator tmp(*this);
            ptr_ = static_cast<const node_type *>(rbtree_base::next_node(ptr_));
            return tmp;
         }
         const_iterator &operator--()
         {
            ptr_ = iterator::prev(ptr_);
            return *this;
         }
         const_iterator operator--(int)
         {
            const_iterator tmp(*this);
            ptr_ = iterator::prev(ptr_);
            return tmp;
         }

         bool operator==(const const_iterator &other) const { return ptr_ == other.ptr_; }
         bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
         bool operator!=(const const_iterator &other) const { return ptr_ != other.ptr_; }
         bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }

         const_iterator(const iterator &other): ptr_(other.ptr_) {}
      private:
         const node_type *ptr_;
         friend class map<KT,T,Compare>;
         const_iterator(const node_type *ptr): ptr_(ptr) {}
      };

      iterator end()
      {
         return iterator(static_cast<node_type *>(rbtree_.end()));
      }
      const_iterator end() const
      {
         return const_iterator(static_cast<const node_type *>(rbtree_.end()));
      }
      const_iterator cend() const
      {
         return const_iterator(static_cast<const node_type *>(rbtree_.end()));
      }

      iterator begin()
      {
         rbnode *root = rbtree_.get_root();
         return root ? iterator(static_cast<node_type *>(rbtree_base::min_node(root))): end();
      }
      const_iterator begin() const
      {
         const rbnode *root = rbtree_.get_root();
         return root ? const_iterator(static_cast<const node_type *>(rbtree_base::min_node(root))): end();
      }
      const_iterator cbegin() const
      {
         const rbnode *root = rbtree_.get_root();
         return root ? const_iterator(static_cast<const node_type *>(rbtree_base::min_node(root))): cend();
      }

      explicit map() {}
      ~map() {}

      map(const map &other)
      {
         rbtree_.assign(other.rbtree_);
      }

      template<class InputIt> map(InputIt first, InputIt last)
      {
         insert(first, last);
      }

      map &operator=(const map &other)
      {
         clear();
         rbtree_.assign(other.rbtree_);
         return *this;
      }

      pair<iterator,bool> insert(const value_type &value)
      {
         pair<node_type *, bool> re = rbtree_.insert_unique(value);
         return pair<iterator,bool>(iterator(re.first), re.second);
      }
      iterator insert(iterator, const value_type &);

      template<class InputIt> void insert(InputIt first, InputIt last);

      T &operator[](const KT &key)
      {
         node_type *n = rbtree_.find(key);
         if (n == rbtree_.end())
            n = rbtree_.insert_unique(value_type(key, typename value_type::second_type())).first;
         return n->data.second;
      }

      T &at(const KT &key) { return rbtree_.find(key)->data.second; }
      const T &at(const KT &key) const { return rbtree_.find(key)->data.second; }

      void clear()
      {
         rbtree_.clear();
      }

      // size_type size() const { return 0 /* TODO */; }
      bool empty() const { return !rbtree_.get_root(); }
      size_type max_size() const { return (size_type)-1 / sizeof(node_type); }

      iterator erase(const_iterator pos);
      iterator erase(const_iterator first, const_iterator last);

      size_type erase(const KT &key)
      {
         node_type *n = rbtree_.remove(key);
         delete n;
         return !!n;
      }

      void swap(map &other);

      //
      // The map template has unique keys (so all ranges are either empty or
      // 1 element long), so the amount of generated template code can be
      // reduced by using only find and lower_bound. 
      //

      iterator find(const KT &key) { return iterator(rbtree_.find(key)); }
      const_iterator find(const KT &key) const { return const_iterator(rbtree_.find(key)); }

      size_type count(const KT &key) const { return rbtree_.find(key) != rbtree_.end(); }

      iterator lower_bound(const KT &key) { return iterator(rbtree_.lower_bound(key)); }
      const_iterator lower_bound(const KT &key) const { return const_iterator(rbtree_.lower_bound(key)); }

      iterator upper_bound(const KT &key);
      const_iterator upper_bound(const KT &key) const;

      pair<iterator, iterator> equal_range(const KT &key);
      pair<const_iterator, const_iterator> equal_range(const KT &key) const;
   };

   template<typename KT, typename T, typename Compare>
   template<class InputIt>
   void map<KT,T,Compare>::insert(InputIt first, InputIt last)
   {
      for (; first != last; ++first)
         rbtree_.insert_unique(value_type(first->first, first->second));
   }

   template<typename KT, typename T, typename Compare>
   typename map<KT,T,Compare>::iterator::node_type *
   map<KT,T,Compare>::iterator::prev(const node_type *n)
   {
      // check if it is the sentinel/header
      if (!n->parent)
         ;
      else if (n->color == rbnode::RED && static_cast<const node_type *>(n->parent->parent) == n)
         n = static_cast<node_type *>(rbtree_base::max_node(n->parent));
      else
         n = static_cast<node_type *>(rbtree_base::prev_node(n));
      return const_cast<node_type *>(n);
   }

   template<typename KT, typename T, typename Compare>
   typename map<KT,T,Compare>::iterator map<KT,T,Compare>::upper_bound(const KT &key)
   {
      node_type *lo = rbtree_.lower_bound(key);
      if (lo == rbtree_.end())
         return end();
      return iterator(static_cast<node_type *>(rbtree_base::next_node(lo)));
   }
   template<typename KT, typename T, typename Compare>
   typename map<KT,T,Compare>::const_iterator map<KT,T,Compare>::upper_bound(const KT &key) const
   {
      const node_type *lo = rbtree_.lower_bound(key);
      if (lo == rbtree_.end())
         return end();
      return const_iterator(static_cast<const node_type *>(rbtree_base::next_node(lo)));
   }

   template<typename KT, typename T, typename Compare>
   pair<typename map<KT,T,Compare>::iterator, typename map<KT,T,Compare>::iterator>
   map<KT,T,Compare>::equal_range(const KT &key)
   {
      node_type *lo = rbtree_.lower_bound(key);
      node_type *up = lo;
      if (lo != rbtree_.end())
         up = static_cast<node_type *>(rbtree_base::next_node(lo));
      return pair<iterator, iterator>(iterator(lo), iterator(up));
   }
   template<typename KT, typename T, typename Compare>
   pair<typename map<KT,T,Compare>::const_iterator, typename map<KT,T,Compare>::const_iterator>
   map<KT,T,Compare>::equal_range(const KT &key) const
   {
      const node_type *lo = rbtree_.lower_bound(key);
      const node_type *up = lo;
      if (lo != rbtree_.end())
         up = static_cast<const node_type *>(rbtree_base::next_node(lo));
      return pair<const_iterator, const_iterator>(const_iterator(lo), const_iterator(up));
   }

   template<class InputIt1, class InputIt2>
   bool equal(InputIt1, InputIt1, InputIt2);

   template <typename KT, typename T, typename Compare>
   bool operator==(const map<KT,T,Compare> &a, const map<KT,T,Compare> &b)
   {
      return ttl::equal(a.begin(), a.end(), b.begin(), b.end());
   }
   template <typename KT, typename T, typename Compare>
   bool operator!=(const map<KT,T,Compare> &a, const map<KT,T,Compare> &b)
   {
      return !(a == b);
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_MAP_HPP_
