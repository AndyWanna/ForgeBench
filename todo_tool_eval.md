# TODO for Hanqiu: tool-evaluation results (Case Study 3, `tab:tool-support`)

Andy is away for a couple of days. This branch (`tool-eval`) is where the evidence behind the source-to-source tool table goes.
At the moment the paper's claims about ScaleHLS, HIDA, AutoSA and StreamHLS have nothing behind them in the repo. Everything
else in the paper can be traced to a command and a file. Please add what you have under `results/tool_eval/`, commit it on this
branch, and push the branch. Andy will review and merge it.

The list is in decreasing order of importance. Item 1 is the minimum the table needs. Each later item is useful but optional, so
stop wherever your results run out, and write down in `results/tool_eval/README.md` what is missing and why.

## What the paper claims (so you know what has to be backed)

From `sec/results.tex` (Case Study 3) and `table/testcases_table.tex` in the Overleaf project:

- Four tools are run on generated GEMM, DNN and LLM designs.
- **GEMM:** all four tools reach synthesis. AutoSA and StreamHLS need minor non-algorithmic edits (◐); ScaleHLS and HIDA are ●.
- **DNN:** ScaleHLS and HIDA get only as far as the transform step. StreamHLS generates code, but that code is not
  synthesizable. AutoSA synthesizes only a subset of the designs (◐).
- **LLM:** every tool stops at parsing (○).
- **F1–F6 marks per tool:** a ✓ in F1 and F2 for all four tools except AutoSA's F2, and ✗ everywhere else.

## 1. Minimum: one piece of evidence per table cell (please do this even if nothing else)

- [ ] **Exactly which designs were fed to each tool.** Give their ForgeBench design ids from
      `manifest/designs/{gemm,dnn,llm}.csv`, or the config names. If they weren't sweep designs, commit the configs under
      `results/tool_eval/designs/`. Use the HLS sources that ForgeBench generated, not hand-edited copies.
- [ ] **Tool versions:** a git commit or release tag for each of ScaleHLS, HIDA, AutoSA and StreamHLS, plus the LLVM/MLIR
      version if it matters.
- [ ] **`results/tool_eval/summary.csv`** with one row per tool × domain:
      `tool,domain,designs_tried,stage_reached,rating,notes`. Here `stage_reached` is one of `parse`, `transform`, `codegen` or
      `synth`, and `rating` is `full`, `half` or `none`, matching ●/◐/○.
- [ ] **One log per cell** that shows the stage reached or the error it stopped on, in `results/tool_eval/logs/<tool>/<domain>/`.
      Trimmed logs are fine: the command plus the last ~50 lines.
- [ ] **The "minor edits" for each ◐ cell** (AutoSA GEMM, AutoSA DNN, StreamHLS GEMM), as a patch against the generated source in
      `results/tool_eval/patches/<tool>/`. This is what shows the edits were non-algorithmic.

## 2. Answer these open questions about the table (in the README; a sentence each is fine)

- [ ] For F1–F6, does ✓ mean "the tool supports this feature" or "this limitation applies to the tool"? The text describes
      F1–F6 as *limitation categories*, but the table header says "HLS Support". Andy needs to know which reading is right so
      the caption can be fixed.
- [ ] AutoSA on DNN synthesizes "a subset". How many designs out of how many?
- [ ] StreamHLS on DNN generates code that is "not synthesizable". What is the Vitis error?
- [ ] LLM "halts at parsing" for all four tools. What does each tool fail on (the operator or construct)?
- [ ] For each F-category mark, the construct that triggered it (one example design and error per mark is enough).

## 3. Per-design outcomes

- [ ] **`results/tool_eval/outcomes.csv`**, one row per design × tool:
      `design_id,domain,tool,stage_reached,failure_category,error_head`. Here `failure_category` is one of F1–F6 or blank, and
      `error_head` is the first error line.

## 4. Re-runnable scripts

- [ ] The command or script you used for each tool, in `results/tool_eval/scripts/`: take a design directory, run the tool and
      record the stage reached. Include the environment setup (a Docker image name or build steps) in the README.

## 5. Nice to have

- [ ] csynth reports (`csynth.xml` only) for the designs that do reach synthesis, plus the original-Vitis numbers for the same
      designs, so a reader can see what each tool did to QoR.
- [ ] Full untrimmed logs. Put these in a tarball outside git. Andy will put it in the Zenodo bundle (`make bundle` already packs
      `results/tool_eval/`, so keep what's in git small: a few MB at most).

## Rules for this repo (it will be public)

- Commit only under `results/tool_eval/` (and delete this file when you're done). Don't change other files; if something
  elsewhere looks wrong, write it in the README instead.
- No local paths (`/usr/scratch/...`, home directories), hostnames, license files or credentials in anything you commit,
  including logs. Replace paths with `<TOOL_ROOT>` / `<DESIGN>`.
- Don't commit generated build trees (`project_1/`, `.mlir` dumps larger than ~1 MB). Put those in the tarball for Zenodo.
- If something in the paper's table turns out to be wrong after re-checking, say so in the README. Don't adjust results to
  match the table.
