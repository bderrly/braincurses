/* windows.h
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#ifndef WINDOWS_H
#define WINDOWS_H

#include "../code.h"

#include <string>
#include <vector>

#include <ncurses.h>

const int INPUT_LENGTH = 7;
const std::string AUTHOR = "Brian Derr";
const std::string COPYRIGHT = "(c) 2002";
const std::string GAME_NAME = "BrainCurses";

enum Colors {RED, WHITE, YELLOW, GREEN, BLUE, PURPLE};

struct Windows {
  WINDOW *top_left;
  WINDOW *top_right;
  WINDOW *left;
  WINDOW *middle;
  WINDOW *right;
  WINDOW *bottom;
  WINDOW *slit;
};

void cleanUpWindow(WINDOW *window);
void closeCurses();
WINDOW *createWindow(int height, int width, int starty, int startx);
void destroyWindow(WINDOW *window);
void displayAnswer(WINDOW *window, std::vector<int> code);
void displayGuess(WINDOW *window, int y, std::vector<int> guess);
void displayMarkers(WINDOW *window, int y, std::vector<int> markers);
bool gameOverPlayAgain(WINDOW *window, bool winner);
std::vector<int> getInput(WINDOW *window);
bool initScreen(Windows *windows);
bool isWinner(std::vector<int> guess);
bool playAgain(WINDOW *window);
void playGame(Windows *windows, int maxGuesses);
void prepareGameBoard(Windows *windows, int maxGuesses);

#endif  // WINDOWS_H
