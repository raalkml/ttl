/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: a template for single-linked list with O(1)
// insertion in the back (keeps tail pointer). Can be used to implement
// a queue, or a FIFO.
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_BACKWARD_LIST_HPP_
#define _TINY_TEMPLATE_LIBRARY_BACKWARD_LIST_HPP_ 1

#include "types.hpp"
#include "utility.hpp"
#include "slist_node.hpp"

namespace ttl
{
   template<typename T>
   class backward_list
   {
   public:
      typedef T value_type;
      typedef T *pointer;
      typedef const T *const_pointer;
      typedef T &reference;
      typedef const T &const_reference;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;

   private:
      struct node: slist_node
      {
         T value;
         node(const T &v): value(v) {}
      };
      slist_node head_;
      slist_node *tail_;

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
         reference operator*() const { return static_cast<backward_list<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<backward_list<T>::node *>(head_)->value; }

         bool operator==(const iterator &other) const { return head_ == other.head_; }
         bool operator!=(const iterator &other) const { return head_ != other.head_; }
         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         slist_node *head_;
         friend class backward_list<T>;
         iterator(slist_node *head): head_(head) {}
      };
      class const_iterator
      {
      public:
         typedef backward_list<T>::iterator iterator;
         typedef T value_type;
         typedef const T *pointer;
         typedef const T &reference;
         typedef ttl::ptrdiff_t difference_type;

         const_iterator() /* head_ left uninitialized */ {}
         const_iterator(const iterator &o): head_(o.head_) {}
         ~const_iterator() {}
         const_iterator &operator++() { head_ = head_->next; return *this; }
         const_iterator operator++(int) { const_iterator tmp(head_); head_ = head_->next; return tmp; }
         reference operator*() const { return static_cast<const backward_list<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<const backward_list<T>::node *>(head_)->value; }

         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         friend class backward_list<T>;
         friend class backward_list<T>::iterator;
         const slist_node *head_;
         const_iterator(const slist_node *head): head_(head) {}
      };

      backward_list()
      {
         head_.next = NULL;
         tail_ = &head_;
      }
      backward_list(const backward_list &other)
      {
         head_.next = NULL;
         tail_ = &head_;
         insert_after(cbefore_begin(), other.cbegin(), other.cend());
      }
      backward_list(size_type n, const T &value)
      {
         head_.next = NULL;
         tail_ = &head_;
         insert_after(cbefore_begin(), n, value);
      }
      template<typename InputIterator>
      backward_list(InputIterator first, InputIterator last)
      {
         head_.next = NULL;
         tail_ = &head_;
         insert_after(cbefore_begin(), first, last);
      }
      ~backward_list() { clear(); }

      backward_list &operator=(const backward_list &other)
      {
         clear();
         insert_after(cbefore_begin(), other.cbegin(), other.cend());
         return *this;
      }

      iterator before_begin() { return iterator(&head_); }
      iterator begin() { return iterator(head_.next); }
      iterator end() { return iterator(NULL); }
      iterator before_end() { return iterator(tail_); }
      const_iterator before_begin() const { return const_iterator(&head_); }
      const_iterator begin() const { return const_iterator(head_.next); }
      const_iterator end() const { return const_iterator(NULL); }
      const_iterator before_end() const { return const_iterator(tail_); }
      const_iterator cbefore_begin() const { return const_iterator(&head_); }
      const_iterator cbegin() const { return const_iterator(head_.next); }
      const_iterator cend() const { return const_iterator(NULL); }
      const_iterator cbefore_end() const { return const_iterator(tail_); }

      bool empty() const { return &head_ == tail_; }
      size_type max_size() const { return (size_type)-1/sizeof(node); }

      reference front() { return static_cast<node *>(head_.next)->value; }
      const_reference front() const { return static_cast<const node *>(head_.next)->value; }
      reference back() { return static_cast<node *>(tail_)->value; }
      const_reference back() const { return static_cast<const node *>(tail_)->value; }

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
         slist_node *n = head_.insert_after(new node(value));
         if (tail_ == &head_)
            tail_ = n;
      }
      void push_back(const T &value)
      {
         tail_ = tail_->insert_after(new node(value));
      }

      void pop_front()
      {
         delete static_cast<node *>(head_.unlink_next());
         if (empty())
            tail_ = &head_;
      }

#if TODO
      void splice_after(const_iterator pos, backward_list &other)
      {
         const_cast<slist_node *>(pos.head_)->splice_after(&other.head_, NULL);
      }
      void splice_after(const_iterator pos, backward_list &, const_iterator it)
      {
         slist_node *p = const_cast<slist_node *>(pos.head_);
         slist_node *o = const_cast<slist_node *>(it.head_);
         if (o == p)
            return;
         p->splice_after(o);
      }
      void splice_after(const_iterator pos, backward_list &, const_iterator first, const_iterator last);
#endif
      iterator insert_after(const_iterator pos, const T &value)
      {
         slist_node *pn = const_cast<slist_node *>(pos.head_);
         slist_node *n = pn->insert_after(new node(value));
         if (pn == tail_)
            tail_ = n;
         return iterator(n);
      }
      void insert_after(const_iterator pos, size_type n, const T &value);
      template<typename InputIterator>
      void insert_after(const_iterator pos, InputIterator first, InputIterator last);

      iterator erase_after(const_iterator pos)
      {
         slist_node *pn = const_cast<slist_node *>(pos.head_);
         slist_node *p = pn->unlink_next();
         if (p == tail_)
            tail_ = pn;
         delete static_cast<node *>(p);
         return iterator(pn->next);
      }
      iterator erase_after(const_iterator pos, const_iterator last);

      void swap(backward_list &other)
      {
         ttl::swap(head_.next, other.head_.next);
         ttl::swap(tail_, other.tail_);
      }

      void clear();

#if TODO
      void resize(size_type);
      void resize(size_type, const T &value);

      void remove(const T &);
      template<typename Predicate>
      void remove_if(Predicate);

      void unique();
      template<typename BinaryPredicate>
      void unique(BinaryPredicate);

      void merge(backward_list &); // merge sorted lists
      template<typename Compare>
      void merge(backward_list &, Compare);

      // O(N*log(N)) sort
      void sort();
      template<typename Compare>
      void sort(Compare);

      void reverse();
#endif
   };
   template<typename T>
   void backward_list<T>::insert_after(const_iterator pos, size_type n, const T &value)
   {
      slist_node *pn = const_cast<slist_node *>(pos.head_);
      slist_node *p = pn;
      while (n--)
         p = p->insert_after(new node(value));
      if (pn == tail_)
         tail_ = p;
   }
   template<typename T>
   template<typename InputIterator>
   void backward_list<T>::insert_after(const_iterator pos, InputIterator first, InputIterator last)
   {
      slist_node *pn = const_cast<slist_node *>(pos.head_);
      slist_node *p = pn;
      for ( ; first != last; ++first)
         p = p->insert_after(new node(*first));
      if (pn == tail_)
         tail_ = pn;
   }
   template<typename T>
   void backward_list<T>::clear()
   {
      tail_ = &head_;
      while (head_.next)
         delete static_cast<node *>(head_.unlink_next());
   }
   template<typename T>
   typename backward_list<T>::iterator backward_list<T>::erase_after(const_iterator pos, const_iterator last)
   {
      slist_node *p = const_cast<slist_node *>(pos.head_);
      while (p->next != last.head_)
         delete static_cast<node *>(p->unlink_next());
      if (last == cend())
         tail_ = p;
      return iterator(p->next);
   }
#if TODO
   template<typename T>
   void backward_list<T>::reverse()
   {
      head_.reverse();
   }
   template<typename T>
   void backward_list<T>::splice_after(const_iterator pos, backward_list &, const_iterator first, const_iterator last)
   {
      slist_node *f = const_cast<slist_node *>(first.head_);

      if (!f)
         return;

      const_cast<slist_node *>(pos.head_)->splice_after(f, const_cast<slist_node *>(last.head_));
   }
#endif
}
#endif // _TINY_TEMPLATE_LIBRARY_BACKWARD_LIST_HPP_

