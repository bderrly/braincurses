LDLIBS=-lncurses

braincurses: windows/windows.o braincurses.o 
	$(LINK.cc) $^ $(LDLIBS) -o $@

clean:
	rm -f braincurses
	rm -f **/*.o
