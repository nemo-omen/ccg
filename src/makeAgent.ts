import { agentTemplate } from "./agentTemplate.ts";

export type AgentOptions = {
  id: number;
  genes: number[];
};

export function makeAgent(opts: AgentOptions): string {
  const template = agentTemplate(opts.id, opts.genes);
  return template;
}
