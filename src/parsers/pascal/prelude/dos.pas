{$H+}
unit dos;
interface
uses sysutils;
const
  ReadOnly = 1;
  Hidden = 2;
  SysFile = 4;
  VolumeID = 8;
  Directory = 16;
  Archive = 32;
  AnyFile = 63;
var
  DosError: integer;
procedure GetDate(var __sh_y, __sh_m, __sh_d, __sh_w: word);
procedure GetTime(var __sh_h, __sh_m, __sh_s, __sh_c: word);
implementation
procedure GetDate(var __sh_y, __sh_m, __sh_d, __sh_w: word);
begin
  DecodeDate(Date, __sh_y, __sh_m, __sh_d);
  __sh_w := DayOfWeek(Date) - 1
end;
procedure GetTime(var __sh_h, __sh_m, __sh_s, __sh_c: word);
begin
  DecodeTime(Time, __sh_h, __sh_m, __sh_s, __sh_c);
  __sh_c := __sh_c div 10
end;
end.
