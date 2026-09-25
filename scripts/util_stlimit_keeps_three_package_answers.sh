#!/usr/bin/env bash
# util_stlimit_keeps_three_package_answers.sh -- the DONE-WHEN of row snobol4-the-graders-stlimit-switch-changes-three-package-answers-tictacto-treeread-hsort
# (ceo 2026-09-25, CEO-1261 package pass): gimpel TICTACTO_driver and TREEREAD_driver and aisnobol HSORT answer the oracle with the
# graders' --stlimit switch OFF and not with it ON; this runs them ON. rc 0 all three answer, 1 a red named, 2 could not run.
export SCRIP_SNO_STMTKW=1
cd "${S4E_HOME:-/home/claude_ceo}/corpus/packages/snobol4/gimpel" || exit 2
for p in TICTACTO TREEREAD; do i=/dev/null; [ -f ${p}_driver.in ] && i=${p}_driver.in; diff -q <(SNO_LIB=.:include timeout 60 "${S4E_HOME:-/home/claude_ceo}/SCRIP/scrip" ${p}_driver.sno < $i 2>/dev/null) ${p}_driver.ref > /dev/null || { echo "RED $p"; exit 1; }; done
cd ../aisnobol || exit 2
diff -q <(timeout 60 "${S4E_HOME:-/home/claude_ceo}/SCRIP/scrip" --run -u HSORT.IN HSORT.sno -- HSORT.IN < /dev/null 2>/dev/null) <(/home/resources/x64/bin/sbl -bf -u HSORT.IN HSORT.sno HSORT.IN < /dev/null 2>/dev/null) > /dev/null || { echo "RED HSORT"; exit 1; }
echo "GREEN: TICTACTO, TREEREAD and HSORT answer the oracle"; exit 0
