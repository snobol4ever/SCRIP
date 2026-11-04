{$mode objfpc}{$H+}
unit generics.collections;
interface
type
  TDictionaryOwnership = (doOwnsKeys, doOwnsValues);
  TDictionaryOwnerships = set of TDictionaryOwnership;
  generic TPair<K, V> = record
    Key: K;
    Value: V;
  end;
  generic TEnumList<T> = class
  private
    __sh_a: array of T;
    __sh_i: Integer;
  public
    constructor Create(const __sh_src: array of T; __sh_n: Integer);
    function MoveNext: Boolean;
    function GetCurrent: T;
    property Current: T read GetCurrent;
  end;
  generic TEnumPairs<K, V> = class
  private
    __sh_k: array of K;
    __sh_v: array of V;
    __sh_i: Integer;
  public
    constructor Create(const __sh_ks: array of K; const __sh_vs: array of V; __sh_n: Integer);
    function MoveNext: Boolean;
    function GetCurrent: specialize TPair<K, V>;
    property Current: specialize TPair<K, V> read GetCurrent;
  end;
  generic TItemList<T> = class
  private
    __sh_a: array of T;
    __sh_n: Integer;
  public
    constructor Create(const __sh_src: array of T; __sh_cnt: Integer);
    function GetEnumerator: specialize TEnumList<T>;
    function ToArray: specialize TArray<T>;
    property Count: Integer read __sh_n;
  end;
  generic TDictionary<K, V> = class
  private
    __sh_h: array of longword;
    __sh_k: array of K;
    __sh_v: array of V;
    __sh_n: Integer;
    __sh_thr: Integer;
    __sh_own: TDictionaryOwnerships;
    function FindSlot(const __sh_key: K; var __sh_hv: longword): Integer;
    procedure Rehash(__sh_size: Integer);
    procedure Prepare;
    procedure RemoveAt(__sh_at: Integer);
    function GetItem(const __sh_key: K): V;
    procedure SetItem(const __sh_key: K; const __sh_val: V);
  public
    constructor Create; overload;
    constructor Create(__sh_cap: Integer); overload;
    constructor Create(__sh_o: TDictionaryOwnerships); overload;
    destructor Destroy; override;
    procedure Add(const __sh_key: K; const __sh_val: V);
    procedure AddOrSetValue(const __sh_key: K; const __sh_val: V);
    function TryAdd(const __sh_key: K; const __sh_val: V): Boolean;
    function ContainsKey(const __sh_key: K): Boolean;
    function TryGetValue(const __sh_key: K; out __sh_val: V): Boolean;
    procedure Remove(const __sh_key: K);
    procedure Clear;
    function GetEnumerator: specialize TEnumPairs<K, V>;
    function Keys: specialize TItemList<K>;
    function Values: specialize TItemList<V>;
    property Count: Integer read __sh_n;
    property Items[const __sh_key: K]: V read GetItem write SetItem; default;
  end;
  generic TObjectDictionary<K, V> = class(specialize TDictionary<K, V>)
  public
    constructor Create(__sh_o: TDictionaryOwnerships); overload;
  end;
  generic THashSet<T> = class
  private
    __sh_d: specialize TDictionary<T, Boolean>;
  public
    constructor Create;
    destructor Destroy; override;
    function Add(const __sh_v: T): Boolean;
    function Remove(const __sh_v: T): Boolean;
    function Contains(const __sh_v: T): Boolean;
    procedure Clear;
    function GetEnumerator: specialize TEnumList<T>;
    function ToArray: specialize TArray<T>;
    function GetCount: Integer;
    property Count: Integer read GetCount;
  end;
  generic TQueue<T> = class
  private
    __sh_a: array of T;
    __sh_head: Integer;
    __sh_n: Integer;
  public
    constructor Create;
    procedure Enqueue(const __sh_v: T);
    function Dequeue: T;
    function Peek: T;
    procedure Clear;
    property Count: Integer read __sh_n;
  end;
  generic TStack<T> = class
  private
    __sh_a: array of T;
    __sh_n: Integer;
  public
    constructor Create;
    procedure Push(const __sh_v: T);
    function Pop: T;
    function Peek: T;
    procedure Clear;
    property Count: Integer read __sh_n;
  end;
  generic TList<T> = class
  private
    __sh_a: array of T;
    __sh_n: Integer;
    function GetItem(__sh_i: Integer): T;
    procedure SetItem(__sh_i: Integer; const __sh_v: T);
  public
    constructor Create;
    function Add(const __sh_v: T): Integer;
    procedure Insert(__sh_i: Integer; const __sh_v: T);
    procedure Delete(__sh_i: Integer);
    function IndexOf(const __sh_v: T): Integer;
    function Contains(const __sh_v: T): Boolean;
    function Remove(const __sh_v: T): Integer;
    procedure Clear;
    procedure Reverse;
    procedure Sort;
    function First: T;
    function Last: T;
    function GetEnumerator: specialize TEnumList<T>;
    function ToArray: specialize TArray<T>;
    property Count: Integer read __sh_n;
    property Items[__sh_i: Integer]: T read GetItem write SetItem; default;
  end;
implementation
uses SysUtils;
var
  __shg_crctab: array[0..255] of longword;
  __shg_crcok: Boolean;
procedure __shg_crcinit;
var i, j: Integer; c: int64;
begin
  for i := 0 to 255 do begin
    c := i;
    for j := 1 to 8 do
      if (c and 1) = 1 then c := (c shr 1) xor $82F63B78 else c := c shr 1;
    __shg_crctab[i] := c;
  end;
  __shg_crcok := true;
end;
function __shg_crcstep(c: int64; b: Integer): int64;
begin
  __shg_crcstep := (c shr 8) xor __shg_crctab[(c xor b) and 255];
end;
function __shg_hash(const __sh_s: string): longword; overload;
var i: Integer; c: int64;
begin
  if not __shg_crcok then __shg_crcinit;
  c := $FFFFFFFF;
  for i := 1 to length(__sh_s) do c := __shg_crcstep(c, ord(__sh_s[i]));
  __shg_hash := c xor $FFFFFFFF;
end;
function __shg_hash(__sh_i: longint): longword; overload;
var j: Integer; c, v: int64;
begin
  if not __shg_crcok then __shg_crcinit;
  c := $FFFFFFFF;
  v := __sh_i and $FFFFFFFF;
  for j := 1 to 4 do begin
    c := __shg_crcstep(c, v and 255);
    v := v shr 8;
  end;
  __shg_hash := c xor $FFFFFFFF;
end;
function __shg_hash(__sh_i: int64): longword; overload;
var j: Integer; c, v: int64;
begin
  if not __shg_crcok then __shg_crcinit;
  c := $FFFFFFFF;
  v := __sh_i;
  for j := 1 to 8 do begin
    c := __shg_crcstep(c, v and 255);
    v := v shr 8;
  end;
  __shg_hash := c xor $FFFFFFFF;
end;
constructor TEnumList.Create(const __sh_src: array of T; __sh_n: Integer);
var i: Integer;
begin
  SetLength(__sh_a, __sh_n);
  for i := 0 to __sh_n - 1 do __sh_a[i] := __sh_src[i];
  __sh_i := -1;
end;
function TEnumList.MoveNext: Boolean;
begin
  Inc(__sh_i);
  Result := __sh_i < Length(__sh_a);
end;
function TEnumList.GetCurrent: T;
begin
  Result := __sh_a[__sh_i];
end;
constructor TEnumPairs.Create(const __sh_ks: array of K; const __sh_vs: array of V; __sh_n: Integer);
var i: Integer;
begin
  SetLength(__sh_k, __sh_n);
  SetLength(__sh_v, __sh_n);
  for i := 0 to __sh_n - 1 do begin
    __sh_k[i] := __sh_ks[i];
    __sh_v[i] := __sh_vs[i];
  end;
  __sh_i := -1;
end;
function TEnumPairs.MoveNext: Boolean;
begin
  Inc(__sh_i);
  Result := __sh_i < Length(__sh_k);
end;
function TEnumPairs.GetCurrent: specialize TPair<K, V>;
begin
  Result.Key := __sh_k[__sh_i];
  Result.Value := __sh_v[__sh_i];
end;
constructor TItemList.Create(const __sh_src: array of T; __sh_cnt: Integer);
var i: Integer;
begin
  SetLength(__sh_a, __sh_cnt);
  for i := 0 to __sh_cnt - 1 do __sh_a[i] := __sh_src[i];
  __sh_n := __sh_cnt;
end;
function TItemList.GetEnumerator: specialize TEnumList<T>;
begin
  Result := specialize TEnumList<T>.Create(__sh_a, __sh_n);
end;
function TItemList.ToArray: specialize TArray<T>;
var i: Integer;
begin
  SetLength(Result, __sh_n);
  for i := 0 to __sh_n - 1 do Result[i] := __sh_a[i];
end;
constructor TDictionary.Create;
begin
  inherited Create;
  __sh_n := 0;
  __sh_thr := 0;
end;
constructor TDictionary.Create(__sh_cap: Integer);
begin
  inherited Create;
  __sh_n := 0;
  __sh_thr := 0;
end;
constructor TDictionary.Create(__sh_o: TDictionaryOwnerships);
begin
  inherited Create;
  __sh_n := 0;
  __sh_thr := 0;
  __sh_own := __sh_o;
end;
destructor TDictionary.Destroy;
begin
  Clear;
  inherited Destroy;
end;
function TDictionary.FindSlot(const __sh_key: K; var __sh_hv: longword): Integer;
var m, i, r: Integer; hh: int64;
begin
  m := Length(__sh_h);
  hh := int64(__shg_hash(__sh_key)) or $80000000;
  __sh_hv := hh;
  if m = 0 then begin
    FindSlot := -1;
    exit;
  end;
  i := 0;
  r := hh and (m - 1);
  while true do begin
    if (__sh_h[r] and $80000000) = 0 then begin
      FindSlot := -1 - r;
      exit;
    end;
    if (__sh_h[r] = hh) and (__sh_k[r] = __sh_key) then begin
      FindSlot := r;
      exit;
    end;
    Inc(i);
    r := (hh + i) and (m - 1);
  end;
end;
procedure TDictionary.Rehash(__sh_size: Integer);
var nh: array of longword; nk: array of K; nv: array of V; i, r, j: Integer; hh: int64;
begin
  SetLength(nh, __sh_size);
  SetLength(nk, __sh_size);
  SetLength(nv, __sh_size);
  for i := 0 to __sh_size - 1 do nh[i] := 0;
  for i := 0 to Length(__sh_h) - 1 do
    if (__sh_h[i] and $80000000) <> 0 then begin
      hh := __sh_h[i];
      r := hh and (__sh_size - 1);
      j := 0;
      while (nh[r] and $80000000) <> 0 do begin
        Inc(j);
        r := (hh + j) and (__sh_size - 1);
      end;
      nh[r] := hh;
      nk[r] := __sh_k[i];
      nv[r] := __sh_v[i];
    end;
  __sh_h := nh;
  __sh_k := nk;
  __sh_v := nv;
  __sh_thr := Round(__sh_size * 0.75) - 1;
end;
procedure TDictionary.Prepare;
begin
  if __sh_n > __sh_thr then Rehash(Length(__sh_h) * 2)
  else if __sh_thr = 0 then Rehash(8);
end;
procedure TDictionary.Add(const __sh_key: K; const __sh_val: V);
var h: longword; r: Integer;
begin
  Prepare;
  r := FindSlot(__sh_key, h);
  if r >= 0 then raise Exception.Create('Duplicates not allowed in dictionary.');
  r := -1 - r;
  __sh_h[r] := h;
  __sh_k[r] := __sh_key;
  __sh_v[r] := __sh_val;
  Inc(__sh_n);
end;
procedure TDictionary.AddOrSetValue(const __sh_key: K; const __sh_val: V);
var h: longword; r: Integer;
begin
  r := FindSlot(__sh_key, h);
  if r >= 0 then __sh_v[r] := __sh_val
  else Add(__sh_key, __sh_val);
end;
function TDictionary.TryAdd(const __sh_key: K; const __sh_val: V): Boolean;
var h: longword; r: Integer;
begin
  r := FindSlot(__sh_key, h);
  Result := r < 0;
  if Result then Add(__sh_key, __sh_val);
end;
function TDictionary.ContainsKey(const __sh_key: K): Boolean;
var h: longword;
begin
  Result := FindSlot(__sh_key, h) >= 0;
end;
function TDictionary.TryGetValue(const __sh_key: K; out __sh_val: V): Boolean;
var h: longword; r: Integer;
begin
  r := FindSlot(__sh_key, h);
  Result := r >= 0;
  if Result then __sh_val := __sh_v[r];
end;
function TDictionary.GetItem(const __sh_key: K): V;
var h: longword; r: Integer;
begin
  r := FindSlot(__sh_key, h);
  if r < 0 then raise Exception.Create('The specified key does not exist.');
  Result := __sh_v[r];
end;
procedure TDictionary.SetItem(const __sh_key: K; const __sh_val: V);
var h: longword; r: Integer;
begin
  r := FindSlot(__sh_key, h);
  if r < 0 then raise Exception.Create('The specified key does not exist.');
  __sh_v[r] := __sh_val;
end;
procedure TDictionary.RemoveAt(__sh_at: Integer);
var m, i, idx, gap, b: Integer; hh: int64; inr: Boolean; ek: K; ev: V;
begin
  m := Length(__sh_h) - 1;
  hh := __sh_h[__sh_at];
  __sh_h[__sh_at] := 0;
  i := __sh_at - (hh and m) + 1;
  gap := __sh_at;
  while true do begin
    idx := (hh + i) and m;
    if (__sh_h[idx] and $80000000) = 0 then break;
    b := __sh_h[idx] and m;
    if gap <= idx then inr := (b > gap) and (b <= idx)
    else inr := (b > gap) or (b <= idx);
    if not inr then begin
      __sh_h[gap] := __sh_h[idx];
      __sh_k[gap] := __sh_k[idx];
      __sh_v[gap] := __sh_v[idx];
      __sh_h[idx] := 0;
      gap := idx;
    end;
    Inc(i);
  end;
  __sh_h[gap] := 0;
  __sh_k[gap] := ek;
  __sh_v[gap] := ev;
  Dec(__sh_n);
end;
procedure TDictionary.Remove(const __sh_key: K);
var h: longword; r: Integer;
begin
  r := FindSlot(__sh_key, h);
  if r >= 0 then RemoveAt(r);
end;
procedure TDictionary.Clear;
begin
  SetLength(__sh_h, 0);
  SetLength(__sh_k, 0);
  SetLength(__sh_v, 0);
  __sh_n := 0;
  __sh_thr := 0;
end;
function TDictionary.GetEnumerator: specialize TEnumPairs<K, V>;
var ks: array of K; vs: array of V; i, c: Integer;
begin
  SetLength(ks, __sh_n);
  SetLength(vs, __sh_n);
  c := 0;
  for i := 0 to Length(__sh_h) - 1 do
    if (__sh_h[i] and $80000000) <> 0 then begin
      ks[c] := __sh_k[i];
      vs[c] := __sh_v[i];
      Inc(c);
    end;
  Result := specialize TEnumPairs<K, V>.Create(ks, vs, c);
end;
function TDictionary.Keys: specialize TItemList<K>;
var ks: array of K; i, c: Integer;
begin
  SetLength(ks, __sh_n);
  c := 0;
  for i := 0 to Length(__sh_h) - 1 do
    if (__sh_h[i] and $80000000) <> 0 then begin
      ks[c] := __sh_k[i];
      Inc(c);
    end;
  Result := specialize TItemList<K>.Create(ks, c);
end;
function TDictionary.Values: specialize TItemList<V>;
var vs: array of V; i, c: Integer;
begin
  SetLength(vs, __sh_n);
  c := 0;
  for i := 0 to Length(__sh_h) - 1 do
    if (__sh_h[i] and $80000000) <> 0 then begin
      vs[c] := __sh_v[i];
      Inc(c);
    end;
  Result := specialize TItemList<V>.Create(vs, c);
end;
constructor TObjectDictionary.Create(__sh_o: TDictionaryOwnerships);
begin
  inherited Create(__sh_o);
end;
constructor THashSet.Create;
begin
  inherited Create;
  __sh_d := specialize TDictionary<T, Boolean>.Create;
end;
destructor THashSet.Destroy;
begin
  __sh_d.Free;
  inherited Destroy;
end;
function THashSet.Add(const __sh_v: T): Boolean;
begin
  Result := __sh_d.TryAdd(__sh_v, true);
end;
function THashSet.Remove(const __sh_v: T): Boolean;
begin
  Result := __sh_d.ContainsKey(__sh_v);
  if Result then __sh_d.Remove(__sh_v);
end;
function THashSet.Contains(const __sh_v: T): Boolean;
begin
  Result := __sh_d.ContainsKey(__sh_v);
end;
procedure THashSet.Clear;
begin
  __sh_d.Clear;
end;
function THashSet.GetEnumerator: specialize TEnumList<T>;
begin
  Result := __sh_d.Keys.GetEnumerator;
end;
function THashSet.ToArray: specialize TArray<T>;
begin
  Result := __sh_d.Keys.ToArray;
end;
function THashSet.GetCount: Integer;
begin
  Result := __sh_d.Count;
end;
constructor TQueue.Create;
begin
  inherited Create;
  __sh_head := 0;
  __sh_n := 0;
end;
procedure TQueue.Enqueue(const __sh_v: T);
begin
  SetLength(__sh_a, __sh_head + __sh_n + 1);
  __sh_a[__sh_head + __sh_n] := __sh_v;
  Inc(__sh_n);
end;
function TQueue.Dequeue: T;
begin
  Result := __sh_a[__sh_head];
  Inc(__sh_head);
  Dec(__sh_n);
end;
function TQueue.Peek: T;
begin
  Result := __sh_a[__sh_head];
end;
procedure TQueue.Clear;
begin
  SetLength(__sh_a, 0);
  __sh_head := 0;
  __sh_n := 0;
end;
constructor TStack.Create;
begin
  inherited Create;
  __sh_n := 0;
end;
procedure TStack.Push(const __sh_v: T);
begin
  SetLength(__sh_a, __sh_n + 1);
  __sh_a[__sh_n] := __sh_v;
  Inc(__sh_n);
end;
function TStack.Pop: T;
begin
  Dec(__sh_n);
  Result := __sh_a[__sh_n];
end;
function TStack.Peek: T;
begin
  Result := __sh_a[__sh_n - 1];
end;
procedure TStack.Clear;
begin
  SetLength(__sh_a, 0);
  __sh_n := 0;
end;
constructor TList.Create;
begin
  inherited Create;
  __sh_n := 0;
end;
function TList.GetItem(__sh_i: Integer): T;
begin
  Result := __sh_a[__sh_i];
end;
procedure TList.SetItem(__sh_i: Integer; const __sh_v: T);
begin
  __sh_a[__sh_i] := __sh_v;
end;
function TList.Add(const __sh_v: T): Integer;
begin
  SetLength(__sh_a, __sh_n + 1);
  __sh_a[__sh_n] := __sh_v;
  Result := __sh_n;
  Inc(__sh_n);
end;
procedure TList.Insert(__sh_i: Integer; const __sh_v: T);
var k: Integer;
begin
  SetLength(__sh_a, __sh_n + 1);
  for k := __sh_n downto __sh_i + 1 do __sh_a[k] := __sh_a[k - 1];
  __sh_a[__sh_i] := __sh_v;
  Inc(__sh_n);
end;
procedure TList.Delete(__sh_i: Integer);
var k: Integer;
begin
  for k := __sh_i to __sh_n - 2 do __sh_a[k] := __sh_a[k + 1];
  Dec(__sh_n);
  SetLength(__sh_a, __sh_n);
end;
function TList.IndexOf(const __sh_v: T): Integer;
var k: Integer;
begin
  Result := -1;
  for k := 0 to __sh_n - 1 do
    if __sh_a[k] = __sh_v then begin
      Result := k;
      exit;
    end;
end;
function TList.Contains(const __sh_v: T): Boolean;
begin
  Result := IndexOf(__sh_v) >= 0;
end;
function TList.Remove(const __sh_v: T): Integer;
begin
  Result := IndexOf(__sh_v);
  if Result >= 0 then Delete(Result);
end;
procedure TList.Clear;
begin
  SetLength(__sh_a, 0);
  __sh_n := 0;
end;
procedure TList.Reverse;
var i: Integer; t: T;
begin
  for i := 0 to __sh_n div 2 - 1 do begin
    t := __sh_a[i];
    __sh_a[i] := __sh_a[__sh_n - 1 - i];
    __sh_a[__sh_n - 1 - i] := t;
  end;
end;
procedure TList.Sort;
var i, j: Integer; t: T;
begin
  for i := 1 to __sh_n - 1 do begin
    t := __sh_a[i];
    j := i - 1;
    while (j >= 0) and (__sh_a[j] > t) do begin
      __sh_a[j + 1] := __sh_a[j];
      j := j - 1;
    end;
    __sh_a[j + 1] := t;
  end;
end;
function TList.First: T;
begin
  Result := __sh_a[0];
end;
function TList.Last: T;
begin
  Result := __sh_a[__sh_n - 1];
end;
function TList.GetEnumerator: specialize TEnumList<T>;
begin
  Result := specialize TEnumList<T>.Create(__sh_a, __sh_n);
end;
function TList.ToArray: specialize TArray<T>;
var i: Integer;
begin
  SetLength(Result, __sh_n);
  for i := 0 to __sh_n - 1 do Result[i] := __sh_a[i];
end;
initialization
  __shg_crcok := false;
end.
