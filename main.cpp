/* main.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "braincurses.h"


const int kMinCodeLength = 4;
const int kMaxCodeLength = 6;

const int kDefaultGuessCount = 10;
const int kMinGuessCount = 1;
const int kMaxGuessCount = 15;


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

  if (code_length > kMaxCodeLength) {
    code_length = kMaxCodeLength;
  } else if (code_length < kMinCodeLength) {
    code_length = kMinCodeLength;
  }

  if (guesses > kMaxGuessCount) {
    guesses = kMaxGuessCount;
  } else if (guesses <= 0) {
    guesses = kDefaultGuessCount;
  }
}

void endwin_atexit_handler() {
  endwin();
}

void endwin_signal_handler(int signal) {
  endwin();
  std::exit(EXIT_SUCCESS);
};

int main(int argc, char* argv[]) {
  int code_length = kMinCodeLength;
  int guesses = kDefaultGuessCount;
  ProcessArgs(argc, argv, code_length, guesses);

  Braincurses bc(code_length, guesses);

  std::signal(SIGINT, endwin_signal_handler);

  int result = std::atexit(endwin_atexit_handler);
  if (result != 0) {
    std::cerr << "atexit registration failed" << std::endl;
    return EXIT_FAILURE;
  }

  bool winner = false;
  do {
    winner = bc.PlayGame();
  } while (bc.GameOverPlayAgain(winner));

  return EXIT_SUCCESS;
}
