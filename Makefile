CPPFLAGS :=
CFLAGS := -Wall -ggdb -O3
CXXFLAGS := -fno-exceptions -fno-rtti
flags :=
SHELL := bash
V := @
x.o: x.cpp templates.hpp
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
	$(V)test -f prevtext && prevtext=$$(< prevtext);\
	    test -n "$$prevtext" || prevtext=0;\
	    test -f prevdec && prevdec=$$(< prevdec);\
	    test -n "$$prevdec" || prevdec=0;\
	    size $@ | \
	    grep -v text | \
	    while read text data bss dec rest; do \
	       echo "text:$$text($$(($$text - $$prevtext))) data:$$data bss:$$bss: dec:$$dec($$(($$dec-$$prevdec)))"; \
	       echo "$$text" >prevtext; echo "$$dec" >prevdec; \
	    done
a.out: x.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $+
