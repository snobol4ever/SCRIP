# SCRIP Pascal: the ISO 7185 errors it does not report

This is the accompanying document ISO 7185:1990 requires of a processor that leaves an error undetected. Section 3 defines an *error* as "a violation by a program of the requirements of this International Standard that a processor is permitted to leave undetected". Section 5.1 (f) lets a complying processor treat each error in one of three ways; way (1) is "there shall be a statement in an accompanying document that the error is not reported, and a note referencing each such statement shall appear in a separate section of the accompanying document". Every row below is such a statement for SCRIP's Pascal frontend, and the PAT program that carries the violation is graded as an acceptance test against `fpc -Miso` (the oracle, which leaves the same error undetected) through `corpus/packages/pascal/pat/ISO_ACCEPTS.tsv`.

Everything SCRIP can detect it reports, and the PAT rejection tests grade that. This list is the remainder, kept short on purpose.

## Ruled by the ceo

| Clause | Error left unreported | PAT program | The oracle (`fpc -Miso` 3.2.2) | Ruling |
|---|---|---|---|---|
| 6.9.3.1 | a `TotalWidth` below one in a `write` parameter | `iso7185prt1758a`, `iso7185prt1839` | accepts, rc 0; SCRIP prints byte-identically in both modes | CEO-1301 |
| 6.9.3.1 | a `FracDigits` below one in a `write` parameter | `iso7185prt1758b`, `iso7185prt1840` | accepts, rc 0; SCRIP prints byte-identically in both modes | CEO-1301 |
| 6.7.1 | the use of an undefined variable (a variable no statement has assigned): the use is not detected in general; a variable that no assignment reaches reads as zero | `iso7185prt1743` | accepts, rc 0, prints a zero; SCRIP prints the same in both modes | cfo yes on the CEO-1301 precedent, 2026-10-09 (corpus `218d94be5`); the ceo may overrule |

SCRIP does detect, and reports, several neighbours of these: the function whose result is never assigned (6.6.2, PAT 1918), the control variable of a `for` statement used after the statement (6.8.3.9, PAT 1811), a variant read after the tag changed (6.5.3.3, PAT 1851), a file buffer or pointer target whose owner is altered while a reference exists (6.5.5, 6.5.4, PAT 1706a, 1706b, 1705).

## Proposed, awaiting the ceo (not ruled)

| Clause | Error | PAT program | What is measured |
|---|---|---|---|
| 6.5.3.3 case C | a read of an undiscriminated variant after a write of another arm | `iso7185prt1702c` | the oracle accepts, rc 0, prints nothing; SCRIP the same. Detecting it would break fpc-graded master entries that pun across arms on purpose (`test_tprec4`, `test_tprec12`), so the proposal is to list it here and grade 1702c as an acceptance test. |
| 6.5.3.3 case D | a write of an undiscriminated variant while a reference to another arm exists | `iso7185prt1702d` | the oracle accepts and prints `i: 99` (it overlays the character's byte onto the integer); SCRIP prints `i: 1` (its arms of different class do not share a slot), so it cannot be graded as an acceptance test against the oracle. It needs a decision: detect case D at the call (a variable actual that is a field of an undiscriminated variant, a callee that writes another arm of the same record), or rule the program ungradable. |

A row moves from "proposed" to "ruled" only on the ceo's ruling, and a program leaves this document the day SCRIP reports its error.
