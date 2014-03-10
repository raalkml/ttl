// vim: sw=3 ts=8 et
#include "t.hpp"
#include <map>
#include "ttl/utility.hpp"

// Explicit template instantiation will instantiate complete template
typedef std::map<int, char> i2cmap;

#include "test_map_common.hpp"
