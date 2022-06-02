/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Tiny Template Library: the implementation of STL bitset
//
// This code is Public Domain
//
#ifndef _TINY_TEMPLATE_LIBRARY_BITSET_HPP_
#define _TINY_TEMPLATE_LIBRARY_BITSET_HPP_ 1

#include <limits.h>
#include "types.hpp"

namespace ttl
{
   template<ttl::size_t N> class bitset
   {
   private:
      typedef unsigned long slot_type;
      slot_type *bits_slot(ttl::size_t pos) { return bits_ + pos / (sizeof(slot_type) * CHAR_BIT); }
      const slot_type *bits_slot(ttl::size_t pos) const { return bits_ + pos / (sizeof(slot_type) * CHAR_BIT); }
      static slot_type bits_bit(ttl::size_t pos) { return pos % (sizeof(slot_type) * CHAR_BIT); }
      slot_type bits_[(N + sizeof(slot_type) * CHAR_BIT - 1) / (sizeof(slot_type) * CHAR_BIT)];

   public:
      static slot_type last_bits()
      {
         return sizeof(bits_)*CHAR_BIT == N ? (slot_type)-1: (1lu << (N % (sizeof(slot_type) * CHAR_BIT))) - 1lu;
      }
      ttl::size_t last_slot_index() const {return N ? sizeof(bits_)/sizeof(*bits_) - 1: 0; }

      // uninitializing ctor to avoid unneeded initialization
      bitset(const bitset &, bool) {}

   public:
      bitset() { reset(); }
      bitset(const bitset &other) { operator=(other); }
      bitset(unsigned long bits);
      explicit bitset(unsigned long long bits);
      explicit bitset(const char *bits, ttl::size_t n = (ttl::size_t)-1, char zero = '0', char one = '1');

      ~bitset() {}

      template<ttl::size_t M> explicit bitset(const bitset<M> &other);
      template<ttl::size_t M> operator bitset<M>() const
      {
         return bitset<M>(*this);
      }
      template<ttl::size_t M> bitset &operator=(const bitset<M> &other);

      slot_type slot(ttl::size_t s) const { return bits_[s]; }

      bitset &operator=(const bitset &other);

      bool operator==(const bitset &other) const;
      bool operator!=(const bitset &other) const { return !operator==(other); }

      struct reference
      {
      private:
         reference(const reference &other); /* none */
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
         return pos < N && (*bits_slot(pos) & (1ul << bits_bit(pos)));
      }
      reference operator[](ttl::size_t pos)
      {
         return reference(bits_slot(pos), (pos < N) * 1ul << bits_bit(pos));
      }

      bool test(ttl::size_t pos) const
      {
         return pos < N && (*bits_slot(pos) & (1ul << bits_bit(pos)));
      }

      bool all() const;
      bool any() const;
      bool none() const;
      ttl::size_t count() const; // count set bits
      ttl::size_t size() const { return N; }
      ttl::size_t capacity() const { return sizeof(bits_) * CHAR_BIT; }

      bitset<N> &operator&=(const bitset<N> &other);
      bitset<N> &operator|=(const bitset<N> &other);
      bitset<N> &operator^=(const bitset<N> &other);
      bitset<N> operator~() const;

      bitset<N> operator<<(ttl::size_t pos) const;
      bitset<N> &operator<<=(ttl::size_t pos);
      bitset<N> operator>>(ttl::size_t pos) const;
      bitset<N> &operator>>=(ttl::size_t pos);

      bitset<N> &set()
      {
         for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
            bits_[i] = (slot_type)-1;
         return *this;
      }
      bitset<N> &set(ttl::size_t pos)
      {
         *bits_slot(pos) |= 1lu << bits_bit(pos);
         return *this;
      }
      bitset<N> &set(ttl::size_t pos, bool value)
      {
         const slot_type mask = (slot_type)value << bits_bit(pos);
         *bits_slot(pos) = (*bits_slot(pos) & ~mask) | mask;
         return *this;
      }

      bitset<N> &reset()
      {
         for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
            bits_[i] = 0;
         return *this;
      }
      bitset<N> &reset(ttl::size_t pos)
      {
         *bits_slot(pos) &= ~(1lu << bits_bit(pos));
         return *this;
      }

      bitset<N> &flip()
      {
         for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
            bits_[i] = ~bits_[i];
         return *this;
      }
      bitset<N> &flip(ttl::size_t pos)
      {
         *bits_slot(pos) ^= 1lu << bits_bit(pos);
         return *this;
      }

      unsigned long to_ulong() const { return (N < sizeof(*bits_) * CHAR_BIT ? last_bits(): (slot_type)-1) & *bits_; }
      unsigned long long to_ullong() const
      {
#if __SIZEOF_LONG__ < __SIZEOF_LONG_LONG__
         const ttl::size_t BPW = sizeof(*bits_) * CHAR_BIT;
         return static_cast<unsigned long long>(to_ulong()) |
            (N < BPW + 1 ? 0ull:
             (static_cast<unsigned long long>(bits_[1] & (N < BPW * 2 ? last_bits(): (slot_type)-1)) << BPW));
#else
         return static_cast<unsigned long long>(to_ulong());
#endif
      }
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
#if __SIZEOF_LONG__ < __SIZEOF_LONG_LONG__
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
      {
         bits_[i] = bits & (unsigned long)-1;
         bits >>= sizeof(slot_type) * CHAR_BIT;
      }
#else
      bits_[0] = bits;
      for (unsigned i = 1; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] = 0;
#endif
   }
   template<ttl::size_t N>
   template<ttl::size_t M>
   bitset<N>::bitset(const bitset<M> &other)
   {
      operator=(other);
   }
   template<ttl::size_t N>
   bitset<N>::bitset(const char *bits, ttl::size_t n, char zero, char one)
   {
      ttl::size_t i;
      for (i = 0; i < n; ++i)
         if (!(bits[i] == zero || bits[i] == one))
            break;
      for (ttl::size_t s = i / (sizeof(slot_type) * CHAR_BIT); s < sizeof(bits_)/sizeof(*bits_); ++s)
         bits_[s] = 0;
      for (; i--; ++bits)
         if (*bits == zero)
            reset(i);
         else if (*bits == one)
            set(i);
         else
            break;
   }
   template<ttl::size_t N>
   inline bool bitset<N>::operator==(const bitset &other) const
   {
      unsigned i;
      for (i = 0; i < last_slot_index(); ++i)
         if (bits_[i] != other.bits_[i])
            return false;
      return !((bits_[i] ^ other.bits_[i]) & last_bits());
   }

   template<ttl::size_t N>
   inline bool bitset<N>::all() const
   {
      unsigned i;
      for (i = 0; i < last_slot_index(); ++i)
         if (bits_[i] != (slot_type)-1)
            return false;
      return (bits_[i] & last_bits()) == last_bits();
   }
   template<ttl::size_t N>
   inline bool bitset<N>::any() const
   {
      unsigned i;
      for (i = 0; i < last_slot_index(); ++i)
         if (bits_[i])
            return true;
      return bits_[i] & last_bits();
   }
   template<ttl::size_t N>
   inline bool bitset<N>::none() const
   {
      unsigned i;
      for (i = 0; i < last_slot_index(); ++i)
         if (bits_[i])
            return false;
      return !(bits_[i] & last_bits());
   }
   template<ttl::size_t N>
   ttl::size_t bitset<N>::count() const
   {
      ttl::size_t c = 0;
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
      {
         slot_type b = bits_[i];
         if (i == last_slot_index())
            b &= last_bits();
#ifdef __GNUC__
         c += __builtin_popcountl(b);
#else
         while (b)
         {
            ++c;
            b &= b - 1;
         }
#endif
      }
      return c;
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
      const ttl::size_t n = N < M ? capacity()/(sizeof(slot_type) * CHAR_BIT): other.capacity()/(sizeof(slot_type) * CHAR_BIT);
      for (i = 0; i < n; ++i)
         bits_[i] = other.slot(i);
      for ( ; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] = 0;
      return *this;
   }

   template<const ttl::size_t N>
   bitset<N> &bitset<N>::operator&=(const bitset<N> &other)
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] &= other.bits_[i];
      return *this;
   }
   template<const ttl::size_t N>
   bitset<N> &bitset<N>::operator|=(const bitset<N> &other)
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] |= other.bits_[i];
      return *this;
   }
   template<const ttl::size_t N>
   bitset<N> &bitset<N>::operator^=(const bitset<N> &other)
   {
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         bits_[i] ^= other.bits_[i];
      return *this;
   }
   template<const ttl::size_t N>
   bitset<N> bitset<N>::operator~() const
   {
      bitset<N> other(*this, true);
      for (unsigned i = 0; i < sizeof(bits_)/sizeof(*bits_); ++i)
         other.bits_[i] = ~bits_[i];
      return other;
   }

   template<const ttl::size_t N>
   bitset<N> bitset<N>::operator<<(ttl::size_t pos) const
   {
      bitset<N> other;
      for (unsigned i = 0; pos < N; ++i, ++pos)
         if (test(i))
            other.set(pos);
      return other;
   }

   template<const ttl::size_t N>
   bitset<N> &bitset<N>::operator<<=(ttl::size_t pos)
   {
      bitset<N> other = *this << pos;
      return *this = other;
   }

   template<const ttl::size_t N>
   bitset<N> bitset<N>::operator>>(ttl::size_t pos) const
   {
      bitset<N> other;
      for (unsigned i = 0; pos < N; ++i, ++pos)
         if (test(pos))
            other.set(i);
      return other;
   }

   template<const ttl::size_t N>
   bitset<N> &bitset<N>::operator>>=(ttl::size_t pos)
   {
      bitset<N> other = *this >> pos;
      return *this = other;
   }

   template<const ttl::size_t N>
   bitset<N> operator&(const bitset<N> &a, const bitset<N> &b)
   {
      return bitset<N>(a) &= b;
   }

   template<const ttl::size_t N>
   bitset<N> operator|(const bitset<N> &a, const bitset<N> &b)
   {
      return bitset<N>(a) |= b;
   }

   template<const ttl::size_t N>
   bitset<N> operator^(const bitset<N> &a, const bitset<N> &b)
   {
      return bitset<N>(a) ^= b;
   }

   template<> inline bitset<0>::slot_type *bitset<0>::bits_slot(ttl::size_t) { return bits_; }
   template<> inline bool bitset<0>::test(ttl::size_t) const { return false; }
   template<> inline bitset<0> &bitset<0>::set() { return *this; }
   template<> inline bitset<0> &bitset<0>::set(ttl::size_t) { return *this; }
   template<> inline bitset<0> &bitset<0>::set(ttl::size_t, bool) { return *this; }
   template<> inline bitset<0> &bitset<0>::reset() { return *this; }
   template<> inline bitset<0> &bitset<0>::reset(ttl::size_t) { return *this; }
   template<> inline bitset<0> &bitset<0>::flip() { return *this; }
   template<> inline bitset<0> &bitset<0>::flip(ttl::size_t) { return *this; }
   template<> inline bool bitset<0>::all() const { return false; }
   template<> inline bool bitset<0>::any() const { return false; }
   template<> inline bool bitset<0>::none() const { return true; }
   template<> inline ttl::size_t bitset<0>::count() const { return 0; }
   template<> inline unsigned long long bitset<0>::to_ullong() const { return 0; }
   template<> inline unsigned long bitset<0>::to_ulong() const { return 0; }
   template<> inline bitset<0> &bitset<0>::operator&=(const bitset<0> &) { return *this; }
   template<> inline bitset<0> &bitset<0>::operator^=(const bitset<0> &) { return *this; }
   template<> inline bitset<0> &bitset<0>::operator|=(const bitset<0> &) { return *this; }
   template<> inline bitset<0> bitset<0>::operator~() const { return bitset<0>(); }
   template<> inline bitset<0> bitset<0>::operator<<(ttl::size_t) const { return bitset<0>(); }
   template<> inline bitset<0> bitset<0>::operator>>(ttl::size_t) const { return bitset<0>(); }

   template<const ttl::size_t N> inline bool operator==(const bitset<0> &, const bitset<N> &b) { return b.none(); }
   template<const ttl::size_t N> inline bool operator==(const bitset<N> &a, const bitset<0> &) { return a.none(); }
   template<const ttl::size_t N> inline bool operator!=(const bitset<0> &, const bitset<N> &b) { return !b.none(); }
   template<const ttl::size_t N> inline bool operator!=(const bitset<N> &a, const bitset<0> &) { return !a.none(); }
   template<> inline bool bitset<0>::operator==(const bitset<0> &) const { return true; }
}

#endif // _TINY_TEMPLATE_LIBRARY_HPP_
