/* code_test.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"
#include "test/littletest.hpp"

LT_BEGIN_SUITE(code)

	Code *code;

	void set_up() {
		code = new Code(0);
		code->createCode();
	}
	void tear_down() {
		delete code;
	}
LT_END_SUITE(code)

LT_BEGIN_AUTO_TEST(code, goodGuess)
	std::vector<int> guess = {2, 5, 4, 2};
	std::vector<int> expected = {2, 2, 2, 2};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(goodGuess)

LT_BEGIN_AUTO_TEST(code, badGuess)
	std::vector<int> guess = {0, 0, 0, 0};
	std::vector<int> expected = {0, 0, 0, 0};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(badGuess)

LT_BEGIN_AUTO_TEST(code, oneCorrect)
	std::vector<int> guess = {2, 3, 1, 0};
	std::vector<int> expected = {2, 0, 0, 0};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(oneCorrect)

LT_BEGIN_AUTO_TEST(code, oneCorrectOnePartial)
	std::vector<int> guess = {2, 4, 1, 0};
	std::vector<int> expected = {2, 1, 0, 0};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(oneCorrectOnePartial)

LT_BEGIN_AUTO_TEST(code, twoCorrectOnePartial)
	std::vector<int> guess = {2, 4, 1, 2};
	std::vector<int> expected = {2, 1, 0, 2};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(twoCorrectOnePartial)

LT_BEGIN_AUTO_TEST(code, allPartial)
	std::vector<int> guess = {4, 2, 2, 5};
	std::vector<int> expected = {1, 1, 1, 1};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(allPartial)

LT_BEGIN_AUTO_TEST(code, fuzzyInput)
	std::vector<int> guess = {0, -1, 10293928, -2};
	std::vector<int> expected = {0, 0, 0, 0};
	std::vector<int> reply = code->isCorrect(guess);
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(fuzzyInput)

LT_BEGIN_AUTO_TEST_ENV()
	AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
