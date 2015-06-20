/* windows.h
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#ifndef WINDOWS_H
#define WINDOWS_H

#include <cstring>
#include <ncurses.h>
#include <string>
#include <term.h>
#include "../braincurses.h"

const int INPUT_LENGTH = 7;
const std::string AUTHOR = "Brian Derr";
const std::string COPYRIGHT = "(c) 2002";
const std::string GAME_NAME = "BrainCurses";

struct Winders {
  WINDOW *top_left;
  WINDOW *top_right;
  WINDOW *left;
  WINDOW *middle;
  WINDOW *right;
  WINDOW *bottom;
  WINDOW *slit;
};

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

void initScreen(Winders *winders);
void cleanUpWindow(WINDOW *local_win);
bool getInput(Guess &g, int x, Winders *winders);
void wrongInput(WINDOW *local_win, Guess &g, int inner);
void dispGuesses(WINDOW *local_win, std::vector<int> guesses, int outer);
void dispMarkers(WINDOW *local_win, int marker_arr[], int outer);
bool isWinner(int marker_arr[]);
void youWin(WINDOW *local_win);
void youLose(WINDOW *local_win);
void dispAnswers(WINDOW *local_win, int answer[]);
bool playAgain(WINDOW *local_win);
void closeCurses(Winders *winders);
void printTopScores(WINDOW *local_win);

extern void getScores(int scoreNum, char *name, int &score);
extern int writeScores(int turns);

#endif  // WINDOWS_H
