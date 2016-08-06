/* braincurses.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include <iostream>
#include <unistd.h>

#include "code.h"
#include "windows.h"


const int MIN_CODE_LENGTH = 4;
const int MAX_CODE_LENGTH = 6;

const int DEFAULT_NUM_GUESSES = 10;
const int MIN_NUM_GUESSES = 1;
const int MAX_NUM_GUESSES = 15;


void PrintUsage() {
  std::cerr << "usage: braincurses [-c code_length] [-g guesses]" << std::endl;
}

void ProcessArgs(int argc, char* argv[], int& code_length, int& guesses) {
  int opt;
  while ((opt = getopt(argc, argv, "c:g:h")) != -1) {
    switch (opt) {
      case 'c':
        code_length = atoi(optarg);
        break;
      case 'g':
        guesses = atoi(optarg);
        break;
      case 'h':
      default:
        PrintUsage();
        exit(EXIT_FAILURE);
    }
  }

  if (code_length > MAX_CODE_LENGTH) {
    code_length = MAX_CODE_LENGTH;
  } else if (code_length < MIN_CODE_LENGTH) {
    code_length = MIN_CODE_LENGTH;
  }

  if (guesses > MAX_NUM_GUESSES) {
    guesses = MAX_NUM_GUESSES;
  } else if (guesses <= 0) {
    guesses = DEFAULT_NUM_GUESSES;
  }
}

int main(int argc, char* argv[]) {
  int code_length = MIN_CODE_LENGTH;
  int guesses = DEFAULT_NUM_GUESSES;
  ProcessArgs(argc, argv, code_length, guesses);

  Braincurses bc(code_length, guesses);
  if (!bc.Initialized()) {
    std::cerr << argv[0] << ": Your terminal cannot display colors." << std::endl;
    exit(EXIT_FAILURE);
  }

#ifdef DEBUG
  Code code(code_length, (unsigned) 0);
#else
  Code code(code_length);
#endif

  bool winner = false;
  do {
    code.Create();

    winner = bc.PlayGame(code);
  } while (bc.GameOverPlayAgain(winner));

  return 0;
}
