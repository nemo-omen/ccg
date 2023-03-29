# Cricket Card Game Genetic Algorithm

Source code for the program that runs this algorithm is located in the `/src`
directory. The code is written in Typescript for the Deno runtime. If you happen
to feel like running it, you will need to make sure
[Deno is installed on your system](https://deno.land/manual@v1.32.1/getting_started/installation).

The following command will run the code and watch for changes to any Typesctipt
files in the `/src` directory:

```bash
deno task dev
```

Or, if you don't want to do it that way you can run:

```bash
deno run -A ./src/main.ts
```

## Modifying the code to generate agents

To run the code, edit `/src/main.ts` and comment or uncomment line 31 or 32
depending on whether you want to generate a batting population or bowling
population. Then, if you want results that are sorted properly,
comment/uncomment lines 51 or 52 for the correct sorting. Finally, if you don't
feel like waitiing several minutes for results, edit the arguments in line 45.

Don't want to run the code? You can see the results of the last generation that
was produced by this code in `finalReport.json` and you can check out some of
the agents that were generated in the `/archive` directory. Finally, the result
of the last compilation/execution can be seen in `./result`.
