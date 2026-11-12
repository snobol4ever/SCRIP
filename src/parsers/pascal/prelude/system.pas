{$H+}
unit system;
interface
const LineEnding = #10;
const DirectorySeparator = '/';
const DriveSeparator = '';
const ExtensionSeparator = '.';
const PathSeparator = ':';
type TMethod = record Code, Data: Pointer; end;
type TSystemCodePage = Word;
var stdout, stderr, erroutput: text;
var DefaultSystemCodePage: TSystemCodePage;
function StrLen(const sv: string): integer;
function BsrQWord(qv: int64): integer;
function BsrDWord(qv: int64): integer;
function BsfQWord(qv: int64): integer;
function BsfDWord(qv: int64): integer;
function PopCnt(qv: int64): integer;
function LowerCase(c: char): char;
function IOResult: integer;
procedure __sh_iocheck;
function __pc_lcase(const s: string): string;
implementation
function __pc_lcase(const s: string): string;
var i: integer;
begin
  __pc_lcase := s;
  for i := 1 to length(s) do
    if (s[i] >= 'A') and (s[i] <= 'Z') then __pc_lcase[i] := chr(ord(s[i]) + 32)
end;
function IOResult: integer;
begin
  IOResult := __pas_ioresult(0)
end;
procedure __sh_iocheck;
var c: integer;
begin
  c := __pas_ioresult(0);
  if c <> 0 then __pas_ioexit(c)
end;
function StrLen(const sv: string): integer;
begin
  StrLen := length(sv)
end;
function BsrQWord(qv: int64): integer;
var bn: integer;
begin
  if qv = 0 then BsrQWord := 255
  else if qv < 0 then BsrQWord := 63
  else
  begin
    bn := 0;
    while qv > 1 do begin qv := qv div 2; bn := bn + 1 end;
    BsrQWord := bn
  end
end;
function BsrDWord(qv: int64): integer;
var bn: integer;
begin
  if qv = 0 then BsrDWord := 255
  else
  begin
    if qv < 0 then qv := qv mod 4294967296 + 4294967296;
    bn := 0;
    while qv > 1 do begin qv := qv div 2; bn := bn + 1 end;
    BsrDWord := bn
  end
end;
function BsfQWord(qv: int64): integer;
var bn: integer;
begin
  if qv = 0 then BsfQWord := 255
  else
  begin
    bn := 0;
    while (qv mod 2) = 0 do begin qv := qv div 2; bn := bn + 1 end;
    BsfQWord := bn
  end
end;
function BsfDWord(qv: int64): integer;
begin
  BsfDWord := BsfQWord(qv)
end;
function PopCnt(qv: int64): integer;
var cn: integer;
begin
  cn := 0;
  if qv < 0 then
  begin
    cn := 1;
    qv := qv + 4611686018427387904;
    qv := qv + 4611686018427387904
  end;
  while qv > 0 do
  begin
    if (qv mod 2) = 1 then cn := cn + 1;
    qv := qv div 2
  end;
  PopCnt := cn
end;
function LowerCase(c: char): char;
begin
  if (c >= 'A') and (c <= 'Z') then LowerCase := chr(ord(c) + 32)
  else LowerCase := c
end;
initialization
  stdout := __pas_stdfile(3);
  stderr := __pas_stdfile(2);
  erroutput := __pas_stdfile(2);
end.
