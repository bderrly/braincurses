/* windows.cpp
 * Copyright © 2002–2015, Brian Derr <brian@derrclan.com>
 */

#include "windows.h"

#include <cstdlib>
#include <string>
#include <vector>


const std::unordered_map<int, std::vector<int>> codePosition = {
	{4, {2, 6, 10, 14}},
	{5, {2, 5, 8, 11, 14}},
	{6, {3, 5, 7, 9, 11, 13}}};

bool initScreen(Windows &windows) {
  if (!initscr()) {
		return false;
	}
	if (!has_colors()) {
		return false;
	}
	start_color();
	if (COLORS < 8) {
		return false;
	}
	use_default_colors();
	for (int i = 0; i < 8; i++) {
		if (i == 6) {
			// COLOR_CYAN is hard to distinguish from COLOR_BLUE; replace it with COLOR_WHITE.
			init_pair(i, i+1, -1);
		} else {
			init_pair(i, i, -1);
		}
	}
	cbreak();
	noecho();
	intrflush(stdscr, FALSE);
	curs_set(1); // visibile cursor 

	windows.emplace("header", createWindow(3, 21, 0, 0));
	windows.emplace("markers", createWindow(17, 17, 3, 0));
	windows.emplace("guesses", createWindow(17, 17, 3, 21));
	windows.emplace("code", createWindow(3, 17, 0, 21));
	windows.emplace("watermark", createWindow(17, 4, 3, 17));
	windows.emplace("input", createWindow(3, 60, 20, 0));
	windows.emplace("info", createWindow(20, 22, 0, 38));

	atexit(closeCurses);
	return true;
}

void prepareGameBoard(Windows &windows, int maxGuesses, Code code) {
  mvwaddstr(windows["header"], 1, 5, GAME_NAME.c_str());
	mvwaddstr(windows["info"], 1, 2, "Colors: ");
	wattron(windows["info"], A_BOLD);

	wattron(windows["info"], COLOR_PAIR(COLOR_RED));
	mvwaddstr(windows["info"], 2, 2, "RED");
	wattroff(windows["info"], COLOR_PAIR(COLOR_RED));

	waddstr(windows["info"], ", ");

	wattron(windows["info"], COLOR_PAIR(COLOR_BLUE));
	waddstr(windows["info"], "BLUE");
	wattroff(windows["info"], COLOR_PAIR(COLOR_BLUE));

	waddstr(windows["info"], ", ");

	mvwaddstr(windows["info"], 3, 2, "WHITE, ");

	wattron(windows["info"], COLOR_PAIR(COLOR_GREEN));
	waddstr(windows["info"], "GREEN");
	wattroff(windows["info"], COLOR_PAIR(COLOR_GREEN));

	waddstr(windows["info"], ", ");

	wattron(windows["info"], COLOR_PAIR(COLOR_YELLOW));
	mvwaddstr(windows["info"], 4, 2, "YELLOW");
	wattroff(windows["info"], COLOR_PAIR(COLOR_YELLOW));

	waddstr(windows["info"], ", ");

	wattron(windows["info"], COLOR_PAIR(COLOR_MAGENTA));
	waddstr(windows["info"], "MAGENTA");
	wattroff(windows["info"], COLOR_PAIR(COLOR_MAGENTA));

	wattroff(windows["info"], A_BOLD);

	displayCode(windows["code"], code, false);

  char guessLabel[3];
  for (int i = 1; i <= maxGuesses; i++) {
    snprintf(guessLabel, 3, "%2d", i);
    mvwaddstr(windows["watermark"], 16 - i, 1, guessLabel);
  }

  wmove(windows["input"], 1, 15);
  wnoutrefresh(windows["header"]);
  wnoutrefresh(windows["code"]);
  wnoutrefresh(windows["info"]);
  wnoutrefresh(windows["watermark"]);
  doupdate();
}

void closeCurses() {
	endwin();
}

WINDOW *createWindow(int height, int width, int starty, int startx) {
  WINDOW *window;

  window = newwin(height, width, starty, startx);
  box(window, 0, 0);
  wrefresh(window);

  return window;
}

void destroyWindow(WINDOW *local_win) {
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}

void cleanUpWindow(WINDOW *window, bool erase) {
	if (erase) werase(window);
  box(window, 0, 0);
  wrefresh(window);
}

void wipeGameBoard(Windows &windows) {
	for (auto kv : windows) {
		cleanUpWindow(kv.second, true);
	}
}

std::vector<int> getInput(WINDOW *window, int codeLength) {
	std::string delStr (INPUT_LENGTH, ' ');
	std::vector<int> guessInput (codeLength, -1);
	//int column[4] = {4, 19, 34, 49};
	int column[6] = {2, 11, 20, 29, 38, 47};
  int input;

	keypad(window, TRUE);
	int x = 0;
	while (true) {
		input = mvwgetch(window, 1, column[x]);
		switch (input) {
			case 'r':
				mvwaddstr(window, 1, column[x], delStr.c_str());
				mvwaddstr(window, 1, column[x], "red");
				guessInput[x] = COLOR_RED;
				x++;
				break;
			case 'g':
				mvwaddstr(window, 1, column[x], delStr.c_str());
				mvwaddstr(window, 1, column[x], "green");
				guessInput[x] = COLOR_GREEN;
				x++;
				break;
			case 'y':
				mvwaddstr(window, 1, column[x], delStr.c_str());
				mvwaddstr(window, 1, column[x], "yellow");
				guessInput[x] = COLOR_YELLOW;
				x++;
				break;
			case 'b':
				mvwaddstr(window, 1, column[x], delStr.c_str());
				mvwaddstr(window, 1, column[x], "blue");
				guessInput[x] = COLOR_BLUE;
				x++;
				break;
			case 'm':
				mvwaddstr(window, 1, column[x], delStr.c_str());
				mvwaddstr(window, 1, column[x], "magenta");
				guessInput[x] = COLOR_MAGENTA;
				x++;
				break;
			case 'w':
				mvwaddstr(window, 1, column[x], delStr.c_str());
				mvwaddstr(window, 1, column[x], "white");
				guessInput[x] = COLOR_CYAN;  // Really COLOR_WHITE
				x++;
				break;
			case KEY_LEFT:
				if (x > 0) {
					x--;
					wmove(window, 1, column[x]);
				}
				break;
			case KEY_RIGHT:
				if (x < codeLength) {
					wmove(window, 1, column[x]);
					x++;
				}
				break;
			case KEY_BACKSPACE:
			case KEY_DC:
				if (x > 0 && x <= codeLength) {
					x--;
					mvwaddstr(window, 1, column[x], delStr.c_str());
					guessInput[x] = -1;
				}
				break;
			default:
				break;
		}
		wrefresh(window);
		if (x == codeLength) {
			break;
		}
	}
	cleanUpWindow(window, true);
  return guessInput;
}

void displayGuess(WINDOW *window, int y, std::vector<int> guess) {
  y = 15 - y;

	wattron(window, A_BOLD);
	int i = 0;
	for (auto x : codePosition.at(guess.size())) {
		mvwaddch(window, y, x, 'X' | COLOR_PAIR(guess[i]));
		i++;
  }
	wattroff(window, A_BOLD);
	wrefresh(window);
}

void displayMarkers(WINDOW *window, int y, std::vector<int> correct) {
  int i, red, white;
	red = white = 0;
  y = 15 - y;

	i = 0;
	for (auto c : correct) {
		if (c == 2) {
			red++;
		} else if (c == 1) {
			white++;
		}
		i++;
  }

	wattron(window, A_BOLD);
	i = 0;
	for (auto x : codePosition.at(correct.size())) {
		if (red > 0) {
			mvwaddch(window, y, x, 'X' | COLOR_PAIR(COLOR_RED));
			red--;
			continue;
		}
		if (white > 0) {
      mvwaddch(window, y, x, 'X' | COLOR_PAIR(COLOR_WHITE));
			white--;
			continue;
		}
		i++;
	}
	wattroff(window, A_BOLD);
  wrefresh(window);
}

bool isWinner(std::vector<int> correct) {
	bool winner = true;
  for (unsigned i = 0; i < correct.size(); i++) {
    if (correct[i] != 2) {
      winner = false;
			break;
		}
  }
  return winner;
}

bool gameOverPlayAgain(WINDOW *window, bool winner) {
  cleanUpWindow(window, true);
	if (winner) {
		mvwaddstr(window, 1, 1, "You win! Congratulations!");
	} else {
		mvwaddstr(window, 1, 1, "You ran out of turns; better luck next time.");
	}
  wrefresh(window);
	return playAgain(window);
}

void displayCode(WINDOW *window, Code code, bool colored) {
	int i = 0;
	for (auto x : codePosition.at(code.getCodeLength())) {
#ifdef DEBUG
		mvwaddch(window, 1, x, 'X' | COLOR_PAIR(code.getCode()[i]) | A_BOLD);
#else
		if (colored) {
			mvwaddch(window, 1, x, 'X' | COLOR_PAIR(code.getCode()[i]) | A_BOLD);
		} else {
			mvwaddch(window, 1, x, 'X');
		}
#endif
		i++;
  }
	wrefresh(window);
}

bool playAgain(WINDOW *window) {
  cleanUpWindow(window, true);
  mvwaddstr(window, 1, 1, "Would you like to play again? ([Y]/n) ");
  wrefresh(window);

  int again = tolower(wgetch(window));
  return (again == 'y' || again == '\n' ? true : false);
}

bool playGame(Windows &windows, Code code, int maxGuesses) {
	bool winner = false;
	std::vector<int> correct;
  for (int i = 0; i < maxGuesses; i++) {
		std::vector<int> guess = getInput(windows["input"], code.getCodeLength());
		correct = code.isCorrect(guess);

    displayGuess(windows["guesses"], i, guess);
    displayMarkers(windows["markers"], i, correct);
		if (isWinner(correct)) {
			winner = true;
			break;
		}
  }
	return winner;
}
