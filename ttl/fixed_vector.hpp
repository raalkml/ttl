/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of STL vector of a fixed
// size of preallocated storage.
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_FIXED_VECTOR_HPP_
#define _TINY_TEMPLATE_LIBRARY_FIXED_VECTOR_HPP_ 1

#include <new>
#include "types.hpp"

namespace ttl
{
   template<class T1, class T2> struct pair;
   template<class ForwardIt, class T> pair<ForwardIt,ForwardIt> equal_range(ForwardIt, ForwardIt, const T &);
   template<class InputIt1, class InputIt2> bool equal(InputIt1, InputIt1, InputIt2, InputIt2);
   template<typename T> void swap(T &, T &);
   template<class iterator1, class iterator2> iterator1 swap_ranges(iterator1, iterator1, iterator2);

   template<typename T, const unsigned int N>
   class fixed_vector
   {
   public:
      typedef T              value_type;
      typedef T             *pointer;
      typedef const T       *const_pointer;
      typedef T             &reference;
      typedef const T       &const_reference;
      typedef T             *iterator;
      typedef const T       *const_iterator;
      typedef ttl::size_t    size_type;
      typedef ttl::ptrdiff_t difference_type;

   private:
      union { char b[sizeof(T)]; } bytes_[N];
      T *last_;
      T *elements() const { return (T *)bytes_; }
      T *end_of_elements() const { return (T *)(bytes_ + N); }

   public:
      fixed_vector(): last_(elements()) {}
      explicit fixed_vector(size_type n);
      explicit fixed_vector(size_type n, const value_type &value);
      fixed_vector(const fixed_vector &other);
      template<typename RandomAccessIterator>
      fixed_vector(RandomAccessIterator first, RandomAccessIterator last);

      ~fixed_vector()
      {
         clear();
      }

      fixed_vector& operator=(const fixed_vector &other);

      void assign(size_type n, const value_type &value);
      template<typename InputIterator>
      void assign(InputIterator first, InputIterator last)
      {
         clear();
         for (; first != last; ++first)
            push_back(*first);
      }

      iterator       begin() { return elements(); }
      const_iterator begin() const { return elements(); }
      iterator       end() { return last_; }
      const_iterator end() const { return last_; }
      const_iterator cbegin() const { return elements(); }
      const_iterator cend() const { return last_; }

      size_type size() const { return last_ - elements(); }
      bool empty() const { return last_ == elements(); }
      bool full() const { return last_ == end_of_elements(); }
      size_type max_size() const { return N; }
      size_type capacity() const { return N; }

      void resize(size_type new_size);

      void resize(size_type new_size, const value_type &x)
      {
         if (new_size < size())
            resize(new_size);
         else
            insert(end(), new_size - size(), x);
      }

      reference operator[](size_type n) { return *(elements() + n); }
      const_reference operator[](size_type n) const { return *(elements() + n); }
      reference at(size_type n) { return (*this)[n]; }
      const_reference at(size_type n) const { return (*this)[n]; }
      reference front() { return *begin(); }
      const_reference front() const { return *begin(); }
      reference back() { return *(end() - 1); }
      const_reference back() const { return *(end() - 1); }
      pointer data() { return pointer(elements()); }
      const_pointer data() const { return const_pointer(elements()); }

      iterator insert(const_iterator pos, size_type n, const value_type &x);

      iterator insert(const_iterator pos, const value_type &x)
      {
         return insert(pos, (size_type)1, x);
      }

      template<typename InputIterator>
      iterator insert(const_iterator pos, InputIterator first, InputIterator last)
      {
         difference_type dist = pos - cbegin();
         for ( ; first != last; ++first)
            insert(++pos, (size_type)1, *first);
         return begin() + dist;
      }

      void push_back(const value_type &x)
      {
         new(last_++) T(x);
      }

      void pop_back()
      {
         (--last_)->~T();
      }

      iterator erase(const_iterator first, const_iterator last);

      iterator erase(const_iterator pos)
      {
         return erase(pos, pos + 1);
      }

      void swap(fixed_vector& x)
      {
         const bool this_a = this->size() < x.size();
         fixed_vector &a = this_a ? *this: x;
         fixed_vector &b = this_a ? x: *this;
         size_type minsiz = this_a ? a.size(): b.size();
         ttl::swap_ranges(a.elements(), a.elements() + minsiz, b.elements());
         for (size_type i = b.size() - a.size(); i--;)
         {
            new (a.elements() + i) T(b.elements()[i]);
            b.elements()[i].~T();
         }
      }

      void clear()
      {
         while (last_ > elements())
            (--last_)->~T();
      }
   };

   template<typename T, const unsigned int N>
   fixed_vector<T,N>::fixed_vector(size_type n):
      last_(elements())
   {
      while (n-- && !full())
         ::new(last_++) T();
   }
   template<typename T, const unsigned int N>
   fixed_vector<T,N>::fixed_vector(size_type n, const value_type &value):
      last_(elements())
   {
      while (n-- && !full())
         ::new(last_++) T(value);
   }
   template<typename T, const unsigned int N>
   fixed_vector<T,N>::fixed_vector(const fixed_vector &other):
      last_(elements())
   {
      for (const_iterator i = other.cbegin(); i != other.cend() && !full(); ++i)
         ::new(last_++) T(*i);
   }
   template<typename T, const unsigned int N>
   template<typename RandomAccessIterator>
   fixed_vector<T,N>::fixed_vector(RandomAccessIterator first, RandomAccessIterator last):
      last_(elements())
   {
      for (; first != last && !full(); ++first)
         push_back(*first);
   }
   template<typename T, const unsigned int N>
   fixed_vector<T,N> &fixed_vector<T,N>::operator=(const fixed_vector &other)
   {
      clear();
      for (const_iterator i = other.cbegin(); i != other.cend(); ++i)
         ::new(last_++) T(*i);
      return *this;
   }
   template<typename T, const unsigned int N>
   void fixed_vector<T,N>::assign(size_type n, const value_type &value)
   {
      clear();
      while (n--)
         ::new(last_++) T(value);
   }
   template<typename T, const unsigned int N>
   void fixed_vector<T,N>::resize(size_type new_size)
   {
      if (new_size < size())
         for (T *pos = elements() + new_size; last_ > pos;)
            (--last_)->~T();
      else
         insert(end(), new_size - size(), value_type());
   }
   template<typename T, const unsigned int N>
   typename fixed_vector<T,N>::iterator fixed_vector<T,N>::insert(const_iterator pos, size_type n, const value_type &x)
   {
      difference_type dist = pos - cbegin();
      if (n)
      {
         T *o;
         const T *i;
         if (pos < end())
         {
            last_ += n;
            for (o = last_, i = pos + n; i > pos; )
            {
               ::new(--o) T(*--i);
               i->~T();
            }
            for (o = elements() + dist; n--; ++o)
               ::new(o) T(x);
         }
         else
         {
            for (o = last_; n--; )
               ::new(last_++) T(x);
         }
      }
      return begin() + dist;
   }
   template<typename T, const unsigned int N>
   typename fixed_vector<T,N>::iterator fixed_vector<T,N>::erase(const_iterator first, const_iterator last)
   {
      difference_type off = first - begin();
      difference_type lastoff = last - begin();
      for (T *f = elements() + off, *l = elements() + lastoff; f != l;)
         (--l)->~T();
      T *o = elements() + off;
      for (T *i = elements() + lastoff; i != last_; ++i)
      {
         ::new(o++) T(*i);
         i->~T();
      }
      last_ = o;
      return begin() + off;
   }
   template<typename T, const unsigned int N>
   inline bool operator==(const fixed_vector<T,N> &a, const fixed_vector<T,N> &b)
   {
      return ttl::equal(a.begin(), a.end(), b.begin(), b.end());
   }
   template<typename T, const unsigned int N>
   inline bool operator!=(const fixed_vector<T,N> &a, const fixed_vector<T,N> &b)
   {
      return !(a == b);
   }
}

#endif // _TINY_TEMPLATE_LIBRARY_FIXED_VECTOR_HPP_
