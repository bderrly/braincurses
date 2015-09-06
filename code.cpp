/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"

#include <string>
#include <random>

Code::Code() {
  initialized = false;
  std::random_device rd;
  seed = rd();
  codeLength = DEFAULT_CODE_LENGTH;
}

Code::Code(int seed) : Code() {
  this->seed = seed;
}

// This is intended solely for unit testing.
Code::Code(std::vector<int> code) {
  this->code = code;
  codeLength = (int) code.size();
}

void Code::initialize() {
  this->engine = std::default_random_engine (seed);
  this->dist = std::uniform_int_distribution<> (1, 6);
  initialized = true;
}

int Code::getRandomNumber() {
  return dist(engine);
}

void Code::createCode() {
  if (!initialized) initialize();
  if (code.size() > 0) {
    code.clear();
  }
  for (int i = 0; i < codeLength; i++) {
    code.push_back(getRandomNumber());
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
  std::vector<int> correct(codeLength, 0);
  std::vector<int> skip(codeLength, 0);

  int i, j;
  for (i = 0; i < codeLength; ++i) {
    if (code[i] == guess[i] && skip[i] != 1) {
      correct[i] = 2;
      skip[i] = 1;
      continue;
    }
    for (j = 0; j < codeLength; ++j) {
      if (code[i] == guess[j] && i != j && skip[j] != 1) {
        correct[j] = 1;
        skip[j] = 1;
        break;
      }
    }
  }
  return correct;
}

void Code::setCodeLength(int codeLength) {
  this->codeLength = codeLength;
}
