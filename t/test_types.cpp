// vim: sw=3 ts=8 et
#include "ttl/type_traits.hpp"
#include "t.hpp"

void test()
{
   printf("int:      is_integral: %d\n", ttl::is_integral<int>::value);
   printf("float:    is_integral: %d\n", ttl::is_integral<float>::value);
   printf("testtype: is_integral: %d\n", ttl::is_integral<testtype>::value);
   printf("int:  is_void: %d\n", ttl::is_void<int>::value);
   printf("void: is_void: %d\n", ttl::is_void<void>::value);
   printf("int:    is_float: %d\n", ttl::is_floating_point<int>::value);
   printf("double: is_float: %d\n", ttl::is_floating_point<double>::value);
   printf("float:  is_float: %d\n", ttl::is_floating_point<float>::value);
   printf("int:  is_signed: %d\n", ttl::is_signed<int>::value);
   printf("uint: is_signed: %d\n", ttl::is_signed<unsigned int>::value);
   printf("int:  is_unsigned: %d\n", ttl::is_unsigned<int>::value);
   printf("uint: is_unsigned: %d\n", ttl::is_unsigned<unsigned int>::value);
}
