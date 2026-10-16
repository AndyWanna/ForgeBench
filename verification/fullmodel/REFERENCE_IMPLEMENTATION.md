# Verify saved production HLS against two PyTorch references

`verify.py` tests the **existing** `top.cpp` and `top.h` of a generated
project in `designs/hls/fullmodel/`. It does not call a model
generator, repair the DUT, change its arithmetic, or use a dedicated tiled
accelerator as a substitute. The original synthesis TCL and testbench are
not executed; a separate testbench supplies inputs and observes outputs.

Each supported source version is paired with:

1. An independently expressed **FP64 PyTorch mathematical graph**.
2. An **integer-code PyTorch fixed-point graph**, modeling the source's
   intermediate widths, casts, truncation, wrapping, and reduction behavior.

The same quantized input and parameter values feed all three executions.
The fixed reference is not an FP64 model followed by output quantization.
It does not repair arithmetic such as `(data_t)49 == -15`, nor reproduce graph
defects such as disconnected parameter ports to force an implementation PASS.
The mathematical reference retains the intended real-valued operations.

The FP64 reference uses independent PyTorch mathematics throughout. For the
fixed Llama reference, RoPE coefficients use the same Vitis float pow/sin/cos
primitives as C simulation because their rounding differs from PyTorch/libm.
This limited shared dependency is disclosed in the report; executed runs save
the library hashes and coefficient tables. The fixed graph remains separate,
but this RoPE comparison does not independently validate those vendor
transcendental functions. No accelerator outputs become reference values.

## Run

`make verify-fullmodel OUT=<new dir>` runs every Table 3 design; single-design commands are in [README.md](README.md). The data path
(word length, rounding and overflow modes) is read from the saved header and exact source contract, never from a command-line
override.

Every output directory must be new. Optional flags:

- `--config FILE` and `--decode-config FILE`: production JSONs, cross-checked
  against the corresponding saved headers. Otherwise matching files under
  `auto_generated_configs/` are detected; a saved project can also be verified
  without its original JSON.
  This checks configuration/interface consistency; it does not regenerate
  the source or prove equivalence of every JSON operation to the source.
- `--seed 42`, `--threads 8`, `--vitis /path/to/vitis_hls`, `--timeout 7200`.
- `--prefill 4 --decode 2`: Llama schedule, within the saved cache capacity.
- `--tokens 1,2,3,4,5,6`: explicit token IDs. The default draws IDs across the
  full vocabulary. Control ports are transferred as int32 values.
- `--inspect-only`: snapshot the sources and check their interface/arithmetic
  without allocating parameters or running C simulation. This is not a PASS.

The registered versions are the 14 Table 3 sources in `designs/hls/fullmodel/`: full-buffer ResNet-18/34/50/101/152
and the context-2048 and context-8192 Llama 3 8B prefill/decode pairs at `ap_fixed<32,10>` (**AP_RND/AP_SAT**), and tiled
ResNet-18/34/50/101/152 at `ap_fixed<16,5>` (**AP_TRN/AP_WRAP**). Repairs are made in the JSON-to-HLS generator, never in a
saved source. The word length is read from the saved header; it controls
raw binary I/O, parameter quantization, fixed arithmetic, and FP64 scaling.
Q16.5 uses a 64-bit accumulator with 22 fractional bits; Q32.10 uses an
80-bit accumulator with 44 fractional bits. The wide PyTorch reference uses
exact integer limbs for GEMM/convolution and Python integers for intermediates
that exceed int64. Source-compatible precision is never inferred from a filename. Source hashes in
`contracts.json` bind a reference pair to an exact implementation. Changed
sources, other precisions, ResNet depths, and other context/tile variants
must have their arithmetic and interface validated before adding a contract;
the script rejects them instead of guessing that an old reference applies.

The FP64 mathematical graph stays the same when changing storage precision;
its inputs are the exact parameter/input values stored by that version.
The fixed reference changes its storage and intermediate arithmetic to match
each version. Full-buffer and tiled implementations share the intended graph
but have separate source contracts and separate actual C++ executions.

| Reference component | Implementation |
|---|---|
| ResNet-18/34/50/101/152 mathematical graph and FP64 operations | [resnet.py](references/resnet.py) |
| Llama prefill/decode mathematical graph and FP64 operations | [llama.py](references/llama.py) |
| Repaired Q16.5 fixed operations | [repaired.py](references/repaired.py) |
| Q32.10 fixed operations and precision-specific reference classes | [wide.py](references/wide.py) |
| Exact source-to-reference version mapping | [contracts.json](references/contracts.json) |

## Verdicts and coverage

The script writes `summary.json`, `summary.txt`, `verification.log`, source
and reference-code snapshots, input manifests, both reference outputs, the
actual C++ outputs, and per-tensor `comparison.json` records.

- **Implementation PASS** requires zero raw integer-code mismatches.
- **Mathematical PASS** requires relative L2 error <= 0.05 for every compared
  tensor. Relative L2 is `||actual/2**(W-I) - fp64||_2 / ||fp64||_2`. Maximum
  absolute error remains a reported diagnostic. For an all-zero FP64 reference,
  where relative L2 is undefined, the 0.1 absolute fallback applies. These are
  synthetic verification limits, not a trained-model accuracy standard.
- All values must be finite, tensor shapes and byte counts must match, and
  original source hashes must remain unchanged.
- Exit code **0** means both comparisons passed (or successful inspection,
  explicitly labelled `INSPECTED_NOT_EXECUTED`); **1** means numerical failure;
  **2** means invalid design/interface, unsupported version, tool failure, or
  incomplete execution. Missing results are never converted into a PASS.

ResNet-18 exports 12 persistent checkpoints without changing the DUT: stem
BN/ReLU output, maxpool output, eight residual-block outputs, global pool,
and all 1,000 logits. These total 1,757,672 codes. Full-buffer checkpoints are
observed through existing externally linked BRAM globals; tiled checkpoints
are observed through existing DRAM ports. ResNet-34 and ResNet-50 each export
20 checkpoints (16 residual blocks). ResNet-101 exports 37 checkpoints and
ResNet-152 exports 54 checkpoints. The bottleneck reference preserves the
declared production projection branch, which has no extra projection BN; it
does not claim equivalence to a torchvision model with pretrained parameters.
Overwritten intermediate values are not claimed as observed checkpoints.

Llama has separate C++, fixed-reference, and FP64-reference cache state.
The runner transfers the actual C++ prefill cache to the C++ decode process;
each reference retains its own cache. Every emitted logit and populated cache
entry is selected for comparison. Invalid constants or unrepresentable
controls stop execution before allocating an 8B parameter store, with
`INVALID_DESIGN` and both comparisons `NOT_RUN`.

## Tests

```bash
python -m pytest verification/fullmodel/tests -q
```

Tests exercise the actual Vitis headers/libraries: BN, signed division,
per-product conversion, pooling, partial linear/reduction tiles, RMSNorm,
SwiGLU, fixed square root and exponential across all 131,072 input codes of
the relevant 17-bit format, and RoPE across 2048 positions. Saved source/interface tests cover both
2048- and 8192-position cache capacities.
Additional tests cover source/interface changes, corrupt inputs, separate
verdicts, nonfinite outputs, FP64 prefill/decode cache continuity, invalid Llama
preflight even with representable token IDs, inspection without a numerical
PASS, and vendor primitive loading in a fresh process.
