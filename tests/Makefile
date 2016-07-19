INCLUDE_DIRS := ../external/bandit ../external/json/src

CXXFLAGS += -Wall -std=c++11
CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

.PHONY : all clean run

all : test run

run : test_table
	./test test_table

test : test.o code_test.o ../code.o
	$(LINK.cc) $^ -o $@

test_table :
	python gentable.py > test_table

clean :
	@- $(RM) test test_table
	@- $(RM) *.o
