unit math;
interface
function Power(base, exponent: extended): extended;
function Floor(x: extended): integer;
function Ceil(x: extended): integer;
function Log10(x: extended): extended;
function Max(a, b: integer): integer;
function Min(a, b: integer): integer;
function FpcMaxDouble(a, b: double): double;
function FpcMinDouble(a, b: double): double;
function IntPower(base: extended; exponent: integer): extended;
procedure SinCos(theta: double; var sinv, cosv: double);
procedure SinCos(theta: extended; var sinv, cosv: extended);
function Sign(x: double): integer;
function CompareValue(a, b: double): integer;
implementation
function Power(base, exponent: extended): extended;
begin
  if exponent = 0.0 then Power := 1.0
  else if (base = 0.0) and (exponent > 0.0) then Power := 0.0
  else if (abs(exponent) <= 2147483647) and (frac(exponent) = 0.0) then Power := IntPower(base, trunc(exponent))
  else Power := exp(exponent * ln(base))
end;
function Floor(x: extended): integer;
begin
  if frac(x) < 0 then Floor := trunc(x) - 1 else Floor := trunc(x)
end;
function Ceil(x: extended): integer;
begin
  if frac(x) > 0 then Ceil := trunc(x) + 1 else Ceil := trunc(x)
end;
function Log10(x: extended): extended;
begin
  Log10 := ln(x) * 0.43429448190325182765
end;
function Max(a, b: integer): integer;
begin
  if a > b then Max := a else Max := b
end;
function Min(a, b: integer): integer;
begin
  if a < b then Min := a else Min := b
end;
function FpcMaxDouble(a, b: double): double;
begin
  if a > b then FpcMaxDouble := a else FpcMaxDouble := b
end;
function FpcMinDouble(a, b: double): double;
begin
  if a < b then FpcMinDouble := a else FpcMinDouble := b
end;
function IntPower(base: extended; exponent: integer): extended;
var i: integer; r: extended;
begin
  if (base = 0.0) and (exponent = 0) then IntPower := 1.0
  else
  begin
    if exponent < 0 then base := 1.0 / base;
    i := abs(exponent);
    r := 1.0;
    while i > 0 do
    begin
      while (i and 1) = 0 do
      begin
        i := i shr 1;
        base := sqr(base)
      end;
      i := i - 1;
      r := r * base
    end;
    IntPower := r
  end
end;
procedure SinCos(theta: double; var sinv, cosv: double); overload;
begin
  sinv := sin(theta);
  cosv := cos(theta)
end;
procedure SinCos(theta: extended; var sinv, cosv: extended); overload;
begin
  sinv := sin(theta);
  cosv := cos(theta)
end;
function Sign(x: double): integer;
begin
  if x > 0 then Sign := 1 else if x < 0 then Sign := -1 else Sign := 0
end;
function CompareValue(a, b: double): integer;
begin
  if a = b then CompareValue := 0 else if a < b then CompareValue := -1 else CompareValue := 1
end;
end.
