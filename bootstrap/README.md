# corpus/SCRIP — SCRIP source tree

This is the SCRIP compiler/interpreter/runtime, written in SCRIP. The
host implementation languages are **Snocone, Icon, and Prolog** (with
Raku and SNOBOL4 reserved for show pieces). Host language is identified
by file extension; the directory does **not** split by host:

| Extension | Host language |
|-----------|---------------|
| `.sc`     | Snocone |
| `.icn`    | Icon |
| `.pl` / `.pro` | Prolog |
| `.sno` / `.inc` / `.spt` | SNOBOL4 / SPITBOL |
| `.raku`   | Raku |

Files in different host languages co-exist in this one directory and
are wired together by the test scripts.

## Pattern-based frontends

The six PARSER-* frontends (PARSER-SN, PARSER-SC, PARSER-RB, PARSER-RK,
PARSER-IC, PARSER-PR) are siblings: they share `Compiland` / `Shift` /
`Reduce` / `Push` / `Pop` / `Top` / `tree` / `nPush` / `nTop` / `nPop`.
Each driver's `Compiland` shares the same spine — only the `Command`
rules and language-specific token rules differ:

```
Compiland = nPush() ARBNO(*Command) reduce('Parse', 'nTop()') nPop();
```

## Snocone-hosted runtime + drivers

Snocone has no `-include` directive; callers pass every file as a blob:

```
scrip --run \
  corpus/SCRIP/global.sc \
  corpus/SCRIP/tree.sc \
  corpus/SCRIP/stack.sc \
  corpus/SCRIP/counter.sc \
  corpus/SCRIP/ShiftReduce.sc \
  corpus/SCRIP/semantic.sc \
  corpus/SCRIP/tdump.sc \
  corpus/SCRIP/parser_<lang>.sc \
  input_file
```

### Shared runtime (Snocone)

| File | Purpose |
|------|---------|
| `global.sc` | Prelude every PARSER-* driver assumes is in scope before any pattern compiles: `&FULLSCAN`, `&MAXLNGTH`, named character constants (`nul`, `bs`, `tab`, `nl`, `cr`, `ff`, `fSlash`, `semicolon`, `bSlash`, etc.), bit-prefix slices (`X0xxxxxxx` … `X11111xxx`), and the literals `TRUE`, `FALSE`, `digits`. UTF table from beauty is intentionally not imported. |
| `tree.sc` | General-purpose tree datatype (`struct tree { t, v, n, c }`). Provides `MakeLeaf`, `MakeNode`, `Append`, `Prepend`, `Insert`, `Remove`, `Equal`, `Visit`. |
| `stack.sc` | Linked-list value stack (`struct link`). Provides `InitStack`, `Push`, `Pop`, `Top`. Used by `ShiftReduce.sc`. |
| `counter.sc` | Stack of integer counters (`struct link_counter`). Provides `InitCounter`, `PushCounter`, `IncCounter`, `DecCounter`, `PopCounter`, `TopCounter`. |
| `ShiftReduce.sc` | Core shift-reduce operations. `Shift(t,v)` pushes a leaf tree; `Reduce(t,n)` pops `n` trees and pushes a parent. Requires `tree.sc` and `stack.sc`. |
| `semantic.sc` | Pattern-building helpers called at pattern-construction time (not match time). Provides `nPush`, `nInc`, `nDec`, `nTop`, `nPop`, `shift`, `reduce`, `pop`, plus `OPSYN` declarations of `~` and `&` infix operators. |
| `tdump.sc` | Tree-printing only — slim port of beauty's `TDump.inc` covering `TLump`/`TValue` plus a thin `TDump` wrapper that just emits `TLump(x, 1024)`. The recursive `Gen()`-wrapping pretty-printer in beauty is **not** in scope; one-line bracketed output is sufficient for crosscheck PASS/FAIL. |

### Per-language frontend drivers (Snocone-hosted)

| File | Goal | Status |
|------|------|--------|
| `parser_snobol4.sc` | `GOAL-PARSER-SNOBOL4.md` | written under PARSER-SN-0 |
| `parser_snocone.sc` | `GOAL-PARSER-SNOCONE.md` | written under PARSER-SC-0 |
| `parser_rebus.sc`   | `GOAL-PARSER-REBUS.md`   | written under PARSER-RB-0 |
| `parser_raku.sc`    | `GOAL-PARSER-RAKU.md`    | written under PARSER-RK-0 |
| `parser_icon.sc`    | `GOAL-PARSER-ICON.md`    | written under PARSER-IC-0 |
| `parser_prolog.sc`  | `GOAL-PARSER-PROLOG.md`  | written under PARSER-PR-0 |

### Per-language helper sidecars (transitional)

A handful of helpers that were taken out of parser files during the
PST cleanup live in sidecars and are loaded by
`SCRIP/scripts/run_scrip_parser.sh` alongside the parser:

| File | Loaded with | Contents |
|------|-------------|----------|
| `icon_helpers.sc` | `parser_icon.sc`  | 4 leaf-push helpers (`push_qlit`, `push_cset`, `push_flit`, `push_kw`) + `notmatch` redef of `match.sc` |
| `raku_helpers.sc` | `parser_raku.sc`  | `push_interp_str`, `dq_unescape`, 9 `finish_*` counter-based variable-arity assemblers |

The sidecars exist because the helpers cannot yet be expressed as
inline `shift`/`reduce` actions — the `finish_*` ones use
`TopCounter`-driven loops and need a function body, and the leaf
constructors hold pattern-match-time literal-typing decisions. They
will fold into the SCRIP runtime or into the grammar once that side
stabilises. Today only `parser_rebus.sc` is genuinely sidecar-free
(no `rebus_helpers.sc` exists); the others still need theirs.

### Smoke test

`smoke.sc` loads the runtime and verifies a Shift/Pop round-trip plus
the global-prelude binding. `SCRIP/scripts/test_scrip.sh` runs it
with the full blob and expects two lines on stdout:

```
bar
global-OK
```

### Style — Snocone files

Today's `.sc` files are **faithful mechanical Snocone ports** of their
`.inc` source-of-truth counterparts in `corpus/programs/snobol4/demo/beauty/`.
Same control flow, same variable names, same `Pop()` no-arg signature,
same `xTrace`-gated `OUTPUT = GT(xTrace, N) ...` tracing, full BegTag/
EndTag tag stacks in `counter.sc`, `OPSYN` active in `semantic.sc`,
`global.sc` faithful to `global.inc` minus the UTF lookup table.

Faithfulness previously exposed a synthetic-label namespace collision
in the Snocone frontend (PARSER-SN-INFRA-5a) — `label_seq` in
`snocone_parse_program()` was reset to 0 per file, so co-loaded files
generated colliding `_Lend_NNNN` names and `label_lookup` resolved
gotos to the wrong target. That bug is **fixed** in `snocone_parse.y`
via a static `g_sc_label_seq` keeping the counter monotonic across the
whole scrip invocation.

## Future hosts

When SCRIP frontends get implemented in Icon, Prolog, etc., those
files land here too — `parser_snobol4.icn`, `parser_snobol4.pl`, and
so on, alongside the `.sc` versions. Cross-host crosschecking
becomes another gate: same input, same IR tree, regardless of which
host implementation produced it.

## Invariants

- All Snocone-hosted frontend drivers MUST share these seven `.sc`
  runtime files byte-for-byte. A driver that needs new shared
  infrastructure adds it to the appropriate runtime file, never to
  the driver itself.
- Runtime fixes belong here first, then propagate to
  `corpus/programs/snocone/demo/beauty/` only if the bug is also there.
  The two paths have intentionally diverged in style; do not unify.
