unit objects;
interface
const
  stOk = 0;
  stError = -1;
  stInitError = -2;
  stReadError = -3;
  stWriteError = -4;
  stGetError = -5;
  stPutError = -6;
  stSeekError = -7;
  stOpenError = -8;
  stCreate = $3C00;
  stOpenRead = $3D00;
  stOpenWrite = $3D01;
  stOpen = $3D02;
  coIndexError = -1;
  coOverflow = -2;
type
  TObject = object
    constructor Init;
    destructor Done;
  end;
  PObject = ^TObject;
  TStream = object(TObject)
    Status: integer;
    ErrorInfo: integer;
    StreamSize: longint;
    Position: longint;
    constructor Init;
    destructor Done;
    function GetSize: longint;
    function GetPos: longint;
    procedure Seek(Pos: longint);
    procedure Error(Code, Info: integer);
  end;
  PStream = ^TStream;
var
  StreamError: pointer;
implementation
constructor TObject.Init;
begin
end;
destructor TObject.Done;
begin
end;
constructor TStream.Init;
begin
  Status := stOk;
  ErrorInfo := 0;
  StreamSize := 0;
  Position := 0
end;
destructor TStream.Done;
begin
end;
function TStream.GetSize: longint;
begin
  GetSize := StreamSize
end;
function TStream.GetPos: longint;
begin
  GetPos := Position
end;
procedure TStream.Seek(Pos: longint);
begin
  Position := Pos
end;
procedure TStream.Error(Code, Info: integer);
begin
  Status := Code;
  ErrorInfo := Info
end;
end.
