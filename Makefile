CXXFLAGS += -Wall
LDLIBS += -lncurses

.PHONY: all clean

all: braincurses

debug: CXXFLAGS += -DDEBUG -ggdb
debug: braincurses

braincurses: windows/windows.o code.o 
	$(LINK.cc) $^ ${LDLIBS} -o $@

clean:
	@- $(RM) braincurses
	@- $(RM) *.o windows/*.o
