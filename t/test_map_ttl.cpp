// vim: sw=3 ts=8 et
#include "t.hpp"
#include <map>
#include <utility>
#include "ttl/utility.hpp"
#include "ttl/array.hpp"
#include "ttl/map.hpp"

// Explicit template instantiation will instantiate complete template
typedef ttl::map<int, char> i2cmap;

namespace ttl
{
   inline bool operator==(const std::map<int,char>::value_type &a,
                          const map<int,char>::value_type &b)
   {
      return a.first == b.first && a.second == b.second;
   }

   inline bool operator==(const pair<const int,char> &a, const pair<int,char> &b)
   {
      return a.first == b.first && a.second == b.second;
   }
}

#include "test_map_common.hpp"
