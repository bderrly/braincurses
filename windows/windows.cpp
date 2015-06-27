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
	init_pair(0, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

	cbreak();
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

  keypad(windows->bottom, TRUE);

  mvwaddstr(windows->top_left, 1, 4, GAME_NAME.c_str());
  //mvwaddstr(windows->right, 18, 2, COPYRIGHT.c_str());
  //mvwaddstr(windows->right, 18, 11, AUTHOR.c_str());
  mvwaddstr(windows->right, 1, 2, "Colors: ");
  mvwaddstr(windows->right, 2, 2, "RED, BLUE, YELLOW");
  mvwaddstr(windows->right, 3, 2, "WHITE, GREEN, PURPLE");
  mvwaddstr(windows->right, 5, 2, "Type \"quit\" to end");
  mvwaddstr(windows->right, 6, 2, "the game.");
  mvwaddstr(windows->right, 8, 2, "Remember to hit the");
  mvwaddstr(windows->right, 9, 2, "enter key after each");
  mvwaddstr(windows->right, 10, 2, "guess!");
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

void closeCurses(Windows *windows) {
  destroyWindow(windows->top_left);
  destroyWindow(windows->top_right);
  destroyWindow(windows->left);
  destroyWindow(windows->middle);
  destroyWindow(windows->right);
  destroyWindow(windows->bottom);
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

void cleanUpWindow(WINDOW *window) {
  werase(window);
  box(window, 0, 0);
  wrefresh(window);
}

std::vector<int> getInput(WINDOW *window) {
	std::vector<int> guess (4, 0);
  char input;
 
	int x = 0;
	while (x < 4) {
		input = mvwgetch(window, 1, 14 * (x+1) - INPUT_LENGTH);

		// TODO(brian): Need to add support for arrow keys and backspace/delete
		// to change a prior response. Also, perhaps not setting the value in
		// guess until after all input is confirmed.
		switch (input) {
			case 'r':
				mvwaddstr(window, 1, 14 * (x+1) - INPUT_LENGTH, "red");
				guess.push_back(COLOR_RED);
				x++;
				break;
			case 'w':
				mvwaddstr(window, 1, 14 * (x+1) - INPUT_LENGTH, "white");
				guess.push_back(COLOR_WHITE);
				x++;
				break;
			case 'b':
				mvwaddstr(window, 1, 14 * (x+1) - INPUT_LENGTH, "blue");
				guess.push_back(COLOR_BLUE);
				x++;
				break;
			case 'y':
				mvwaddstr(window, 1, 14 * (x+1) - INPUT_LENGTH, "yellow");
				guess.push_back(COLOR_YELLOW);
				x++;
				break;
			case 'g':
				mvwaddstr(window, 1, 14 * (x+1) - INPUT_LENGTH, "green");
				guess.push_back(COLOR_GREEN);
				x++;
				break;
			case 'p':
				mvwaddstr(window, 1, 14 * (x+1) - INPUT_LENGTH, "purple");
				guess.push_back(COLOR_MAGENTA);
				x++;
				break;
			default:
				mvwdelch(window, 1, 14 * (x+1) - INPUT_LENGTH);
				break;
		}
	}
	cleanUpWindow(window);
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

		mvwaddch(window, y, x, 'X' | COLOR_PAIR(guess[i]) | A_BOLD);
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

    if (correct[i] == 2) {
      mvwaddch(window, y, x, 'X' | COLOR_PAIR(RED) | A_BOLD);
		}
		if (correct[i] == 1) {
      mvwaddch(window, y, x, 'X' | COLOR_PAIR(WHITE) | A_BOLD);
		}
  }
  wrefresh(window);
}

bool isWinner(std::vector<int> correct) {
	bool winner = true;
  for (int i = 0; i < 4; i++) {
    if (!correct[i] == 2) {
      winner = false;
			break;
		}
  }
  return winner;
}

bool gameOverPlayAgain(WINDOW *window, bool winner) {
  cleanUpWindow(window);
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
  cleanUpWindow(window);
  mvwaddstr(window, 1, 1, "Would you like to play again? ([Y]/n) ");
  wrefresh(window);

	//nocbreak();
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
	closeCurses(windows);
	
  return 0;
}
