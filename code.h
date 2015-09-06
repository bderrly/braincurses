/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <random>
#include <vector>


const int DEFAULT_CODE_LENGTH = 4;

class Code {
  private:
    std::vector<int> code;
    int codeLength;
    std::uniform_int_distribution<int> dist;
    std::default_random_engine engine;
    bool initialized;
    int seed;

    int getRandomNumber();

  public:
    Code();
    Code(int seed);
    Code(std::vector<int> seed);

    void createCode();
    std::vector<int> getCode();
    int getCodeLength();
    void initialize();
    std::vector<int> isCorrect(std::vector<int> guess);
    void setCodeLength(int codeLength);
};

#endif
