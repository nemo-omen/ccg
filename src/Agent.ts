import { random } from "./utils.ts";
import { agentTemplate } from "./agentTemplate.ts";
import type { AgentStats } from "./AgentStats.ts";

type AgentOptions = {
  targetScore: number;
  numChromasomes: number;
  battingGene?: number[];
  bowlingGene?: number[];
  parentGenes?: number[];
  type: string;
  minValue?: number;
  maxValue?: number;
};

class Agent {
  id: string;
  gene: number[];
  targetScore: number;
  template: string;
  file?: string;
  stats?: AgentStats;
  bowlingFitness?: number;
  battingFitness?: number;
  overallFitness?: number;
  numChromasomes?: number;
  type: string;
  minValue: number;
  maxValue: number;

  constructor(opts: AgentOptions) {
    const {
      targetScore,
      numChromasomes,
      parentGenes,
      type,
    } = opts;

    this.id = "Offspring" + this.makeid(8);

    this.targetScore = targetScore;
    this.numChromasomes = opts.numChromasomes;
    this.type = type;
    this.minValue = opts.minValue || 0;
    this.maxValue = opts.maxValue || 100;

    if (parentGenes) {
      this.gene = [...parentGenes];
    } else {
      this.gene = this.makeRandomGene(numChromasomes);
    }

    this.template = this.buildTemplate();
  } // constructor

  setStats(stats: AgentStats) {
    this.stats = stats;
    this.calculateFitness();
  }

  calculateFitness() {
    const {
      battingBallsPerWicket,
      bowlingBallsPerWicket,
      battingRunsPerWicket,
      bowlingRunsPerWicket,
      battingRunsPerHundredBalls,
      bowlingRunsPerHundredBalls,
    } = this.stats;

    const battingFitnessStats = [
      battingBallsPerWicket,
      battingRunsPerWicket,
      battingRunsPerHundredBalls,
    ];

    const bowlingFitnessStats = [
      bowlingBallsPerWicket,
      bowlingRunsPerWicket,
      bowlingRunsPerHundredBalls,
    ];

    const bowlingFitnessScores: number[] = [];
    const battingFitnessScores: number[] = [];

    for (const stat of battingFitnessStats) {
      battingFitnessScores.push(stat);
    }

    for (const stat of bowlingFitnessStats) {
      bowlingFitnessScores.push(100 - stat);
    }

    let baf = 0;

    for (const score of battingFitnessScores) {
      baf += score;
    }

    this.battingFitness = baf / battingFitnessScores.length;

    // this.battingFitness = battingFitnessScores.reduce((a, b) => (a + b)) /
    //   battingFitnessScores.length;

    // this.bowlingFitness = bowlingFitnessScores.reduce((a, b) => (a + b)) /
    //   bowlingFitnessScores.length;
    let bof = 0;
    for (const score of bowlingFitnessScores) {
      bof += score;
    }

    this.bowlingFitness = bof / bowlingFitnessScores.length;

    this.overallFitness = (this.bowlingFitness + this.battingFitness) / 2;
  }

  mutate(mutationRate: number, min: number, max: number) {
    for (let i = 0; i < this.gene.length; i++) {
      if (Math.random() < mutationRate) {
        this.gene[i] = random(min, max);
      }
    }
  }

  crossover(partner: Agent): Agent {
    const midpoint = random(0, this.numChromasomes!);
    const crossoverGenes: number[] = [];

    for (let i = 0; i < this.numChromasomes!; i++) {
      if (i > midpoint) {
        crossoverGenes[i] = this.gene[i];
      } else {
        crossoverGenes[i] = partner.gene[i];
      }
    }

    const child = new Agent({
      numChromasomes: this.numChromasomes!,
      parentGenes: crossoverGenes,
      targetScore: this.targetScore,
      type: this.type,
      minValue: this.minValue,
      maxValue: this.maxValue,
    });

    return child;
  }

  makeRandomGene(n: number): number[] {
    const thisGene: number[] = [];

    for (let i = 0; i < n; i++) {
      thisGene.push(random(this.minValue, this.maxValue));
    }

    return thisGene;
  }

  buildTemplate(): string {
    return agentTemplate(this.id, this.type, this.gene);
  }

  async write(path: string): Promise<boolean> {
    try {
      await Deno.writeTextFile(
        path,
        this.template,
      );

      this.file = path;

      return true;
    } catch (error) {
      console.error(error);

      return false;
    }
  }

  makeid(length: number) {
    let result = "";
    const characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const charactersLength = characters.length;
    let counter = 0;
    while (counter < length) {
      result += characters.charAt(Math.floor(Math.random() * charactersLength));
      counter += 1;
    }
    return result;
  }
}

export default Agent;
