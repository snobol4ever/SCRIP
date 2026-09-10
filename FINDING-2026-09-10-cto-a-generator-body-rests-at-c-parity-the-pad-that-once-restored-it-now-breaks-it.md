# FINDING 2026-09-10 (cto) — a generator body rests at C parity: the pad that once restored it now breaks it

Row: `icon-a-suspend-plus-a-call-through-a-procedure-value-segfaults-and-diffn-diffu-now-reach-it` (hq_R's mint on CEO-465; kept with the cto by CEO-498; CEO-501 asked for the comparison with hq_S's geddump finding).

## The fault, measured

hq_R's witness: `v1(g, n)` suspends AND calls `g(i)` through a procedure value; `helper` calls `string()`. SIGSEGV in both modes inside glibc's `__vsnprintf_internal`, at `movaps %xmm0,-0xc0(%rbp)` with `si_addr == 0` -- a general-protection fault from a misaligned 16-byte store, not a bad pointer. The stack mapping was intact; `bidlen` was not garbage (393381 is `string`'s encoded id). rsp at the call: 8 mod 16 where the ABI wants 0.

The chain, gdb-measured on `111779ed2` (exact entry rsp, `break *sym`):

| hop | v1 (crash) | v2 (no suspend, works) |
|---|---|---|
| call-value box | 0 mod 16 | 0 mod 16 |
| `rt_call_value_gen_h` entry | **0 mod 16** (`sub rsp, 8` then `call`) | 8 mod 16 (no pad) |
| `rt_proc_call_gen_h`, `rt_proc_call_open_p`, `rt_proc_enter` | 0 | 8 |
| `FN__helper` entry (after `rt_proc_enter`'s seven pushes) | **8 mod 16** | 0 mod 16 |

`FN__helper` must be entered at 0 mod 16: that is what the staged direct call gives (`push; push; jmp` from a box at 0), and what v3 (suspend, direct call) measures. The procedure-value path enters it at 8 because the C runtime was entered at 0: the box padded.

## Why the pad exists, and why it inverted

`bb_call_value.cpp` set `n2_align = icn_gen_regime() && g_emit.flat_gen` and wrapped its two runtime calls (`rt_call_value_spine_prep`, `rt_call_value_gen_h`) in `sub rsp, 8` / `add rsp, 8`. Commit `5f4b2d4c2` (2026-08-29, row icon-n2-apply-nested-coexpr) added it with the reason written in the message: *"an N-2 generator's own body deliberately rests at the old carve's 8-mod-16 parity ... not the ordinary call-ready 0-mod-16"*. Since then the N-2 word, CFO-36 (the landing words pushed after the prologue call) and hq_U's `d78d0bfca` (the PL-CALL-ALIGN pad moved across the call, carve +8) moved the generator body to 0 mod 16 like every other box: `FN__v1` on `db870418b` is entered at 0 and carves 528 (0 mod 16); on `111779ed2` it is entered at 8 and carves 680 (8 mod 16); both land the boxes at 0. The pad was a claim spanning two sites (the carve in `emit.cpp`, the call sites in the template) held by memory, and no gate held it. The crash reproduces on `db870418b`, before hq_U's move, so it is not that commit's regression; it is the pad's.

## The cure

Delete the pad: the `n2_align` predicate and its four `IF(...)` lines. Nothing else. v1, v2, v3 and a fourth witness (suspend + `g ! [i]` + `create` inside the generator, the 2026-08-29 row's own shape) print icont's output in both modes; the row's DONE-WHEN reads 0.

## The instrument

`scripts/test_gate_icn_generator_regime_calls_the_runtime_at_c_parity.sh`, wired in `make test`: four witnesses x two modes against icont's output. Proven red on the pre-cure binary (v1 and v4 crash rc=139 in both modes) and green on the cure.

## Relation to hq_S's geddump finding (CEO-501, CEO-506)

hq_S's FINDING, `.github` `FINDING-2026-09-10-hq_S-a-byrd-box-is-entered-by-jmp-so-the-sub-rsp-8-that-is-correct-for-a-call-entry-is-what-misaligns-the-runtime-call.md`, names the mechanism: the box is entered by JMP and inherits the caller's 0 mod 16, so the `sub rsp, 8` is the call-entry idiom applied to a jmp entry. hq_U's ten-line repro (a generator calling through a procedure value) and A/B on `3bbdfc8c7` measure the pad as the whole cause: pad on, `pvg`, `geddump`, `diffu`, `diffn` rc=139 both modes; pad off, `diffu` and `diffn` byte-identical to their `.std` and `geddump` (its own `.dat`, 313 lines) byte-identical, both modes. Same class: a 16-byte store at a frame off by 8. Different site: hq_S's is `rbp` at 8 mod 16 on the first generator pthread through `scrip_coexpr_activate`; this one is on the caller's own stack, in emitted code, before any co-expression exists (v1 creates none). This cure touches no runtime and does not move the pthread's initial rsp; geddump needs its own re-measure on this landing.

## The fourth arm: geddump (CEO-506)

hq_S's witness, `geddump.icn` from the IPL with `0 INDI` on stdin, segfaulted in both modes on the same pad. On the cure it prints `Run-time error 103 / File geddump.icn; Line 229 / string expected / offending value: &null` and exits 1 in both modes -- and icont, linked with the same path, prints `Run-time error 103 / File gedcom.icn; Line 229` and exits 1 on that input: the oracle's own verdict, reached where the segfault was. Two residues show in the diff: the file name is the main file's (the `ICN$LINE` hook carries `stmt_src_get_file()` of the proc's own file only at the proc's first statement, and a proc linked from `gedcom.icn` reports the linker's main file), and the per-frame traceback lines are absent (named in the report-leak FINDING).

## Measured

Five smokes green both modes; comment gate 0; Icon master both-modes 755/757 (`every_scan_replace_13`, `record_every_replace_12` the standing reds); SNOBOL4 master 1893/1917 FAIL=0 both modes. IPL-SUITE-LINE

## Residue

- The `ICN$LINE` hook (`IR_CALL` box) and the `CALL_ICON` box enter the runtime at different parities in the same body; the runtime's C code at `-O0` tolerates it and glibc does not. A parity census over every emitted `call ...@PLT` (rsp mod 16 at the call, one gdb pass per witness) is the general instrument; this gate holds the generator regime only.
- `test_gate_icn_loadfunc_returns_a_callable.sh` grepped the SNOBOL4 report text (`ERROR 216`) that `111779ed2` replaced with icont's; it went red on origin with that landing and is repaired here to accept icont's shape.
