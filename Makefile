CXXFLAGS += -Wall
LDLIBS += -lncurses

.PHONY: all clean

all: braincurses

braincurses: windows/windows.o code.o 
	$(LINK.cc) $^ ${LDLIBS} -o $@

clean:
	@- $(RM) braincurses
	@- $(RM) *.o windows/*.o
