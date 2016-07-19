#!/usr/bin/python
#
# Yes, this is the same alorithm used in code.cpp; I've spot checked a number
# of entries and they were all correct. I should maybe come up with a better
# way to prove this is correct.

import json
import itertools
import sys


def iscorrect(guess):
    secret = (0, 1, 2, 3)
    skip = [0, 0, 0, 0]
    correct = [0, 0, 0, 0]

    for i in range(4):
        if secret[i] == guess[i]:
            if skip[i] != 1:
                correct[i] = 2
                skip[i] = 1
                continue
        for j in range(4):
            if i != j and secret[i] == guess[j]:
                if skip[j] != 1:
                    correct[j] = 1
                    skip[j] = 1
                    break

    return (guess, sorted(tuple(correct), reverse=True))


def main():
    product = itertools.product(range(5), repeat=4)
    answers = map(iscorrect, product)
    json.dump(tuple(answers), sys.stdout)

if __name__ == '__main__':
    main()

