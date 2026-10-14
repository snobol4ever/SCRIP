unit sysutils;
interface
type
  Exception = class
  private
    FMessage: string;
  public
    constructor Create(const msg: string);
    function ToString: string; override;
    property Message: string read FMessage write FMessage;
  end;
  EAbort = class(Exception);
  EConvertError = class(Exception);
  EInvalidCast = class(Exception);
  ENotImplemented = class(Exception);
  EVariantError = class(Exception);
  EInvalidOpException = class(Exception);
  EArgumentException = class(Exception);
  EArgumentOutOfRangeException = class(EArgumentException);
  EArgumentNilException = class(EArgumentException);
  EInOutError = class(Exception);
  EExternal = class(Exception);
  EIntError = class(EExternal);
  EDivByZero = class(EIntError);
  ERangeError = class(EIntError);
  EIntOverflow = class(EIntError);
  EMathError = class(EExternal);
  EInvalidOp = class(EMathError);
  EZeroDivide = class(EMathError);
  EOverflow = class(EMathError);
  EUnderflow = class(EMathError);
  EAccessViolation = class(EExternal);
  EHeapMemoryError = class(Exception);
  EInvalidPointer = class(EHeapMemoryError);
  EOutOfMemory = class(EHeapMemoryError);
function IntToStr(n: integer): string;
function StrToInt(s: string): integer;
function UpperCase(s: string): string;
function LowerCase(s: string): string;
function TrimLeft(s: string): string;
function TrimRight(s: string): string;
function Trim(s: string): string;
function StrToIntDef(s: string; d: integer): integer;
function CompareStr(a, b: string): integer;
function CompareText(a, b: string): integer;
function AnsiCompareStr(a, b: string): integer;
function AnsiCompareText(a, b: string): integer;
function SameText(a, b: string): boolean;
function QuotedStr(s: string): string;
implementation
constructor Exception.Create(const msg: string);
begin
  FMessage := msg
end;
function Exception.ToString: string;
begin
  ToString := ClassName + ': ' + FMessage
end;
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
var i, v: integer; neg, ok: boolean;
begin
  i := 1;
  v := 0;
  neg := false;
  ok := false;
  while (i <= length(s)) and (s[i] = ' ') do i := i + 1;
  if (i <= length(s)) and ((s[i] = '-') or (s[i] = '+')) then begin neg := s[i] = '-'; i := i + 1 end;
  if (i <= length(s)) and (s[i] = '$') then
  begin
    i := i + 1;
    while (i <= length(s)) and (pos(upcase(s[i]), '0123456789ABCDEF') > 0) do begin v := v * 16 + pos(upcase(s[i]), '0123456789ABCDEF') - 1; i := i + 1; ok := true end
  end
  else
    while (i <= length(s)) and (s[i] >= '0') and (s[i] <= '9') do begin v := v * 10 + (ord(s[i]) - ord('0')); i := i + 1; ok := true end;
  if not ok or (i <= length(s)) then raise EConvertError.Create('"' + s + '" is an invalid integer');
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
function StrToIntDef(s: string; d: integer): integer;
var i, v: integer; neg, ok: boolean;
begin
  i := 1;
  v := 0;
  neg := false;
  ok := false;
  while (i <= length(s)) and (s[i] = ' ') do i := i + 1;
  if (i <= length(s)) and ((s[i] = '-') or (s[i] = '+')) then begin neg := s[i] = '-'; i := i + 1 end;
  if (i <= length(s)) and (s[i] = '$') then
  begin
    i := i + 1;
    while (i <= length(s)) and (pos(upcase(s[i]), '0123456789ABCDEF') > 0) do begin v := v * 16 + pos(upcase(s[i]), '0123456789ABCDEF') - 1; i := i + 1; ok := true end
  end
  else
    while (i <= length(s)) and (s[i] >= '0') and (s[i] <= '9') do begin v := v * 10 + (ord(s[i]) - ord('0')); i := i + 1; ok := true end;
  if not ok or (i <= length(s)) then StrToIntDef := d
  else if neg then StrToIntDef := -v
  else StrToIntDef := v
end;
function CompareStr(a, b: string): integer;
var i, n: integer;
begin
  n := length(a);
  if length(b) < n then n := length(b);
  i := 1;
  while (i <= n) and (a[i] = b[i]) do i := i + 1;
  if i <= n then CompareStr := ord(a[i]) - ord(b[i])
  else CompareStr := length(a) - length(b)
end;
function AnsiCompareStr(a, b: string): integer;
begin
  AnsiCompareStr := CompareStr(a, b)
end;
function AnsiCompareText(a, b: string): integer;
begin
  AnsiCompareText := CompareText(a, b)
end;
function CompareText(a, b: string): integer;
var x, y: string; i, n: integer;
begin
  x := UpperCase(a);
  y := UpperCase(b);
  n := length(x);
  if length(y) < n then n := length(y);
  i := 1;
  while (i <= n) and (x[i] = y[i]) do i := i + 1;
  if i <= n then CompareText := ord(x[i]) - ord(y[i])
  else CompareText := length(x) - length(y)
end;
function SameText(a, b: string): boolean;
begin
  SameText := CompareText(a, b) = 0
end;
function QuotedStr(s: string): string;
var r: string; i: integer;
begin
  r := '''';
  for i := 1 to length(s) do if s[i] = '''' then r := r + '''''' else r := r + s[i];
  QuotedStr := r + ''''
end;
end.
