/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <map>
#include <vector>

class Code {
 private:
	std::vector<int> code;
	std::map<int,int> digits;
	int getRandomNumber();

 public:
  Code();
	Code(int seed);
  ~Code();

	void createCode();
	std::vector<int> getCode();
	std::vector<int> isCorrect(std::vector<int> guess);
};

#endif  // CODE_H
