CPPFLAGS := -I..
CFLAGS ?= -Wall -ggdb
CXXFLAGS ?= -fno-exceptions -fno-rtti
ASMFLAGS ?= -fverbose-asm -dP
flags ?= -O1
V ?= @

sources := $(wildcard test*.cpp) t.cpp
testsrcs = $(filter test%.cpp,$(sources))
tests = $(patsubst %.cpp,%,$(testsrcs))

tests all: $(tests) all-in-one
clean:
	$(RM) $(patsubst %.cpp,%.report,$(testsrcs)) all-in-one.report
	$(RM) $(patsubst %.cpp,%.to,$(sources)) all-in-one
	$(RM) $(patsubst %.cpp,%.d,$(sources))
	$(RM) $(patsubst %.cpp,%.o,$(sources))
	$(RM) $(tests)

%.o: %.cpp ; $(CXX) -o $@ -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
%.s: %.cpp ; $(CXX) -o $@ -S $(CPPFLAGS) $(ASMFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
%.E: %.cpp ; $(CXX) -o $@ -E $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<

-include $(patsubst %.cpp,%.d,$(sources))
%.d: %.cpp
	$(CXX) -o $@ -MM -MG -MQ '$(patsubst %.cpp,%.o,$<)' $< $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags)

test%: t.o test%.o
	$(CXX) -o $@ $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(flags) $+

mangled_test_name := $(shell echo 'void test(){}' | \
   $(CXX) -x c++ -o t.to.tmp -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) - && \
   nm -g -f posix t.to.tmp | if read f eol; then echo $$f; fi; $(RM) t.to.tmp)

%.to: %.o
	objcopy $< $@ --redefine-sym $(mangled_test_name)=$(shell echo 'void $(*F)(){}' | \
	   $(CXX) -x c++ -o $@.tmp -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) - && \
	   nm -g -f posix $@.tmp | if read f eol; then echo $$f; fi; $(RM) $@.tmp)

all-in-one: $(patsubst %.cpp,%.to,$(testsrcs)) t.o
	$(V)$(subst s,:,$(findstring s,$(MAKEFLAGS))) echo "generating test() function for $@..."
	$(V)( \
	   echo '#include <stdio.h>'; \
	   for n in $(tests); \
	   do echo "void $$n();"; done; \
	   echo 'void test() {'; \
	   for n in $(tests); \
	   do echo 'puts(" *** '$$n' ***");'$$n'();'; done; \
	   echo '}'; \
	) | $(CXX) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) -x c++ -c - -o t.to
	$(CXX) -o $@ $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(flags) $+ t.to

%.report: %
	-$(V): $(eval SHELL:=bash); \
	    test -f $<.prevtext && prevtext=$$(< $<.prevtext);\
	    test -n "$$prevtext" || prevtext=0;\
	    test -f $<.prevdec && prevdec=$$(< $<.prevdec);\
	    test -n "$$prevdec" || prevdec=0;\
	    test -f $<.prevdata && prevdata=$$(< $<.prevdata);\
	    test -n "$$prevdata" || prevdata=0;\
	    test -f $<.prevbss && prevbss=$$(< $<.prevbss);\
	    test -n "$$prevbss" || prevbss=0;\
	    size $< | \
	    grep -v text | \
	    while read text data bss dec rest; do \
	       printf 'text:%d%+d data:%d%+d bss:%d%+d dec:%d%+d\n' \
	       $$text $$(($$text - $$prevtext)) \
	       $$data $$(($$data-$$prevdata))\
	       $$bss $$(($$bss-$$prevbss))\
	       $$dec $$(($$dec-$$prevdec)); \
	       echo "$$text" >$<.prevtext; \
	       echo "$$data" >$<.prevdata; \
	       echo "$$bss" >$<.prevbss; \
	       echo "$$dec" >$<.prevdec; \
	    done > $@
	@test -t 1 && echo -ne '\033[01m$@:	';cat $@;test -t 1 && echo -ne '\033[0m'

reports .NOTPARALLEL: $(patsubst %.cpp,%.report,$(testsrcs)) all-in-one.report
	$(V)for r in $(patsubst %.cpp,%.report,$(testsrcs)) all-in-one.report; do echo -n "$$r:	"; cat "$$r"; done
report: tests
	$(MAKE) reports

#valgrind: ttltest
#	valgrind --tool=memcheck --leak-check=full $(abspath $<) $(ARGS)
#gdb:
#	-tmp=$$(tempfile -p gdb || echo gdb$$$$.tmp) && echo run >"$$tmp"; \
#	test -f "$$tmp" && opt="-x $$tmp";\
#	gdb -q $$opt --args ttltest $(ARGS);\
#	rc=$$?;\
#	rm -f "$$tmp";\
#	exit $$rc
.PHONY: valgrind gdb all tests report reports clean
