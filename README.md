# BrainCurses

A simple code-breaking game.

The object of the game is to guess the correct colors in the correct order.
[Wikipedia][] has more details about game play, the history of the game, and
Donald Knuth's algorithm for solving in under five steps.

## Game play

The interface is rather simple to master. Simply type the first letter of the
color you would like to guess for each position.  For example, I want to guess
the color red first. I will type 'r' which will be completed to 'red' and the
input cursor advanced to the next field.  Continue until you have selected four
colors. The available colors are displayed in the information panel on the
right.

After the fourth color has been selected the input window will clear and some
markers will be displayed on the left window. A red 'X' signifies one of your
selections was both the correct color and the correct position; whereas a white
'X' signifies only the correct color. Be advised, the markers are filled from
the left starting with any reds and then whites. This means that a red 'X' in
the first column does not necessarily mean that your guess in the first column
was the correct color and position.

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
