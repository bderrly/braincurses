/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <map>
#include <vector>

enum Colors { RED, WHITE, YELLOW, GREEN, BLUE, PURPLE };

class Code {
 private:
  int code[4];
	std::map<int,int> digits;
	int getRandomNumber();

 public:
  Code();
	Code(int seed);
  ~Code();

	void createCode();
	std::vector<int> isCorrect(const int []);
};

#endif  // CODE_H
