// CS 4318, spring 2023
// Agent Challenge 4: Cricket card game
//
// Here's the main function that plays all agents against each other and
// summarizes the results.

#include "ccg.h"

extern const int numAgents;
extern int (*agentFunc[])(Hand, Card, bool, const MatchState &);
extern string agentStr[];

MatchState playCricketCardGameMatch(
    int (*agentA)(Hand, Card, bool, const MatchState &),
    int (*agentB)(Hand, Card, bool, const MatchState &), bool printAllDetails);

int main() {
  CricketStats battingStats[numAgents], bowlingStats[numAgents];
  int i, j, k, numAWon, numBWon, numDrawn, numDraws[numAgents],
      numLosses[numAgents], numTied, numTies[numAgents], numWins[numAgents],
      order[numAgents], pointsAsA[numAgents], pointsAsB[numAgents], temp;
  MatchState match;

  srandom(time(0));

  cout << "Agent Challenge 4: Cricket card game\n"
       << "Iteration results\n";
  for (i = 0; i < numAgents; i += 1) {
    numWins[i] = 0;
    numLosses[i] = 0;
    numDraws[i] = 0;
    numTies[i] = 0;
    pointsAsA[i] = 0;
    pointsAsB[i] = 0;
  }
  for (i = 0; i < numAgents; i += 1) {
    for (j = 0; j < numAgents; j += 1) {
      if (i != j) {
        numAWon = 0;
        numBWon = 0;
        numDrawn = 0;
        numTied = 0;
        cout << "\n"
             << "A = " << agentStr[i] << ", B = " << agentStr[j] << ":\n"
             << flush;
        for (k = playExhibitionMatches ? 0 : 1; k <= 200; k += 1) {
          if (k == 0) {
            cout << "Exhibition match:\n";
          }
          match = playCricketCardGameMatch(agentFunc[i], agentFunc[j], k == 0);
          cout << "   A " << match.getRuns(0) << "-" << match.getWickets(0)
               << "(" << match.getBalls(0) << "), B " << match.getRuns(1) << "-"
               << match.getWickets(1) << "(" << match.getBalls(1) << ")";
          switch (match.getResult()) {
            case aWin:
              cout << "   >>> A wins <<<\n";
              break;
            case bWin:
              cout << "   >>> B wins <<<\n";
              break;
            case drawnMatch:
              cout << "   >>> match drawn <<<\n";
              break;
            case tiedMatch:
              cout << "   >>> match tied <<<\n";
              break;
          }
          if (k == 0) {
            cout << "Official matches:\n";
          } else {
            switch (match.getResult()) {
              case aWin:
                numAWon += 1;
                numWins[i] += 1;
                pointsAsA[i] += pointsForWin;
                numLosses[j] += 1;
                pointsAsB[j] += pointsForLoss;
                break;
              case bWin:
                numBWon += 1;
                numLosses[i] += 1;
                pointsAsA[i] += pointsForLoss;
                numWins[j] += 1;
                pointsAsB[j] += pointsForWin;
                break;
              case drawnMatch:
                numDrawn += 1;
                numDraws[i] += 1;
                pointsAsA[i] += pointsForDraw;
                numDraws[j] += 1;
                pointsAsB[j] += pointsForDraw;
                break;
              case tiedMatch:
                numTied += 1;
                numTies[i] += 1;
                pointsAsA[i] += pointsForTie;
                numTies[j] += 1;
                pointsAsB[j] += pointsForTie;
                break;
            }
            battingStats[i].runs += match.getRuns(0);
            battingStats[i].wickets += match.getWickets(0);
            battingStats[i].balls += match.getBalls(0);
            bowlingStats[i].runs += match.getRuns(1);
            bowlingStats[i].wickets += match.getWickets(1);
            bowlingStats[i].balls += match.getBalls(1);
            battingStats[j].runs += match.getRuns(1);
            battingStats[j].wickets += match.getWickets(1);
            battingStats[j].balls += match.getBalls(1);
            bowlingStats[j].runs += match.getRuns(0);
            bowlingStats[j].wickets += match.getWickets(0);
            bowlingStats[j].balls += match.getBalls(0);
          }
        }
        cout << "A (" << agentStr[i] << ") wins " << numAWon << ", B ("
             << agentStr[j] << ") wins " << numBWon << ", draws " << numDrawn
             << ", ties " << numTied << "\n";
      }
    }
  }

  for (i = 0; i < numAgents; i += 1) {
    order[i] = i;
  }
  for (i = 0; i < numAgents - 1; i += 1) {
    for (j = i + 1; j < numAgents; j += 1) {
      if (pointsAsA[order[i]] + pointsAsB[order[i]] <
              pointsAsA[order[j]] + pointsAsB[order[j]] ||
          (pointsAsA[order[i]] + pointsAsB[order[i]] ==
               pointsAsA[order[j]] + pointsAsB[order[j]] &&
           agentStr[order[i]] > agentStr[order[j]])) {
        temp = order[i];
        order[i] = order[j];
        order[j] = temp;
      }
    }
  }
  cout << "\n\n"
       << "Overall standings:                                    points        "
          " batting statistics      bowling statistics\n"
       << "                    points     W     L     D     T  as A  as B      "
          "R/W     R/hB    B/W     R/W     R/hB    B/W\n";
  cout << fixed;
  for (i = 0; i < numAgents; i += 1) {
    cout << setw(20) << left << agentStr[order[i]] << " " << setw(5) << right
         << pointsAsA[order[i]] + pointsAsB[order[i]] << " " << setw(5) << right
         << numWins[order[i]] << " " << setw(5) << right << numLosses[order[i]]
         << " " << setw(5) << right << numDraws[order[i]] << " " << setw(5)
         << right << numTies[order[i]] << " " << setw(5) << right
         << pointsAsA[order[i]] << " " << setw(5) << right
         << pointsAsB[order[i]] << " " << setprecision(2) << setw(9) << right
         << static_cast<double>(battingStats[order[i]].runs) /
                battingStats[order[i]].wickets
         << " " << setprecision(2) << setw(7) << right
         << 100 * static_cast<double>(battingStats[order[i]].runs) /
                battingStats[order[i]].balls
         << " " << setprecision(2) << setw(7) << right
         << static_cast<double>(battingStats[order[i]].balls) /
                battingStats[order[i]].wickets
         << " " << setprecision(2) << setw(7) << right
         << static_cast<double>(bowlingStats[order[i]].runs) /
                bowlingStats[order[i]].wickets
         << " " << setprecision(2) << setw(7) << right
         << 100 * static_cast<double>(bowlingStats[order[i]].runs) /
                bowlingStats[order[i]].balls
         << " " << setprecision(2) << setw(7) << right
         << static_cast<double>(bowlingStats[order[i]].balls) /
                bowlingStats[order[i]].wickets
         << "\n";
  }
  return 0;
}

MatchState playCricketCardGameMatch(
    int (*agentA)(Hand, Card, bool, const MatchState &),
    int (*agentB)(Hand, Card, bool, const MatchState &), bool printAllDetails) {
  // Play a match of the cricket card game between given agents.
  Card battingCard, bowlingCard;
  Hand battingHand, bowlingHand, tempHand;
  int battingPlay, bowlingPlay, lastBowlingCardNumber;
  int (*battingAgent)(Hand, Card, bool, const MatchState &);
  int (*bowlingAgent)(Hand, Card, bool, const MatchState &);
  MatchState match;

  battingAgent = agentA;
  bowlingAgent = agentB;
  while (match.stillPlaying()) {
    if (printAllDetails) {
      cout << "   A " << match.getRuns(0) << "-" << match.getWickets(0) << "("
           << match.getBalls(0);
      if (!match.isInFirstInnings()) {
        cout << "), B " << match.getRuns(1) << "-" << match.getWickets(1) << "("
             << match.getBalls(1);
      }
      cout << "); "
           << (numBallsPerMatch - match.getBalls(0) - match.getBalls(1))
           << " balls to go\n"
           << "      " << (match.isInFirstInnings() ? "B" : "A")
           << "\'s hand: " << bowlingHand.toString() << "   "
           << (match.isInFirstInnings() ? "A" : "B")
           << "\'s hand: " << battingHand.toString() << "\n";
    }
    lastBowlingCardNumber = bowlingCard.getNumber();
    bowlingPlay = (*bowlingAgent)(bowlingHand, bowlingCard, false, match);
    bowlingCard = bowlingHand.getCard(bowlingPlay);
    bowlingHand.randomizeCard(bowlingPlay);
    if (printAllDetails) {
      cout << "      " << (match.isInFirstInnings() ? "B" : "A") << " bowls "
           << bowlingCard.toString() << "; ";
    }
    if (bowlingCard.getNumber() == lastBowlingCardNumber) {
      match.scoreRuns(1);
      if (printAllDetails) {
        cout << "umpire calls \"wide\" and "
             << (match.isInFirstInnings() ? "A" : "B") << " scores 1 run.\n";
      }
    } else if (bowlingCard.getNumber() == lastBowlingCardNumber + 1) {
      match.scoreRuns(numRuns(lastBowlingCardNumber));
      if (printAllDetails) {
        cout << "umpire calls \"bye\" and "
             << (match.isInFirstInnings() ? "A" : "B") << " scores "
             << numRuns(lastBowlingCardNumber) << " run"
             << (numRuns(lastBowlingCardNumber) == 1 ? "" : "s") << ".\n";
      }
    } else {
      battingPlay = (*battingAgent)(battingHand, bowlingCard, true, match);
      battingCard = battingHand.getCard(battingPlay);
      battingHand.randomizeCard(battingPlay);
      if (printAllDetails) {
        cout << (match.isInFirstInnings() ? "A" : "B") << " plays "
             << battingCard.toString() << " and ";
      }
      if (battingCard.getSuit() == bowlingCard.getSuit()) {
        match.scoreRuns(
            numRuns(battingCard.getNumber() - bowlingCard.getNumber()));
        if (printAllDetails) {
          cout << "scores "
               << numRuns(battingCard.getNumber() - bowlingCard.getNumber())
               << " run"
               << (numRuns(battingCard.getNumber() - bowlingCard.getNumber()) ==
                           1
                       ? ""
                       : "s")
               << ".\n";
        }
      } else if (battingCard.getNumber() >= bowlingCard.getNumber()) {
        match.scoreRuns(0);
        if (printAllDetails) {
          cout << "scores 0 runs.\n";
        }
      } else {
        match.takeWicket();
        if (printAllDetails) {
          cout << "is out.\n";
        }
      }
    }
    if (!match.isInFirstInnings() && match.getBalls(1) == 0) {
      battingAgent = agentB;
      bowlingAgent = agentA;
      tempHand = battingHand;
      battingHand = bowlingHand;
      bowlingHand = tempHand;
    }
  }

  return match;
}
