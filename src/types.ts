export type AgentStats = {
  name: string;
  points: number;
  numWins: number;
  numLosses: number;
  numTies: number;
  numDraws: number;
  pointsAsA: number;
  pointsAsB: number;
  battingBallsPerWicket: number;
  battingRunsPerHundredBalls: number;
  battingRunsPerWicket: number;
  bowlingBallsPerWicket: number;
  bowlingRunsPerHundredBalls: number;
  bowlingRunsPerWicket: number;
};

export type ExecutionResult = {
  ok: boolean;
  results: AgentStats[] | null;
};

export type ExecReturn = {
  code: number;
  output: string;
};
