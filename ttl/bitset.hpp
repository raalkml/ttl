/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: the implementation of STL bitset
//
#ifndef _TINY_TEMPLATE_LIBRARY_BITSET_HPP_
#define _TINY_TEMPLATE_LIBRARY_BITSET_HPP_ 1

#include "types.hpp"

namespace ttl
{
   template<ttl::size_t N> class bitset
   {
   private:
      typedef unsigned long slot_type;
      slot_type *bits_slot(ttl::size_t pos) { return bits_ + pos / (sizeof(slot_type) * CHAR_BIT); }
      const slot_type *bits_slot(ttl::size_t pos) const { return bits_ + pos / (sizeof(slot_type) * CHAR_BIT); }
      static ttl::size_t bits_bit(ttl::size_t pos) { return pos % (sizeof(slot_type) * CHAR_BIT); }
      slot_type bits_[(N + sizeof(slot_type) * CHAR_BIT - 1) / (sizeof(slot_type) * CHAR_BIT)];
   public:
      bitset() { reset(); }
      bitset(const bitset &other);
      template<ttl::size_t M> bitset(const bitset<M> &other);
      bitset(unsigned long bits);
      bitset(unsigned long long bits);
      bitset(const char *bits, ttl::size_t n = (ttl::size_t)-1, char zero = '0', char one = '1');

      ~bitset() {}

      bitset &operator=(const bitset &other);
      template<ttl::size_t M> bitset &operator=(const bitset<M> &other);

      bool operator==(const bitset &other);
      bool operator!=(const bitset &other) { return !operator==(other); }

      struct reference
      {
      private:
         friend class bitset<N>;
         reference(slot_type *bits, slot_type mask): bits_(bits), mask_(mask) {}
         slot_type *bits_;
         slot_type mask_;
      public:
         reference& operator=(bool v)
         {
            *bits_ = (*bits_ & ~mask_) | (-v & mask_);
            return *this;
         }
         ~reference() {}
         reference& operator=(const reference &other)
         {
            *bits_ = (*bits_ & mask_) | (-other.operator bool() & mask_);
            return *this;
         }
         operator bool() const { return (*bits_ & mask_) != 0; }
         bool operator~() const { return (*bits_ & mask_) == 0; }
         reference &flip() { *bits_ ^= mask_; return *this; }
      };

      bool operator[](ttl::size_t pos) const
      {
         return *bits_slot(pos) & (1 << bits_bit(pos));
      }
      reference operator[](ttl::size_t pos)
      {
         return reference(bits_slot(pos), 1 << bits_bit(pos));
      }

      bool test(ttl::size_t pos) const
      {
         return (*bits_slot(pos) & (1 << bits_bit(pos))) != 0ul;
      }

      bool all() const;
      bool any() const;
      bool none() const;
      ttl::size_t count() const;
      ttl::size_t size() const { return sizeof(bits_) * CHAR_BIT; }

      bitset<N>& operator&=(const bitset<N>& other);
      bitset<N>& operator|=(const bitset<N>& other);
      bitset<N>& operator^=(const bitset<N>& other);
      bitset<N> operator~() const;

      bitset<N> operator<<(ttl::size_t pos) const;
      bitset<N>& operator<<=(ttl::size_t pos);
      bitset<N> operator>>(ttl::size_t pos) const;
      bitset<N>& operator>>=(ttl::size_t pos);

      bitset<N>& set()
      {
         for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
            bits_[i] = (unsigned long)-1;
         return *this;
      }
      bitset<N>& set(ttl::size_t pos)
      {
         *bits_slot(pos) |= 1 << bits_bit(pos);
         return *this;
      }
      bitset<N>& set(ttl::size_t pos, bool value)
      {
         const unsigned long mask = (unsigned long)value << bits_bit(pos);
         *bits_slot(pos) = (*bits_slot(pos) & ~mask) | mask;
         return *this;
      }

      bitset<N>& reset()
      {
         for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
            bits_[i] = 0;
         return *this;
      }
      bitset<N>& reset(ttl::size_t pos)
      {
         return *this;
      }

      bitset<N>& flip();
      bitset<N>& flip(ttl::size_t pos);

      unsigned long to_ulong() const;
      unsigned long to_ullong() const;
   };

   template<ttl::size_t N>
   inline bitset<N>::bitset(unsigned long bits)
   {
      bits_[0] = bits;
      for (unsigned i = 1; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] = 0;
   }
   template<ttl::size_t N>
   bitset<N>::bitset(unsigned long long bits)
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
      {
         bits_[i] = bits & (unsigned long)-1;
         bits >>= sizeof(slot_type) * CHAR_BIT;
      }
   }
   template<ttl::size_t N>
   bitset<N>::bitset(const char *bits, ttl::size_t n, char zero, char one)
   {
      reset();
      for (ttl::size_t i = 0; i < n; ++i, ++bits)
         if (*bits == zero)
            reset(i);
         else if (*bits == one)
            set(i);
         else
            break;
   }
   template<ttl::size_t N>
   inline bool bitset<N>::operator==(const bitset &other)
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         if (bits_[i] != other.bits_[i])
            return false;
      return true;
   }

   template<ttl::size_t N>
   inline bool bitset<N>::all() const
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         if (bits_[i] != (slot_type)-1)
            return false;
      return true;
   }
   template<ttl::size_t N>
   inline bool bitset<N>::any() const
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         if (bits_[i])
            return true;
      return false;
   }
   template<ttl::size_t N>
   inline bool bitset<N>::none() const
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         if (bits_[i])
            return false;
      return true;
   }
   template<ttl::size_t N>
   inline bitset<N> &bitset<N>::operator=(const bitset &other)
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] = other.bits_[i];
      return *this;
   }
   template<ttl::size_t N>
   template<ttl::size_t M>
   bitset<N> &bitset<N>::operator=(const bitset<M> &other)
   {
      unsigned i;
      for (i = 0; i < (N < M ? sizeof(bits_)/sizeof(*bits_): sizeof(other.bits_)/sizeof(*other.bits_)); ++i)
         bits_[i] = other.bits_[i];
      for ( ; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] = 0;
      return *this;
   }
template<const ttl::size_t N> bitset<N> operator&( const bitset<N>& lhs, const bitset<N>& rhs );
template<const ttl::size_t N> bitset<N> operator|( const bitset<N>& lhs, const bitset<N>& rhs );
template<const ttl::size_t N> bitset<N> operator^( const bitset<N>& lhs, const bitset<N>& rhs );

}

#endif // _TINY_TEMPLATE_LIBRARY_HPP_
