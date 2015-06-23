/* code.cpp
 * Copyright © 2015, Brian Derr <brian@derrclan.com>
 */

#include "code.h"

#include <cstdlib>
#include <time>


Code::Code() {
	srand(time(NULL));
}

Code::~Code() {}

int Code::getRandomNumber() {
	return rand() % 6 + 1;
}

// createCode should be called each time a new secret code is needed.
void Code::createCode() {
	for (int i = 0; i < 4; i++) {
		code[i] = getRandomNumber();
	}
}

// Each index will have either 0, 1, or 2.
// 0 == not a match
// 1 == correct color
// 2 == correct color and column
std::vector<int> Code::isCorrect(const int guesses[]) {
	correct = std::vector<int>(4, 0);
	bool[4] used = {0, 0, 0, 0};

	for (int i = 0; i < 4; i++) {
		if guess[i] == code[i] {
			correct[i] = 2;
			used[i] = true;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (used[i]) {
			continue;
		}
		for (int j = 0; j < 4; j++) {
			if (correct[i] = 0 && !used[j]) {	
				if (guess[i] == code[j]) {
					correct[i] = 1;
					used[j] = true;
				}
			}
		}
	}
	return correct;
}
