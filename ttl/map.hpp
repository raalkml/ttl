/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: the map template implementation
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_MAP_HPP_
#define _TINY_TEMPLATE_LIBRARY_MAP_HPP_ 1

#include "utility.hpp"
#include "functional.hpp"
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
         iterator &operator--()
         {
            ptr_ = static_cast<node_type *>(rbtree_base::prev_node(ptr_));
            return *this;
         }
         iterator operator--(int)
         {
            iterator tmp(*this);
            ptr_ = static_cast<node_type *>(rbtree_base::prev_node(ptr_));
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
      };
      struct const_iterator
      {
         typedef typename map<KT,T,Compare>::node_type node_type;
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
            ptr_ = static_cast<const node_type *>(rbtree_base::prev_node(ptr_));
            return *this;
         }
         const_iterator operator--(int)
         {
            const_iterator tmp(*this);
            ptr_ = static_cast<const node_type *>(rbtree_base::prev_node(ptr_));
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

      iterator begin()
      {
         return iterator(static_cast<node_type *>(rbtree_base::min_node(rbtree_.get_root())));
      }
      const_iterator begin() const
      {
         return const_iterator(static_cast<const node_type *>(rbtree_base::min_node(rbtree_.get_croot())));
      }
      const_iterator cbegin() const
      {
         return const_iterator(static_cast<const node_type *>(rbtree_base::min_node(rbtree_.get_croot())));
      }

      iterator end() { return iterator(0); }
      const_iterator end() const { return const_iterator(0); }
      const_iterator cend() const { return const_iterator(0); }

      explicit map() {}
      ~map() {}

      map(const map& other);
      template<class InputIt> map(InputIt first, InputIt last);
      map &operator=(const map &other);

      pair<iterator,bool> insert(const value_type &value)
      {
         node_type *n = rbtree_.insert_unique(value);
         return pair<iterator,bool>(iterator(n), n);
      }
      iterator insert(iterator, const value_type &);
      template<class InputIt>
      void insert(InputIt first, InputIt last);

      T &operator[](const KT &key)
      {
         node_type *n = rbtree_.find(key);
         if (!n)
            n = rbtree_.insert_unique(value_type(key, typename value_type::second_type()));
         return n->data.second;
      }

      T &at(const KT &key) { return rbtree_.find(key)->data.second; }
      const T &at(const KT &key) const { return rbtree_.find(key)->data.second; }

      void clear()
      {
         rbtree_.clear();
      }

      size_type size() const { return 0 /* TODO */; }
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

      size_type count(const KT &key) const { return !!rbtree_.find(key); }

      iterator lower_bound(const KT &key) { return iterator(rbtree_.lower_bound(key)); }
      const_iterator lower_bound(const KT &key) const { return const_iterator(rbtree_.lower_bound(key)); }

      iterator upper_bound(const KT &key)
      {
         node_type *lo = rbtree_.lower_bound(key);
         return iterator(lo ? static_cast<node_type *>(rbtree_base::next_node(lo)): 0);
      }
      const_iterator upper_bound(const KT &key) const
      {
         const node_type *lo = rbtree_.lower_bound(key);
         return const_iterator(lo ? static_cast<const node_type *>(rbtree_base::next_node(lo)): 0);
      }

      pair<iterator, iterator> equal_range(const KT &key)
      {
         node_type *lo = rbtree_.lower_bound(key);
         node_type *up = lo ? static_cast<node_type *>(rbtree_base::next_node(lo)): 0;
         return pair<iterator, iterator>(iterator(lo), iterator(up));
      }
      pair<const_iterator, const_iterator> equal_range(const KT &key) const
      {
         const node_type *lo = rbtree_.lower_bound(key);
         const node_type *up = lo ? static_cast<node_type *>(rbtree_base::next_node(lo)): 0;
         return pair<const_iterator, const_iterator>(const_iterator(lo), const_iterator(up));
      }
   };

}
#endif // _TINY_TEMPLATE_LIBRARY_MAP_HPP_
