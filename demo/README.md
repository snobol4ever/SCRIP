# demo/ — SCRIP demos

A `.scrip` file is a **polyglot program**: the same task expressed in fenced sections per
language — ```` ```SNOBOL4 ````, ```` ```Icon ````, and so on — one document, every frontend.
Each demo directory holds the program and its `.expected` output.

## The ten demos

| Dir | Program | Task |
|---|---|---|
| `demo01` | `hello` | Hello World |
| `demo02` | `wordcount` | word counting |
| `demo03` | `roman` | Roman numerals (recursive digit-strip, after Gimpel) |
| `demo04` | `palindrome` | palindrome test |
| `demo05` | `fib` | Fibonacci |
| `demo06` | `sieve` | prime sieve |
| `demo07` | `caesar` | Caesar cipher |
| `demo08` | `sort` | sorting |
| `demo09` | `rpn` | RPN calculator |
| `demo10` | `anagram` | anagram finder |

## `family_net/` — the cross-language linkage demo

`run.sh` + `family.csv`: **SNOBOL4 parses CSV → Prolog infers → Icon formats** — three
languages calling each other through real EXPORT/IMPORT linkage. Its compiled artifacts
were removed with the retired JVM path (s267); the pipeline returns with the JVM template
encoder. The script is the demo's specification until then.

History: this folder once also carried the five canonical SNOBOL4 demo programs and
per-backend artifacts; those live canonically in `corpus/programs/snobol4/` now (full text
in git history).
