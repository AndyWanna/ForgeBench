# ForgeBench. `make help` lists the targets. Targets marked [Vitis] / [torch] / [g++] need that tool; the rest run from a fresh clone.
PY ?= python3
.DEFAULT_GOAL := help

.PHONY: help install install-verify paper table3 table7 section42 verification-tables catapult-vs-vitis stage-tables check test selfcheck \
        verify-fullmodel regression-save regression-compare manifest bundle clean

help:
	@echo "install              pip install -e .            (forgebench CLI + analysis dependencies)"
	@echo "install-verify       pip install -e '.[verify]'  (adds torch + pytest for the full-model verifier and tests)"
	@echo "paper                every paper table and figure below, from results/ -> paper_artifacts/out/"
	@echo "  table3             Table 3 full-scale models (resources + C-sim error)"
	@echo "  section42          Figs 6-8 and Table 5 (ML sweep csynth / impl)"
	@echo "  table7             Table 7 modularization"
	@echo "  verification-tables  operator / co-sim / Catapult / Table 7 verification tables (+ manifest/verification/*.csv)"
	@echo "  catapult-vs-vitis  Catapult vs Vitis synthesis of 1,000 GEMM designs (figure)"
	@echo "  stage-tables       stages passed per design set (tab:synth_summary, tab:status)"
	@echo "check                regenerate every committed design and sweep config; byte-compare with designs/"
	@echo "test                 unit tests                                    [torch]"
	@echo "selfcheck            compile + run every whole-design config with g++ against a float golden   [g++]"
	@echo "verify-fullmodel     C simulation of every Table 3 design vs the PyTorch references, OUT=<new dir> [ONLY=<regex>]   [Vitis, torch]"
	@echo "regression-save / regression-compare   generator byte-compare around a code change (BASE=<file>)"
	@echo "manifest             rebuild manifest/designs/*.csv (maintainers: needs the raw lean archives in checkpoints/)"
	@echo "bundle               assemble the Zenodo bundle in release/_bundle (maintainers)"

install:
	$(PY) -m pip install -e .

install-verify:
	$(PY) -m pip install -e '.[verify]'

paper: table3 section42 table7 verification-tables catapult-vs-vitis stage-tables

table3:
	$(PY) -m paper_artifacts.table3_fullmodel

section42:
	$(PY) -m paper_artifacts.section42

table7:
	$(PY) -m forgebench.modular_designs.parse_reports

verification-tables:
	$(PY) verification/experiments/make_tables.py

catapult-vs-vitis:
	mkdir -p paper_artifacts/out/figs
	$(PY) -m paper_artifacts.catapult_vs_vitis plot --out results/catapult_vs_vitis --fig paper_artifacts/out/figs/catapult_vs_vitis.png

stage-tables:
	$(PY) -m paper_artifacts.stage_tables

check:
	$(PY) -m verification.check_designs --sweep

test:
	$(PY) -m pytest -q

selfcheck:
	for d in gemm dnn llm; do $(PY) -m verification.gcc_selfcheck $$d || exit 1; done

verify-fullmodel:
	@test -n "$(OUT)" || { echo "usage: make verify-fullmodel OUT=<new dir> [ONLY=<regex>] [SEEDS=...] [PAR=3]"; exit 1; }
	OUT=$(OUT) ONLY='$(ONLY)' SEEDS='$(SEEDS)' PAR=$(or $(PAR),3) PY=$(PY) bash verification/experiments/run_fullmodel.sh

BASE ?= verification/_regression/before.json
regression-save:
	$(PY) -m verification.regression_check --save $(BASE)

regression-compare:
	$(PY) -m verification.regression_check --compare $(BASE)

manifest:
	$(PY) manifest/rekey_results.py && $(PY) manifest/build_manifest.py --check-disk && $(PY) manifest/build_registry.py

bundle:
	PY=$(PY) bash release/build_release.sh

clean:
	rm -rf paper_artifacts/out forgebench_out
	find . -name __pycache__ -prune -exec rm -rf {} +
