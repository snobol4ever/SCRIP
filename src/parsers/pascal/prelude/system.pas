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
const vtInteger = 0;
const vtBoolean = 1;
const vtChar = 2;
const vtExtended = 3;
const vtString = 4;
const vtPointer = 5;
const vtPChar = 6;
const vtObject = 7;
const vtClass = 8;
const vtAnsiString = 11;
const vtCurrency = 12;
const vtInt64 = 16;
const vtQWord = 17;
const vtUnicodeString = 18;
type PExtended = ^Extended;
type TVarRec = record VType: Byte; VInteger: LongInt; VBoolean: Boolean; VChar: Char; VExtended: PExtended; VString: Pointer; VPointer: Pointer; VPChar: Pointer; VObject: TObject; VClass: Pointer; VAnsiString: Pointer; VCurrency: Pointer; VInt64: Int64; VQWord: QWord; VUnicodeString: Pointer; end;
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
function __pc_vr_int(x: Int64): TVarRec;
function __pc_vr_int64(x: Int64): TVarRec;
function __pc_vr_bool(x: Boolean): TVarRec;
function __pc_vr_char(x: Char): TVarRec;
function __pc_vr_ext(x: Extended): TVarRec;
function __pc_vr_str(const x: AnsiString): TVarRec;
function __pc_vr_sstr(const x: ShortString): TVarRec;
function __pc_vr_ptr(x: Pointer): TVarRec;
function __pc_vr_obj(x: TObject): TVarRec;
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
function __pc_vr_int(x: Int64): TVarRec;
var r: TVarRec;
begin
  r.VType := vtInteger; r.VInteger := x; __pc_vr_int := r
end;
function __pc_vr_int64(x: Int64): TVarRec;
var r: TVarRec;
begin
  r.VType := vtInt64; r.VInt64 := x; __pc_vr_int64 := r
end;
function __pc_vr_bool(x: Boolean): TVarRec;
var r: TVarRec;
begin
  r.VType := vtBoolean; r.VBoolean := x; __pc_vr_bool := r
end;
function __pc_vr_char(x: Char): TVarRec;
var r: TVarRec;
begin
  r.VType := vtChar; r.VChar := x; __pc_vr_char := r
end;
function __pc_vr_ext(x: Extended): TVarRec;
var r: TVarRec; e: PExtended;
begin
  new(e); e^ := x; r.VType := vtExtended; r.VExtended := e; __pc_vr_ext := r
end;
function __pc_vr_str(const x: AnsiString): TVarRec;
var r: TVarRec;
begin
  r.VType := vtAnsiString; r.VAnsiString := Pointer(x); __pc_vr_str := r
end;
function __pc_vr_sstr(const x: ShortString): TVarRec;
var r: TVarRec;
begin
  r.VType := vtString; r.VString := Pointer(x); __pc_vr_sstr := r
end;
function __pc_vr_ptr(x: Pointer): TVarRec;
var r: TVarRec;
begin
  r.VType := vtPointer; r.VPointer := x; __pc_vr_ptr := r
end;
function __pc_vr_obj(x: TObject): TVarRec;
var r: TVarRec;
begin
  r.VType := vtObject; r.VObject := x; __pc_vr_obj := r
end;
initialization
  stdout := __pas_stdfile(3);
  stderr := __pas_stdfile(2);
  erroutput := __pas_stdfile(2);
end.
