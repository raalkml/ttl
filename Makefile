export CPPFLAGS :=
export CFLAGS := -Wall -ggdb
export CXXFLAGS := -fno-exceptions -fno-rtti
export ASMFLAGS := -fverbose-asm -dP
export flags := -O1 -foptimize-sibling-calls
export ARGS :=
export V := @

export TESTS := $(sort $(basename $(notdir $(wildcard t/test*)))) all-in-one

all:
	$(MAKE) -C t report

tests:
	$(MAKE) -C t

test: tests
	./t/all-in-one

runtests: tests
	$(V)set -e; for t in $(basename $(notdir $(TESTS))); do "./t/$$t"; done


clean valgrind gdb report reports:
	$(MAKE) -C t $@

.PHONY: valgrind gdb all test tests runtests clean report reports
