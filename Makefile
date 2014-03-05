export CPPFLAGS :=
export CFLAGS := -Wall -ggdb
export CXXFLAGS := -fno-exceptions -fno-rtti
export ASMFLAGS := -fverbose-asm -dP
export flags := -O1
export ARGS :=
export V := @
export TESTS

TESTS ?= $(sort $(basename $(notdir $(wildcard t/test*)))) all-in-one

all: ttltest.o.report ttltest.report
	-$(V)for f in $+; do echo -n "$$f: "; cat "$$f"; done
	$(MAKE) -C t report

tests:
	$(MAKE) -C t

test: tests
	./t/all-in-one

runtests: tests
	$(V)set -e; for t in $(basename $(notdir $(TESTS))); do "./t/$$t"; done

ttltest.o: ttltest.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
ttltest.s: ttltest.cpp
	$(CXX) -S -o $@ $(CPPFLAGS) $(ASMFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
ttltest.E: ttltest.cpp
	$(CXX) -E -o $@ $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags) $<
-include ttltest.d
%.d: %.cpp
	$(CXX) -MM -MG -o $@ -MQ '$(patsubst %.cpp,%.o,$<)' $< $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(flags)
ttltest: ttltest.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(flags) -o $@ $+

%.report: %
	-$(V): $(eval SHELL:=bash) ; \
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

valgrind: ttltest
	valgrind --tool=memcheck --leak-check=full $(abspath $<) $(ARGS)
gdb:
	-tmp=$$(tempfile -p gdb || echo gdb$$$$.tmp) && echo run >"$$tmp"; \
	test -f "$$tmp" && opt="-x $$tmp";\
	gdb -q $$opt --args ttltest $(ARGS);\
	rc=$$?;\
	rm -f "$$tmp";\
	exit $$rc

clean:
	$(RM) ttltest.o ttltest.d ttltest
	$(MAKE) -C t $@

.PHONY: valgrind gdb all test tests runtests clean
