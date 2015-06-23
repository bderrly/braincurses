/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <vector>

enum Colors { RED, WHITE, YELLOW, GREEN, BLUE, PURPLE };

class Code {
 private:
  int code[4];
	int getRandomNumber();

 public:
  Code();
  ~Code();

	void createCode();
	std::vector<int> isCorrect(std:vector<int> guesses);
};

#endif  // CODE_H
