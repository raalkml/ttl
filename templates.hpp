/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// A library of trivial templates designed to workaround some of the
// shortcomings of CoLibry.
//
// The templates are mostly ports of STL templates (as STL is made
// impossible to use by disabling standard include directories) and
// should be mostly compatible.
//
// There is no allocator support and the O(log N) guarantee might be
// not implemented. Also reverse iterators might be missing.
// Usually only equality operators are implemented, if any.
//
// Works without RTTI and exceptions.
//
#ifndef _TRIVIAL_TEMPLATE_LIBRARY_HPP_
#define _TRIVIAL_TEMPLATE_LIBRARY_HPP_ 1

#include <new>

namespace ttl
{
   typedef unsigned int size_t;
   typedef int ptrdiff_t;

   template<typename RandomAccessIterator>
   inline ttl::ptrdiff_t distance(RandomAccessIterator first, RandomAccessIterator last)
   {
      return last - first;
   }
   template<typename RandomAccessIterator, typename Distance>
   inline void advance(RandomAccessIterator &i, Distance d)
   {
      i += d;
   }

   template<class InputIt1, class InputIt2>
   inline bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
   {
      for (; first1 != last1; ++first1, ++first2)
         if (!(*first1 == *first2))
            return false;
      return true;
   }

   template<typename iterator, typename size_type, typename value_type>
   inline iterator fill_n(iterator first, size_type count, const value_type& value)
   {
      while (count--)
         *first++ = value;
      return first;
   }

   template<typename value_type>
   inline void swap(value_type &a, value_type &b)
   {
      value_type tmp = a;
      a = b;
      b = tmp;
   }

   template<class iterator1, class iterator2>
   inline void iter_swap(iterator1 a, iterator2 b)
   {
      swap(*a, *b);
   }

   template<class iterator1, class iterator2>
   inline iterator1 swap_ranges(iterator1 first1, iterator1 last1, iterator2 first2)
   {
      for (; first1 != last1; ++first1, ++first2)
         iter_swap(first1, first2);
      return first2;
   }

   template<typename T>
   struct less
   {
      typedef T first_argument_type;
      typedef T second_argument_type;
      typedef bool result_type;
      bool operator()(const T &a, const T &b) const { return a < b; }
   };

   template<typename T1, typename T2>
   struct pair
   {
      typedef T1 first_type;
      typedef T2 second_type;

      T1 first;
      T2 second;

      pair() {}
      pair(T1 _first, T2 _second): first(_first), second(_second) {}
      template<typename U1, typename U2>
      pair(const pair<U1,U2> &other): first(other.first), second(other.second) {}
      template<typename U1, typename U2>
      pair &operator=(const pair<U1,U2> &other)
      {
         first = other.first;
         second = other.second;
         return *this;
      }

      void swap(pair &other)
      {
         ttl::swap(first, other.first);
         ttl::swap(second, other.second);
      }
   };
   template<class T1, class T2>
   inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y)
   {
      return x.first == y.first && x.second == y.second;
   }
   template<class T1, class T2>
   inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y)
   {
      return !(x == y);
   }
   template<class T1, class T2>
   inline pair<T1, T2> make_pair(T1 first, T2 second)
   {
      return pair<T1, T2>(first, second);
   }

   template<typename T, const ttl::size_t N>
   struct array
   {
      typedef T                                     value_type;
      typedef value_type &                          reference;
      typedef const value_type &                    const_reference;
      typedef value_type *                          iterator;
      typedef const value_type *                    const_iterator;
      typedef ttl::size_t                           size_type;
      typedef ttl::ptrdiff_t                        difference_type;

      value_type elements_[N ? N : 1];

      void fill(const value_type &v)
      {
         ttl::fill_n(begin(), size(), v);
      }

      void swap(array &other)
      {
         ttl::swap_ranges(begin(), end(), other.begin());
      }

      iterator       begin() { return iterator(&elements_[0]); }
      const_iterator begin() const { return const_iterator(&elements_[0]); }
      iterator       end() { return iterator(&elements_[N]); }
      const_iterator end() const { return const_iterator(&elements_[N]); }
      const_iterator cbegin() const { return const_iterator(&elements_[0]); }
      const_iterator cend() const { return const_iterator(&elements_[N]); }

      size_type size() const { return N; }
      size_type max_size() const { return N; }
      bool empty() const { return size() == 0; }

      reference operator[](size_type i) { return elements_[i]; }
      const_reference operator[](size_type i) const { return elements_[i]; }
      reference at(size_type i) { return elements_[i]; }
      const_reference at(size_type i) const { return elements_[i]; }
      reference front() { return *begin(); }
      const_reference front() const { return *begin(); }
      reference back() { return N ? *(end() - 1) : *end(); }
      const_reference back() const { return N ? *(end() - 1) : *end(); }

      T* data() { return &elements_[0]; }
      const T* data() const { return &elements_[0]; }
   };

   template<typename T, const ttl::size_t N>
   inline bool operator==(const array<T, N> &a, const array<T, N> &b)
   {
      return a.size() == b.size() && ttl::equal(a.begin(), a.end(), b.begin());
   }
   template<typename T, const ttl::size_t N>
   inline bool operator!=(const array<T, N> &a, const array<T, N> &b)
   {
      return !(a == b);
   }

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
      vector(): elements_(NULL), last_(NULL), end_of_elements_(NULL) {}
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
         last_->~T();
         --last_;
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
      elements_(NULL), last_(NULL), end_of_elements_(NULL)
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

#if 0
   template<typename T> class forward_list
   {
   };

   template<typename T> class list
   {
   };

   template<typename T> class set // collection of unique keys
   {
   };
#endif

   template<typename KT, typename T, typename Compare = ttl::less<KT> >
   class map // unique keys to values
   {
   public:
      typedef KT key_type;
      typedef T mapped_type;
      typedef ttl::pair<KT, T> value_type;
      typedef ttl::size_t size_type;
      typedef ttl::ptrdiff_t difference_type;
      typedef Compare key_compare;

      typedef value_type &reference;
      typedef const value_type &const_reference;

      typedef value_type *pointer;
      typedef const value_type *const_pointer;

      typedef value_type *iterator;
      typedef const value_type *const_iterator;

   public:
      explicit map(): elements_(NULL), last_(NULL), end_of_elements_(NULL) {}
      explicit map(const Compare &c): elements_(NULL), last_(NULL), end_of_elements_(NULL), comp_(c) {}
      map(const map& other): comp_(other.comp_) {}
      template<class InputIt>
      map(InputIt first, InputIt last, const Compare & = Compare());
      map &operator=(const map &other);

      T &operator[](const KT &key);
      T &at(const KT &key);
      const T &at(const KT &key) const;

      iterator       begin() { return elements_; }
      const_iterator begin() const { return elements_; }
      iterator       end() { return last_; }
      const_iterator end() const { return last_; }
      const_iterator cbegin() const { return elements_; }
      const_iterator cend() const { return last_; }

      size_type size() const { return last_ - elements_; }
      bool empty() const { return last_ == elements_; }
      size_type max_size() const { return (size_type)-1 / sizeof(value_type); }

      void clear();

      ttl::pair<iterator,bool> insert( const value_type &value );
      iterator insert( const_iterator hint, const value_type& value );
      template< class InputIt >
      void insert( InputIt first, InputIt last );

      iterator erase(const_iterator pos);
      iterator erase(const_iterator first, const_iterator last);
      size_type erase(const key_type &key);

      void swap(map &other);

      size_type count(const KT &key) const;
      iterator find(const KT &key);
      const_iterator find(const KT &key) const;

      ttl::pair<iterator,iterator> equal_range(const KT &key);
      ttl::pair<const_iterator,const_iterator> equal_range(const KT &key) const;

      iterator lower_bound(const KT &key);
      const_iterator lower_bound(const KT &key) const;

      iterator upper_bound(const KT &key);
      const_iterator upper_bound(const KT &key) const;

      key_compare key_comp() const { return comp_; }

      struct value_compare
      {
      protected:
         friend class map<KT, T, Compare>;
         Compare comp;
         value_compare(Compare c): comp(c) {}
      public:
         typedef value_type first_argument_type;
         typedef value_type second_argument_type;
         typedef bool result_type;

         bool operator()(const value_type &a, const value_type &b) const
         {
            return comp(a.first, b.first);
         }
      };
      value_compare value_comp() const;

   private:
      value_type *elements_, *last_, *end_of_elements_;
      Compare comp_;
   };

#if 0
   template<typename Key, typename Compare> class multiset
   {
   public:
      typedef Key     key_type;
      typedef Key     value_type;
      typedef Compare key_compare;
      typedef Compare value_compare;
      typedef typename _Key_alloc_type::pointer             pointer;
      typedef typename _Key_alloc_type::const_pointer       const_pointer;
      typedef typename _Key_alloc_type::reference           reference;
      typedef typename _Key_alloc_type::const_reference     const_reference;
      typedef typename _Rep_type::const_iterator            iterator;
      typedef typename _Rep_type::const_iterator            const_iterator;
      typedef typename _Rep_type::size_type                 size_type;
      typedef typename _Rep_type::difference_type           difference_type;

      multiset();
      explicit multiset(const _Compare &__comp);
      template<typename _InputIterator>
      multiset(_InputIterator __first, _InputIterator __last);
      template<typename _InputIterator>
      multiset(_InputIterator __first, _InputIterator __last, const _Compare& __comp);
      multiset(const multiset &);

      multiset &operator=(const multiset &);

      key_compare key_comp() const;

      iterator begin() const;
      iterator end() const;
      const_iterator cbegin() const;
      const_iterator cend() const;

      bool empty() const;
      size_type size() const;
      size_type max_size() const;

      swap(multiset &);

      iterator insert(const value_type& __x);
      iterator insert(iterator __position, const value_type& __x);
      template<typename _InputIterator>
      void insert(_InputIterator __first, _InputIterator __last);

      void erase(iterator __position);
      size_type erase(const key_type& __x);
      void erase(iterator __first, iterator __last);
      void clear();

      size_type count(const key_type& __x) const;
      iterator find(const key_type& __x);
      const_iterator find(const key_type& __x) const;
      iterator lower_bound(const key_type& __x);
      const_iterator lower_bound(const key_type& __x) const;
      iterator upper_bound(const key_type& __x);
      const_iterator upper_bound(const key_type& __x) const;
      ttl::pair<iterator, iterator> equal_range(const key_type& __x);
      ttl::pair<const_iterator, const_iterator> equal_range(const key_type& __x) const;

   };

   template<typename T> class multimap
   {
   };
#endif // NO
}

#endif // _TRIVIAL_TEMPLATE_LIBRARY_HPP_
