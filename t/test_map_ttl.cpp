// vim: sw=3 ts=8 et
#include "t.hpp"
#include <map>
#include <utility>
#include "ttl/utility.hpp"
#include "ttl/array.hpp"
#include "ttl/map.hpp"

// Explicit template instantiation will instantiate complete template
typedef ttl::map<int, char> i2cmap;

#include "test_map_common.hpp"
