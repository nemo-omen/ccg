// CS 4318, spring 2023
// Agent Challenge 4: Cricket card game
//
// Here are the functions available to each agent.

#include "ccg.h"

int Card::randomInt(int n)
{
   // Return a random nonnegative integer less than n.
   int r;
   if (n <= 0)
   {
      return 0;
   }
   do
   {
      r = random();
   }
   while (r >= INT_MAX / n * n);
   return r / (INT_MAX / n);
}

string Card::toString() const
{
   // Convert a card to a convenient string representation.
   stringstream strstr;
   strstr << "[" << number;
   switch (suit)
   {
   case pads:
      strstr << "$]";
      break;
   case gloves:
      strstr << "@]";
      break;
   case bats:
      strstr << "!]";
      break;
   case balls:
      strstr << "*]";
      break;
   case stumps:
      strstr << "#]";
      break;
   }
   return strstr.str();
}

Hand::Hand()
{
   // Fill new hand with random cards.
   int whichCard;
   for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1)
   {
      cards[whichCard].randomize();
   }
}

string Hand::toString() const
{
   // Convert a hand of cards to a convenient string representation.
   int whichCard;
   string str;
   for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1)
   {
      str += cards[whichCard].toString();
   }
   return str;
}

MatchState::MatchState()
{
   // Start a new match from scratch.
   innings = 0;
   stats[0].runs = 0;
   stats[0].wickets = 0;
   stats[0].balls = 0;
   stats[1].runs = 0;
   stats[1].wickets = 0;
   stats[1].balls = 0;
   result = unfinished;
}

void MatchState::checkForResult()
{
   // Check to see whether the match is finished and update accordingly.
   if (stats[0].runs > stats[1].runs && stats[1].wickets >= 10)
   {
      // side batting first win by runs
      innings = 2;
      result = aWin;
   }
   else if (stats[1].runs > stats[0].runs && stats[0].wickets >= 10)
   {
      // side batting second win by wickets
      innings = 2;
      result = bWin;
   }
   else if (stats[0].runs == stats[1].runs && stats[0].wickets >= 10 && stats[1].wickets >= 10)
   {
      // both finish equal: tie
      innings = 2;
      result = tiedMatch;
   }
   else if (stats[0].balls + stats[1].balls >= numBallsPerMatch)
   {
      // still no result at ball limit: draw
      innings = 2;
      result = drawnMatch;
   }
}

void MatchState::scoreRuns(int numRuns)
{
   // Score run(s) and update the match statistics.
   if (stillPlaying())
   {
      stats[innings].runs += numRuns;
      stats[innings].balls += 1;
      checkForResult();
   }
}

void MatchState::takeWicket()
{
   // Take a wicket and update the match statistics.
   if (stillPlaying())
   {
      stats[innings].wickets += 1;
      stats[innings].balls += 1;
      if (stats[innings].wickets >= 10)
      {
         innings += 1;
      }
      checkForResult();
   }
}

int numRuns(int cardDifference)
{
   // Return the number of runs scored based on the difference between two cards.
   if (cardDifference >= 10)
   {
      return 6;
   }
   if (cardDifference >= 7)
   {
      return 4;
   }
   if (cardDifference >= 6)
   {
      return 3;
   }
   if (cardDifference >= 4)
   {
      return 2;
   }
   if (cardDifference >= 1)
   {
      return 1;
   }
   return 0;
}
