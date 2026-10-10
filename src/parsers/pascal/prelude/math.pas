unit math;
interface
function Power(base, exponent: real): real;
function Floor(x: real): integer;
function Ceil(x: real): integer;
function Log10(x: real): real;
function Max(a, b: integer): integer;
function Min(a, b: integer): integer;
implementation
function Power(base, exponent: real): real;
var r: real; k, i: integer;
begin
  if exponent = trunc(exponent) then
  begin
    k := trunc(exponent);
    r := 1.0;
    for i := 1 to abs(k) do r := r * base;
    if k < 0 then r := 1.0 / r;
    Power := r
  end
  else Power := exp(exponent * ln(base))
end;
function Floor(x: real): integer;
begin
  if (x < 0) and (x <> trunc(x)) then Floor := trunc(x) - 1 else Floor := trunc(x)
end;
function Ceil(x: real): integer;
begin
  if (x > 0) and (x <> trunc(x)) then Ceil := trunc(x) + 1 else Ceil := trunc(x)
end;
function Log10(x: real): real;
begin
  Log10 := ln(x) / ln(10.0)
end;
function Max(a, b: integer): integer;
begin
  if a > b then Max := a else Max := b
end;
function Min(a, b: integer): integer;
begin
  if a < b then Min := a else Min := b
end;
end.
