# HANDOFF — 11th run (Claude Sonnet 4.6, 2026-06-08)

## Repo state
- Branch: `main`, HEAD: `1eef039`
- Build: CLEAN (`make scrip` + `make libscrip_rt` → both green)
- All standing gates: GREEN
  - smoke m2 PASS=5 FAIL=0 (HARD GATE)
  - smoke m3 PASS=4 FAIL=1 (arith: BINARY path unresolved-fwd-ref, pre-existing)
  - smoke m4 PASS=4 FAIL=1 (same)
  - GATE-3 m2=114/115  m3=25/90-FAIL  m4=50/55-FAIL (ratchet floors: m3≥25, m4≥50)
  - PL-HY-FENCE PASS
  - g_vstack 0
  - seg_byte/SL_B outside templates 0
  - `g_resolve_env` in IR_interp.c | grep rt_is == 0
  - `rt_is@PLT|rt_is_lint@PLT` in bb_is_cmp.cpp == 0

## What landed this session

### M34-2 COMPLETE (commit `2127c82`)

Three TEXT arms in `bb_is_cmp.cpp` that called `rt_is@PLT` / `rt_is_lint@PLT`
replaced with `rt_is_cell@PLT` / `rt_is_cell_lit@PLT`. Root cause: both old
functions delegate to `rt_arith()` which reads `g_resolve_env[slot]` for
IR_LOGICVAR args — NULL for GZ-admitted procs in m4 → arithmetic results all 0.

| Arm | LHS | RHS | Old | New |
|-----|-----|-----|-----|-----|
| Binary | IR_LOGICVAR | IR_ARITH(binary) | `rt_is@PLT` | `rt_is_cell@PLT` |
| Lit-dst | IR_LIT_I | IR_ARITH(binary) | `rt_is_lint@PLT` | `rt_is_cell_lit@PLT` |
| Unary | IR_LOGICVAR | IR_ARITH(unary) | `rt_is@PLT` | `rt_is_cell@PLT` |

All arms: `sub rsp,8` / `add rsp,8` for ABI alignment. `icm_arg_load_lit` for
operands: `lea rcx,[r12+GZ_CELL_OFF(slot)]` for LOGICVAR, `mov rcx,value` for LIT_I.

`rt_is_cell` op table completed (binary: `//` `mod` `rem` `div` `gcd` `/\` `\/`
`xor` `>>` `<<`; unary: `\` `msb`).

New `rt_is_cell_lit(long lval, const char *op, int lk, void *larg, double ld,
int rk, void *rarg, double rd)` — mirrors `rt_is_cell` but compares result to
`lval` instead of writing to a cell. For `N is Expr` where N is IR_LIT_I.

**GATE-3 gain: m3 23→25 (+2), m4 45→50 (+5). Zero regressions.**
Newly passing: rung23_sign, rung23_power, rung23_max_min, rung23_bitwise,
rung29_gcd (m4); rung23_truncate, rung23_sign (m3).

## Next session setup
```
cd /home/claude
git clone https://ghp_TOKEN@github.com/snobol4ever/SCRIP.git SCRIP
git clone https://ghp_TOKEN@github.com/snobol4ever/.github
cd SCRIP
bash scripts/install_system_packages.sh
bash scripts/build_scrip.sh
make libscrip_rt
bash scripts/test_smoke_prolog.sh          # m2 must be 5/5 HARD
bash scripts/test_prolog_rung_suite.sh     # m3≥25  m4≥50  m2≥114
bash scripts/test_gate_bb_one_box.sh       # PASS
grep -rn 'g_resolve_env' src/interp/IR_interp.c | grep rt_is   # 0
grep -n 'rt_is@PLT\|rt_is_lint@PLT' src/emitter/BB_templates/bb_is_cmp.cpp  # 0
```
Then proceed to **M34-3**.

## Next opener: M34-3

Write `scripts/test_gate_pl_m34_parity.sh`:

```bash
#!/usr/bin/env bash
# For each rung*.pl: run m3 (--run) and m4 (run_prolog_via_x86_backend.sh).
# FAIL if one side has output and other aborts/empty.
# FAIL if both have output but differ.
# EXCISED if both abort/empty identically.
# Gate: zero FAIL required before PL-GZ-FENCE.
CORPUS=/home/claude/corpus/programs/prolog
SCRIP=/home/claude/SCRIP/scrip
pass=0; fail=0; excised=0
for pl in "$CORPUS"/rung*.pl; do
    m3=$(timeout 10 "$SCRIP" --run "$pl" 2>/dev/null)
    m4=$(timeout 10 bash /home/claude/SCRIP/scripts/run_prolog_via_x86_backend.sh "$pl" 2>/dev/null)
    if [ -z "$m3" ] && [ -z "$m4" ]; then
        excised=$((excised+1))
    elif [ "$m3" = "$m4" ]; then
        pass=$((pass+1))
    else
        echo "FAIL $(basename $pl): m3=$(echo "$m3"|head -1|cut -c1-40) m4=$(echo "$m4"|head -1|cut -c1-40)"
        fail=$((fail+1))
    fi
done
echo "--- M34-PARITY: PASS=$pass FAIL=$fail EXCISED=$excised ---"
[ $fail -eq 0 ]
```

After M34-3 establishes the honest parity baseline, proceed to:

**M34-4** — Delete 3 `rt_last_ok@PLT` sites from `bb_goal.cpp`.
Read `bb_goal.cpp` first (Prolog source for verdict semantics). Replace with
return-value wiring per THE LAWS. Gate: `grep -rn 'rt_last_ok'
src/emitter/BB_templates/` == 0.

**M34-5** — Parity seal: audit m4 `pl_rich_body_root` pass-throughs vs m3.
Any rung passing m4 via legacy tier but failing m3 → add to PL-GZ-9 queue.
