/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"


Code::Code(int length) : Code(length, std::random_device{}()) {}

Code::Code(int length, unsigned int seed) :
    dist_(std::uniform_int_distribution<>(1, 6)), engine_(std::default_random_engine(seed)),
    length_(length) {}

Code::Code(std::vector<int> secret) : code_(secret), length_(secret.size()) {}

// void Code::initialize(unsigned int seed) {
//   engine_ = std::default_random_engine (seed);
//   dist_ = std::uniform_int_distribution<> (1, 6);
//   initialized_ = true;
// }

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
