{$H+}
unit ioerr;
interface
uses sysutils;
procedure __sh_iochecke;
implementation
procedure __sh_iochecke;
var c: integer;
begin
  c := __pas_ioresult(0);
  if c <> 0 then raise EInOutError.Create(__pas_ioerrmsg(c))
end;
end.
