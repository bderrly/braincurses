CXXFLAGS += -Wall -std=c++11
LDLIBS += -lncurses

.PHONY: all clean debug test

all: braincurses

debug: CXXFLAGS += -DDEBUG -ggdb -O0
debug: braincurses

braincurses: braincurses.o code.o windows.o
	$(LINK.cc) $^ ${LDLIBS} -o $@

test: code_test

code_test: code_test.o code.o
	$(LINK.cc) $^ -o $@
	./code_test

clean:
	@- $(RM) braincurses code_test
	@- $(RM) *.o
