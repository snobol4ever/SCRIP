unit strutils;
interface
function ReverseString(s: string): string;
function DupeString(s: string; n: integer): string;
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
end.
