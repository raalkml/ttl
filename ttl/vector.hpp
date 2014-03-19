/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: an implementation of STL vector.
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_VECTOR_HPP_
#define _TINY_TEMPLATE_LIBRARY_VECTOR_HPP_ 1

#include <new>
#include "types.hpp"
#include "utility.hpp"
#include "algorithm.hpp"

namespace ttl
{
   template<typename T>
   class vector
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
      T *elements_, *last_, *end_of_elements_;

   public:
      vector(): elements_(0), last_(0), end_of_elements_(0) {}
      explicit vector(size_type n, const value_type &value = value_type());
      vector(const vector &other);
      template<typename RandomAccessIterator>
      vector(RandomAccessIterator first, RandomAccessIterator last);

      ~vector()
      {
         clear();
         ::operator delete(elements_);
      }

      vector& operator=(const vector &other);

      void assign(size_type n, const value_type &value);
      template<typename InputIterator>
      void assign(InputIterator first, InputIterator last)
      {
         clear();
         for (; first != last; ++first)
            push_back(*first);
      }

      iterator       begin() { return elements_; }
      const_iterator begin() const { return elements_; }
      iterator       end() { return last_; }
      const_iterator end() const { return last_; }
      const_iterator cbegin() const { return elements_; }
      const_iterator cend() const { return last_; }

      size_type size() const { return last_ - elements_; }
      bool empty() const { return size() == 0; }
      size_type max_size() const { return (ttl::size_t)-1 / sizeof(T); }
      size_type capacity() const { return end_of_elements_ - elements_; }

      void resize(size_type new_size);

      void resize(size_type new_size, const value_type &x)
      {
         if (new_size < size())
            resize(new_size);
         else
            insert(end(), new_size - size(), x);
      }

      void reserve(size_type n);

      reference operator[](size_type n) { return *(elements_ + n); }
      const_reference operator[](size_type n) const { return *(elements_ + n); }
      reference at(size_type n) { return (*this)[n]; }
      const_reference at(size_type n) const { return (*this)[n]; }
      reference front() { return *begin(); }
      const_reference front() const { return *begin(); }
      reference back() { return *(end() - 1); }
      const_reference back() const { return *(end() - 1); }
      pointer data() { return pointer(elements_); }
      const_pointer data() const { return const_pointer(elements_); }

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
         if (last_ < end_of_elements_)
            new(last_++) T(x);
         else
            insert(end(), (size_type)1, x);
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

      void swap(vector& x)
      {
         ttl::swap(this->elements_, x.elements_);
         ttl::swap(this->last_, x.last_);
         ttl::swap(this->end_of_elements_, x.end_of_elements_);
      }

      void clear()
      {
         while (last_ > elements_)
            (--last_)->~T();
      }
   };

   template<typename T>
   vector<T>::vector(size_type n, const value_type &value)
   {
      last_ = elements_ = static_cast<T *>(::operator new(n * sizeof(T)));
      end_of_elements_ = elements_ + n;
      while (n--)
         ::new(last_++) T(value);
   }
   template<typename T>
   vector<T>::vector(const vector &other)
   {
      last_ = elements_ = static_cast<T *>(::operator new(other.size() * sizeof(T)));
      end_of_elements_ = elements_ + other.size();
      for (const_iterator i = other.cbegin(); i != other.cend(); ++i)
         ::new(last_++) T(*i);
   }
   template<typename T>
   template<typename RandomAccessIterator>
   vector<T>::vector(RandomAccessIterator first, RandomAccessIterator last):
      elements_(0), last_(0), end_of_elements_(0)
   {
      reserve(ttl::distance(first, last));
      for (; first != last; ++first)
         push_back(*first);
   }
   template<typename T>
   vector<T> &vector<T>::operator=(const vector &other)
   {
      clear();
      reserve(other.capacity());
      for (const_iterator i = other.cbegin(); i != other.cend(); ++i)
         ::new(last_++) T(*i);
      return *this;
   }
   template<typename T>
   void vector<T>::assign(size_type n, const value_type &value)
   {
      clear();
      reserve(n);
      while (n--)
         ::new(last_++) T(value);
   }
   template<typename T>
   void vector<T>::resize(size_type new_size)
   {
      if (new_size < size())
         for (T *pos = elements_ + new_size; last_ > pos;)
            (--last_)->~T();
      else
         insert(end(), new_size - size(), value_type());
   }
   template<typename T>
   void vector<T>::reserve(size_type n)
   {
      if (elements_ + n < end_of_elements_)
         return;
      T *newelements = static_cast<T *>(::operator new(n * sizeof(T)));
      T *o = newelements, *i = elements_;
      for (; i != last_; ++i)
         ::new(o++) T(*i);
      for (; i > elements_; )
         (--i)->~T();
      ::operator delete(elements_);
      elements_ = newelements;
      end_of_elements_ = elements_ + n;
      last_ = o;
   }
   template<typename T>
   typename vector<T>::iterator vector<T>::insert(const_iterator pos, size_type n, const value_type &x)
   {
      difference_type dist = pos - cbegin();
      if (n)
      {
         size_type free = end_of_elements_ - last_;
         T *o;
         const T *i;
         if (free < n)
         {
            size_type newcapacity = size() + n;
            T *newelements = o = static_cast<T *>(::operator new(newcapacity * sizeof(T)));
            for (i = elements_; i != pos; ++i)
               ::new(o++) T(*i);
            while (n--)
               ::new(o++) T(x);
            for (; i != last_; ++i)
               ::new(o++) T(*i);
            for (; i > elements_; )
               (--i)->~T();
            ::operator delete(elements_);
            elements_ = newelements;
            end_of_elements_ = elements_ + newcapacity;
            last_ = o;
         }
         else
         {
            if (pos < end())
            {
               last_ += n;
               for (o = last_, i = pos + n; i > pos; )
               {
                  ::new(--o) T(*--i);
                  i->~T();
               }
               for (o = elements_ + dist; n--; ++o)
                  ::new(o) T(x);
            }
            else
            {
               for (o = last_; n--; )
                  ::new(last_++) T(x);
            }
         }
      }
      return begin() + dist;
   }
   template<typename T>
   typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
   {
      difference_type off = first - begin();
      difference_type lastoff = last - begin();
      for (T *f = elements_ + off, *l = elements_ + lastoff; f != l;)
         (--l)->~T();
      T *o = elements_ + off;
      for (T *i = elements_ + lastoff; i != last_; ++i)
      {
         ::new(o++) T(*i);
         i->~T();
      }
      last_ = o;
      return begin() + off;
   }
   template<typename T>
   inline bool operator==(const vector<T> &a, const vector<T> &b)
   {
      return a.size() == b.size() && ttl::equal(a.begin(), a.end(), b.begin());
   }
   template<typename T>
   inline bool operator!=(const vector<T> &a, const vector<T> &b)
   {
      return !(a == b);
   }
}

#endif // _TINY_TEMPLATE_LIBRARY_VECTOR_HPP_
