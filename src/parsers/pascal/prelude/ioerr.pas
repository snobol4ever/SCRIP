{$H+}
unit ioerr;
interface
uses sysutils;
procedure __sh_iochecke;
function __sh_divchk(d: int64): int64;
function __sh_rdiv(a, b: double): double;
implementation
function __sh_divchk(d: int64): int64;
begin
  if d = 0 then raise EDivByZero.Create('Division by zero');
  __sh_divchk := d
end;
procedure __sh_iochecke;
var c: integer; e: EInOutError;
begin
  c := __pas_ioresult(0);
  if c <> 0 then begin
    e := EInOutError.Create(__pas_ioerrmsg(c));
    e.ErrorCode := c;
    raise e
  end
end;
function __sh_rdiv(a, b: double): double;
begin
  if (b = 0) and (a = a) then
    if a = 0 then raise EInvalidOp.Create('Invalid floating point operation')
    else raise EZeroDivide.Create('Floating point division by zero');
  __sh_rdiv := a / b
end;
end.
