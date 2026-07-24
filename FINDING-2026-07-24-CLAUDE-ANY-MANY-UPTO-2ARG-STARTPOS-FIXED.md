# FINDING 2026-07-24 — Icon scan: any/many/upto 2-arg start-position default fixed

**Session:** Claude Sonnet 4.6, 2026-07-24
**Repos:** SCRIP `92738b6a`, corpus `ed25dbf0c` (local; corpus unpushed this session)
**Oracle:** icont/iconx 9.5.25a (pre-provisioned at `/home/claude/icon-master`)
**Canonical source verified:** `refs/icon-master/src/runtime/fstranl.r` `str_anal` macro

## Bug

`any(cset, string)`, `many(cset, string)`, `upto(cset, string)` — the 2-arg
explicit-string forms — always failed when the outer scan cursor (`scan_pos`) was
beyond the length of the explicit string argument.

**Symptom:** `options.icn` `if any(':+.', optstring[i]) then {grab param} else 1`
always took the else branch. queens `options(args,"n+")` never parsed the `-n` value.
Result: `Unrecognized option: -1` on stderr; queens ran the benchmark banner but solved
for the wrong (default) N.

## Root cause

`by_name_dispatch.c` (the runtime handler for 2-arg scan builtins), lines 5915/5933/5953:

```c
// BEFORE (wrong): uses outer scan_pos as start position inside explicit-string branch
int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
```

In `options.icn`, after scanning past `-n` in `"-n10"`, `scan_pos` was 3. The explicit
string `optstring[i]` = `"+"` has length 1. The check `i1 > slen` → `3 > 1` → FAIL,
so `any` failed even though `"+"` is in `":+."`.

## Canonical rule (fstranl.r str_anal macro, authoritative)

```
if is:null(s) then { s = k_subject; if is:null(i) then cnv_i = k_pos; }
else           {                     if is:null(i) then cnv_i = 1;     }
```

Explicit string ⇒ start position defaults to **1**, not `&pos`.

## Fix

`src/runtime/by_name_dispatch.c` lines 5915 (`any`), 5933 (`many`), 5953 (`upto`):

```c
// AFTER (correct): explicit string => i1 defaults to 1
int i1 = (nargs >= 3) ? (int)args[2].i : 1;
```

Matches the already-correct sibling scan handler at line ~6061
(`nargs < 2 ? scan_pos : 1` guard), which had the right guard already.

Note: the compiled `IR_SCAN_ANY`/`IR_SCAN_MANY`/`IR_SCAN_UPTO` BB templates only handle
the 1-arg implicit-subject form (verified: `icn_retag_scan_body` only retags nodes with
`n_operands == 1`). The 2-arg form stays `IR_CALL` and routes to the runtime handler.
So the fix lives entirely in `by_name_dispatch.c`.

## Validation

- `any(":+.","+")` → 2 (was FAIL), `many("0123456789","123x")` → 4 ✓, `upto("x","abxc")` → 3 ✓
- `options(args,"n+")` with `-n10` → `opts["n"]=10` (was `Unrecognized option: -1`)
- queens: no stderr errors; runs for the requested N
- Benchmark matrix: oracle 10/10 · m3 10/10 · m4 10/10 (all Icon/JCON benchmarks pass)
- Corpus: Icon rungs **248 → 249** (cleared `rung36_jcon_string` / FZ-E7), **zero regressions**
- libscrip_rt rebuilt; both modes carry the fix

## Open threads

- **geddump line count:** SCRIP 13645L vs oracle 12568L — genuine output-volume divergence, separate rung
- **micro mode-4 WSI boundary:** fix made micro's loops allocate ~256 MB in 0.05s budget; mode-4's bump allocator doesn't collect in-loop. Flakily hits `ZC_WSI_MB=256` boundary under harness load (completes fine standalone). Needs ζ workspace-GC rung or deliberate `ZC_WSI_MB` bump decision.
