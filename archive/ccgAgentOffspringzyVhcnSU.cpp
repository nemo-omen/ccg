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
int ccgAgentOffspringzyVhcnSU(Hand hand, Card lastBowledCard, bool isBatting,
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
  int battingCriteriaGene[10] = {889, 33, 421,
    1, 322, 132, 148, 622,
    631, 124};
  int bowlingCriteriaGene[8] = {545, 45, 177, 604, 470, 327, 421, 793};

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
        if (myNumber >= bowledNumber) {
          // favor the least valuable card we can play
          // that still saves us from losing a wicket
          if (myNumber == getLowestStillSafe(bowledNumber, hand)) {
            quality += battingCriteriaGene[1];
          }
          // weigh the card according to whether it's the lowest
          // of its suit in the hand
          if (myNumber != getLowestOfSuit(mySuit, hand)) {
            quality += battingCriteriaGene[2];
          } else {
            quality += battingCriteriaGene[3];
          }
        }

        if (myNumber < bowledNumber) {
          // if the card is lower than the bowled card
          // we're about to lose a wicket. give highest
          // weight to the lowest possible card.
          if (myNumber == getLowestOfHand(hand)) {
            quality += battingCriteriaGene[4];
          }

          if (mySuit == getMostFrequentSuit(hand, match)) {
            // give some weight to a card that is lower but
            // also has the most frequently occurring suit in the hand
            quality -= battingCriteriaGene[5];
            // quality += 5;
          } else {
            quality -= battingCriteriaGene[6];
          }
        }

      } else {
        // okay, we have a card of the same suit

        // if it's of a higher value, add quality according
        // to how many runs it can expect to get
        if (myNumber > bowledNumber) {
          quality += battingCriteriaGene[7] * numRuns(myNumber - bowledNumber);
        } else {
          if (myNumber == getLowestOfSuit(mySuit, hand)) {
            // give higher priority to the lowest card of
            // the same suit
            quality += battingCriteriaGene[8];
          } else {
            quality += battingCriteriaGene[9];
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

      // give preference to cards belonging to the suit
      // represented most frequently in the hand
      if (mySuit == getMostFrequentSuit(hand, match)) {
        quality += bowlingCriteriaGene[1];

        // add some to the lowest of that suit
        if (myNumber == getLowestOfSuit(mySuit, hand)) {
          quality += bowlingCriteriaGene[2];
        }
      }

      // encourage the agent to play ascending
      // cards of the same suit in order to deplete
      // the opponent's stock of usable cards
      if (mySuit == bowledSuit) {
        quality += bowlingCriteriaGene[3];
        if (myNumber > bowledNumber) {
          quality += bowlingCriteriaGene[4];
        }
      } else {
        quality += bowlingCriteriaGene[5];
      }

      // make sure the card's number isn't too close to the
      // last card's number
      if (myNumber != bowledNumber + 1 || myNumber != bowledNumber) {
        quality += bowlingCriteriaGene[6];
      }

      if (myNumber == bowledNumber + 1 || myNumber == bowledNumber) {
        quality -= bowlingCriteriaGene[7];
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
