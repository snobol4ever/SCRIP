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
  generic TFPGMapKeyCompareFunc<K> = function(const __sh_a, __sh_b: K): Integer;
  generic TFPGMap<K, V> = class
  private
    __sh_keys: array of K;
    __sh_vals: array of V;
    __sh_n: Integer;
    __sh_cap: Integer;
    __sh_sorted: Boolean;
    __sh_cmp: specialize TFPGMapKeyCompareFunc<K>;
    function GetKey(__sh_i: Integer): K;
    procedure PutKey(__sh_i: Integer; const __sh_k: K);
    function GetData(__sh_i: Integer): V;
    procedure PutData(__sh_i: Integer; const __sh_v: V);
    function GetKeyData(const __sh_k: K): V;
    procedure PutKeyData(const __sh_k: K; const __sh_v: V);
    function KeyLess(const __sh_a, __sh_b: K): Boolean;
    function KeyEq(const __sh_a, __sh_b: K): Boolean;
    procedure Expand;
  public
    constructor Create;
    destructor Destroy; override;
    function Add(const __sh_k: K; const __sh_v: V): Integer;
    function Find(const __sh_k: K; out __sh_i: Integer): Boolean;
    function IndexOf(const __sh_k: K): Integer;
    procedure Delete(__sh_i: Integer);
    function Remove(const __sh_k: K): Integer;
    procedure Clear;
    property Count: Integer read __sh_n;
    property Capacity: Integer read __sh_cap;
    property Sorted: Boolean read __sh_sorted write __sh_sorted;
    property OnCompare: specialize TFPGMapKeyCompareFunc<K> read __sh_cmp write __sh_cmp;
    property Keys[__sh_i: Integer]: K read GetKey write PutKey;
    property Data[__sh_i: Integer]: V read GetData write PutData;
    property KeyData[const __sh_k: K]: V read GetKeyData write PutKeyData; default;
  end;
implementation
uses SysUtils;
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
constructor TFPGMap.Create;
begin
  inherited Create;
  __sh_n := 0;
  __sh_cap := 0;
  __sh_sorted := False;
  __sh_cmp := nil;
end;
destructor TFPGMap.Destroy;
begin
  Clear;
  inherited Destroy;
end;
function TFPGMap.GetKey(__sh_i: Integer): K;
begin
  Result := __sh_keys[__sh_i];
end;
procedure TFPGMap.PutKey(__sh_i: Integer; const __sh_k: K);
begin
  __sh_keys[__sh_i] := __sh_k;
end;
function TFPGMap.GetData(__sh_i: Integer): V;
begin
  Result := __sh_vals[__sh_i];
end;
procedure TFPGMap.PutData(__sh_i: Integer; const __sh_v: V);
begin
  __sh_vals[__sh_i] := __sh_v;
end;
function TFPGMap.KeyLess(const __sh_a, __sh_b: K): Boolean;
begin
  if Assigned(__sh_cmp) then Result := __sh_cmp(__sh_a, __sh_b) < 0
  else Result := __sh_a < __sh_b;
end;
function TFPGMap.KeyEq(const __sh_a, __sh_b: K): Boolean;
begin
  if Assigned(__sh_cmp) then Result := __sh_cmp(__sh_a, __sh_b) = 0
  else Result := __sh_a = __sh_b;
end;
procedure TFPGMap.Expand;
var inc: Integer;
begin
  if __sh_n < __sh_cap then exit;
  if __sh_cap > 127 then inc := __sh_cap shr 2
  else if __sh_cap > 8 then inc := 16
  else if __sh_cap > 3 then inc := 8
  else inc := 4;
  __sh_cap := __sh_cap + inc;
  SetLength(__sh_keys, __sh_cap);
  SetLength(__sh_vals, __sh_cap);
end;
function TFPGMap.Find(const __sh_k: K; out __sh_i: Integer): Boolean;
var lo, hi, mid: Integer;
begin
  Result := False;
  if __sh_sorted then begin
    lo := 0;
    hi := __sh_n - 1;
    while lo <= hi do begin
      mid := (lo + hi) div 2;
      if KeyEq(__sh_keys[mid], __sh_k) then begin
        __sh_i := mid;
        Result := True;
        exit;
      end;
      if KeyLess(__sh_keys[mid], __sh_k) then lo := mid + 1 else hi := mid - 1;
    end;
    __sh_i := lo;
  end else begin
    for mid := 0 to __sh_n - 1 do
      if KeyEq(__sh_keys[mid], __sh_k) then begin
        __sh_i := mid;
        Result := True;
        exit;
      end;
    __sh_i := __sh_n;
  end;
end;
function TFPGMap.IndexOf(const __sh_k: K): Integer;
var i: Integer;
begin
  if Find(__sh_k, i) then Result := i else Result := -1;
end;
function TFPGMap.Add(const __sh_k: K; const __sh_v: V): Integer;
var i, j: Integer;
begin
  if __sh_sorted then Find(__sh_k, i) else i := __sh_n;
  Expand;
  for j := __sh_n - 1 downto i do begin
    __sh_keys[j + 1] := __sh_keys[j];
    __sh_vals[j + 1] := __sh_vals[j];
  end;
  __sh_keys[i] := __sh_k;
  __sh_vals[i] := __sh_v;
  Inc(__sh_n);
  Result := i;
end;
function TFPGMap.GetKeyData(const __sh_k: K): V;
var i: Integer;
begin
  if Find(__sh_k, i) then Result := __sh_vals[i]
  else raise Exception.Create('Key not found');
end;
procedure TFPGMap.PutKeyData(const __sh_k: K; const __sh_v: V);
var i: Integer;
begin
  if Find(__sh_k, i) then __sh_vals[i] := __sh_v
  else Add(__sh_k, __sh_v);
end;
procedure TFPGMap.Delete(__sh_i: Integer);
var j: Integer;
begin
  for j := __sh_i to __sh_n - 2 do begin
    __sh_keys[j] := __sh_keys[j + 1];
    __sh_vals[j] := __sh_vals[j + 1];
  end;
  Dec(__sh_n);
end;
function TFPGMap.Remove(const __sh_k: K): Integer;
begin
  Result := IndexOf(__sh_k);
  if Result >= 0 then Delete(Result);
end;
procedure TFPGMap.Clear;
begin
  SetLength(__sh_keys, 0);
  SetLength(__sh_vals, 0);
  __sh_n := 0;
  __sh_cap := 0;
end;
end.
