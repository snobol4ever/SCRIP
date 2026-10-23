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
procedure ClrScr;
begin
  CurX := 1;
  CurY := 1
end;
procedure ClrEol;
begin
end;
procedure GotoXY(x, y: integer);
begin
  if (x >= 1) and (y >= 1) and (x <= OrgX2 - OrgX1 + 1) and (y <= OrgY2 - OrgY1 + 1) then
  begin
    CurX := x;
    CurY := y
  end
end;
function WhereX: integer;
begin
  WhereX := CurX
end;
function WhereY: integer;
begin
  WhereY := CurY
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
  if (x1 >= 1) and (y1 >= 1) and (x2 >= x1) and (y2 >= y1) and (x2 <= ScreenWidth) and (y2 <= ScreenHeight) then
  begin
    OrgX1 := x1;
    OrgY1 := y1;
    OrgX2 := x2;
    OrgY2 := y2;
    CurX := 1;
    CurY := 1
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
  sleep(ms)
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
  OrgY2 := 25
end.
