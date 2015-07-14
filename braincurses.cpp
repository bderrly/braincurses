/* braincurses.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include <iostream>

#include "code.h"
#include "windows.h"

#define DEFAULT_NUM_GUESSES 10

int main(int argv, char *argc[]) {
  int maxGuesses = (argc > 1) ? atoi(argv[1]) : DEFAULT_NUM_GUESSES;
  if (maxGuesses <= 0) maxGuesses = DEFAULT_NUM_GUESSES;
  if (maxGuesses > 15) maxGuesses = 15;

	Windows windows;
	if (!initScreen(windows)) {
    std::cerr << argv[0] << ": Your terminal cannot display colors." << std::endl;
    exit(1);
	}

	bool winner = false;

	Code code;
	do {
		code.createCode();

		wipeGameBoard(windows);
		prepareGameBoard(windows, maxGuesses, code);
		winner = playGame(windows, code, maxGuesses);
		displayCode(windows["code"], code.getCode(), true);
	} while (gameOverPlayAgain(windows["input"], winner));

  return 0;
}
