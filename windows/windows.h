/* windows.h
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#ifndef WINDOWS_H
#define WINDOWS_H

#include "../code.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <ncurses.h>

const int INPUT_LENGTH = 7;
const std::string AUTHOR = "Brian Derr";
const std::string COPYRIGHT = "(c) 2002";
const std::string GAME_NAME = "BrainCurses";

typedef std::unordered_map<std::string, WINDOW*> Windows;

void cleanUpWindow(WINDOW *window, bool erase);
void closeCurses();
WINDOW *createWindow(int height, int width, int starty, int startx);
void destroyWindow(WINDOW *window);
void displayCode(WINDOW *window, std::vector<int> code, bool colored);
void displayGuess(WINDOW *window, int y, std::vector<int> guess);
void displayMarkers(WINDOW *window, int y, std::vector<int> markers);
bool gameOverPlayAgain(WINDOW *window, bool winner);
std::vector<int> getInput(WINDOW *window);
bool initScreen(Windows &windows);
bool isWinner(std::vector<int> guess);
bool playAgain(WINDOW *window);
bool playGame(Windows &windows, Code code, int maxGuesses);
void prepareGameBoard(Windows &windows, int maxGuesses);
void wipeGameBoard(Windows &windows);

#endif
