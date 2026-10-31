{$H+}
unit ioerr;
interface
uses sysutils;
procedure __sh_iochecke;
implementation
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
end.
