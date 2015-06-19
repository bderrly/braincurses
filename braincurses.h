/* braincurses.h
 * Copyright © 2002, Brian Derr <brian@derrclan.com>
 */

#ifndef BRAINCURSES_H
#define BRAINCURSES_H

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

using namespace std;

enum Colors { NONE, RED, WHITE, YELLOW, GREEN, BLUE, PURPLE };

class Answer
{
   private:
      int answer[4];
      void getRdmNums();

   protected:

   public:
      Answer();
     ~Answer();

      int grabAnswer( int x );
};

class Guess
{
   private:
      int guess[4];
      int markers[4]; // used to keep track if a guess is in the correct place
                      // 0 = black, 1 = white, 3 = none
      typedef map<int,string> mGuess;
      void setInput( string str, int tracker );
      void setMarkers( int bMarker, int wMarker );

   protected:
      
   public:
      Guess();
      Guess( int guess1, int guess2, int guess3, int guess4 );
     ~Guess();
      
      bool isValid( string tmp );
      void compareWithAnswer( Answer ans );
      void showMarkers( int array[] );
      void showGuesses( int array[] );
      void quitGame();

      friend class Answer;
};


#endif // BRAINCURSES_H
