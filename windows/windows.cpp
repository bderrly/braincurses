/* windows.cpp
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#include "windows.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_NUM_GUESSES 10

bool initScreen(Windows *windows) {
  initscr();
	if (!has_colors()) {
		return false;
	}
	start_color();
	use_default_colors();
	for (int i = 0; i < COLORS; i++) {
		init_pair(i, i, -1);
	}
	cbreak();
	noecho();
	intrflush(stdscr, FALSE);
	atexit(closeCurses);
	return true;
}

void prepareGameBoard(Windows *windows, int maxGuesses) {
	curs_set(1); // visibile cursor 

  windows->top_left = createWindow(3, 18, 0, 0);
  windows->top_right = createWindow(3, 18, 0, 18);
  windows->left = createWindow(17, 17, 3, 0);
  windows->middle = createWindow(17, 17, 3, 19);
  windows->right = createWindow(20, 24, 0, 36);
  windows->bottom = createWindow(3, 60, 20, 0);
  windows->slit = createWindow(17, 2, 3, 17);

  mvwaddstr(windows->top_left, 1, 4, GAME_NAME.c_str());
  mvwaddstr(windows->right, 1, 2, "Colors: ");
	wattron(windows->right, COLOR_PAIR(COLOR_RED));
	mvwaddstr(windows->right, 2, 2, "RED");
	wattroff(windows->right, COLOR_PAIR(COLOR_RED));
	waddstr(windows->right, ", ");
	wattron(windows->right, COLOR_PAIR(COLOR_BLUE));
	waddstr(windows->right, "BLUE");
	wattroff(windows->right, COLOR_PAIR(COLOR_BLUE));
	waddstr(windows->right, ", ");
	wattron(windows->right, COLOR_PAIR(COLOR_YELLOW));
	waddstr(windows->right, "YELLOW");
	wattroff(windows->right, COLOR_PAIR(COLOR_YELLOW));

	mvwaddstr(windows->right, 3, 2, "WHITE, ");
	wattron(windows->right, COLOR_PAIR(COLOR_GREEN));
	waddstr(windows->right, "GREEN");
	wattroff(windows->right, COLOR_PAIR(COLOR_GREEN));
	waddstr(windows->right, ", ");
	wattron(windows->right, COLOR_PAIR(COLOR_MAGENTA));
	waddstr(windows->right, "MAGENTA");
	wattroff(windows->right, COLOR_PAIR(COLOR_MAGENTA));
  mvwaddstr(windows->top_right, 1, 3, "X");
  mvwaddstr(windows->top_right, 1, 7, "X");
  mvwaddstr(windows->top_right, 1, 11, "X");
  mvwaddstr(windows->top_right, 1, 15, "X");

  char guessLabel[3];
  for (int i = 1; i <= maxGuesses; i++) {
    snprintf(guessLabel, 3, "%2d", i);
    mvwaddstr(windows->slit, 16 - i, 0, guessLabel);
  }
  wmove(windows->bottom, 1, 15);
  wnoutrefresh(windows->top_left);
  wnoutrefresh(windows->top_right);
  wnoutrefresh(windows->right);
  wnoutrefresh(windows->slit);
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
	if (erase) {
		werase(window);
	}
  box(window, 0, 0);
  wrefresh(window);
}

std::vector<int> getInput(WINDOW *window) {
	std::string delStr (INPUT_LENGTH, ' ');
	int guessInput[4] = {NONE, NONE, NONE, NONE};
	int column[4] = {4, 19, 34, 49};
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
				guessInput[x] = COLOR_WHITE;
				x++;
				break;
			case KEY_LEFT:
				if (x > 0) {
					x--;
					wmove(window, 1, column[x]);
				}
				break;
			case KEY_RIGHT:
				if (x < 3) {
					wmove(window, 1, column[x]);
					x++;
				}
				break;
			case KEY_BACKSPACE:
			case KEY_DC:
				if (x > 0 && x <= 4) {
					x--;
					mvwaddstr(window, 1, column[x], delStr.c_str());
					guessInput[x] = -1;
				}
				break;
			default:
				break;
		}
		wrefresh(window);
		if (x == 4) {
			break;
		}
	}
	cleanUpWindow(window, true);
	std::vector<int> guess (guessInput, guessInput + sizeof(guessInput) / sizeof(int));
  return guess;
}

void displayGuess(WINDOW *window, int y, std::vector<int> guess) {
  int i, x;
  y = 15 - y;

  for (i = 0; i < 4; i++) {
    if (i == 0)
      x = 2;
    else if (i == 1)
      x = 6;
    else if (i == 2)
      x = 10;
    else if (i == 3)
      x = 14;

		wattron(window, A_BOLD);
		mvwaddch(window, y, x, 'X' | COLOR_PAIR(guess[i]));
    wrefresh(window);
  }
}

void displayMarkers(WINDOW *window, int y, std::vector<int> correct) {
  int i, x;
  y = 15 - y;
  for (i = 0; i < 4; i++) {
    if (i == 0)
      x = 2;
    else if (i == 1)
      x = 6;
    else if (i == 2)
      x = 10;
    else if (i == 3)
      x = 14;

		wattron(window, A_BOLD);
    if (correct[i] == 2) {
      mvwaddch(window, y, x, 'X' | COLOR_PAIR(COLOR_RED));
		}
		if (correct[i] == 1) {
      mvwaddch(window, y, x, 'X' | COLOR_PAIR(COLOR_WHITE));
		}
  }
  wrefresh(window);
}

bool isWinner(std::vector<int> correct) {
	bool winner = true;
  for (int i = 0; i < 4; i++) {
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
		mvwaddstr(window, 1, 1, "You win the game! Congratulations!");
	} else {
		mvwaddstr(window, 1, 1, "You ran out of turns! Better luck next time.");
	}
  wrefresh(window);
	return playAgain(window);
}

void displayAnswer(WINDOW *window, std::vector<int> answer) {
  int i, x;
  for (i = 0; i < 4; i++) {
    if (i == 0)
      x = 3;
    else if (i == 1)
      x = 7;
    else if (i == 2)
      x = 11;
    else if (i == 3)
      x = 15;

		mvwaddch(window, 1, x, 'X' | COLOR_PAIR(answer[i]) | A_BOLD);
  }
	curs_set(0);  // invisible cursor
	wrefresh(window);
}

bool playAgain(WINDOW *window) {
  cleanUpWindow(window, true);
  mvwaddstr(window, 1, 1, "Would you like to play again? ([Y]/n) ");
  wrefresh(window);

  char again = wgetch(window);

  return (tolower(again) == 'y' ? true : false);
}

void playGame(Windows *windows, int maxGuesses) {
	Code code;
	code.createCode();
  bool winner = false;

  for (int i = 0; i < maxGuesses; i++) {
		std::vector<int> guess = getInput(windows->bottom);
		std::vector<int> correct = code.isCorrect(guess);

    displayGuess(windows->middle, i, guess);
    displayMarkers(windows->left, i, correct);

    if (isWinner(correct)) {
      winner = true;
      break;
    }
  }

	displayAnswer(windows->top_right, code.getCode());
	if (gameOverPlayAgain(windows->bottom, winner)) {
		playGame(windows, maxGuesses);
	}
}


int main(int argv, char *argc[]) {
  int maxGuesses = DEFAULT_NUM_GUESSES;
  if (argv == 2) maxGuesses = atoi(argc[1]);
  if (maxGuesses <= 0) maxGuesses = DEFAULT_NUM_GUESSES;
  if (maxGuesses > 15) maxGuesses = 15;

	Windows *windows = new Windows;
	if (!initScreen(windows)) {
    std::cerr << "braincurses: Your terminal cannot display colors.\n"
         << "Gameplay is not possible without colors." << std::endl;
    exit(1);
	}
	prepareGameBoard(windows, maxGuesses);
	playGame(windows, maxGuesses);
	
  return 0;
}
