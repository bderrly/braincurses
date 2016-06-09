/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <random>
#include <vector>


// The Code object creates the random code the user is to guess.
class Code {
  public:
    // Code(const Code&) = delete;
    Code(int length);
    Code(int length, int seed);
    // For unit testing.
    Code(int length, std::vector<int> secret);
    void Create();
    int Length() const { return length_; };
    std::vector<int> Get() const { return code_; };
    std::vector<int> IsCorrect(std::vector<int> &guess) const;
    void SetLength(int length);

  private:
    int GetRandomNumber();
    void initialize(unsigned int seed);

    std::vector<int> code_;
    std::uniform_int_distribution<int> dist_;
    std::default_random_engine engine_;
    bool initialized_;
    int length_;
    std::random_device rd_;
};

#endif
