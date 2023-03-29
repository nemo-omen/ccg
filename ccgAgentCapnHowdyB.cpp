// YOUR NAME: Jeff Caldwell
//
// CS 4318, spring 2023
// Agent Challenge 4: Cricket card game
//
// Rename this file and the function below.  For example, if your agent name
// is Jones, rename this ccgAgentSmith.cpp file to ccgAgentJones.cpp and the
// ccgAgentSmith function below to ccgAgentJones.  Complete your agent
// function and turn it in on Blackboard.  Random-number generation is not
// allowed; your agent must be entirely deterministic.  Feel free to create
// other agents--each in a separate .cpp file--for yours to play against,
// but turn in only one.  Test your agent(s) with
//
//    nice bash ccgBuild.bash
//
// and then
//
//    nice ./ccgRunSim
//
// Each submitted agent will play each other, batting first and batting
// second an equal number of times, to determine the standings, which will
// be posted soon after the agents are due.

#include <vector>

#include "ccg.h"
namespace {
// If you need to define any new types or functions, put them here in
// this unnamed namespace.  But no variables allowed!

cardSuit getMostFrequentSuit(const Hand &hand, const MatchState &match) {
  int i, suitNum;
  int max = INT_MIN;
  Card card;
  cardSuit maxSuit;
  int suitCount[numSuits] = {0};

  for (i = 0; i < numCardsPerHand; i++) {
    card = hand.getCard(i);
    suitNum = static_cast<int>(card.getSuit());
    suitCount[suitNum] += 1;
  }

  for (i = 0; i < numSuits; i++) {
    if (suitCount[i] > max) {
      max = suitCount[i];
      maxSuit = static_cast<cardSuit>(i);
    }
  }

  return maxSuit;
}

int getLowestOfSuit(const cardSuit suit, const Hand &hand) {
  Card current, lowest;
  int min = INT_MAX;
  int i;

  for (i = 0; i < numCardsPerHand; i++) {
    current = hand.getCard(i);
    if (current.getSuit() == suit) {
      if (current.getNumber() < min) {
        min = current.getNumber();
        lowest = current;
      }
    }
  }

  return min;
}

int getHighestOfHand(Hand &hand) {
  int max = INT_MIN;
  int i;

  for (i = 0; i < numCardsPerHand; i++) {
    Card card = hand.getCard(i);
    if (card.getNumber() > max) {
      max = card.getNumber();
    }
  }

  return max;
}

int getLowestOfHand(const Hand &hand) {
  int min = INT_MAX;
  int i;

  for (i = 0; i < numCardsPerHand; i++) {
    Card card = hand.getCard(i);

    if (card.getNumber() < min) {
      min = card.getNumber();
    }
  }

  return min;
}

int getLowestStillSafe(const int numToBeat, const Hand &hand) {
  int lowest = INT_MAX;
  int i;

  for (i = 0; i < numCardsPerHand; i++) {
    Card card = hand.getCard(i);
    if (card.getNumber() < lowest && card.getNumber() >= numToBeat) {
      lowest = card.getNumber();
    }
  }

  return lowest;
}

}  // namespace

// Rename and complete this agent function.
int ccgAgentCapnHowdyA(Hand hand, Card lastBowledCard, bool isBatting,
                       const MatchState &match) {
  // Your function must end up returning a valid int between 0 and
  // numCardsPerHand - 1. No random-number generation allowed! hand.getCard(0)
  // gives the first card in your hand. hand.getCard(0).getNumber() gives the
  // number of that card, 1 to 11. hand.getCard(0).getSuit() gives the suit of
  // that card. match.getRuns(0) gives the number of runs scored by player A so
  // far. numRuns(d) gives the number of runs scored when the card difference is
  // d. See the definitions of the Hand, Card and MatchState classes for more
  // helpful functions.
  Card card;
  cardSuit mySuit, bowledSuit;
  int bestCard, bestQuality, quality, whichCard, myNumber, bowledNumber;
  int battingCriteriaGene[7] = {188, 74, 4, 104, 133, 173, 35};

  // int bowlingCriteriaGene[5] = {10, 50, 25, 100, 25};
  int bowlingCriteriaGene[5] = {43, 39, 12, 49, 0};

  if (isBatting) {
    // When batting, play the card with the highest quality according to
    // weighted criteria.
    bestQuality = INT_MIN;
    bestCard = 0;
    for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
      card = hand.getCard(whichCard);
      mySuit = card.getSuit();
      myNumber = card.getNumber();
      bowledSuit = lastBowledCard.getSuit();
      bowledNumber = lastBowledCard.getNumber();
      quality = 0;

      // first, handle cases where our suit is not the same
      // as the bowled suit
      if (mySuit != bowledSuit) {
        // highest quality goes to cards of equal number
        if (myNumber == bowledNumber) {
          quality += battingCriteriaGene[0];
        }

        // if we have a card with a higher number
        if (myNumber > bowledNumber) {
          quality += battingCriteriaGene[1];
          // favor the least valuable card we can play
          // that still saves us from losing a wicket
          if (myNumber == getLowestStillSafe(bowledNumber, hand)) {
            quality += battingCriteriaGene[2];
          }
        }

        if (myNumber < bowledNumber) {
          // if the card is lower than the bowled card
          // we're about to lose a wicket. give highest
          // weight to the lowest possible card.
          if (myNumber == getLowestOfHand(hand)) {
            quality += battingCriteriaGene[3];
          }

          if (mySuit == getMostFrequentSuit(hand, match)) {
            // give some weight to a card that is lower but
            // also has the most frequently occurring suit in the hand
            quality += battingCriteriaGene[4];
          }
        }

      } else {
        // okay, we have a card of the same suit

        // if it's of a higher value, add quality according
        // to how many runs it can expect to get
        if (myNumber > bowledNumber) {
          quality += battingCriteriaGene[5] * numRuns(myNumber - bowledNumber);
        } else {
          if (myNumber == getLowestOfSuit(mySuit, hand)) {
            // give higher priority to the lowest card of
            // the same suit
            quality += battingCriteriaGene[6];
          }
        }
      }

      if (quality > bestQuality) {
        bestQuality = quality;
        bestCard = whichCard;
      }
    }
    return bestCard;
  } else  // is bowling
  {
    // When bowling, play the card with the highest quality according to
    // weighted criteria.
    bestQuality = 0;
    bestCard = 0;
    for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
      card = hand.getCard(whichCard);
      quality = 0;
      mySuit = card.getSuit();
      myNumber = card.getNumber();
      bowledSuit = lastBowledCard.getSuit();
      bowledNumber = lastBowledCard.getNumber();

      // give a little boost to cards of higher value
      quality += myNumber + bowlingCriteriaGene[0];

      if (mySuit == getMostFrequentSuit(hand, match)) {
        quality += bowlingCriteriaGene[1];
        if (myNumber == getLowestOfSuit(mySuit, hand)) {
          quality += bowlingCriteriaGene[2];
        }
      }

      if (mySuit == bowledSuit) {
        if (myNumber > bowledNumber) {
          quality += bowlingCriteriaGene[3];
        } else {
          quality -= bowlingCriteriaGene[4];
        }
      }

      // Create your own bowling criteria!
      // Have we found a better card to play?
      if (quality > bestQuality) {
        bestQuality = quality;
        bestCard = whichCard;
      }
    }
    return bestCard;
  }
}

/*

 - First, carefully comment your code above to clarify how it works.
 - Here, describe your approach and analyze it.  How exactly did you develop
   and test it?  What are its strengths and weaknesses?  Why do you expect
   it to do well against the other submitted agents?
 - Also make a note here if you talked about the assignment with anyone or
   gave or received any kind of help.

   -- PRELIMINARY APPROACH --
   - When batting, favor a varied hand with as many suits as possible
     - try to hold both highs and lows for every suit type

     So far, for iteration 1, this isn't working very well.
     I may be able to use this approach as a stopgap when the hand
     is especially bad, but honestly, it feels like it should be
     abandoned in favor of a more direct approach.

     Initially, when playing through the batting practice, it felt
     like a winning approach was to keep a varied hand in order to respond
     to the bowler's card suitably. I should adapt to the different
     conditions within this environment -- my agent can't respond to the
 bowler's card because it only has access to the card the bowler played before
 the current round.

     It's not a winning strategy. At least not by itself.
*/
