import { move } from "https://deno.land/std@0.181.0/fs/mod.ts";
import type { AgentStats, ExecReturn, ExecutionResult } from "./types.ts";
import Agent from "./Agent.ts";

const testDir = ".";

type MatchupResult = {
  name: string;
  overallFitness: number;
  battingFitness: number;
  bowlingFitness: number;
  battingValues: number[];
  bowlingValues: number[];
  result: ExecutionResult;
};

let compiledResults: AgentStats[] = [];
const agents: Agent[] = [];

function random(min: number, max: number): number {
  min = Math.ceil(min);
  max = Math.floor(max);

  // The maximum is exclusive and the minimum is inclusive
  return Math.floor(Math.random() * (max - min)) + min;
}

function addAgentToReport(agent: Agent) {
  agents.push(agent);
}

async function removeAgentFile(agentName: string): Promise<boolean> {
  const src = `${testDir}/ccgAgent${agentName}.cpp`;
  const dest = `${testDir}/archive/${src}`;

  try {
    await move(src, dest);
    return true;
  } catch (error) {
    console.error(error);
    return false;
  }
}

function report(): AgentStats[] {
  return compiledResults;
}

function agentReport(): Agent[] {
  return agents;
}

async function writeAgents(members: Agent[]): Promise<boolean> {
  for (const agent of members) {
    const filePath = `${testDir}/ccgAgent${agent.id}.cpp`;
    try {
      await agent.write(filePath);
      console.log(`Wrote agent ${agent.id} source to ${agent.file}`);
      // return true;
    } catch (error) {
      console.error(error);
      console.log(`Failed to write agent ${agent.id}`);
      return false;
    }
  }
  return true;
}

async function writeResults(results: string) {
  try {
    const resData = await Deno.readTextFile("./results.json");
    const oldResults = JSON.parse(resData);
    const newResults = JSON.parse(results);
    const allResults = { ...oldResults, ...newResults };
    await Deno.writeTextFile(`./results.json`, JSON.stringify(allResults));
    console.log("Results written to ./results.json");
  } catch (error) {
    console.log("No old results.");
    await Deno.writeTextFile(`./results.json`, results);
    console.log("Results written to ./results.json");
  }
}

async function exec(): Promise<ExecReturn> {
  let output: string;
  const process = Deno.run({
    cmd: [
      "nice",
      `${testDir}/ccgRunSim`,
    ],
    stdout: "piped",
    stderr: "piped",
  });

  const [{ code }, rawOutput, rawError] = await Promise.all([
    process.status(),
    process.output(),
    process.stderrOutput(),
  ]);

  if (code === 0) {
    output = new TextDecoder().decode(rawOutput);
    await Deno.stdout.write(rawOutput);
  } else {
    const errorString = new TextDecoder().decode(rawError);
    output = errorString;
    console.error(errorString);
  }

  return { code, output };
}

async function compileAndExec(): Promise<ExecutionResult> {
  const compileCode = await compile();
  let exResult: ExecutionResult;

  if (compileCode === 0) {
    console.log("Done compiling agents. Ready to execute.");
    const { code, output } = await exec();

    if (code === 0) {
      console.log("Done executing matchups. Writing results.");
      writeResults(output);
      const parsedResults: AgentStats[] = JSON.parse(output);
      compiledResults = [...compiledResults, ...parsedResults];
      // console.log({ parsedResults });
      exResult = { ok: true, results: parsedResults };
    } else {
      console.log("Something went wrong when executing the matchups.");
      exResult = { ok: false, results: null };
    }
  } else {
    console.log("Compiling not completed. Something weird happened?");
    exResult = { ok: false, results: null };
  }

  return exResult;
}

async function compile(): Promise<number> {
  const process = Deno.run({
    cmd: [
      "nice",
      `${testDir}/ccgBuild.bash`,
    ],
    stdout: "piped",
    stderr: "piped",
  });

  const [{ code }, rawOutput, rawError] = await Promise.all([
    process.status(),
    process.output(),
    process.stderrOutput(),
  ]);

  if (code === 0) {
    await Deno.stdout.write(rawOutput);
  } else {
    const errorString = new TextDecoder().decode(rawError);
    const msgString = new TextDecoder().decode(rawOutput);
    console.error(errorString);
    console.error(msgString);
  }

  return code;
}

export {
  addAgentToReport,
  agentReport,
  compile,
  compileAndExec,
  exec,
  random,
  removeAgentFile,
  report,
  writeAgents,
  writeResults,
};
