import Agent from "./Agent.ts";
import type { AgentStats } from "./AgentStats.ts";
import {
  addAgentToReport,
  compileAndExec,
  random,
  removeAgentFile,
  writeAgents,
} from "./utils.ts";

type PopulationOptions = {
  populationSize: number;
  maxValue?: number;
  minValue?: number;
  currentGeneration?: number;
  target: number;
  numChromasomes: number;
  mutationRate: number;
  poptype: string;
};

class Population {
  populationSize: number;
  maxValue: number;
  minValue: number;
  currentGeneration: number;
  members: Agent[] = [];
  target: number;
  numChromasomes: number;
  mutationRate: number;
  poptype: string;

  constructor(opts: PopulationOptions) {
    const { populationSize, maxValue, minValue } = opts;
    this.populationSize = populationSize;
    this.maxValue = maxValue ?? 200;
    this.minValue = minValue ?? 0;
    this.currentGeneration = opts.currentGeneration ?? 0;
    this.target = opts.target;
    this.numChromasomes = opts.numChromasomes;
    this.mutationRate = opts.mutationRate;
    this.poptype = opts.poptype;

    for (let i = 0; i < populationSize; i++) {
      this.members.push(
        new Agent({
          targetScore: this.target,
          numChromasomes: this.numChromasomes,
          type: this.poptype,
          minValue: this.minValue,
          maxValue: this.maxValue,
        }),
      );
    }
  }

  async run(): Promise<boolean> {
    const agentWriteResult = await writeAgents(this.members);
    let ran: boolean;

    if (agentWriteResult) {
      console.log(
        "Wrote all agent source files. Ready to proceed with execution.",
      );
      const executionResult = await compileAndExec();

      if (executionResult.ok) {
        this.updateAgentStats(executionResult.results!);
        ran = true;
        // console.log(this.members);
      } else {
        console.error("Execution did not run properly. Cannot update agents.");
        ran = false;
      }
    } else {
      console.log("Error writing agent source files.");
      ran = false;
    }
    return ran;
  }

  selectMatingPool(): Agent[] {
    const pool: Agent[] = [];

    this.members.forEach((agent) => {
      let f: number;
      if (this.poptype === "bowling") {
        f = Math.round(agent.bowlingFitness! / 10);
      } else if (this.poptype === "batting") {
        f = Math.round(agent.battingFitness! / 10);
      } else {
        f = Math.round(agent.overallFitness! / 10);
      }

      for (let i = 0; i < f; i++) {
        pool.push(agent);
      }
    });
    return pool;
  }

  async reproduce(matingPool: Agent[]) {
    for (let i = 0; i < this.members.length; i++) {
      const parentA = matingPool[random(0, matingPool.length)];
      const parentB = matingPool[random(0, matingPool.length)];

      console.log({ parentA });
      console.log({ parentB });
      const child = parentA.crossover(parentB);

      child.mutate(this.mutationRate, this.maxValue, this.minValue);
      this.members[i] = child;
    }
    // await this.run();
  }

  async evolve(generations: number): Promise<boolean> {
    let pool: Agent[];
    for (let i = 0; i < generations; i++) {
      pool = this.selectMatingPool();
      this.reproduce(pool);

      // create a set of agents from the gene pool
      const removalPool: Set<Agent> = new Set(pool);
      // remove old agent source files
      for (const agent of removalPool) {
        addAgentToReport(agent);
        try {
          await removeAgentFile(agent.id);
        } catch (error) {
          console.error(error);
        }
      }

      await this.run();
    }

    // remove leftover agent source files
    for (const agent of this.members) {
      addAgentToReport(agent);
      try {
        await removeAgentFile(agent.id);
      } catch (error) {
        console.error(error);
      }
    }

    return true;
  }

  updateAgentStats(results: AgentStats[]) {
    console.log("Updating individual agent stats");

    for (const stat of results) {
      const targetAgent = this.members.filter((agent) =>
        agent.id === stat.name
      )[0];

      if (targetAgent) {
        targetAgent.setStats(stat);
      }
    }
  }
}

export default Population;
