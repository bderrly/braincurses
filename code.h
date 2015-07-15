/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <map>
#include <vector>


const int DEFAULT_CODE_LENGTH = 4;

class Code {
  private:
    std::vector<int> code;
    int codeLength;
    std::map<int,int> digits;
    int getRandomNumber();

  public:
    Code();
    Code(int seed);
    ~Code();

    void createCode();
    std::vector<int> getCode();
    int getCodeLength();
    std::vector<int> isCorrect(std::vector<int> guess);
    void setCodeLength(int codeLength);
};

#endif
