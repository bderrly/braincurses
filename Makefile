CXXFLAGS += -Wall -std=c++11
LDLIBS += -lncurses

.PHONY: all clean debug

all: braincurses

debug: CXXFLAGS += -DDEBUG -ggdb -O0
debug: braincurses

braincurses: braincurses.o code.o windows.o
	$(LINK.cc) $^ ${LDLIBS} -o $@

clean:
	@- $(RM) braincurses
	@- $(RM) *.o
