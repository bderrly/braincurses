CC=g++
CFLAGS=-Wall
CURSES_LIBS=-lncurses

ncurses: windows/windows.o braincurses.o 
	${CC} ${CFLAGS} windows/windows.o braincurses.o ${CURSES_LIBS} -o braincurses 

windows.o: windows.cpp
	${CC} ${CFLAGS} -c windows/windows.cpp

braincurses.o: braincurses.cpp
	${CC} ${CFLAGS} -c braincurses.cpp

install:
	install -o root -g root braincurses /usr/local/bin/

uninstall:
	rm /usr/local/bin/braincurses

clean:
	rm -f braincurses
	rm -f *.o
	rm -f curses/*.o
