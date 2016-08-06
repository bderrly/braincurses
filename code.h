/* code.h
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#ifndef CODE_H
#define CODE_H

#include <algorithm>
#include <iterator>
#include <random>
#include <vector>


namespace code {
  const int kNope = 0;
  const int kAlmost = 1;
  const int kNailedIt = 2;
  const int kUsed = -1;
}

// The Code class creates the random code the user is to guess.
class Code {
  public:

    // Code();
    Code(int length);
    Code(int length, unsigned int seed);
    Code(std::vector<int> secret); // For unit testing.

    void Create();
    std::vector<int> Get() const { return code_; };
    int Length() const { return length_; };

    // Where the magic happens.
    //
    // Compare the user-supplied guess provided as iterators to the secret code_.
    // If the color and column match between the guess and the secret code then a
    // code::kNailedIt is added to the return vector. If the color matches but not the
    // column then code::kAlmost, and if there is no match for the guess then code::kNope.
    //
    // The result vector is returned sorted with the closest match at the
    // beginning.
    //
    // Examples:
    // guess = {0, 1, 1, 2};
    // code = {0, 1, 2, 3};
    // result = {code::kNailedIt, code::kNailedIt, code::kAlmost, code::kNope};
    //
    // guess = {0, 1, 1, 2};
    // code = {1, 1, 2, 3};
    // result = {code::kNailedIt, code::kAlmost, code::kAlmost, code::kNope}
    // template <typename Iterator>
    template <class Iterator>
    std::vector<int> IsCorrect(Iterator guess_start, Iterator guess_end) const {
      if (std::equal(code_.begin(), code_.end(), guess_start)) {
        // It's our lucky day!
        std::vector<int> result(4, 2);
        return result;
      }

      std::vector<int> code = code_;
      std::vector<int> guess(guess_start, guess_end);
      std::vector<int> result;

      // First pass to mark all guesses that are fully correct.
      for (unsigned int i = 0; i < guess.size(); i++) {
        if (guess[i] == code[i]) {
          result.push_back(code::kNailedIt);
          guess[i] = code::kUsed;
          code[i] = code::kUsed;
        }
      }

      // Second pass to mark guesses that are the correct color but in the wrong location
      // as well as those that are completely wrong.
      for (int i = 0; i < 8; i++) {
        auto guess_count = std::count(guess.begin(), guess.end(), i);
        auto code_count = std::count(code.begin(), code.end(), i);
        if (guess_count > 0 && code_count > 0) {
          int difference = guess_count - code_count;
          if (difference > 0) {
            std::fill_n(std::back_inserter(result), difference, code::kNope);
            std::fill_n(std::back_inserter(result), code_count, code::kAlmost);
          } else {
            std::fill_n(std::back_inserter(result), guess_count, code::kAlmost);
          }
        }
      }

      // We need to fill the result vector up with code::kNope's to the size expected.
      std::fill_n(std::back_inserter(result), guess.size() - result.size(), code::kNope);

      if (result.size() < 4 || result.size() > 6) {
        // throw an exception?
      }
      std::sort(result.begin(), result.end(), std::greater<int>());
      return result;
    }

  private:
    int GetRandomNumber();

    std::vector<int> code_;
    std::uniform_int_distribution<int> dist_;
    std::default_random_engine engine_;
    int length_;
};

#endif  // CODE_H
