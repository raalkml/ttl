/////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
//                        Tiny Template Library
//
//                     This code is Public Domain
//
// A library of STL-inspired templates designed to work in conditions
// of constrained resources, but also without RTTI and exceptions.
//
// All of the used algorithms and data structures either have prior art
// or are trivial and well known.
//
// Care was taken to avoid patented or prohibitively licensed code,
// even if this meant suboptimal code.
//
//
// Some templates are implementations of STL interfaces, some are not.
//
// There is no allocator support and the O(log N) and similar complexity
// guarantees might be not implemented.
//
// The reverse iterators are not implemented.
//
// Usually only equality operators are implemented, if any.
//
// There is generally no streaming operator support.
//
// This header is optional. The feature headers below should be usable on
// their own, unless the code needs features from the others.
//
#ifndef _TINY_TEMPLATE_LIBRARY_HPP_
#define _TINY_TEMPLATE_LIBRARY_HPP_ 1

#include "types.hpp"
#include "type_traits.hpp"
#include "functional.hpp"
#include "utility.hpp"
#include "algorithm.hpp"
#include "array.hpp"
#include "vector.hpp"
#include "fixed_vector.hpp"
#include "forward_list.hpp"
#include "backward_list.hpp"
#include "lazy_queue.hpp"
#include "list.hpp"
#include "map.hpp"
#include "set.hpp"
#include "vector_map.hpp"
#include "sorted_vector_map.hpp"
#include "bitset.hpp"

namespace ttl
{
   //
   // TODO: items below
   //

   // Lists with a fixed predefined storage
   template<typename T, const ttl::size_t N> class fixed_list {};
   template<typename T, const ttl::size_t N> class fixed_forward_list {};
   template<typename T, const ttl::size_t N> class fixed_backward_list {};

   // Red-black tree derivations
   template<typename Key, typename Compare> class multiset {};
   template<typename T> class multimap {};
}

#endif // _TINY_TEMPLATE_LIBRARY_HPP_
