/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of STL forward_list
//
#ifndef _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
#define _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_ 1

#include "types.hpp"

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
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
   private:
      struct node_base
      {
         node_base *next;
      };
      struct node: node_base
      {
         T value;
         node(const T &v): value(v) {}
      };
      node_base head_;
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
         forward_list<T>::node_base *head_;
         friend class forward_list<T>;
         iterator(forward_list<T>::node_base *head): head_(head) {}
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
         const_iterator(const const_iterator &o): head_(o.head_) {}
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
         const forward_list<T>::node_base *head_;
         const_iterator(const forward_list<T>::node_base *head): head_(head) {}
      };

      forward_list() { head_.next = NULL; }
      forward_list(const forward_list &other)
      {
         head_.next = NULL;
         copy_nodes_inorder(&head_.next, other.head_.next);
      }
      forward_list(size_type n, const T &value)
      {
         head_.next = NULL;
         push_nodes_front(n, value);
      }
      ~forward_list() { clear(); }

      forward_list &operator=(const forward_list &other)
      {
         clear();
         copy_nodes_inorder(&head_.next, other.head_.next);
         return *this;
      }

      iterator before_begin() { return iterator(&head_); }
      iterator begin() { return iterator(head_.next); }
      iterator end() { return iterator(NULL); }
      const_iterator before_begin() const { return const_iterator(&head_); }
      const_iterator begin() const { return const_iterator(head_.next); }
      const_iterator end() const { return const_iterator(NULL); }
      const_iterator cbefore_begin() const { return const_iterator(&head_); }
      const_iterator cbegin() const { return const_iterator(head_.next); }
      const_iterator cend() const { return const_iterator(NULL); }

      bool empty() const { return !head_.next; }
      size_type max_size() const { return (size_type)-1/sizeof(node); }

      reference front() { return static_cast<node *>(head_.next)->value; }
      const_reference front() const { return static_cast<const node *>(head_.next)->value; }

      template<typename InputIterator>
      void assign(InputIterator first, InputIterator last);
      void assign(size_type n, const T &value)
      {
         clear();
         push_nodes_front(n, value);
      }

   private:
      static node_base *insert_after(node_base *pos, const T &value)
      {
         node *n = new node(value);
         n->next = pos->next;
         pos->next = n;
         return n;
      }
      static void erase_after_internal(node_base *pos)
      {
         node *n = static_cast<node *>(pos->next);
         pos->next = n->next;
         delete n;
      }

   public:
      void push_front(const T &value)
      {
         insert_after(&head_, value);
      }

      void pop_front()
      {
         erase_after_internal(&head_);
      }

      void splice_after(const_iterator pos, forward_list &other) {}
      void splice_after(const_iterator pos, forward_list &other, const_iterator it);
      void splice_after(const_iterator pos, forward_list &other, const_iterator first, const_iterator last);

      iterator insert_after(const_iterator pos, const T &value)
      {
         return iterator(insert_after(const_cast<node_base *>(pos.head_), value));
      }
      void insert_after(const_iterator pos, size_type n, const T &value)
      {
         forward_list tmp(n, value);
         splice_after(pos, tmp);
      }
      template<typename InputIterator>
      void insert_after(const_iterator pos, InputIterator first, InputIterator last);

      iterator erase_after(const_iterator pos)
      {
         node_base *p = const_cast<node_base *>(pos.head_);
         erase_after_internal(p);
         return iterator(p->next);
      }
      iterator erase_after(const_iterator pos, const_iterator last)
      {
         node_base *p = const_cast<node_base *>(pos.head_);
         while (p->next != last.head_)
            erase_after_internal(p);
         return iterator(p->next);
      }

      void swap(forward_list &other);

      void resize(size_type);
      void resize(size_type, const T &value);

      void clear()
      {
         for (node_base *n = head_.next; head_.next;)
         {
            n = n->next;
            delete static_cast<node *>(head_.next);
            head_.next = n;
         }
      }

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

      void reverse()
      {
         node_base *reversed = NULL;
         node_base *orig = head_.next;
         while (orig)
         {
            node_base *n = orig;
            orig = n->next;
            n->next = reversed;
            reversed = n;
         }
         head_.next = reversed;
      }
   private:
      void copy_nodes_inorder(node_base **tail, const node_base *other);
      void push_nodes_front(size_type n, const T &value);
   };
   template<typename T>
   void forward_list<T>::copy_nodes_inorder(node_base **tail, const node_base *other)
   {
      for (const node_base *h = other; h; h = h->next)
      {
         node *t = new node(static_cast<const node *>(h)->value);
         t->next = NULL;
         *tail = t;
         tail = &t->next;
      }
   }
   template<typename T>
   void forward_list<T>::push_nodes_front(size_type n, const T &value)
   {
      while (n--)
      {
         node *t = new node(value);
         t->next = head_.next;
         head_.next = t;
      }
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
