# BrainCurses

A simple code-breaking game.

The object of the game is to guess the correct colors in the correct order.
[Wikipedia][] has more details about game play, the history of the game, and
Donald Knuth's algorithm for solving in under five steps.


## Game play

### Guessing

To enter a guess, for example, the color red, type 'r' which will be completed
to 'red'. The input cursor will be advanced to the next field. Continue until
you have selected four colors. The available colors are displayed in the
information panel on the right.

### Scoring

After the final color has been entered, the input window will clear and some
markers will be displayed on the left window. A red 'X' signifies one of your
selections was both the correct color and the correct position; whereas a white
'X' signifies only the correct color.

*Note*: the markers are filled from the left starting with any red markers
followed by white. This means that a red 'X' in the first column does not
necessarily mean that your guess in the first column was the correct color and
position.

You have, by default, ten guesses before the secret code will be revealed. If
you have not already guessed the secret code after the tenth guess you lose the
game.


## Installation

First you need to compile the binary. The prerequisites are:

   - Ncurses
   - make
   - a C++ compiler

`make`

The game works the best with a dark background and white foreground. If you're
using a custom color scheme, e.g., solarized, the colors may not be easy to
distinguish.


[WikiPedia]: https://en.wikipedia.org/wiki/Mastermind_(board_game)
	"Mastermind"
