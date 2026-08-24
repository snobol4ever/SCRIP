#!/usr/bin/env bash
# jcon_selfhost_build.sh -- bootstrap the full SCRIP JCON self-host toolchain from a fresh container.
# Builds: scrip (-O0), libscrip_rt.so (-O0 for feature work / -O2 for perf), the 17-module SCRIP-jtran,
# the 2-module SCRIP-jlink, and the JCON Java runtime jcon.zip. All artifacts land in $JT (default /home/claude/jt).
# Feature work uses -O0 (RULES: -O2 only for performance work); pass PERF=1 for the -O2 runtime build.
# Usage: bash scripts/jcon_selfhost_build.sh            # feature build (-O0 runtime)
#        PERF=1 bash scripts/jcon_selfhost_build.sh     # perf build (-O2 runtime)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -euo pipefail

SCRIP=${SCRIP:-$S4A/work/SCRIP}
CORPUS=${CORPUS:-$S4A/work/corpus}
JCONREPO=${JCONREPO:-$S4A/work/jcon-master/jcon-master}
ICONREPO=${ICONREPO:-$S4A/work/icon-master/icon-master}
JT=${JT:-$S4A/jt}
RT="$SCRIP/out"
JCOMPILER="$CORPUS/icon/jcon-compiler"
SEMI="$SCRIP/tools/semicolonize_icon.py"
if [ "${PERF:-0}" = "1" ]; then RTOPT='-O2 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer';
else RTOPT='-O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer'; fi

echo "== [1/7] link repos to expected paths + refs =="
ln -sfn "$SCRIP" $S4E/SCRIP; ln -sfn "$CORPUS" $S4E/corpus
mkdir -p "$SCRIP/refs"; ln -sfn "$JCONREPO" "$SCRIP/refs/jcon-master"; ln -sfn "$ICONREPO" "$SCRIP/refs/icon-master"

echo "== [2/7] system packages (build tools + JDK for jcon.zip) =="
bash "$SCRIP/scripts/install_system_packages.sh" >/dev/null 2>&1 || true
command -v javac >/dev/null 2>&1 || apt-get install -y openjdk-21-jdk-headless >/dev/null 2>&1

echo "== [3/7] build scrip (-O0 from CBASE) =="
( cd "$SCRIP" && rm -f scrip && make -j"$(nproc)" scrip >/tmp/jcon_build_scrip.log 2>&1 )
[ -x "$SCRIP/scrip" ] || { echo "FAIL scrip"; tail -15 /tmp/jcon_build_scrip.log; exit 1; }

echo "== [4/7] build libscrip_rt.so  RT_OPT='$RTOPT' =="
( cd "$SCRIP" && make -j"$(nproc)" RT_OPT="$RTOPT" libscrip_rt >/tmp/jcon_build_rt.log 2>&1 )
[ -f "$RT/libscrip_rt.so" ] || { echo "FAIL libscrip_rt"; tail -15 /tmp/jcon_build_rt.log; exit 1; }

echo "== [5/7] stage 17 jtran modules in $JT (2 generated via SCRIP + semicolonized) =="
mkdir -p "$JT/tmp"; cp "$JCOMPILER"/*.icn "$JT"/
"$SCRIP/scrip" --run "$JT/oplexgen.icn"      </dev/null >"$JT/do_ops_raw.icn"    2>/dev/null
"$SCRIP/scrip" --run "$JT/interfacegen.icn"  </dev/null >"$JT/interface_raw.icn" 2>/dev/null
python3 "$SEMI" "$JT/do_ops_raw.icn"    "$JT/do_ops.icn"
python3 "$SEMI" "$JT/interface_raw.icn" "$JT/interface.icn"

echo "== [6/7] compile+link SCRIP-jtran (17 mods) and SCRIP-jlink (2 mods) =="
TRANSRC="dump.icn do_ops.icn preprocessor.icn lexer.icn ast.icn parse.icn ir.icn keyword.icn irgen.icn gen_bc.icn gen_symbolic.icn gen_dot.icn gen_ucode.icn optimize.icn interface.icn bytecode.icn jtran_main.icn"
LINKSRC="linker.icn bytecode.icn"
( cd "$JT" && SCRIP_BETA_ELIDE_OFF=1 "$SCRIP/scrip" --compile $TRANSRC </dev/null >jtran.s 2>jtran_compile.err )
gcc -no-pie "$JT/jtran.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -o "$JT/jtran" 2>/dev/null
( cd "$JT" && SCRIP_BETA_ELIDE_OFF=1 "$SCRIP/scrip" --compile $LINKSRC </dev/null >jlink.s 2>jlink_compile.err )
gcc -no-pie "$JT/jlink.s" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -o "$JT/jlink" 2>/dev/null
[ -x "$JT/jtran" ] && [ -x "$JT/jlink" ] || { echo "FAIL jtran/jlink link"; exit 1; }

echo "== [7/7] build JCON Java runtime jcon.zip (iTrampoline via SCRIP + javac + jar) =="
JD="$JCONREPO/jcon"
python3 "$SEMI" "$JD/trampgen.icn" /tmp/trampgen_sc.icn
"$SCRIP/scrip" --run /tmp/trampgen_sc.icn </dev/null >"$JD/iTrampoline.java" 2>/dev/null
( cd "$JD" && CLASSPATH=.. javac -O *.java 2>/dev/null )
mkdir -p "$JCONREPO/bin"
( cd "$JCONREPO" && jar cfM bin/jcon.zip jcon/*.class )

echo "== DONE =="
echo "jtran:    $JT/jtran    ($(wc -l <"$JT/jtran.s") asm lines)"
echo "jlink:    $JT/jlink    ($(wc -l <"$JT/jlink.s") asm lines)"
echo "jcon.zip: $JCONREPO/bin/jcon.zip"
echo "libscrip_rt.so RT_OPT: $RTOPT"
