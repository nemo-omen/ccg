// CS 4318, spring 2023
// Agent Challenge 4: Cricket card game
//
// Here are the #includes and definitions available to each agent.

#ifndef CCG_H
#define CCG_H

#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const int numBallsPerMatch = 1080;
const int numCardsPerHand = 6;
const int numCardsPerSuit = 11;
const int numSuits = 5;
const int playExhibitionMatches = true;
const int pointsForDraw = 1;
const int pointsForLoss = 0;
const int pointsForTie = 3;
const int pointsForWin = 2 * pointsForTie;

// Type to keep track of card suits.
enum cardSuit {pads, gloves, bats, balls, stumps};

// Type to keep track of the result of one cricket match.
enum matchResult {aWin, bWin, drawnMatch, tiedMatch, unfinished};

// Keeps track of one card used in the game.
class Card
{
private:
   int number;
   cardSuit suit;
   static int randomInt(int n);
public:
   Card() {randomize();}
   int getNumber() const {return number;}
   cardSuit getSuit() const {return suit;}
   void randomize() {number = randomInt(numCardsPerSuit) + 1; suit = static_cast<cardSuit>(randomInt(numSuits));}
   string toString() const;
};

// Keeps track of one player's hand of cards.
class Hand
{
private:
   Card cards[numCardsPerHand];
public:
   Hand();
   Card getCard(int which) const {return cards[which >= 0 && which < numCardsPerHand ? which : 0];}
   void randomizeCard(int which) {cards[which >= 0 && which < numCardsPerHand ? which : 0].randomize();}
   string toString() const;
};

// Keeps track of basic cricket statistics.
struct CricketStats
{
   int runs;
   int wickets;
   int balls;
   CricketStats() {runs = 0; wickets = 0; balls = 0;}
};

// Keeps track of one match.
class MatchState
{
private:
   int innings;
   CricketStats stats[2];
   matchResult result;
   void checkForResult();
public:
   MatchState();
   int getBalls(int innings) const {return innings == 0 || innings == 1 ? stats[innings].balls : 0;}
   bool isInFirstInnings() const {return innings == 0;}
   int getResult() const {return result;}
   int getRuns(int innings) const {return innings == 0 || innings == 1 ? stats[innings].runs : 0;}
   int getWickets(int innings) const {return innings == 0 || innings == 1 ? stats[innings].wickets : 0;}
   void scoreRuns(int numRuns);
   bool stillPlaying() const {return result == unfinished;}
   void takeWicket();
};

int numRuns(int cardDifference);

#endif // #ifndef CCG_H
