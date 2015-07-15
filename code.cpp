/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

Code::Code() {
  Code(time(NULL));
}

Code::Code(int seed) {
  srand(seed);
  codeLength = DEFAULT_CODE_LENGTH;
  for (int i = 0; i < 6; i++) {
    digits[i] = 0;
  }
}

Code::~Code() {}

int Code::getRandomNumber() {
  return rand() % 6 + 1;
}

void Code::createCode() {
  if (code.size() > 0) {
    code.clear();
  }
  for (int i = 0; i < codeLength; i++) {
    code.push_back(getRandomNumber());
    digits[code[i]]++;
  }
}

std::vector<int> Code::getCode() {
  return code;
}

int Code::getCodeLength() {
  return codeLength;
}

// Each index will have either 0, 1, or 2.
// 0 == not a match
// 1 == correct color
// 2 == correct color and column
std::vector<int> Code::isCorrect(std::vector<int> guess) {
  std::vector<int> correct (guess.size(), 0);

  unsigned i, j;
  std::map<int,int> digitsUsed;
  for (i = 0; i < guess.size(); i++) {
    digitsUsed[i] = 0;
  }

  for (i = 0; i < guess.size(); i++) {
    if (guess[i] == code[i]) {
      correct[i] = 2;
      digitsUsed[code[i]]++;
    }
  }

  for (i = 0; i < guess.size(); i++) {
    for (j = 0; j < guess.size(); j++) {
      if (correct[i] == 0) {
        if (guess[i] == code[j] && digitsUsed[code[j]] < digits[code[j]]) {
          correct[i] = 1;
          digitsUsed[code[j]]++;
        }
      }
    }
  }
  return correct;
}

void Code::setCodeLength(int codeLength) {
  this->codeLength = codeLength;
}
