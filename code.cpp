/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"

#include <stdexcept>


Code::Code(int length) : initialized_(false), length_(length) {
  initialize(rd_());
}

Code::Code(int length, int seed) : initialized_(false), length_(length) {
  initialize((unsigned) seed);
}

Code::Code(int length, std::vector<int> secret) : 
    code_(secret), initialized_(true), length_(length) {}

void Code::initialize(unsigned int seed) {
  engine_ = std::default_random_engine (seed);
  dist_ = std::uniform_int_distribution<> (1, 6);
  initialized_ = true;
}

int Code::GetRandomNumber() {
  return dist_(engine_);
}

// Create a new code that is length_ long.
void Code::Create() {
  if (code_.size() > 0) {
    code_.clear();
  }
  for (int i = 0; i < length_; ++i) {
    code_.push_back(GetRandomNumber());
  }
}

// Each index will have either 0, 1, or 2.
// 0 == not a match
// 1 == correct color
// 2 == correct color and column
std::vector<int> Code::IsCorrect(std::vector<int> &guess) const {
  std::vector<int> correct(code_.size(), 0);
  std::vector<int> skip(code_.size(), 0);

  std::vector<int>::size_type i, j;
  for (i = 0; i != code_.size(); i++) {
    if (code_[i] == guess[i] && skip[i] != 1) {
      correct[i] = 2;
      skip[i] = 1;
      continue;
    }
    for (j = 0; j != code_.size(); j++) {
      if (code_[i] == guess[j] && i != j && skip[j] != 1) {
        correct[j] = 1;
        skip[j] = 1;
        break;
      }
    }
  }
  return correct;
}

void Code::SetLength(int length) {
  if (length > 6 || length < 4) {
    throw std::invalid_argument("length must be in the range [4, 6]");
  }
  length_ = length;
}
