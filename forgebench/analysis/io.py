"""CSV IO + normalization for the analysis package.

Reads the unified metrics.csv emitted by `forgebench.analysis.collect` (columns from
schema.CSV_COLUMNS) into a DataFrame (design, domain, cycles, *_util, ...).
"""
import os

import pandas as pd

_NUMERIC = ["cycles", "bram", "dsp", "ff", "lut",
            "bram_util", "dsp_util", "ff_util", "lut_util"]


def _coerce_numeric(df):
    for col in _NUMERIC:
        if col in df.columns:
            df[col] = pd.to_numeric(df[col], errors="coerce")  # 'undef' -> NaN
    return df


def read_metrics_csv(path):
    """Read a unified metrics.csv (already normalized column names)."""
    return _coerce_numeric(pd.read_csv(path))


def read_metrics_dir(dirpath):
    """Concatenate every *.csv in a directory of unified metrics files.

    Used for the per-domain outputs written by forgebench.analysis.collect
    (e.g. results/ml_sweep/csynth/metrics_{gemm,dnn,llm}.csv).
    """
    frames = []
    for fn in sorted(os.listdir(dirpath)):
        if fn.endswith(".csv"):
            frames.append(read_metrics_csv(os.path.join(dirpath, fn)))
    return pd.concat(frames, ignore_index=True, sort=False) if frames else pd.DataFrame()


_FEASIBLE_UTIL_COLS = ["bram_util", "dsp_util", "ff_util", "lut_util"]


def feasible_only(df):
    """Keep only designs that fit the device (all resource utilizations <= 100%)."""
    keep = pd.Series(True, index=df.index)
    for col in _FEASIBLE_UTIL_COLS:
        if col in df.columns:
            keep &= df[col].isna() | (df[col] <= 100)
    return df[keep]
