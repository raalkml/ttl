// vim: sw=3 ts=8 et
#include <stddef.h>
#include "ttl/ttl.hpp"
#include "t.hpp"

template class ttl::array<int, 3>;
template class ttl::forward_list<testtype>;
template class ttl::backward_list<testtype>;
template class ttl::bitset<256>;
template class ttl::lazy_queue<testtype>;
template class ttl::pair<int,char>;
template class ttl::pair_vector<char, int>;
template class ttl::vector<testtype>;
template class ttl::fixed_vector<testtype, 16>;
template class ttl::array<ttl::pair<int, char>, 3>;
// template class ttl::map<int, char>; // incomplete rbtree

void test()
{
   printf("ttl::size_t %lu\n", (unsigned long)sizeof(ttl::size_t));
   printf("ttl::ptrdiff_t %lu\n", (unsigned long)sizeof(ttl::ptrdiff_t));
   assert(sizeof(ttl::size_t) == sizeof(::size_t));
   assert(sizeof(ttl::ptrdiff_t) == sizeof(::ptrdiff_t));
}
