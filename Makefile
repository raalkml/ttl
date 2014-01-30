CPPFLAGS :=
CFLAGS := -Wall -ggdb
CXXFLAGS := -fno-exceptions -fno-rtti
flags := -O3
SHELL := bash
V := @

all: a.out

x.o: x.cpp ttl/ttl.hpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
	$(V)test -f $@.prevtext && prevtext=$$(< $@.prevtext);\
	    test -n "$$prevtext" || prevtext=0;\
	    test -f $@.prevdec && prevdec=$$(< $@.prevdec);\
	    test -n "$$prevdec" || prevdec=0;\
	    size $@ | \
	    grep -v text | \
	    while read text data bss dec rest; do \
	       echo "text:$$text($$(($$text - $$prevtext))) data:$$data bss:$$bss: dec:$$dec($$(($$dec-$$prevdec)))"; \
	       echo "$$text" >$@.prevtext; echo "$$dec" >$@.prevdec; \
	    done
x.s: x.cpp ttl/ttl.hpp
	$(CXX) -S -o $@ $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
a.out: x.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $+
	$(V)test -f $@.prevtext && prevtext=$$(< $@.prevtext);\
	    test -n "$$prevtext" || prevtext=0;\
	    test -f $@.prevdec && prevdec=$$(< $@.prevdec);\
	    test -n "$$prevdec" || prevdec=0;\
	    size $@ | \
	    grep -v text | \
	    while read text data bss dec rest; do \
	       echo "text:$$text($$(($$text - $$prevtext))) data:$$data bss:$$bss: dec:$$dec($$(($$dec-$$prevdec)))"; \
	       echo "$$text" >$@.prevtext; echo "$$dec" >$@.prevdec; \
	    done

valgrind: a.out
	valgrind --tool=memcheck --leak-check=full $(abspath $<)
gdb:
	tmp=$$(tempfile -p gdb) && echo run >"$$tmp";\
	    gdb -q -x "$$tmp"  --args a.out;\
	    rc=$$?;\
	    rm "$$tmp";\
	    exit $$rc
.PHONY: valgrind gdb
