/* braincurses.cpp
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#include "braincurses.h"

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>

int getRandomNumber() {
	// TODO(brian): Move seed to main().
  static bool ran = false;
  if (!ran) {
    srand(time(NULL));
    ran = true;
  }
	return rand() % 6;
}

Guess::Guess() {
  for (int i = 0; i < 4; i++) {
    guesses[i] = 0;
    markers[i] = 0;
  }
}

Guess::~Guess() {}

bool Guess::isValid(std::string tmp) {
  bool done = false;
  static int tracker = 0;
  if (tracker == 4) tracker = 0;
  if (tmp == "red" || tmp == "r") {
    setInput("red", tracker);
    tracker++;
    done = true;
  } else if (tmp == "white" || tmp == "w") {
    setInput("white", tracker);
    tracker++;
    done = true;
  } else if (tmp == "yellow" || tmp == "y") {
    setInput("yellow", tracker);
    tracker++;
    done = true;
  } else if (tmp == "green" || tmp == "g") {
    setInput("green", tracker);
    tracker++;
    done = true;
  } else if (tmp == "blue" || tmp == "b") {
    setInput("blue", tracker);
    tracker++;
    done = true;
  } else if (tmp == "purple" || tmp == "p") {
    setInput("purple", tracker);
    tracker++;
    done = true;
  } else if (tmp == "quit") {
    quitGame();
  } else
    done = false;

  return (done);
}

void Guess::setInput(std::string str, int tracker) {
  if (str == "red")
    guesses[tracker] = RED;
  else if (str == "white")
    guesses[tracker] = WHITE;
  else if (str == "yellow")
    guesses[tracker] = YELLOW;
  else if (str == "green")
    guesses[tracker] = GREEN;
  else if (str == "blue")
    guesses[tracker] = BLUE;
  else if (str == "purple")
    guesses[tracker] = PURPLE;
  else {
    std::cerr << "braincurses: incorrect input" << std::endl;
    exit(1);
  }
}

void Guess::compareWithAnswer(int answer[]) {
  int bMarker = 0, wMarker = 0;
  // bMarker indicates that the guess is the correct color and placement
  // wMarker indicates that a guess is correct color but not placement

  int i;

  for (i = 0; i < 4; i++) {
    if (guesses[i] == answer[i]) {
      bMarker++;
    }
  }

  int num_len = 10;
  int guess_num[num_len];
  int ans_num[num_len];

  for (i = 0; i < num_len; i++) {
    guess_num[i] = 0;
    ans_num[i] = 0;
  }

  for (i = 0; i < 4; i++) {
    guess_num[guesses[i]]++;
    ans_num[answer[i]]++;
  }

  for (i = 0; i < num_len; i++) {
    if (ans_num[i] <= guess_num[i])
      wMarker += ans_num[i];
    else
      wMarker += guess_num[i];
  }

  wMarker -= bMarker;
  if (wMarker < 0) wMarker = 0;
  // my thinking here is that there will be bMarker more wMarkers than
  // needed since a bMarker is inherently a wMarker

  setMarkers(bMarker, wMarker);
}

void Guess::setMarkers(int bMarker, int wMarker) {
  int i;

  if (bMarker > 0)
    for (i = 0; i < bMarker; i++) markers[i] = 0;

  if (wMarker > 0)
    for (i = bMarker; i < wMarker + bMarker; i++) markers[i] = 1;

  int count = bMarker + wMarker;

  if (count == 0)
    for (i = count; i < 4; i++) markers[i] = 3;
  else
    for (i = count; i < 4; i++) markers[i] = 3;
}

void Guess::showMarkers(int array[]) {
  for (int i = 0; i < 4; i++) {
	 	array[i] = markers[i];
	}
}

std::vector<int> Guess::getGuesses() {
	return guesses;
}

void Guess::quitGame() {
  std::cout << "Bye, thanks for playing." << std::endl;
  exit(0);
}
