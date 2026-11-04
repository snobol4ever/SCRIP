{$mode objfpc}{$H+}
unit fgl;
interface
type
  generic TFPGListCompareFunc<T> = function(const __sh_a, __sh_b: T): Integer;
  generic TFPGList<T> = class
  private
    __sh_items: array of T;
    __sh_n: Integer;
    function GetItem(__sh_i: Integer): T;
    procedure PutItem(__sh_i: Integer; const __sh_v: T);
    procedure SetCount(__sh_c: Integer);
  public
    constructor Create;
    destructor Destroy; override;
    function Add(const __sh_v: T): Integer;
    procedure Insert(__sh_i: Integer; const __sh_v: T);
    procedure Delete(__sh_i: Integer);
    function Remove(const __sh_v: T): Integer;
    function IndexOf(const __sh_v: T): Integer;
    function Extract(const __sh_v: T): T;
    procedure Exchange(__sh_i, __sh_j: Integer);
    procedure Clear;
    function First: T;
    function Last: T;
    procedure Sort(__sh_cmp: specialize TFPGListCompareFunc<T>);
    property Count: Integer read __sh_n write SetCount;
    property Items[__sh_i: Integer]: T read GetItem write PutItem; default;
  end;
implementation
constructor TFPGList.Create;
begin
  inherited Create;
  __sh_n := 0;
end;
destructor TFPGList.Destroy;
begin
  Clear;
  inherited Destroy;
end;
function TFPGList.GetItem(__sh_i: Integer): T;
begin
  Result := __sh_items[__sh_i];
end;
procedure TFPGList.PutItem(__sh_i: Integer; const __sh_v: T);
begin
  __sh_items[__sh_i] := __sh_v;
end;
procedure TFPGList.SetCount(__sh_c: Integer);
begin
  if __sh_c > Length(__sh_items) then SetLength(__sh_items, __sh_c);
  __sh_n := __sh_c;
end;
function TFPGList.Add(const __sh_v: T): Integer;
begin
  if __sh_n >= Length(__sh_items) then SetLength(__sh_items, __sh_n * 2 + 4);
  __sh_items[__sh_n] := __sh_v;
  Result := __sh_n;
  Inc(__sh_n);
end;
procedure TFPGList.Insert(__sh_i: Integer; const __sh_v: T);
var k: Integer;
begin
  Add(__sh_v);
  for k := __sh_n - 1 downto __sh_i + 1 do __sh_items[k] := __sh_items[k - 1];
  __sh_items[__sh_i] := __sh_v;
end;
procedure TFPGList.Delete(__sh_i: Integer);
var k: Integer;
begin
  for k := __sh_i to __sh_n - 2 do __sh_items[k] := __sh_items[k + 1];
  Dec(__sh_n);
end;
function TFPGList.IndexOf(const __sh_v: T): Integer;
var k: Integer;
begin
  Result := -1;
  for k := 0 to __sh_n - 1 do
    if __sh_items[k] = __sh_v then begin
      Result := k;
      exit;
    end;
end;
function TFPGList.Remove(const __sh_v: T): Integer;
begin
  Result := IndexOf(__sh_v);
  if Result >= 0 then Delete(Result);
end;
function TFPGList.Extract(const __sh_v: T): T;
var k: Integer;
begin
  k := IndexOf(__sh_v);
  if k >= 0 then begin
    Result := __sh_items[k];
    Delete(k);
  end else Result := __sh_v;
end;
procedure TFPGList.Exchange(__sh_i, __sh_j: Integer);
var __sh_t: T;
begin
  __sh_t := __sh_items[__sh_i];
  __sh_items[__sh_i] := __sh_items[__sh_j];
  __sh_items[__sh_j] := __sh_t;
end;
procedure TFPGList.Clear;
begin
  SetLength(__sh_items, 0);
  __sh_n := 0;
end;
function TFPGList.First: T;
begin
  Result := __sh_items[0];
end;
function TFPGList.Last: T;
begin
  Result := __sh_items[__sh_n - 1];
end;
procedure TFPGList.Sort(__sh_cmp: specialize TFPGListCompareFunc<T>);
var i, j: Integer; __sh_t: T;
begin
  for i := 1 to __sh_n - 1 do begin
    __sh_t := __sh_items[i];
    j := i - 1;
    while (j >= 0) and (__sh_cmp(__sh_items[j], __sh_t) > 0) do begin
      __sh_items[j + 1] := __sh_items[j];
      j := j - 1;
    end;
    __sh_items[j + 1] := __sh_t;
  end;
end;
end.
