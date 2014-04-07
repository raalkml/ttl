/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of "lazy queue"
//
// A single-linked list, maintaining its head and tail pointers and pooling its
// removed nodes in a list of dead nodes, to avoid allocation next time they're
// needed. The dead nodes can be deallocated on explicit request (with the
// method "clean").
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_LAZY_QUEUE_HPP_
#define _TINY_TEMPLATE_LIBRARY_LAZY_QUEUE_HPP_ 1

#include <new>
#include "types.hpp"
#include "slist_node.hpp"

namespace ttl
{
   template<typename T> void swap(T &, T &);

   template<typename T>
   class lazy_queue
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
      slist_node dead_;

      node *get_node(const T &v)
      {
         node *n = static_cast<node *>(dead_.next ? dead_.unlink_next(): ::operator new(sizeof(node)));
         ::new(&n->value) T(v);
         return n;
      }
      void put_node(node *n)
      {
         n->value.~T();
         dead_.insert_after(n);
      }

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
         reference operator*() const { return static_cast<lazy_queue<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<lazy_queue<T>::node *>(head_)->value; }

         bool operator==(const iterator &other) const { return head_ == other.head_; }
         bool operator!=(const iterator &other) const { return head_ != other.head_; }
         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         slist_node *head_;
         friend class lazy_queue<T>;
         iterator(slist_node *head): head_(head) {}
      };
      class const_iterator
      {
      public:
         typedef lazy_queue<T>::iterator iterator;
         typedef T value_type;
         typedef const T *pointer;
         typedef const T &reference;
         typedef ttl::ptrdiff_t difference_type;

         const_iterator() /* head_ left uninitialized */ {}
         const_iterator(const iterator &o): head_(o.head_) {}
         ~const_iterator() {}
         const_iterator &operator++() { head_ = head_->next; return *this; }
         const_iterator operator++(int) { const_iterator tmp(head_); head_ = head_->next; return tmp; }
         reference operator*() const { return static_cast<const lazy_queue<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<const lazy_queue<T>::node *>(head_)->value; }

         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         friend class lazy_queue<T>;
         friend class lazy_queue<T>::iterator;
         const slist_node *head_;
         const_iterator(const slist_node *head): head_(head) {}
      };

      lazy_queue()
      {
         head_.next = 0;
         tail_ = &head_;
         dead_.next = 0;
      }
      lazy_queue(const lazy_queue &other)
      {
         head_.next = 0;
         tail_ = &head_;
         dead_.next = 0;
         insert_after(cbefore_begin(), other.cbegin(), other.cend());
      }
      lazy_queue(size_type n, const T &value)
      {
         head_.next = 0;
         tail_ = &head_;
         dead_.next = 0;
         insert_after(cbefore_begin(), n, value);
      }
      ~lazy_queue()
      {
         while (head_.next)
            delete static_cast<node *>(head_.unlink_next());
         cleanup();
      }

      void cleanup()
      {
         while (dead_.next)
            ::delete(static_cast<node *>(dead_.unlink_next()));
      }

      lazy_queue &operator=(const lazy_queue &other)
      {
         clear();
         insert_after(cbefore_begin(), other.cbegin(), other.cend());
         return *this;
      }

      iterator before_begin() { return iterator(&head_); }
      iterator begin() { return iterator(head_.next); }
      iterator end() { return iterator(0); }
      iterator before_end() { return iterator(tail_); }
      const_iterator before_begin() const { return const_iterator(&head_); }
      const_iterator begin() const { return const_iterator(head_.next); }
      const_iterator end() const { return const_iterator(0); }
      const_iterator before_end() const { return const_iterator(tail_); }
      const_iterator cbefore_begin() const { return const_iterator(&head_); }
      const_iterator cbegin() const { return const_iterator(head_.next); }
      const_iterator cend() const { return const_iterator(0); }
      const_iterator cbefore_end() const { return const_iterator(tail_); }

      bool empty() const { return !head_.next; }
      size_type max_size() const { return (size_type)-1/sizeof(node); }

      reference front() { return static_cast<node *>(head_.next)->value; }
      const_reference front() const { return static_cast<const node *>(head_.next)->value; }
      reference back() { return static_cast<node *>(tail_)->value; }
      const_reference back() const { return static_cast<const node *>(tail_)->value; }

      void push_front(const T &value)
      {
         slist_node *n = head_.insert_after(get_node(value));
         if (tail_ == &head_)
            tail_ = n;
      }
      void push_back(const T &value)
      {
         tail_ = tail_->insert_after(get_node(value));
      }

      void pop_front()
      {
         put_node(static_cast<node *>(head_.unlink_next()));
         if (empty())
            tail_ = &head_;
      }

      iterator insert_after(const_iterator pos, const T &value)
      {
         slist_node *pn = const_cast<slist_node *>(pos.head_);
         slist_node *n = pn->insert_after(get_node(value));
         if (pn == tail_)
            tail_ = n;
         return iterator(n);
      }
      void insert_after(const_iterator pos, size_type n, const T &value);
      template<typename InputIterator>
      void insert_after(const_iterator pos, InputIterator first, InputIterator last);

      iterator erase_after(const_iterator pos);
      iterator erase_after(const_iterator pos, const_iterator last);

      void swap(lazy_queue &other)
      {
         ttl::swap(head_.next, other.head_.next);
         ttl::swap(tail_, other.tail_);
      }

      void clear();

      void remove(const T &); // TODO
      template<typename Predicate>
      void remove_if(Predicate); // TODO

      void roll_head() // pop_front(); push_back();
      {
         if (head_.next && head_.next != tail_)
         {
            slist_node *n = head_.unlink_next();
            tail_ = tail_->insert_after(n);
         }
      }
   };
   template<typename T>
   void lazy_queue<T>::insert_after(const_iterator pos, size_type n, const T &value)
   {
      slist_node *pn = const_cast<slist_node *>(pos.head_);
      slist_node *p = pn;
      while (n--)
         p = p->insert_after(get_node(value));
      if (pn == tail_)
         tail_ = p;
   }
   template<typename T>
   template<typename InputIterator>
   void lazy_queue<T>::insert_after(const_iterator pos, InputIterator first, InputIterator last)
   {
      slist_node *pn = const_cast<slist_node *>(pos.head_);
      slist_node *p = pn;
      for ( ; first != last; ++first)
         p = p->insert_after(get_node(*first));
      if (pn == tail_)
         tail_ = p;
   }
   template<typename T>
   typename lazy_queue<T>::iterator lazy_queue<T>::erase_after(const_iterator pos)
   {
      slist_node *pn = const_cast<slist_node *>(pos.head_);
      slist_node *p = pn->unlink_next();
      if (p == tail_)
         tail_ = pn;
      put_node(static_cast<node *>(p));
      return iterator(pn->next);
   }
   template<typename T>
   typename lazy_queue<T>::iterator lazy_queue<T>::erase_after(const_iterator pos, const_iterator last)
   {
      slist_node *p = const_cast<slist_node *>(pos.head_);
      while (p->next != last.head_)
         put_node(static_cast<node *>(p->unlink_next()));
      if (last == cend())
         tail_ = p;
      return iterator(p->next);
   }
   template<typename T>
   void lazy_queue<T>::clear()
   {
      while (head_.next)
         put_node(static_cast<node *>(head_.unlink_next()));
      tail_ = &head_;
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_LAZY_QUEUE_HPP_
