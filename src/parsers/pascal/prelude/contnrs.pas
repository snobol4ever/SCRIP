{$H+}
unit contnrs;
interface
type
  TFPDataHashTable = class
  private
    FKeys: array of string;
    FData: array of Pointer;
    FCount: Integer;
    function IndexOfKey(const __sh_k: string): Integer;
  public
    constructor Create;
    destructor Destroy; override;
    procedure Add(const __sh_k: string; __sh_p: Pointer);
    function Find(const __sh_k: string): Pointer;
    procedure Delete(const __sh_k: string);
    procedure Clear;
    property Count: Integer read FCount;
  end;
implementation
constructor TFPDataHashTable.Create;
begin
  FCount := 0;
  SetLength(FKeys, 0);
  SetLength(FData, 0)
end;
destructor TFPDataHashTable.Destroy;
begin
  Clear
end;
function TFPDataHashTable.IndexOfKey(const __sh_k: string): Integer;
var __sh_i: Integer; __sh_r: Integer;
begin
  __sh_r := -1;
  for __sh_i := 0 to FCount - 1 do
    if (__sh_r < 0) and (FKeys[__sh_i] = __sh_k) then __sh_r := __sh_i;
  IndexOfKey := __sh_r
end;
procedure TFPDataHashTable.Add(const __sh_k: string; __sh_p: Pointer);
begin
  if FCount = Length(FKeys) then
  begin
    SetLength(FKeys, FCount * 2 + 4);
    SetLength(FData, FCount * 2 + 4)
  end;
  FKeys[FCount] := __sh_k;
  FData[FCount] := __sh_p;
  FCount := FCount + 1
end;
function TFPDataHashTable.Find(const __sh_k: string): Pointer;
var __sh_i: Integer;
begin
  __sh_i := IndexOfKey(__sh_k);
  if __sh_i < 0 then Find := nil else Find := FData[__sh_i]
end;
procedure TFPDataHashTable.Delete(const __sh_k: string);
var __sh_i, __sh_j: Integer;
begin
  __sh_i := IndexOfKey(__sh_k);
  if __sh_i >= 0 then
  begin
    for __sh_j := __sh_i to FCount - 2 do
    begin
      FKeys[__sh_j] := FKeys[__sh_j + 1];
      FData[__sh_j] := FData[__sh_j + 1]
    end;
    FCount := FCount - 1
  end
end;
procedure TFPDataHashTable.Clear;
begin
  FCount := 0;
  SetLength(FKeys, 0);
  SetLength(FData, 0)
end;
end.
