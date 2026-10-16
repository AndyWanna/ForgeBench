"""Rebinding the data_t / acc_t / math_t typedefs of a saved design (hand-written modularized designs, Table 7)."""
import re


def typedef_re(name):
    return re.compile(r"typedef\s+ap_fixed\s*<[^;]*>\s*" + name + r"\s*;")


def types(dtype_text, op_params=None):
    """-> {typedef name: C type}. data_t is the format under test; acc_t / math_t (accumulators; operand of hls::sqrt/exp) follow
    op_params acc_type / acc_rounding / acc_overflow exactly as a generated operator's ACC / ACCM do, else they are data_t."""
    from forgebench.backends import current
    if dtype_text == "float":
        return {"data_t": "float", "acc_t": "float", "math_t": "float"}
    acc, accm = current().acc_decls(dtype_text, op_params or {})
    data = current().type_decl(dtype_text)
    return {"data_t": data, "acc_t": data if acc == "data_t" else acc, "math_t": data if accm == "data_t" else accm}
