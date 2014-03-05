// vim: sw=3 ts=8 et
#include "t.hpp"

testtype::testtype():
   value(-1)
{
   printf("%p %s\n", this, __func__);
}

testtype::testtype(int v):
   value(v)
{
   printf("%p %s(int v) %d\n", this, __func__, value);
}

testtype::testtype(const testtype &o):
   value(o.value)
{
   printf("%p %s(const testtype &) %d\n", this, __func__, value);
}

testtype &testtype::operator=(const testtype &o)
{
   value = o.value;
   printf("%p %s %d\n", this, __func__, value);
   return *this;
}

testtype::~testtype()
{
   printf("%p %s %d\n", this, __func__, value);
}

namespace t
{

   bool is_path_separator(const char *s)
   {
      return '/' == *s ||
#ifdef __CYGWIN__
         '\\' == *s ||
#endif
         false;
   }

   char *basename(const char *s)
   {
      char *e = (char *)s + strlen(s);
      while (e > s)
         if (is_path_separator(--e))
            break;
      if (is_path_separator(e))
         ++e;
      return e;
   }

   extern int argc;
   extern char **argv;

   static char def_argv0[] = "t";
   static char *def_argv[] = {def_argv0, NULL};
   int argc = 1;
   char **argv = def_argv;
}

void test();

int main(int argc, char *argv[])
{
   t::argc = argc;
   t::argv = argv;
   printf("*** %s ***\n", t::basename(argv[0]));
   test();
   printf("*** %s finished ***\n", t::basename(argv[0]));
   return 0;
}
