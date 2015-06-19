/* braincurses.cpp
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#include "braincurses.h"

using namespace std;

Answer::Answer() { getRdmNums(); }

Answer::~Answer() {}

void Answer::getRdmNums() {
  static bool ran = false;
  int i;
  if (ran == false) {
    srand(time(0) * getpid());
    ran = true;
  }

  for (i = 0; i < 4; i++)
    answer[i] = 1 + (int)(6.0 * rand() / (RAND_MAX + 1.0));
}

int Answer::grabAnswer(int x) { return (answer[x]); }

Guess::Guess() {
  // Initialize the variables
  int i;
  for (i = 0; i < 4; i++) {
    guess[i] = 0;
    markers[i] = 0;
  }
}

Guess::~Guess() {}

bool Guess::isValid(string tmp) {
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

void Guess::setInput(string str, int tracker) {
  if (str == "red")
    guess[tracker] = RED;
  else if (str == "white")
    guess[tracker] = WHITE;
  else if (str == "yellow")
    guess[tracker] = YELLOW;
  else if (str == "green")
    guess[tracker] = GREEN;
  else if (str == "blue")
    guess[tracker] = BLUE;
  else if (str == "purple")
    guess[tracker] = PURPLE;
  else {
    cerr << "braincurses: incorrect input" << endl;
    exit(1);
  }
}

void Guess::compareWithAnswer(Answer ans) {
  // Assertion:  You will call this function with your current Answer object
  // for it to correctly compare against user inputed guesses

  int bMarker = 0, wMarker = 0;
  // bMarker indicates that the guess is the correct color and placement
  // wMarker indicates that a guess is correct color but not placement

  int i;

  for (i = 0; i < 4; i++) {
    if (guess[i] == ans.grabAnswer(i)) {
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
    guess_num[guess[i]]++;
    ans_num[ans.grabAnswer(i)]++;
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
  int i;
  for (i = 0; i < 4; i++) array[i] = markers[i];
}

void Guess::showGuesses(int array[]) {
  int i;
  for (i = 0; i < 4; i++) array[i] = guess[i];
}

void Guess::quitGame() {
  system("clear");
  cout << "Bye, thanks for playing." << endl;
  exit(0);
}
