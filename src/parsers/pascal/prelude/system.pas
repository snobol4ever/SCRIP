unit system;
interface
function StrLen(const sv: string): integer;
function BsrQWord(qv: int64): integer;
function BsrDWord(qv: int64): integer;
function BsfQWord(qv: int64): integer;
function BsfDWord(qv: int64): integer;
function PopCnt(qv: int64): integer;
implementation
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
end.
