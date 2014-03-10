// vim: sw=3 ts=8 et

// To make a better comparison with STL, uninline some code which is located
// in libstdc++: iterator increment/decrement and insertion/deletion of the
// tree nodes.

#include "t.hpp"
#define RBTREE_INLINEABLE /* do not inline */
#define RBTREE_INCLUDE_INLINEABLE 1 /* include the bodies of RBTREE_INLINEABLE here */
#include "ttl/rbtree.hpp"
#include "ttl/map.hpp"

// Explicit template instantiation will instantiate complete template
typedef ttl::map<int, char> i2cmap;

#include "test_map_common.hpp"
