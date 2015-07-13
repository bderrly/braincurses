#include "code.h"
#include "test/littletest.hpp"

LT_BEGIN_SUITE(code)
	void set_up() {}
	void tear_down() {}
LT_END_SUITE(code)

LT_BEGIN_AUTO_TEST(code, goodGuess)
	Code code (0);
	code.createCode();
	std::vector<int> correctGuess = {2, 5, 4, 2};
	std::vector<int> expected = {2, 2, 2, 2};
	std::vector<int> reply = code.isCorrect(correctGuess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(goodGuess)

LT_BEGIN_AUTO_TEST(code, badGuess)
	Code code (0);
	code.createCode();
	std::vector<int> wrongGuess = {0, 0, 0, 0};
	std::vector<int> expected = {0, 0, 0, 0};
	std::vector<int> reply = code.isCorrect(wrongGuess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(badGuess)

LT_BEGIN_AUTO_TEST(code, oneCorrect)
	Code code (0);
	code.createCode();
	std::vector<int> wrongGuess = {2, 3, 1, 0};
	std::vector<int> expected = {2, 0, 0, 0};
	std::vector<int> reply = code.isCorrect(wrongGuess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(oneCorrect)

LT_BEGIN_AUTO_TEST(code, oneCorrectOnePartial)
	Code code (0);
	code.createCode();
	std::vector<int> wrongGuess = {2, 4, 1, 0};
	std::vector<int> expected = {2, 1, 0, 0};
	std::vector<int> reply = code.isCorrect(wrongGuess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(oneCorrectOnePartial)

LT_BEGIN_AUTO_TEST(code, allOnes)
	Code code (0);
	code.createCode();
	std::vector<int> wrongGuess = {4, 2, 2, 5};
	std::vector<int> expected = {1, 1, 1, 1};
	std::vector<int> reply = code.isCorrect(wrongGuess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(allOnes)

LT_BEGIN_AUTO_TEST_ENV()
	AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
