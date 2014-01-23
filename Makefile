CPPFLAGS :=
CFLAGS := -Wall -ggdb -O3
CXXFLAGS := -fno-exceptions -fno-rtti
SHELL := bash
V := @
x.o: x.cpp templates.hpp
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $<
	$(V)size $@ | \
	grep -v text | \
	while read text data bss dec rest; do \
	   echo "text:$$text($$(($$text - $$(< prevtext)))) data:$$data bss:$$bss: dec:$$dec($$(($$dec-$$(< prevdec))))"; \
	   echo "$$text" >prevtext; echo "$$dec" >prevdec; \
	done
