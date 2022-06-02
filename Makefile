export ARGS :=
export V := @

export TESTS := $(sort $(basename $(notdir $(wildcard t/test*.cpp)))) all-in-one

tests:
	$(MAKE) -C t

all:
	$(MAKE) -C t report

test: tests
	./t/all-in-one

runtests: tests
	$(V)set -e; for t in $(basename $(notdir $(TESTS))); do "./t/$$t"; done


clean depclean distclean clean-reports valgrind gdb report reports:
	$(MAKE) -C t $@

.PHONY: valgrind gdb all test tests runtests report reports clean depclean distclean clean-reports
