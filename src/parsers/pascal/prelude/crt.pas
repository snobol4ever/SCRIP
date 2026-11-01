unit crt;
interface
const
  Black = 0;
  Blue = 1;
  Green = 2;
  Cyan = 3;
  Red = 4;
  Magenta = 5;
  Brown = 6;
  LightGray = 7;
  DarkGray = 8;
  LightBlue = 9;
  LightGreen = 10;
  LightCyan = 11;
  LightRed = 12;
  LightMagenta = 13;
  Yellow = 14;
  White = 15;
  Blink = 128;
  BW40 = 0;
  CO40 = 1;
  BW80 = 2;
  CO80 = 3;
  Mono = 7;
  C40 = 1;
  C80 = 3;
var
  TextAttr: byte;
  LastMode: word;
  ScreenWidth: word;
  ScreenHeight: word;
  WindMin: word;
  WindMax: word;
  CheckBreak: boolean;
  CheckEOF: boolean;
  CheckSnow: boolean;
  DirectVideo: boolean;
procedure ClrScr;
procedure ClrEol;
procedure GotoXY(x, y: integer);
function WhereX: integer;
function WhereY: integer;
procedure TextColor(c: byte);
procedure TextBackground(c: byte);
procedure HighVideo;
procedure LowVideo;
procedure NormVideo;
procedure Window(x1, y1, x2, y2: integer);
procedure InsLine;
procedure DelLine;
procedure Delay(ms: integer);
function KeyPressed: boolean;
function ReadKey: char;
procedure Sound(hz: integer);
procedure NoSound;
procedure TextMode(m: integer);
procedure CursorOn;
procedure CursorOff;
procedure CursorBig;
implementation
var
  CurX, CurY: integer;
  OrgX1, OrgY1, OrgX2, OrgY2: integer;
  CrtLastAnsi: boolean;
  CrtAnsiCode: ansistring;
procedure CrtRaw(const s: ansistring);
begin
  write(s)
end;
function CrtFullWin: boolean;
begin
  CrtFullWin := (OrgX1 = 1) and (OrgY1 = 1) and (OrgX2 = ScreenWidth) and (OrgY2 = ScreenHeight)
end;
procedure CrtGoto(x, y: integer);
begin
  if y - CurY = 1 then CrtRaw(#10);
  CurX := x;
  CurY := y
end;
procedure CrtLn;
begin
  if CurY = OrgY2 then
  begin
    if CrtFullWin then
    begin
      CrtRaw(#10#13);
      CurX := OrgX1;
      CurY := OrgY2
    end
    else CrtGoto(OrgX1, OrgY2)
  end
  else CrtGoto(OrgX1, CurY + 1)
end;
procedure CrtTty(const s: ansistring);
var i: integer;
begin
  CrtRaw(s);
  for i := 1 to length(s) do
    if s[i] = #8 then
    begin
      if CurX > 1 then CurX := CurX - 1
    end
    else
    begin
      CurX := CurX + 1;
      if CurX > ScreenWidth then CurX := 255
    end
end;
function CrtSpaces(n: integer): ansistring;
var r: ansistring; i: integer;
begin
  r := '';
  for i := 1 to n do r := r + ' ';
  CrtSpaces := r
end;
function CrtPara(var h: ansistring): integer;
var j, k, code: integer;
begin
  j := pos(';', h);
  if j = 0 then j := length(h);
  k := 0;
  val(copy(h, 3, j - 3), k, code);
  if code <> 0 then k := 0;
  delete(h, 3, j - 2);
  if k = 0 then k := 1;
  CrtPara := k
end;
function CrtMin(a, b: integer): integer;
begin
  if a < b then CrtMin := a else CrtMin := b
end;
function CrtMax(a, b: integer): integer;
begin
  if a > b then CrtMax := a else CrtMax := b
end;
procedure GotoXY(x, y: integer);
begin
  if (x >= 1) and (y >= 1) and (x <= OrgX2 - OrgX1 + 1) and (y <= OrgY2 - OrgY1 + 1) then CrtGoto(x + OrgX1 - 1, y + OrgY1 - 1)
end;
procedure ClrScr;
begin
  if CrtFullWin then
  begin
    CurX := 1;
    CurY := 1
  end
  else GotoXY(1, 1)
end;
procedure ClrEol;
begin
end;
procedure CrtWrite(const s: ansistring);
var i, j, x, y, sendbytes: integer; found: boolean; c: char;
  procedure CrtSendText;
  var leftx: integer;
  begin
    while sendbytes > 0 do
    begin
      leftx := OrgX2 - CurX + 1;
      if sendbytes >= leftx then
      begin
        CrtTty(copy(s, i - sendbytes, leftx));
        sendbytes := sendbytes - leftx;
        CrtLn
      end
      else
      begin
        CrtTty(copy(s, i - sendbytes, sendbytes));
        sendbytes := 0
      end
    end
  end;
begin
  sendbytes := 0;
  i := 1;
  while i <= length(s) do
  begin
    if (s[i] = #27) or CrtLastAnsi then
    begin
      CrtSendText;
      CrtLastAnsi := false;
      j := i;
      found := false;
      while (j <= length(s)) and not found do
      begin
        c := s[j];
        found := not ((c = #27) or (c = '[') or ((c >= '0') and (c <= '9')) or (c = ';') or (c = '?'));
        j := j + 1
      end;
      CrtAnsiCode := CrtAnsiCode + copy(s, i, j - i);
      if found then
      begin
        c := CrtAnsiCode[length(CrtAnsiCode)];
        if c = 'H' then
        begin
          y := CrtPara(CrtAnsiCode);
          x := CrtPara(CrtAnsiCode);
          GotoXY(x, y)
        end
        else if c = 'J' then
        begin
          if CrtPara(CrtAnsiCode) = 2 then ClrScr
        end
        else if c = 'K' then ClrEol
        else if c = 'A' then GotoXY(CurX, CrtMax(CurY - CrtPara(CrtAnsiCode), OrgY1))
        else if c = 'B' then GotoXY(CurX, CrtMin(CurY + CrtPara(CrtAnsiCode), OrgY2))
        else if c = 'C' then GotoXY(CrtMin(CurX + CrtPara(CrtAnsiCode), OrgX2), CurY)
        else if c = 'D' then GotoXY(CrtMax(CurX - CrtPara(CrtAnsiCode), OrgX1), CurY)
        else if (c <> 'm') and (c <> 'h') then found := false
      end
      else
      begin
        CrtLastAnsi := true;
        found := true
      end;
      if not CrtLastAnsi then CrtAnsiCode := '';
      if found then i := j - 1 else sendbytes := sendbytes + 1
    end
    else
    begin
      c := s[i];
      if c = #13 then
      begin
        CrtSendText;
        CrtGoto(OrgX1, CurY)
      end
      else if c = #10 then
      begin
        CrtSendText;
        CrtLn
      end
      else if c = #9 then
      begin
        CrtSendText;
        CrtTty(CrtSpaces(9 - ((CurX - 1) div 8 mod 2) * 8))
      end
      else if c = #8 then
      begin
        CrtSendText;
        CrtTty(#8)
      end
      else sendbytes := sendbytes + 1
    end;
    i := i + 1
  end;
  if sendbytes > 0 then CrtSendText
end;
function WhereX: integer;
begin
  WhereX := CurX - OrgX1 + 1
end;
function WhereY: integer;
begin
  WhereY := CurY - OrgY1 + 1
end;
procedure TextColor(c: byte);
begin
  TextAttr := (c and 143) or (TextAttr and 112)
end;
procedure TextBackground(c: byte);
begin
  TextAttr := ((c and 7) * 16) or (TextAttr and 143)
end;
procedure HighVideo;
begin
  TextAttr := TextAttr or 8
end;
procedure LowVideo;
begin
  TextAttr := TextAttr and 247
end;
procedure NormVideo;
begin
  TextAttr := 7
end;
procedure Window(x1, y1, x2, y2: integer);
begin
  if (x1 <= x2) and (x2 <= ScreenWidth) and (y1 <= y2) and (y2 <= ScreenHeight) then
  begin
    OrgX1 := x1;
    OrgY1 := y1;
    OrgX2 := x2;
    OrgY2 := y2;
    WindMin := (y1 - 1) * 256 + (x1 - 1);
    WindMax := (y2 - 1) * 256 + (x2 - 1);
    GotoXY(1, 1)
  end
end;
procedure InsLine;
begin
end;
procedure DelLine;
begin
end;
procedure Delay(ms: integer);
begin
  sleep(ms / 1000)
end;
function KeyPressed: boolean;
begin
  KeyPressed := true
end;
function ReadKey: char;
var c: char;
begin
  if eof(input) then ReadKey := chr(0)
  else
  begin
    read(c);
    ReadKey := c
  end
end;
procedure Sound(hz: integer);
begin
end;
procedure NoSound;
begin
end;
procedure TextMode(m: integer);
begin
  LastMode := m;
  TextAttr := 7;
  ClrScr
end;
procedure CursorOn;
begin
end;
procedure CursorOff;
begin
end;
procedure CursorBig;
begin
end;
begin
  TextAttr := 7;
  LastMode := 3;
  ScreenWidth := 80;
  ScreenHeight := 25;
  WindMin := 0;
  WindMax := 24 * 256 + 79;
  CheckBreak := false;
  CheckEOF := false;
  CheckSnow := false;
  DirectVideo := true;
  CurX := 1;
  CurY := 1;
  OrgX1 := 1;
  OrgY1 := 1;
  OrgX2 := 80;
  OrgY2 := 25;
  CrtLastAnsi := false;
  CrtAnsiCode := ''
end.
