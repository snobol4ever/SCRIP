unit sysutils;
interface
function IntToStr(n: integer): string;
function StrToInt(s: string): integer;
function UpperCase(s: string): string;
function LowerCase(s: string): string;
function TrimLeft(s: string): string;
function TrimRight(s: string): string;
function Trim(s: string): string;
implementation
function IntToStr(n: integer): string;
var r: string; neg: boolean;
begin
  setlength(r, 0);
  neg := n < 0;
  if neg then n := -n;
  repeat
    r := chr(ord('0') + n mod 10) + r;
    n := n div 10
  until n = 0;
  if neg then r := '-' + r;
  IntToStr := r
end;
function StrToInt(s: string): integer;
var i, v: integer; neg: boolean;
begin
  i := 1;
  v := 0;
  neg := false;
  while (i <= length(s)) and (s[i] = ' ') do i := i + 1;
  if (i <= length(s)) and ((s[i] = '-') or (s[i] = '+')) then begin neg := s[i] = '-'; i := i + 1 end;
  while (i <= length(s)) and (s[i] >= '0') and (s[i] <= '9') do begin v := v * 10 + (ord(s[i]) - ord('0')); i := i + 1 end;
  if neg then v := -v;
  StrToInt := v
end;
function UpperCase(s: string): string;
var r: string; i: integer;
begin
  r := s;
  for i := 1 to length(r) do
    if (r[i] >= 'a') and (r[i] <= 'z') then r[i] := chr(ord(r[i]) - 32);
  UpperCase := r
end;
function LowerCase(s: string): string;
var r: string; i: integer;
begin
  r := s;
  for i := 1 to length(r) do
    if (r[i] >= 'A') and (r[i] <= 'Z') then r[i] := chr(ord(r[i]) + 32);
  LowerCase := r
end;
function TrimLeft(s: string): string;
var r: string; i: integer;
begin
  i := 1;
  while (i <= length(s)) and (s[i] <= ' ') do i := i + 1;
  setlength(r, 0);
  while i <= length(s) do begin r := r + s[i]; i := i + 1 end;
  TrimLeft := r
end;
function TrimRight(s: string): string;
var r: string; i, j: integer;
begin
  j := length(s);
  while (j >= 1) and (s[j] <= ' ') do j := j - 1;
  setlength(r, 0);
  for i := 1 to j do r := r + s[i];
  TrimRight := r
end;
function Trim(s: string): string;
begin
  Trim := TrimRight(TrimLeft(s))
end;
end.
