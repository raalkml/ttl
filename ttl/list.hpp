/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of STL list
// (a double linked list)
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
#define _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_ 1

#include "types.hpp"

namespace ttl
{
   struct list_node
   {
      list_node *prev, *next;

      void init() { next = prev = this; }

      list_node *insert_before(list_node *n)
      {
         prev->next = n;
         n->next = this;
         n->prev = prev;
         prev = n;
         return n;
      }

      void unlink()
      {
         next->prev = prev;
         prev->next = next;
      }

      void splice(list_node *first, list_node *last)
      {
         last->prev->next = this;
         first->prev->next = last;
         this->prev->next = first;
         list_node *t = this->prev;
         this->prev = last->prev;
         last->prev = first->prev;
         first->prev = t;
      }

      void reverse()
      {
         list_node *p = this;
         do
         {
            list_node *t = p->next;
            p->next = p->prev;
            p->prev = t;
            p = t;
         }
         while (p != this);
      }

#if 1
      static void swap(list_node *a, list_node *b)
      {
         if (a != a->next)
         {
            list_node *t;
            if (b != b->next)
            {
               t = a->prev, a->prev = b->prev, b->prev = t; // swap
               t = a->next, a->next = b->next, b->next = t; // swap
               a->prev->next = a->next->prev = a;
               b->prev->next = b->next->prev = b;
               return;
            }
            else
            {
               t = a, a = b, b = t; // swap
            }
         }
         a->prev = b->prev;
         a->next = b->next;
         b->prev->next = b->next->prev = a;
         b->prev = b->next = b;
      }
#else // same as above, but more readable, and sometimes results in less code
      static void halfswap(list_node *a, list_node *b)
      {
         a->prev = b->prev;
         a->next = b->next;
         b->prev->next = b->next->prev = a;
         b->prev = b->next = b;
      }
      static void swap(list_node *a, list_node *b)
      {
         if (a != a->next)
         {
            if (b != b->next)
            {
               list_node t = { &t, &t };
               halfswap(&t, a);
               halfswap(a, b);
               halfswap(b, &t);
            }
            else
               halfswap(b, a);
         }
         else
            halfswap(a, b);
      }
#endif
   };
   template<typename T>
   class list
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
      struct node: list_node
      {
         T value;
         node(const T &v): value(v) {}
      };
      list_node head_;

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
         iterator &operator--() { head_ = head_->prev; return *this; }
         iterator operator--(int) { iterator tmp(head_); head_ = head_->prev; return tmp; }
         reference operator*() const { return static_cast<list<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<list<T>::node *>(head_)->value; }

         bool operator==(const iterator &other) const { return head_ == other.head_; }
         bool operator!=(const iterator &other) const { return head_ != other.head_; }
         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         list_node *head_;
         friend class list<T>;
         iterator(list_node *head): head_(head) {}
      };
      class const_iterator
      {
      public:
         typedef list<T>::iterator iterator;
         typedef T value_type;
         typedef const T *pointer;
         typedef const T &reference;
         typedef ttl::ptrdiff_t difference_type;

         const_iterator() /* head_ left uninitialized */ {}
         const_iterator(const iterator &o): head_(o.head_) {}
         ~const_iterator() {}
         const_iterator &operator++() { head_ = head_->next; return *this; }
         const_iterator operator++(int) { const_iterator tmp(head_); head_ = head_->next; return tmp; }
         const_iterator &operator--() { head_ = head_->prev; return *this; }
         const_iterator operator--(int) { const_iterator tmp(head_); head_ = head_->prev; return tmp; }
         reference operator*() const { return static_cast<const list<T>::node *>(head_)->value; }
         pointer operator->() const { return &static_cast<const list<T>::node *>(head_)->value; }

         bool operator==(const const_iterator &other) const { return head_ == other.head_; }
         bool operator!=(const const_iterator &other) const { return head_ != other.head_; }

      private:
         friend class list<T>;
         friend class list<T>::iterator;
         const list_node *head_;
         const_iterator(const list_node *head): head_(head) {}
      };

      list() { head_.init(); }
      list(const list &other)
      {
         head_.init();
         insert(cbegin(), other.cbegin(), other.cend());
      }
      list(size_type n, const T &value)
      {
         head_.init();
         insert(cend(), n, value);
      }
      template<typename InputIterator>
      list(InputIterator first, InputIterator last)
      {
         head_.init();
         insert(end(), first, last);
      }
      ~list() { clear(); }

      list &operator=(const list &other)
      {
         clear();
         insert(begin(), other.cbegin(), other.cend());
         return *this;
      }

      iterator begin() { return iterator(head_.next); }
      iterator end() { return iterator(&head_); }
      const_iterator begin() const { return const_iterator(head_.next); }
      const_iterator end() const { return const_iterator(&head_); }
      const_iterator cbegin() const { return const_iterator(head_.next); }
      const_iterator cend() const { return const_iterator(&head_); }

      bool empty() const { return head_.next == &head_; }
      size_type max_size() const { return (size_type)-1/sizeof(node); }

      reference front() { return static_cast<node *>(head_.next)->value; }
      const_reference front() const { return static_cast<const node *>(head_.next)->value; }
      reference back() { return static_cast<node *>(head_.prev)->value; }
      const_reference back() const { return static_cast<const node *>(head_.prev)->value; }

      template<typename InputIterator>
      void assign(InputIterator first, InputIterator last)
      {
         clear();
         insert(end(), first, last);
      }

      void assign(size_type n, const T &value)
      {
         clear();
         insert(end(), n, value);
      }

      void push_front(const T &value)
      {
         head_.next->insert_before(new node(value));
      }

      void push_back(const T &value)
      {
         head_.insert_before(new node(value));
      }

      void pop_front()
      {
         node *p = static_cast<node *>(head_.next);
         p->unlink();
         delete p;
      }

      void pop_back()
      {
         node *p = static_cast<node *>(head_.prev);
         p->unlink();
         delete p;
      }

      void splice(const_iterator pos, list &other)
      {
         list_node *p = const_cast<list_node *>(pos.head_);
         p->splice(other.head_.next, &other.head_);
      }
      void splice(const_iterator pos, list &, const_iterator it)
      {
         list_node *p = const_cast<list_node *>(pos.head_);
         list_node *o = const_cast<list_node *>(it.head_);
         if (o != p)
            p->splice(o, o->next);
      }
      void splice(const_iterator pos, list &, const_iterator first, const_iterator last)
      {
         list_node *p = const_cast<list_node *>(pos.head_);
         p->splice(const_cast<list_node *>(first.head_), const_cast<list_node *>(last.head_));
      }

      iterator insert(const_iterator pos, const T &value)
      {
         list_node *p = const_cast<list_node *>(pos.head_);
         return iterator(p->insert_before(new node(value)));
      }
      void insert(const_iterator pos, size_type n, const T &value);
      template<typename InputIterator>
      void insert(const_iterator pos, InputIterator first, InputIterator last);

      iterator erase(const_iterator pos)
      {
         list_node *p = const_cast<list_node *>(pos.head_);
         list_node *next = p->next;
         p->unlink();
         delete static_cast<node *>(p);
         return iterator(next);
      }
      iterator erase(const_iterator pos, const_iterator last);

      void swap(list &other)
      {
         list_node::swap(&head_, &other.head_);
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

      void merge(list &); // merge sorted lists
      template<typename Compare>
      void merge(list &, Compare);

      // O(N*log(N)) sort
      void sort();
      template<typename Compare>
      void sort(Compare);

      void reverse();
   };
   template<typename T>
   void list<T>::insert(const_iterator pos, size_type n, const T &value)
   {
      list_node *p = const_cast<list_node *>(pos.head_);
      while (n--)
         p->insert_before(new node(value));
   }
   template<typename T>
   template<typename InputIterator>
   void list<T>::insert(const_iterator pos, InputIterator first, InputIterator last)
   {
      list_node *p = const_cast<list_node *>(pos.head_);
      for ( ; first != last; ++first)
         p->insert_before(new node(*first));
   }
   template<typename T>
   void list<T>::clear()
   {
      while (head_.next != &head_)
      {
         node *p = static_cast<node *>(head_.next);
         head_.next = head_.next->next;
         delete p;
      }
   }
   template<typename T>
   typename list<T>::iterator list<T>::erase(const_iterator pos, const_iterator last)
   {
      list_node *p = const_cast<list_node *>(pos.head_);
      while (p->next != last.head_)
      {
         node *next = static_cast<node *>(p->next);
         p->unlink();
         delete static_cast<node *>(p);
         p = next;
      }
      return iterator(p->next);
   }
   template<typename T>
   void list<T>::reverse()
   {
      head_.reverse();
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
