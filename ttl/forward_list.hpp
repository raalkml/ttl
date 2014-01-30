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
      class iterator
      {
         typedef T value_type;
         typedef T *pointer;
         typedef T &reference;
         typedef ttl::ptrdiff_t difference_type;
      };
      class const_iterator
      {
         typedef forward_list<T>::iterator iterator;
         typedef T value_type;
         typedef const T *pointer;
         typedef const T &reference;
         typedef ttl::ptrdiff_t difference_type;
      };
      typedef T value_type;
      typedef T *pointer;
      typedef const T *const_pointer;
      typedef T &reference;
      typedef const T &const_reference;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
   private:
      struct node
      {
         struct node *next;
         T value;
         node(const T &v): value(v) {}
      };
      node *head_;
   public:
      forward_list(): head_(NULL) {}
      forward_list(const forward_list &other): head_(NULL)
      {
         copy_nodes_inorder(&head_, other.head_);
      }
      forward_list(size_type n, const T &value): head_(NULL)
      {
         push_nodes_front(n, value);
      }
      ~forward_list() { clear(); }

      forward_list &operator=(const forward_list &other)
      {
         clear();
         copy_nodes_inorder(&head_, other.head_);
         return *this;
      }

      iterator before_begin();
      iterator begin();
      iterator end();
      const_iterator before_begin() const;
      const_iterator begin() const;
      const_iterator end() const;
      const_iterator cbefore_begin() const;
      const_iterator cbegin() const;
      const_iterator cend() const;

      bool empty() const { return !head_; }
      size_type max_size() const { return (size_type)-1/sizeof(node); }

      reference front();
      const_reference front() const;

      template<typename InputIterator>
      void assign(InputIterator first, InputIterator last);
      void assign(size_type n, const T &value)
      {
         clear();
         push_nodes_front(n, value);
      }

      void push_front(const T &value)
      {
         push_nodes_front(1, value);
      }

      void pop_front()
      {
         node *n = head_;
         head_ = head_->next;
         delete n;
      }

      iterator insert_after(const_iterator pos, const T &value);
      void insert_after(const_iterator pos, size_type n, const T &value);
      template<typename InputIterator>
      void insert_after(const_iterator pos, InputIterator first, InputIterator last);

      iterator erase_after(const_iterator pos);
      iterator erase_after(const_iterator pos, const_iterator last);

      void swap(forward_list &other);

      void resize(size_type);
      void resize(size_type, const T &value);

      void clear()
      {
         while (head_)
         {
            node *n = head_->next;
            delete head_;
            head_ = n;
         }
      }

      void splice_after(const_iterator pos, forward_list &other);
      void splice_after(const_iterator pos, forward_list &other, const_iterator it);
      void splice_after(const_iterator pos, forward_list &other, const_iterator first, const_iterator last);

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
         node *reversed = NULL;
         while (head_)
         {
            node *n = head_;
            head_ = head_->next;
            n->next = reversed;
            reversed = n;
         }
         head_ = reversed;
      }
   private:
      void copy_nodes_inorder(node **tail, const node *other);
      void push_nodes_front(size_type n, const T &value);
   };
   template<typename T>
   void forward_list<T>::copy_nodes_inorder(node **tail, const node *other)
   {
      for (const node *h = other; h; h = h->next)
      {
         node *t = new node(h->value);
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
         t->next = head_;
         head_ = t;
      }
   }
}
#endif // _TINY_TEMPLATE_LIBRARY_FORWARD_LIST_HPP_
