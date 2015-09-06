/* braincurses.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include <iostream>
#include <unistd.h>

#include "code.h"
#include "windows.h"


const int DEFAULT_NUM_GUESSES = 10;

void printUsage(std::string binary) {
  std::cerr << "usage: " << binary << " [-c code length] [-g number of guesses]" << std::endl;
}

int main(int argc, char *argv[]) {
  extern const int DEFAULT_CODE_LENGTH;
  int codeLength = DEFAULT_CODE_LENGTH;
  int guesses = DEFAULT_NUM_GUESSES;
  int opt;
  while ((opt = getopt(argc, argv, "g:hc:")) != -1) {
    switch (opt) {
      case 'c':
        codeLength = atoi(optarg);
        break;
      case 'g':
        guesses = atoi(optarg);
        break;
      case 'h':
      default:
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (codeLength > 6) {
    codeLength = 6;
  }

  if (codeLength < 4) {
    codeLength = 4;
  }

  if (guesses > 15) {
    guesses = 15;
  }

  if (guesses <= 0) {
    guesses = DEFAULT_NUM_GUESSES;
  }

  Windows windows;
  if (!initScreen(windows)) {
    std::cerr << argv[0] << ": Your terminal cannot display colors." << std::endl;
    exit(EXIT_FAILURE);
  }

  bool winner = false;

#ifdef DEBUG
  Code code(0);
#else
  Code code;
#endif
  code.setCodeLength(codeLength);

  do {
    code.createCode();

    wipeGameBoard(windows);
    prepareGameBoard(windows, guesses, code);
    winner = playGame(windows, code, guesses);
    displayCode(windows["code"], code, true);
  } while (gameOverPlayAgain(windows["input"], winner));

  return 0;
}
