unit strutils;
interface
function ReverseString(s: string): string;
function DupeString(s: string; n: integer): string;
function PosEx(sub, s: string; offset: integer): integer;
function LeftStr(s: string; n: integer): string;
function RightStr(s: string; n: integer): string;
function MidStr(s: string; start, n: integer): string;
function IntToBin(vl: int64; dg: integer): string;
type TSysCharSet = set of char;
function WordPosition(wn: integer; const sv: string; const wd: TSysCharSet): integer;
function ExtractWordPos(wn: integer; const sv: string; const wd: TSysCharSet; var pn: integer): string;
function ExtractWord(wn: integer; const sv: string; const wd: TSysCharSet): string;
function WordCount(const sv: string; const wd: TSysCharSet): integer;
function Dec2Numb(nv: int64; ln, bs: integer): string;
implementation
function ReverseString(s: string): string;
var r: string; i: integer;
begin
  setlength(r, 0);
  for i := length(s) downto 1 do r := r + s[i];
  ReverseString := r
end;
function DupeString(s: string; n: integer): string;
var r: string; i: integer;
begin
  setlength(r, 0);
  for i := 1 to n do r := r + s;
  DupeString := r
end;
function PosEx(sub, s: string; offset: integer): integer;
var i, j, r: integer; ok: boolean;
begin
  r := 0;
  i := offset;
  if offset < 1 then i := length(s) + 1;
  while (r = 0) and (length(sub) > 0) and (i + length(sub) - 1 <= length(s)) do
  begin
    ok := true;
    j := 1;
    while ok and (j <= length(sub)) do begin ok := s[i + j - 1] = sub[j]; j := j + 1 end;
    if ok then r := i;
    i := i + 1
  end;
  PosEx := r
end;
function LeftStr(s: string; n: integer): string;
begin
  LeftStr := copy(s, 1, n)
end;
function RightStr(s: string; n: integer): string;
begin
  if n > length(s) then n := length(s);
  RightStr := copy(s, length(s) - n + 1, n)
end;
function MidStr(s: string; start, n: integer): string;
begin
  MidStr := copy(s, start, n)
end;
function IntToBin(vl: int64; dg: integer): string;
var rs: string; bt: integer; ng: boolean;
begin
  setlength(rs, 0);
  ng := vl < 0;
  if ng then begin vl := vl + 4611686018427387904; vl := vl + 4611686018427387904 end;
  for bt := 0 to dg - 1 do
  begin
    if (bt >= 63) and ng then rs := '1' + rs
    else if (vl mod 2) = 1 then rs := '1' + rs
    else rs := '0' + rs;
    vl := vl div 2
  end;
  IntToBin := rs
end;
function WordPosition(wn: integer; const sv: string; const wd: TSysCharSet): integer;
var ix, cn, ls, rp: integer;
begin
  ix := 1; cn := 0; ls := length(sv); rp := 0;
  while (ix <= ls) and (cn <> wn) do
  begin
    while (ix <= ls) and (sv[ix] in wd) do ix := ix + 1;
    if ix <= ls then cn := cn + 1;
    if cn <> wn then
      while (ix <= ls) and not (sv[ix] in wd) do ix := ix + 1
    else rp := ix
  end;
  WordPosition := rp
end;
function ExtractWordPos(wn: integer; const sv: string; const wd: TSysCharSet; var pn: integer): string;
var ix, jx: integer;
begin
  ix := WordPosition(wn, sv, wd);
  pn := ix;
  if ix = 0 then ExtractWordPos := ''
  else
  begin
    jx := ix;
    while (jx <= length(sv)) and not (sv[jx] in wd) do jx := jx + 1;
    ExtractWordPos := copy(sv, ix, jx - ix)
  end
end;
function ExtractWord(wn: integer; const sv: string; const wd: TSysCharSet): string;
var pn: integer;
begin
  ExtractWord := ExtractWordPos(wn, sv, wd, pn)
end;
function WordCount(const sv: string; const wd: TSysCharSet): integer;
var ix, cn, ls: integer; inw: boolean;
begin
  cn := 0; ls := length(sv); inw := false;
  for ix := 1 to ls do
    if sv[ix] in wd then inw := false
    else
    begin
      if not inw then cn := cn + 1;
      inw := true
    end;
  WordCount := cn
end;
function Dec2Numb(nv: int64; ln, bs: integer): string;
var rs: string; dg: integer;
begin
  setlength(rs, 0);
  if nv = 0 then rs := '0';
  while nv > 0 do
  begin
    dg := nv mod bs;
    if dg < 10 then rs := chr(48 + dg) + rs else rs := chr(55 + dg) + rs;
    nv := nv div bs
  end;
  while length(rs) < ln do rs := '0' + rs;
  Dec2Numb := rs
end;
end.
