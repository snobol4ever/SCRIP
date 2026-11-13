{$mode objfpc}{$H+}
unit gdeque;

{ SCRIP's own generic double-ended queue (row fpc-source-gdeque-is-a-prelude-unit): the surface the fpc-source
  members use, over a dynamic array. The observable behaviour mirrors fpc 3.2.2's gdeque.pp: Insert pushes back
  and shifts right, Erase shifts left and pops back, Size is the element count. Cut from fpc, not vendored. }

interface

type
  generic TDeque<T> = class
  private
    FData: array of T;
    FCount: Integer;
  public
    constructor Create;
    function Size: Integer;
    procedure PushBack(Value: T);
    procedure Insert(Position: Integer; Value: T);
    procedure Erase(Position: Integer);
  end;

implementation

constructor TDeque.Create;
begin
  FCount := 0;
end;

function TDeque.Size: Integer;
begin
  Size := FCount;
end;

procedure TDeque.PushBack(Value: T);
begin
  if FCount = Length(FData) then
    SetLength(FData, 2 * Length(FData) + 4);
  FData[FCount] := Value;
  Inc(FCount);
end;

procedure TDeque.Insert(Position: Integer; Value: T);
var
  I: Integer;
begin
  PushBack(Value);
  for I := FCount - 1 downto Position + 1 do
    FData[I] := FData[I - 1];
  FData[Position] := Value;
end;

procedure TDeque.Erase(Position: Integer);
var
  I: Integer;
begin
  if Position <= FCount then
  begin
    if FCount > 1 then
      for I := Position to FCount - 2 do
        FData[I] := FData[I + 1];
    Dec(FCount);
  end;
end;

end.
