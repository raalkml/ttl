//////////////////////////////////////////////////////////// vim: sw=3 ts=8 et
//
// Trivial Template Library Test Framework
//
//
// This work is PUBLIC DOMAIN
//

#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace t
{
   bool is_path_separator(const char *);
   char *basename(const char *);

   extern int argc;
   extern char **argv;
}

template <class C> inline const C &constify(C &c) { return c; }

struct testtype
{
   static bool verbose;
   int value;
   testtype();
   testtype(int);
   testtype(const testtype &);
   testtype &operator=(const testtype &);
   ~testtype();
};

inline bool operator==(const testtype &a, const testtype &b)
{
   return a.value == b.value;
}

namespace t
{

   template <class T>
   struct equal_to
   {
      typedef T value_type;
      T value;
      equal_to() {}
      equal_to(const T &v): value(v) {}
      bool operator()(const T &a) const { return a.value == value; }
   };

}


template<class Iterator, typename Distance>
inline Iterator advanceIt(Iterator i, Distance d)
{
   while (d--)
      ++i;
   return i;
}

template<class Iterator>
void print_iter(const char *title, Iterator first, Iterator last)
{
   fputs(title, stdout);
   for (; first != last; ++first)
   {
      typename Iterator::reference r = *first;
      printf('\x21' <= r.value && r.value < 127 ? " '%c'": " %d", first->value);
   }
   fputs(".\n", stdout);
}

