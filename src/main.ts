import { agentReport, report } from "./utils.ts";
import Population from "./Population.ts";

async function generate(
  populationSize: number,
  target: number,
  mutationRate: number,
  generations: number,
): Promise<boolean> {
  const battingPopulation = new Population({
    populationSize: populationSize,
    target: target,
    numChromasomes: 7,
    poptype: "batting",
    mutationRate: mutationRate,
    maxValue: 100,
    minValue: -100,
  });

  const bowlingPopulation = new Population({
    populationSize: populationSize,
    target: target,
    numChromasomes: 8,
    maxValue: 1000,
    minValue: 0,
    poptype: "bowling",
    mutationRate: mutationRate,
  });

  // uncomment/comment to select population type
  // const population = battingPopulation;
  const population = bowlingPopulation;
  console.log({ population });

  const didRun = await population.run();
  let evolutionDone = false;

  if (didRun) {
    evolutionDone = await population.evolve(generations);
  }

  return evolutionDone;
}

const finished = await generate(10, 100, 0.05, 100);

if (finished) {
  const _finalReport = report();
  const finalAgentReport = agentReport().sort((a, b) =>
    // b.overallFitness! - a.overallFitness!
    b.bowlingFitness! - a.bowlingFitness!
    // b.battingFitness! - a.battingFitness!
  );
  const paredDownReport = finalAgentReport.map((obj) => {
    const { targetScore, template, ...newObj } = obj;
    return newObj;
  });

  await Deno.writeTextFile(
    "./finalReport.json",
    JSON.stringify(paredDownReport),
  );
  console.log(paredDownReport);
}
