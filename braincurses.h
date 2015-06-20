/* braincurses.h
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#ifndef BRAINCURSES_H
#define BRAINCURSES_H

#include <string>
#include <vector>

enum Colors { RED, WHITE, YELLOW, GREEN, BLUE, PURPLE };

class Guess {
 private:
  std::vector<int> guesses;
	std::vector<int> markers;  // used to keep track if a guess is in the correct place
                   // 0 = black, 1 = white, 3 = none
  void setInput(std::string str, int tracker);
  void setMarkers(int bMarker, int wMarker);

 public:
  Guess();
  Guess(int guess1, int guess2, int guess3, int guess4);
  ~Guess();

  bool isValid(std::string tmp);
  void compareWithAnswer(int answer[]);
  void showMarkers(int array[]);
	std::vector<int> getGuesses();
  void quitGame();
};

#endif  // BRAINCURSES_H
