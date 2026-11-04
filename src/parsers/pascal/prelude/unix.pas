{$H+}
unit unix;
interface
function GetHostName: string;
function GetDomainName: string;
implementation
function __sh_ukread(const __sh_p: string): string;
var __sh_f: text; __sh_l: string;
begin
  __sh_l := '';
  assign(__sh_f, __sh_p);
  reset(__sh_f);
  if not eof(__sh_f) then readln(__sh_f, __sh_l);
  close(__sh_f);
  __sh_ukread := __sh_l
end;
function GetHostName: string;
begin
  GetHostName := __sh_ukread('/proc/sys/kernel/hostname')
end;
function GetDomainName: string;
begin
  GetDomainName := __sh_ukread('/proc/sys/kernel/domainname')
end;
end.
