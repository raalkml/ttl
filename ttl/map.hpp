/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: the map template implementation
//
#ifndef _TINY_TEMPLATE_LIBRARY_MAP_HPP_
#define _TINY_TEMPLATE_LIBRARY_MAP_HPP_ 1

namespace ttl
{
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
      value_compare value_comp() const { return value_compare(comp_); }

   private:
      value_type *elements_, *last_, *end_of_elements_;
      Compare comp_;
   };
}
#endif // _TINY_TEMPLATE_LIBRARY_MAP_HPP_
