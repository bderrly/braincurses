/* braincurses.h
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#ifndef BRAINCURSES_H
#define BRAINCURSES_H

#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "code.h"

const int INPUT_LENGTH = 7;
const std::string GAME_NAME = "BrainCurses";

const std::string kCodeWindow = "code";
const std::string kGuessWindow = "guess";
const std::string kHeaderWindow = "header";
const std::string kInfoWindow = "info";
const std::string kInputWindow = "input";
const std::string kMarkerWindow = "marker";
const std::string kWatermarkWindow = "watermark";


class Braincurses {
  public:
    Braincurses(int code_length, int guesses);
    bool GameOverPlayAgain(bool winner);
    bool Initialized() const { return initialized_; };
    bool PlayGame(const Code& code);

  private:
    void CleanUpWindow(WINDOW* window);
    WINDOW *CreateWindow(int height, int width, int starty, int startx);
    void DisplayCode(const Code& code, bool colored);
    void DisplayGuess(int y, std::vector<int> guess);
    void DisplayMarkers(int y, std::vector<int> correct);
    std::vector<int> GetInput();
    void InitializeNcurses();
    bool IsWinner(std::vector<int> guess);
    bool PlayAgain();
    void PrepareBoard(const Code& code);
    void WipeBoard();

    int code_length_;
    int guesses_;
    bool initialized_;
    std::unordered_map<std::string, WINDOW*> windows_;
};

void endwin_handler(void);

#endif  // BRAINCURSES_H
