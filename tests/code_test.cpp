#include <bandit/bandit.h>
#include <json.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "../code.h"

using namespace bandit;
using json = nlohmann::json;


// template <class Iterator>
// std::stringstream printVector(Iterator start, Iterator end) {
//   std::stringstream ss;
//   for (auto i = start; i != end; ++i) {
//     ss << *i << ", ";
//   }
//   return ss;
// }


go_bandit([](){
  describe("length", [](){
    Code code(4);

    it("should match constructor initializer", [&](){
        AssertThat(code.Length(), Equals(4));
    });

    it("setting works", [&](){
        code.SetLength(5);
        AssertThat(code.Length(), Equals(5));
    });
  });

  describe("guesses", [](){
    std::vector<int> secret = {0, 1, 2, 3};
    Code code(4, secret);
    std::ifstream test_table("test_table");
    json tests(test_table);
    test_table.close();

    it("length still good", [&](){
        AssertThat(code.Length(), Equals(4));
    });

    it("work", [&](){
      for (auto &test : tests) {
        json::iterator begin = test[0].begin();
        json::iterator end = test[0].end();
        std::vector<int> expected = test[1];
        std::vector<int> ret = code.IsCorrect<json::iterator>(begin, end);
        try {
          AssertThat(ret, EqualsContainer(expected));
        } catch (AssertionException &ex) {
          std::cout << ex.GetMessage() << std::endl;
          // std::cout << "guess: " << printVector(test[0].begin(), test[0].end()).str()
          //     << "expected: " << printVector(test[1].begin(), test[1].end()).str()
          //     << "actual: " << printVector(ret.begin(), ret.end()).str() << std::endl;
        }
      }
    });
  });
});


