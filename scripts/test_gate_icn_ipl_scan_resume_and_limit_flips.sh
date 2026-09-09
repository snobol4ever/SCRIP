R="$(cd "$(dirname "$0")" && pwd)"; R=/home/claude_R
. "$R/SCRIP/scripts/lib_icon_ipl_isolation.sh"
PKG="$R/corpus/packages/icon/ipl"; SCRIP="$R/SCRIP/scrip"
T=$(mktemp -d); trap 'rm -rf "$T"; ipl_isolation_cleanup' EXIT
ipl_isolation_init "$PKG"
rc=0
for base in datmerge ibrow miu; do
  icn=$(ls "$PKG"/*/"$base".icn | head -1); std="${icn%.icn}.std"
  declare -a A=(); ipl_argv_read "$icn" A
  export IPL_ISO_SUBDIR="$(basename "$(dirname "$std")")" IPL_ISO_FIXTURES="$icn"
  sin=/dev/null; [ -f "${icn%.icn}.in" ] && sin="${icn%.icn}.in"
  if [ "${#A[@]}" -gt 0 ]; then ipl_isolation_run "$T/$base.m3" 60 "$sin" "$SCRIP" --run "$icn" -- "${A[@]}"; else ipl_isolation_run "$T/$base.m3" 60 "$sin" "$SCRIP" --run "$icn"; fi
  diff -q "$std" "$T/$base.m3" >/dev/null || { echo "RED $base m3"; rc=1; }
  "$SCRIP" --compile "$icn" > "$T/$base.s" 2>/dev/null </dev/null
  gcc -no-pie "$T/$base.s" -L"$R/SCRIP/out" -lscrip_rt -Wl,-rpath,"$R/SCRIP/out" -o "$T/$base.bin" 2>/dev/null
  if [ "${#A[@]}" -gt 0 ]; then ipl_isolation_run "$T/$base.m4" 60 "$sin" "$T/$base.bin" "${A[@]}"; else ipl_isolation_run "$T/$base.m4" 60 "$sin" "$T/$base.bin"; fi
  diff -q "$std" "$T/$base.m4" >/dev/null || { echo "RED $base m4"; rc=1; }
  [ $rc -eq 0 ] && echo "GREEN $base m3+m4"
done
exit $rc
