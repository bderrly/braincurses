/* windows.cpp
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#include "windows.h"

#include <cstdlib>
#include <iostream>
#include <string>

#define DEFAULT_NUM_GUESSES 10

int main(int argv, char *argc[]) {
  int maxGuesses = DEFAULT_NUM_GUESSES;
  if (argv == 2) maxGuesses = atoi(argc[1]);
  if (maxGuesses <= 0) maxGuesses = DEFAULT_NUM_GUESSES;
  if (maxGuesses > 15) maxGuesses = 15;
  initscr();

  if (has_colors()) {
    start_color();

    init_pair(0, COLOR_RED, COLOR_BLACK);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  } else {
    std::cerr << "braincurses: Your terminal cannot display colors.\n"
         << "Gameplay is not possible without colors." << std::endl;
    exit(99);
  }

  cbreak();
  curs_set(1);

  Winders winders;

  winders.top_left = create_newwin(3, 18, 0, 0);
  winders.top_right = create_newwin(3, 18, 0, 18);
  winders.left = create_newwin(17, 17, 3, 0);
  winders.middle = create_newwin(17, 17, 3, 19);
  winders.right = create_newwin(20, 24, 0, 36);
  winders.bottom = create_newwin(3, 60, 20, 0);

  keypad(winders.bottom, TRUE);

  // --- make a small window for the ruler --- //
  WINDOW *slit;
  slit = newwin(17, 2, 3, 17);
  wrefresh(slit);
  // --- all done with that --- //

  // ----------------------------- //
  //    set up unchanging texts    //
  char game_name[12] = "BrainCurses";
  char author[11] = "Brian Derr";
  char copyright[9] = "(c) 2002";
  char hidden = 'X';
  mvwaddstr(winders.top_left, 1, 4, game_name);
  mvwaddstr(winders.right, 18, 2, copyright);
  mvwaddstr(winders.right, 18, 11, author);
  mvwaddstr(winders.right, 1, 2, "Colors: ");
  mvwaddstr(winders.right, 2, 2, "RED, BLUE, YELLOW");
  mvwaddstr(winders.right, 3, 2, "WHITE, GREEN, PURPLE");
  mvwaddstr(winders.right, 5, 2, "Type \"quit\" to end");
  mvwaddstr(winders.right, 6, 2, "the game.");
  mvwaddstr(winders.right, 8, 2, "Remember to hit the");
  mvwaddstr(winders.right, 9, 2, "enter key after each");
  mvwaddstr(winders.right, 10, 2, "guess!");
  mvwaddch(winders.top_right, 1, 3, hidden);
  mvwaddch(winders.top_right, 1, 7, hidden);
  mvwaddch(winders.top_right, 1, 11, hidden);
  mvwaddch(winders.top_right, 1, 15, hidden);

  char guessLabel[3];
  for (int i = 1; i <= maxGuesses; i++) {
    snprintf(guessLabel, 3, "%2d", i);
    mvwaddstr(slit, 16 - i, 0, guessLabel);
  }
  wmove(winders.bottom, 1, 15);
  wnoutrefresh(winders.top_left);
  wnoutrefresh(winders.top_right);
  wnoutrefresh(winders.right);
  wnoutrefresh(slit);
  doupdate();
  // ----------------------------- //

  int marker_arr[4];
  int *guesses;

	// TODO(brian): HACK! Don't forget to actually create an answer.
	int answer[4] = {0, 0, 0, 0};

  int outer = 0, inner = 0;
  bool winner = false;
  while (outer < maxGuesses) {
    Guess guess;
    for (inner = 0; inner < 4; inner++) {
      if (!getInput(guess, inner, winders)) {
        wrongInput(winders.bottom, guess, inner);
        inner -= 1;
      }
    }  // end "inner" loop

    cleanUpWindow(winders.bottom);
    guess.compareWithAnswer(answer);
    guesses = guess.getGuesses();
    dispGuesses(winders.middle, guesses, outer);
    guess.showMarkers(marker_arr);
    dispMarkers(winders.left, marker_arr, outer);

    if (isWinner(marker_arr)) {
      winner = true;
      break;
    }

    outer++;
  }  // end "outer" while loop

  if (winner == true) {
    youWin(winders.bottom);
    dispAnswers(winders.top_right, answer);
    // writeScores( outer );
  } else {
    youLose(winders.bottom);
    dispAnswers(winders.top_right, answer);
  }

  wgetch(winders.bottom);
  if (playAgain(winders.bottom)) main(argv, argc);

  // wrap the show up
  closeCurses(winders);
  Guess g;
  g.quitGame();

  return (0);
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);

  return (local_win);
}

void destroy_win(WINDOW *local_win) {
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}

void cleanUpWindow(WINDOW *local_win) {
  werase(local_win);
  box(local_win, 0, 0);
  wrefresh(local_win);
}

bool getInput(Guess &g, int x, Winders winders) {
  bool done = false;
  char input[INPUT_LENGTH];
  mvwgetnstr(winders.bottom, 1, 14 * (x + 1) - INPUT_LENGTH, input,
             INPUT_LENGTH);

	std::string guess = input;
  if (!strcmp(input, "quit")) {
    closeCurses(winders);
    g.quitGame();
  } else if (g.isValid(guess)) {
    done = true;
	}

  return (done);
}

void wrongInput(WINDOW *local_win, Guess &g, int x) {
  // first lets clear the window
  int places[4] = {7, 21, 35, 49};
  if (x == 0) {
    wmove(local_win, 1, places[0]);
    waddstr(local_win, "                                               ");
  } else if (x == 1) {
    wmove(local_win, 1, places[1]);
    waddstr(local_win, "                                   ");
  } else if (x == 2) {
    wmove(local_win, 1, places[2]);
    waddstr(local_win, "                        ");
  } else {
    wmove(local_win, 1, places[3]);
    waddstr(local_win, "         ");
  }
  wrefresh(local_win);
}

void dispGuesses(WINDOW *local_win, int *guesses, int outer) {
  int i, y, x;
  y = 15 - outer;
  for (i = 0; i < 4; i++) {
    if (i == 0)
      x = 2;
    else if (i == 1)
      x = 6;
    else if (i == 2)
      x = 10;
    else if (i == 3)
      x = 14;
    switch (guesses[i]) {
      case RED:
        mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(0) | A_BOLD);
        break;
      case WHITE:
        mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(1) | A_BOLD);
        break;
      case YELLOW:
        mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(2) | A_BOLD);
        break;
      case GREEN:
        mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(3) | A_BOLD);
        break;
      case BLUE:
        mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(4) | A_BOLD);
        break;
      case PURPLE:
        mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(5) | A_BOLD);
        break;
    }
    wrefresh(local_win);
  }
}

void dispMarkers(WINDOW *local_win, int marker_arr[], int outer) {
  int i, y, x;
  y = 15 - outer;
  for (i = 0; i < 4; i++) {
    if (i == 0)
      x = 2;
    else if (i == 1)
      x = 6;
    else if (i == 2)
      x = 10;
    else if (i == 3)
      x = 14;

    if (marker_arr[i] == 0)
      mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(0) | A_BOLD);
    else if (marker_arr[i] == 1)
      mvwaddch(local_win, y, x, 'X' | COLOR_PAIR(1) | A_BOLD);
  }  // end for
  wrefresh(local_win);
}

bool isWinner(int marker_arr[]) {
  int i;
  bool winner = true;

  for (i = 0; i < 4; i++) {
    if (marker_arr[i] == 0)
      continue;
    else {
      winner = false;
      break;
    }
  }  // end for
  return (winner);
}

void youWin(WINDOW *local_win) {
  cleanUpWindow(local_win);
  mvwaddstr(local_win, 1, 1, "You win the game!  Congratulations!");
  wrefresh(local_win);
}

void youLose(WINDOW *local_win) {
  cleanUpWindow(local_win);
  mvwaddstr(local_win, 1, 1, "You ran out of turns!  Better luck next time.");
  wrefresh(local_win);
}

void dispAnswers(WINDOW *local_win, int answer[]) {
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

    switch (answer[i]) {
      case RED:
        mvwaddch(local_win, 1, x, 'X' | COLOR_PAIR(0) | A_BOLD);
        break;
      case WHITE:
        mvwaddch(local_win, 1, x, 'X' | COLOR_PAIR(1) | A_BOLD);
        break;
      case YELLOW:
        mvwaddch(local_win, 1, x, 'X' | COLOR_PAIR(2) | A_BOLD);
        break;
      case GREEN:
        mvwaddch(local_win, 1, x, 'X' | COLOR_PAIR(3) | A_BOLD);
        break;
      case BLUE:
        mvwaddch(local_win, 1, x, 'X' | COLOR_PAIR(4) | A_BOLD);
        break;
      case PURPLE:
        mvwaddch(local_win, 1, x, 'X' | COLOR_PAIR(5) | A_BOLD);
        break;
    }

    wrefresh(local_win);
    curs_set(0);
  }
}

bool playAgain(WINDOW *local_win) {
  noecho();
  cleanUpWindow(local_win);
  mvwaddstr(local_win, 1, 1, "Would you like to play again?  (y/n)");
  wrefresh(local_win);

  char again;
  again = wgetch(local_win);

  echo();

  return (again == 'y' ? true : false);
}

void closeCurses(Winders winders) {
  destroy_win(winders.top_left);
  destroy_win(winders.top_right);
  destroy_win(winders.left);
  destroy_win(winders.middle);
  destroy_win(winders.right);
  destroy_win(winders.bottom);

  endwin();
}
