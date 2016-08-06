/* braincurses.cpp
 * Copyright © 2002–2015, Brian Derr <brian@derrclan.com>
 */

#include "braincurses.h"

#include <cstdlib>
#include <unordered_map>
#include <vector>


// Maps the x-coordinates (within a WINDOW) for different codeLengths.
const std::unordered_map<int, std::vector<int>> codePosition = {
    {4, {2, 6, 10, 14}},
    {5, {2, 5, 8, 11, 14}},
    {6, {3, 5, 7, 9, 11, 13}}};


Braincurses::Braincurses(int code_length, int guesses) : 
    code_length_(code_length), guesses_(guesses), initialized_(false) {
  InitializeNcurses();
}

void Braincurses::InitializeNcurses() {
  initscr();

  if (has_colors() == FALSE) {
    // TODO: throw an exception.
  }

  start_color();
  if (COLORS < 8) {
    // TODO: throw an exception.
  }
  use_default_colors();

  // Only using the first eight color pairs.
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
  curs_set(1);

  windows_.emplace(kHeaderWindow, CreateWindow(3, 21, 0, 0));
  windows_.emplace(kMarkerWindow, CreateWindow(17, 17, 3, 0));
  windows_.emplace(kGuessWindow, CreateWindow(17, 17, 3, 21));
  windows_.emplace(kCodeWindow, CreateWindow(3, 17, 0, 21));
  windows_.emplace(kWatermarkWindow, CreateWindow(17, 4, 3, 17));
  windows_.emplace(kInputWindow, CreateWindow(3, 60, 20, 0));
  windows_.emplace(kInfoWindow, CreateWindow(20, 22, 0, 38));

  atexit(endwin_handler);
  initialized_ = true;
}

void Braincurses::PrepareBoard(const Code& code) {
  mvwaddstr(windows_[kHeaderWindow], 1, 5, GAME_NAME.c_str());
  mvwaddstr(windows_[kInfoWindow], 1, 2, "Colors: ");
  wattron(windows_[kInfoWindow], A_BOLD);

  wattron(windows_[kInfoWindow], COLOR_PAIR(COLOR_RED));
  mvwaddstr(windows_[kInfoWindow], 2, 2, "RED");
  wattroff(windows_[kInfoWindow], COLOR_PAIR(COLOR_RED));

  waddstr(windows_[kInfoWindow], ", ");

  wattron(windows_[kInfoWindow], COLOR_PAIR(COLOR_BLUE));
  waddstr(windows_[kInfoWindow], "BLUE");
  wattroff(windows_[kInfoWindow], COLOR_PAIR(COLOR_BLUE));

  waddstr(windows_[kInfoWindow], ", ");

  mvwaddstr(windows_[kInfoWindow], 3, 2, "WHITE, ");

  wattron(windows_[kInfoWindow], COLOR_PAIR(COLOR_GREEN));
  waddstr(windows_[kInfoWindow], "GREEN");
  wattroff(windows_[kInfoWindow], COLOR_PAIR(COLOR_GREEN));

  waddstr(windows_[kInfoWindow], ", ");

  wattron(windows_[kInfoWindow], COLOR_PAIR(COLOR_YELLOW));
  mvwaddstr(windows_[kInfoWindow], 4, 2, "YELLOW");
  wattroff(windows_[kInfoWindow], COLOR_PAIR(COLOR_YELLOW));

  waddstr(windows_[kInfoWindow], ", ");

  wattron(windows_[kInfoWindow], COLOR_PAIR(COLOR_MAGENTA));
  waddstr(windows_[kInfoWindow], "MAGENTA");
  wattroff(windows_[kInfoWindow], COLOR_PAIR(COLOR_MAGENTA));

  wattroff(windows_[kInfoWindow], A_BOLD);

  DisplayCode(code, false);

  char guessLabel[3];
  for (int i = 1; i <= guesses_; i++) {
    snprintf(guessLabel, 3, "%2d", i);
    mvwaddstr(windows_[kWatermarkWindow], 16 - i, 1, guessLabel);
  }

  wmove(windows_[kInputWindow], 1, 15);
  wnoutrefresh(windows_[kHeaderWindow]);
  wnoutrefresh(windows_[kCodeWindow]);
  wnoutrefresh(windows_[kInfoWindow]);
  wnoutrefresh(windows_[kWatermarkWindow]);
  doupdate();
}

WINDOW* Braincurses::CreateWindow(int height, int width, int starty, int startx) {
  WINDOW* window;

  window = newwin(height, width, starty, startx);
  box(window, 0, 0);
  wrefresh(window);

  return window;
}

void Braincurses::CleanUpWindow(WINDOW* window) {
  werase(window);
  box(window, 0, 0);
  wrefresh(window);
}

void Braincurses::WipeBoard() {
  for (auto kv : windows_) {
    CleanUpWindow(kv.second);
  }
}

std::vector<int> Braincurses::GetInput() {
  std::string delStr (INPUT_LENGTH, ' ');
  std::vector<int> guessInput (code_length_, -1);
  //int column[4] = {4, 19, 34, 49};
  int column[6] = {2, 11, 20, 29, 38, 47};
  int input;

  auto local_window = windows_[kInputWindow];

  keypad(local_window, TRUE);
  int x = 0;
  while (true) {
    input = mvwgetch(local_window, 1, column[x]);
    switch (input) {
      case 'r':
        mvwaddstr(local_window, 1, column[x], delStr.c_str());
        mvwaddstr(local_window, 1, column[x], "red");
        guessInput[x] = COLOR_RED;
        x++;
        break;
      case 'g':
        mvwaddstr(local_window, 1, column[x], delStr.c_str());
        mvwaddstr(local_window, 1, column[x], "green");
        guessInput[x] = COLOR_GREEN;
        x++;
        break;
      case 'y':
        mvwaddstr(local_window, 1, column[x], delStr.c_str());
        mvwaddstr(local_window, 1, column[x], "yellow");
        guessInput[x] = COLOR_YELLOW;
        x++;
        break;
      case 'b':
        mvwaddstr(local_window, 1, column[x], delStr.c_str());
        mvwaddstr(local_window, 1, column[x], "blue");
        guessInput[x] = COLOR_BLUE;
        x++;
        break;
      case 'm':
        mvwaddstr(local_window, 1, column[x], delStr.c_str());
        mvwaddstr(local_window, 1, column[x], "magenta");
        guessInput[x] = COLOR_MAGENTA;
        x++;
        break;
      case 'w':
        mvwaddstr(local_window, 1, column[x], delStr.c_str());
        mvwaddstr(local_window, 1, column[x], "white");
        guessInput[x] = COLOR_CYAN;  // Really COLOR_WHITE
        x++;
        break;
      case KEY_LEFT:
        if (x > 0) {
          x--;
          wmove(local_window, 1, column[x]);
        }
        break;
      case KEY_RIGHT:
        if (x < code_length_) {
          wmove(local_window, 1, column[x]);
          x++;
        }
        break;
      case KEY_BACKSPACE:
      case KEY_DC:
        if (x > 0 && x <= code_length_) {
          x--;
          mvwaddstr(local_window, 1, column[x], delStr.c_str());
          guessInput[x] = -1;
        }
        break;
      default:
        break;
    }
    wrefresh(local_window);
    if (x == code_length_) {
      break;
    }
  }
  CleanUpWindow(local_window);
  return guessInput;
}

void Braincurses::DisplayGuess(int y, std::vector<int> guess) {
  auto local_window = windows_[kGuessWindow];

  y = 15 - y;

  wattron(local_window, A_BOLD);
  int i = 0;
  for (auto x : codePosition.at(guess.size())) {
    mvwaddch(local_window, y, x, 'X' | COLOR_PAIR(guess[i]));
    i++;
  }
  wattroff(local_window, A_BOLD);
  wrefresh(local_window);
}

void Braincurses::DisplayMarkers(int y, std::vector<int> correct) {
  auto local_window = windows_[kMarkerWindow];

  y = 15 - y;

  wattron(local_window, A_BOLD);

  std::vector<int> x_positions = codePosition.at(code_length_);
  for (int i = 0; i < code_length_; i++) {
    int marker = correct[i];
    int x = x_positions[i];

    if (marker == 2) {  // NAILED_IT
      mvwaddch(local_window, y, x, 'X' | COLOR_PAIR(COLOR_RED));
      continue;
    } else if (marker == 1) {  // ALMOST
      mvwaddch(local_window, y, x, 'X' | COLOR_PAIR(COLOR_WHITE));
      continue;
    }
  }
  wattroff(local_window, A_BOLD);
  wrefresh(local_window);
}

bool Braincurses::IsWinner(std::vector<int> correct) {
  bool winner = true;
  for (unsigned i = 0; i < correct.size(); i++) {
    if (correct[i] != 2) {
      winner = false;
      break;
    }
  }
  return winner;
}

bool Braincurses::GameOverPlayAgain(bool winner) {
  CleanUpWindow(windows_[kInputWindow]);
  if (winner) {
    mvwaddstr(windows_[kInputWindow], 1, 1, "You win! Congratulations!");
  } else {
    mvwaddstr(windows_[kInputWindow], 1, 1, "You ran out of turns; better luck next time.");
  }
  wrefresh(windows_[kInputWindow]);
  return PlayAgain();
}

void Braincurses::DisplayCode(const Code& code, bool colored) {
  auto local_window = windows_[kCodeWindow];

  int i = 0;
  for (auto x : codePosition.at(code.Length())) {
#ifdef DEBUG
    mvwaddch(local_window, 1, x, 'X' | COLOR_PAIR(code.Get()[i]) | A_BOLD);
#else
    if (colored) {
      mvwaddch(local_window, 1, x, 'X' | COLOR_PAIR(code.Get()[i]) | A_BOLD);
    } else {
      mvwaddch(local_window, 1, x, 'X');
    }
#endif
    i++;
  }
  wrefresh(local_window);
}

bool Braincurses::PlayAgain() {
  CleanUpWindow(windows_[kInputWindow]);
  mvwaddstr(windows_[kInputWindow], 1, 1, "Would you like to play again? ([Y]/n) ");
  wrefresh(windows_[kInputWindow]);

  int again = tolower(wgetch(windows_[kInputWindow]));
  return (again == 'y' || again == '\n' ? true : false);
}

bool Braincurses::PlayGame(const Code& code) {
  WipeBoard();
  PrepareBoard(code);

  bool winner = false;
  std::vector<int> correct;
  for (int i = 0; i < guesses_; i++) {
    std::vector<int> guess = GetInput();
    correct = code.IsCorrect(guess.begin(), guess.end());

    DisplayGuess(i, guess);
    DisplayMarkers(i, correct);
    if (IsWinner(correct)) {
      winner = true;
      break;
    }
  }
  DisplayCode(code, true);
  return winner;
}

void endwin_handler(void) {
  endwin();
}
