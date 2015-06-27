/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

Code::Code() {
	Code(time(NULL));
}

Code::Code(int seed) {
	srand(seed);
	for (int i = 0; i < 6; i++) {
		digits[i] = 0;
	}
}

Code::~Code() {}

int Code::getRandomNumber() {
	return rand() % 6 + 1;
}

void Code::createCode() {
	for (int i = 0; i < 4; i++) {
		code[i] = getRandomNumber();
		digits[code[i]]++;
	}
}

std::vector<int> Code::getCode() {
	return code;
}

// Each index will have either 0, 1, or 2.
// 0 == not a match
// 1 == correct color
// 2 == correct color and column
std::vector<int> Code::isCorrect(std::vector<int> guess) {
	std::vector<int> correct = std::vector<int>(4, 0);

	std::map<int,int> digitsUsed;
	for (int i = 0; i < 6; i++) {
		digitsUsed[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		if (guess[i] == code[i]) {
			correct[i] = 2;
			digitsUsed[code[i]]++;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (correct[i] == 0) {
				if (guess[i] == code[j] && digitsUsed[code[j]] < digits[code[j]]) {
					correct[i] = 1;
					digitsUsed[code[j]]++;
				}
			}
		}
	}
	return correct;
}
