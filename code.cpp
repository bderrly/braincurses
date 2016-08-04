/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"

// I learned that you must actually declare AND initialize static member variables.
// Assigning values in the header file is not enough. Not defining these here results
// in linker errors like "undefined reference to Code::NOPE".
// http://stackoverflow.com/questions/5603101
const int Code::NOPE = 0;
const int Code::ALMOST = 1;
const int Code::NAILED_IT = 2;
const int Code::USED = -1;

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
