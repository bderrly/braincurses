#include <bandit/bandit.h>
#include <vector>

#include "../code.h"

using namespace bandit;

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

      it("work", [&](){
          std::vector<int> expected = {2, 2, 2, 2};
          AssertThat(code.IsCorrect(secret), EqualsContainer(expected));
          });
      });
});
