#include "code.h"
#include "test/littletest.hpp"

LT_BEGIN_SUITE(code)
	void set_up() {}
	void tear_down() {}
LT_END_SUITE(code)

LT_BEGIN_AUTO_TEST(code, goodGuess)
	Code *code = new Code(0);
	code->createCode();
	int correctGuess[] = {2, 5, 4, 2};
	int realCode[] = {2, 2, 2, 2};
	std::vector<int> expected = 
			std::vector<int>(realCode, realCode + sizeof(realCode) / sizeof(int));

	std::vector<int> reply = code->isCorrect(correctGuess);
	delete code;
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(goodGuess)

LT_BEGIN_AUTO_TEST(code, badGuess)
	Code *code = new Code(0);
	code->createCode();
	int wrongGuess[] = {0, 0, 0, 0};
	int realCode[] = {0, 0, 0, 0};
	std::vector<int> expected = 
			std::vector<int>(realCode, realCode + sizeof(realCode) / sizeof(int));

	std::vector<int> reply = code->isCorrect(wrongGuess);
	delete code;
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(badGuess)

LT_BEGIN_AUTO_TEST(code, oneCorrect)
	Code *code = new Code(0);
	code->createCode();
	int wrongGuess[] = {2, 3, 1, 0};
	int realCode[] = {2, 0, 0, 0};
	std::vector<int> expected = 
			std::vector<int>(realCode, realCode + sizeof(realCode) / sizeof(int));

	std::vector<int> reply = code->isCorrect(wrongGuess);
	delete code;
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(oneCorrect)

LT_BEGIN_AUTO_TEST(code, oneCorrectOnePartial)
	Code *code = new Code(0);
	code->createCode();
	int wrongGuess[] = {2, 4, 1, 0};
	int realCode[] = {2, 1, 0, 0};
	std::vector<int> expected = 
			std::vector<int>(realCode, realCode + sizeof(realCode) / sizeof(int));

	std::vector<int> reply = code->isCorrect(wrongGuess);
	delete code;
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(oneCorrectOnePartial)

LT_BEGIN_AUTO_TEST(code, allOnes)
	Code *code = new Code(0);
	code->createCode();
	int wrongGuess[] = {4, 2, 2, 5};
	int realCode[] = {1, 1, 1, 1};
	std::vector<int> expected = 
			std::vector<int>(realCode, realCode + sizeof(realCode) / sizeof(int));

	std::vector<int> reply = code->isCorrect(wrongGuess);
	delete code;
	LT_ASSERT_COLLECTIONS_EQ(expected.begin(), expected.end(), reply.begin())
LT_END_AUTO_TEST(allOnes)


LT_BEGIN_AUTO_TEST_ENV()
	AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
