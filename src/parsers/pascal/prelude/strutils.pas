unit strutils;
interface
function ReverseString(s: string): string;
function DupeString(s: string; n: integer): string;
function PosEx(sub, s: string; offset: integer): integer;
function LeftStr(s: string; n: integer): string;
function RightStr(s: string; n: integer): string;
function MidStr(s: string; start, n: integer): string;
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
end.
