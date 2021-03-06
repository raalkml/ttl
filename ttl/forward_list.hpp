/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of STL forward_list
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
#define _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_ 1

#include "types.hpp"
#include "slist_node.hpp"

namespace ttl
{
   template<typename T>
   class forward_list
   {
   public:
      typedef T value_type;
      typedef T *pointer;
      typedef const T *const_pointer;
      typedef T &reference;
      typedef const T &const_reference;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;

   private:
      struct node: slist_node
      {
         T value;
         node(const T &v): value(v) {}
      };
      slist_node head_;

   public:
      class const_iterator;

      class iterator
      {
      public:
         typedef T value_type;
         typedef T *pointer;
         typedef T &reference;
         typedef ttl::ptrdiff_t difference_type;

         iterator() /* head_ left uninitialized */ {}
         ~iterator() {}
         iterator &operator++() { head_ = head_->next; return *this; }
         iterator operator++(int) { iterator tmp(head_); head_ = head_->next; return tmp; }
         reference operator*() const { return static_cast<forward_list<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<forward_list<T>::node *>(head_)->value; }

         bool operator==(const iterator &other) const { return head_ == other.head_; }
         bool operator!=(const iterator &other) const { return head_ != other.head_; }
         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         slist_node *head_;
         friend class forward_list<T>;
         iterator(slist_node *head): head_(head) {}
      };
      class const_iterator
      {
      public:
         typedef forward_list<T>::iterator iterator;
         typedef T value_type;
         typedef const T *pointer;
         typedef const T &reference;
         typedef ttl::ptrdiff_t difference_type;

         const_iterator() /* head_ left uninitialized */ {}
         const_iterator(const iterator &o): head_(o.head_) {}
         ~const_iterator() {}
         const_iterator &operator++() { head_ = head_->next; return *this; }
         const_iterator operator++(int) { const_iterator tmp(head_); head_ = head_->next; return tmp; }
         reference operator*() const { return static_cast<const forward_list<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<const forward_list<T>::node *>(head_)->value; }

         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         friend class forward_list<T>;
         friend class forward_list<T>::iterator;
         const slist_node *head_;
         const_iterator(const slist_node *head): head_(head) {}
      };

      forward_list() { head_.next = 0; }
      forward_list(const forward_list &other)
      {
         head_.next = 0;
         insert_after(cbefore_begin(), other.cbegin(), other.cend());
      }
      forward_list(size_type n, const T &value)
      {
         head_.next = 0;
         insert_after(cbefore_begin(), n, value);
      }
      template<typename InputIterator>
      forward_list(InputIterator first, InputIterator last)
      {
         head_.next = 0;
         insert_after(cbefore_begin(), first, last);
      }
      ~forward_list() { clear(); }

      forward_list &operator=(const forward_list &other)
      {
         clear();
         insert_after(cbefore_begin(), other.cbegin(), other.cend());
         return *this;
      }

      iterator before_begin() { return iterator(&head_); }
      iterator begin() { return iterator(head_.next); }
      iterator end() { return iterator(0); }
      const_iterator before_begin() const { return const_iterator(&head_); }
      const_iterator begin() const { return const_iterator(head_.next); }
      const_iterator end() const { return const_iterator(0); }
      const_iterator cbefore_begin() const { return const_iterator(&head_); }
      const_iterator cbegin() const { return const_iterator(head_.next); }
      const_iterator cend() const { return const_iterator(0); }

      bool empty() const { return !head_.next; }
      size_type max_size() const { return (size_type)-1/sizeof(node); }

      reference front() { return static_cast<node *>(head_.next)->value; }
      const_reference front() const { return static_cast<const node *>(head_.next)->value; }

      template<typename InputIterator>
      void assign(InputIterator first, InputIterator last)
      {
         clear();
         insert_after(cbefore_begin(), first, last);
      }

      void assign(size_type n, const T &value)
      {
         clear();
         insert_after(cbefore_begin(), n, value);
      }

      void push_front(const T &value)
      {
         head_.insert_after(new node(value));
      }

      void pop_front()
      {
         delete static_cast<node *>(head_.unlink_next());
      }

      void splice_after(const_iterator pos, forward_list &other)
      {
         const_cast<slist_node *>(pos.head_)->splice_after(&other.head_, 0);
      }
      void splice_after(const_iterator pos, forward_list &, const_iterator it)
      {
         slist_node *p = const_cast<slist_node *>(pos.head_);
         slist_node *o = const_cast<slist_node *>(it.head_);
         if (o == p)
            return;
         p->splice_after(o);
      }
      void splice_after(const_iterator pos, forward_list &, const_iterator first, const_iterator last);

      iterator insert_after(const_iterator pos, const T &value)
      {
         return iterator(const_cast<slist_node *>(pos.head_)->insert_after(new node(value)));
      }
      void insert_after(const_iterator pos, size_type n, const T &value);
      template<typename InputIterator>
      void insert_after(const_iterator pos, InputIterator first, InputIterator last);

      iterator erase_after(const_iterator pos)
      {
         slist_node *p = const_cast<slist_node *>(pos.head_);
         delete static_cast<node *>(p->unlink_next());
         return iterator(p->next);
      }
      iterator erase_after(const_iterator pos, const_iterator last);

      void swap(forward_list &other)
      {
         ttl::swap(head_.next, other.head_.next);
      }

      void resize(size_type);
      void resize(size_type, const T &value);

      void clear();

      void remove(const T &);
      template<typename Predicate>
      void remove_if(Predicate);

      void unique();
      template<typename BinaryPredicate>
      void unique(BinaryPredicate);

      void merge(forward_list &); // merge sorted lists
      template<typename Compare>
      void merge(forward_list &, Compare);

      // O(N*log(N)) sort
      void sort();
      template<typename Compare>
      void sort(Compare);

      void reverse();
   };
   template<typename T>
   void forward_list<T>::insert_after(const_iterator pos, size_type n, const T &value)
   {
      slist_node *p = const_cast<slist_node *>(pos.head_);
      while (n--)
         p = p->insert_after(new node(value));
   }
   template<typename T>
   template<typename InputIterator>
   void forward_list<T>::insert_after(const_iterator pos, InputIterator first, InputIterator last)
   {
      slist_node *p = const_cast<slist_node *>(pos.head_);
      for ( ; first != last; ++first)
         p = p->insert_after(new node(*first));
   }
   template<typename T>
   void forward_list<T>::clear()
   {
      while (head_.next)
         delete static_cast<node *>(head_.unlink_next());
   }
   template<typename T>
   typename forward_list<T>::iterator forward_list<T>::erase_after(const_iterator pos, const_iterator last)
   {
      slist_node *p = const_cast<slist_node *>(pos.head_);
      while (p->next != last.head_)
         delete static_cast<node *>(p->unlink_next());
      return iterator(p->next);
   }
   template<typename T>
   void forward_list<T>::reverse()
   {
      head_.reverse();
   }
   template<typename T>
   void forward_list<T>::splice_after(const_iterator pos, forward_list &, const_iterator first, const_iterator last)
   {
      slist_node *f = const_cast<slist_node *>(first.head_);

      if (!f)
         return;

      const_cast<slist_node *>(pos.head_)->splice_after(f, const_cast<slist_node *>(last.head_));
   }

   template<class InputIt1, class InputIt2>
   bool equal(InputIt1, InputIt1, InputIt2, InputIt2);

   template <typename T>
   bool operator==(const forward_list<T> &a, const forward_list<T> &b)
   {
      return ttl::equal(a.begin(), a.end(), b.begin(), b.end());
   }
   template <typename T>
   bool operator!=(const forward_list<T> &a, const forward_list<T> &b)
   {
      return !(a == b);
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
