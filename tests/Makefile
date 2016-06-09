INCLUDE_DIRS := ../external/bandit

CXXFLAGS += -Wall -std=c++11
CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

all: clean test run

run:
	./test

test: test.o code_test.o ../code.o
	$(LINK.cc) $^ -o $@

clean:
	@- $(RM) test
