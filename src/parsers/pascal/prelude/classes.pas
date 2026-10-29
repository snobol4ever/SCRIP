{$H+}
unit classes;
interface
uses sysutils;
type
  EListError = class(Exception);
  TDuplicates = (dupAccept, dupIgnore, dupError);
  TStringList = class;
  TStringListSortCompare = function(List: TStringList; Index1, Index2: Integer): Integer;
  TListSortCompare = function(Item1, Item2: Pointer): Integer;
  TList = class
  private
    FItems: array of Pointer;
    FCount: Integer;
    function Get(Index: Integer): Pointer;
    procedure Put(Index: Integer; Item: Pointer);
  public
    function Add(Item: Pointer): Integer;
    procedure Insert(Index: Integer; Item: Pointer);
    procedure Delete(Index: Integer);
    function Remove(Item: Pointer): Integer;
    function IndexOf(Item: Pointer): Integer;
    function First: Pointer;
    function Last: Pointer;
    procedure Clear;
    procedure Exchange(Index1, Index2: Integer);
    procedure Sort(Compare: TListSortCompare);
    property Count: Integer read FCount;
    property Items[Index: Integer]: Pointer read Get write Put; default;
  end;
  TStringList = class
  private
    FStrings: array of string;
    FObjects: array of TObject;
    FCount: Integer;
    FSorted: Boolean;
    FCaseSensitive: Boolean;
    FStrictDelimiter: Boolean;
    FDuplicates: TDuplicates;
    FDelimiter: Char;
    FQuoteChar: Char;
    FNameValueSeparator: Char;
    function GetString(Index: Integer): string;
    procedure PutString(Index: Integer; const S: string);
    function GetObject(Index: Integer): TObject;
    procedure PutObject(Index: Integer; AObject: TObject);
    procedure SetSorted(Value: Boolean);
    procedure SetCaseSensitive(Value: Boolean);
    function GetText: string;
    procedure SetText(const Value: string);
    function GetDelimitedText: string;
    procedure SetDelimitedText(const Value: string);
    function GetCommaText: string;
    procedure SetCommaText(const Value: string);
    function GetName(Index: Integer): string;
    function GetValue(const Name: string): string;
    procedure SetValue(const Name, Value: string);
    function GetValueFromIndex(Index: Integer): string;
    function CompareStr2(const A, B: string): Integer;
    procedure InsertItem(Index: Integer; const S: string; AObject: TObject);
    procedure ParseDelimited(const Value: string; AStrict: Boolean; AQuote, ADelim: Char);
    function BuildDelimited(AStrict: Boolean; AQuote, ADelim: Char): string;
  public
    constructor Create;
    function Add(const S: string): Integer;
    function AddObject(const S: string; AObject: TObject): Integer;
    procedure Append(const S: string);
    procedure AddStrings(Source: TStringList);
    procedure Insert(Index: Integer; const S: string);
    procedure InsertObject(Index: Integer; const S: string; AObject: TObject);
    procedure Delete(Index: Integer);
    procedure Clear;
    procedure Exchange(Index1, Index2: Integer);
    procedure Move(CurIndex, NewIndex: Integer);
    function IndexOf(const S: string): Integer;
    function IndexOfName(const Name: string): Integer;
    function IndexOfObject(AObject: TObject): Integer;
    function Find(const S: string; var Index: Integer): Boolean;
    procedure Sort;
    procedure CustomSort(CompareFn: TStringListSortCompare);
    procedure Assign(Source: TStringList);
    function Equals(Other: TStringList): Boolean;
    procedure BeginUpdate;
    procedure EndUpdate;
    procedure LoadFromFile(const FileName: string);
    procedure SaveToFile(const FileName: string);
    property Count: Integer read FCount;
    property Strings[Index: Integer]: string read GetString write PutString; default;
    property Objects[Index: Integer]: TObject read GetObject write PutObject;
    property Sorted: Boolean read FSorted write SetSorted;
    property CaseSensitive: Boolean read FCaseSensitive write SetCaseSensitive;
    property Duplicates: TDuplicates read FDuplicates write FDuplicates;
    property Delimiter: Char read FDelimiter write FDelimiter;
    property QuoteChar: Char read FQuoteChar write FQuoteChar;
    property StrictDelimiter: Boolean read FStrictDelimiter write FStrictDelimiter;
    property NameValueSeparator: Char read FNameValueSeparator write FNameValueSeparator;
    property Text: string read GetText write SetText;
    property DelimitedText: string read GetDelimitedText write SetDelimitedText;
    property CommaText: string read GetCommaText write SetCommaText;
    property Names[Index: Integer]: string read GetName;
    property Values[const Name: string]: string read GetValue write SetValue;
    property ValueFromIndex[Index: Integer]: string read GetValueFromIndex;
  end;
implementation
procedure OpenForRead(var f: Text; const n: string);
begin
  Assign(f, n);
  Reset(f)
end;
procedure OpenForWrite(var f: Text; const n: string);
begin
  Assign(f, n);
  Rewrite(f)
end;
function IndexErr(Index: Integer): string;
begin
  IndexErr := 'List index (' + IntToStr(Index) + ') out of bounds'
end;
function TList.Get(Index: Integer): Pointer;
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  Result := FItems[Index]
end;
procedure TList.Put(Index: Integer; Item: Pointer);
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  FItems[Index] := Item
end;
function TList.Add(Item: Pointer): Integer;
begin
  if FCount = Length(FItems) then SetLength(FItems, FCount * 2 + 4);
  FItems[FCount] := Item;
  Result := FCount;
  FCount := FCount + 1
end;
procedure TList.Insert(Index: Integer; Item: Pointer);
var k: Integer;
begin
  if (Index < 0) or (Index > FCount) then raise EListError.Create(IndexErr(Index));
  if FCount = Length(FItems) then SetLength(FItems, FCount * 2 + 4);
  for k := FCount downto Index + 1 do FItems[k] := FItems[k - 1];
  FItems[Index] := Item;
  FCount := FCount + 1
end;
procedure TList.Delete(Index: Integer);
var k: Integer;
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  for k := Index to FCount - 2 do FItems[k] := FItems[k + 1];
  FCount := FCount - 1
end;
function TList.IndexOf(Item: Pointer): Integer;
var k: Integer;
begin
  Result := -1;
  k := 0;
  while (Result < 0) and (k < FCount) do
  begin
    if FItems[k] = Item then Result := k;
    k := k + 1
  end
end;
function TList.Remove(Item: Pointer): Integer;
begin
  Result := IndexOf(Item);
  if Result >= 0 then Delete(Result)
end;
function TList.First: Pointer;
begin
  if FCount = 0 then raise EListError.Create(IndexErr(0));
  Result := FItems[0]
end;
function TList.Last: Pointer;
begin
  if FCount = 0 then raise EListError.Create(IndexErr(-1));
  Result := FItems[FCount - 1]
end;
procedure TList.Clear;
begin
  SetLength(FItems, 0);
  FCount := 0
end;
procedure TList.Exchange(Index1, Index2: Integer);
var t: Pointer;
begin
  if (Index1 < 0) or (Index1 >= FCount) then raise EListError.Create(IndexErr(Index1));
  if (Index2 < 0) or (Index2 >= FCount) then raise EListError.Create(IndexErr(Index2));
  t := FItems[Index1];
  FItems[Index1] := FItems[Index2];
  FItems[Index2] := t
end;
procedure TList.Sort(Compare: TListSortCompare);
var tmp: array of Pointer; w, lo, mid, hi, a, b, k, i: Integer;
begin
  if FCount < 2 then exit;
  SetLength(tmp, FCount);
  w := 1;
  while w < FCount do
  begin
    lo := 0;
    while lo < FCount do
    begin
      mid := lo + w;
      if mid > FCount then mid := FCount;
      hi := lo + 2 * w;
      if hi > FCount then hi := FCount;
      a := lo; b := mid; k := lo;
      while (a < mid) and (b < hi) do
      begin
        if Compare(FItems[b], FItems[a]) < 0 then begin tmp[k] := FItems[b]; b := b + 1 end
        else begin tmp[k] := FItems[a]; a := a + 1 end;
        k := k + 1
      end;
      while a < mid do begin tmp[k] := FItems[a]; a := a + 1; k := k + 1 end;
      while b < hi do begin tmp[k] := FItems[b]; b := b + 1; k := k + 1 end;
      lo := lo + 2 * w
    end;
    for i := 0 to FCount - 1 do FItems[i] := tmp[i];
    w := w * 2
  end
end;
constructor TStringList.Create;
begin
  FCount := 0;
  FSorted := false;
  FCaseSensitive := false;
  FStrictDelimiter := false;
  FDuplicates := dupAccept;
  FDelimiter := ',';
  FQuoteChar := '"';
  FNameValueSeparator := '='
end;
function TStringList.CompareStr2(const A, B: string): Integer;
begin
  if FCaseSensitive then Result := CompareStr(A, B) else Result := CompareText(A, B)
end;
function TStringList.GetString(Index: Integer): string;
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  Result := FStrings[Index]
end;
procedure TStringList.PutString(Index: Integer; const S: string);
begin
  if FSorted then raise EListError.Create('Operation not allowed on sorted list');
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  FStrings[Index] := S
end;
function TStringList.GetObject(Index: Integer): TObject;
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  Result := FObjects[Index]
end;
procedure TStringList.PutObject(Index: Integer; AObject: TObject);
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  FObjects[Index] := AObject
end;
procedure TStringList.InsertItem(Index: Integer; const S: string; AObject: TObject);
var k: Integer;
begin
  if FCount = Length(FStrings) then
  begin
    SetLength(FStrings, FCount * 2 + 4);
    SetLength(FObjects, FCount * 2 + 4)
  end;
  for k := FCount downto Index + 1 do
  begin
    FStrings[k] := FStrings[k - 1];
    FObjects[k] := FObjects[k - 1]
  end;
  FStrings[Index] := S;
  FObjects[Index] := AObject;
  FCount := FCount + 1
end;
function TStringList.Find(const S: string; var Index: Integer): Boolean;
var L, R, I, C: Integer;
begin
  Result := false;
  Index := -1;
  if not FSorted then raise EListError.Create('Find needs a sorted list');
  L := 0;
  R := FCount - 1;
  while L <= R do
  begin
    I := L + (R - L) div 2;
    C := CompareStr2(S, FStrings[I]);
    if C > 0 then L := I + 1
    else
    begin
      R := I - 1;
      if C = 0 then
      begin
        Result := true;
        if FDuplicates <> dupAccept then L := I
      end
    end
  end;
  Index := L
end;
function TStringList.Add(const S: string): Integer;
begin
  Result := AddObject(S, nil)
end;
function TStringList.AddObject(const S: string; AObject: TObject): Integer;
begin
  if not FSorted then Result := FCount
  else if Find(S, Result) then
  begin
    if FDuplicates = dupIgnore then exit;
    if FDuplicates = dupError then raise EListError.Create('String list does not allow duplicates')
  end;
  InsertItem(Result, S, AObject)
end;
procedure TStringList.Append(const S: string);
var r: Integer;
begin
  r := Add(S)
end;
procedure TStringList.AddStrings(Source: TStringList);
var k: Integer;
begin
  for k := 0 to Source.Count - 1 do AddObject(Source.Strings[k], Source.Objects[k])
end;
procedure TStringList.Insert(Index: Integer; const S: string);
begin
  InsertObject(Index, S, nil)
end;
procedure TStringList.InsertObject(Index: Integer; const S: string; AObject: TObject);
begin
  if FSorted then raise EListError.Create('Operation not allowed on sorted list');
  if (Index < 0) or (Index > FCount) then raise EListError.Create(IndexErr(Index));
  InsertItem(Index, S, AObject)
end;
procedure TStringList.Delete(Index: Integer);
var k: Integer;
begin
  if (Index < 0) or (Index >= FCount) then raise EListError.Create(IndexErr(Index));
  for k := Index to FCount - 2 do
  begin
    FStrings[k] := FStrings[k + 1];
    FObjects[k] := FObjects[k + 1]
  end;
  FCount := FCount - 1;
  FStrings[FCount] := '';
  FObjects[FCount] := nil
end;
procedure TStringList.Clear;
begin
  SetLength(FStrings, 0);
  SetLength(FObjects, 0);
  FCount := 0
end;
procedure TStringList.Exchange(Index1, Index2: Integer);
var s: string; o: TObject;
begin
  if (Index1 < 0) or (Index1 >= FCount) then raise EListError.Create(IndexErr(Index1));
  if (Index2 < 0) or (Index2 >= FCount) then raise EListError.Create(IndexErr(Index2));
  s := FStrings[Index1]; FStrings[Index1] := FStrings[Index2]; FStrings[Index2] := s;
  o := FObjects[Index1]; FObjects[Index1] := FObjects[Index2]; FObjects[Index2] := o
end;
procedure TStringList.Move(CurIndex, NewIndex: Integer);
var s: string; o: TObject;
begin
  if CurIndex <> NewIndex then
  begin
    s := Strings[CurIndex];
    o := Objects[CurIndex];
    Delete(CurIndex);
    InsertItem(NewIndex, s, o)
  end
end;
function TStringList.IndexOf(const S: string): Integer;
var k: Integer;
begin
  if FSorted then
  begin
    if not Find(S, Result) then Result := -1
  end
  else
  begin
    Result := -1;
    k := 0;
    while (Result < 0) and (k < FCount) do
    begin
      if CompareStr2(FStrings[k], S) = 0 then Result := k;
      k := k + 1
    end
  end
end;
function TStringList.IndexOfObject(AObject: TObject): Integer;
var k: Integer;
begin
  Result := -1;
  k := 0;
  while (Result < 0) and (k < FCount) do
  begin
    if FObjects[k] = AObject then Result := k;
    k := k + 1
  end
end;
procedure TStringList.CustomSort(CompareFn: TStringListSortCompare);
var perm, tmp: array of Integer; i, w, lo, mid, hi, a, b, k: Integer; ns: array of string; no: array of TObject;
begin
  if FCount < 2 then exit;
  SetLength(perm, FCount);
  SetLength(tmp, FCount);
  for i := 0 to FCount - 1 do perm[i] := i;
  w := 1;
  while w < FCount do
  begin
    lo := 0;
    while lo < FCount do
    begin
      mid := lo + w;
      if mid > FCount then mid := FCount;
      hi := lo + 2 * w;
      if hi > FCount then hi := FCount;
      a := lo; b := mid; k := lo;
      while (a < mid) and (b < hi) do
      begin
        if CompareFn(Self, perm[b], perm[a]) < 0 then begin tmp[k] := perm[b]; b := b + 1 end
        else begin tmp[k] := perm[a]; a := a + 1 end;
        k := k + 1
      end;
      while a < mid do begin tmp[k] := perm[a]; a := a + 1; k := k + 1 end;
      while b < hi do begin tmp[k] := perm[b]; b := b + 1; k := k + 1 end;
      lo := lo + 2 * w
    end;
    for i := 0 to FCount - 1 do perm[i] := tmp[i];
    w := w * 2
  end;
  SetLength(ns, FCount);
  SetLength(no, FCount);
  for i := 0 to FCount - 1 do begin ns[i] := FStrings[perm[i]]; no[i] := FObjects[perm[i]] end;
  for i := 0 to FCount - 1 do begin FStrings[i] := ns[i]; FObjects[i] := no[i] end
end;
function CompareByText(List: TStringList; Index1, Index2: Integer): Integer;
begin
  CompareByText := List.CompareStr2(List.FStrings[Index1], List.FStrings[Index2])
end;
procedure TStringList.Sort;
begin
  CustomSort(@CompareByText)
end;
procedure TStringList.SetSorted(Value: Boolean);
begin
  if Value and not FSorted then
  begin
    FSorted := true;
    Sort
  end
  else FSorted := Value
end;
procedure TStringList.SetCaseSensitive(Value: Boolean);
begin
  if Value <> FCaseSensitive then
  begin
    FCaseSensitive := Value;
    if FSorted then Sort
  end
end;
procedure TStringList.Assign(Source: TStringList);
var k: Integer;
begin
  Clear;
  FDelimiter := Source.FDelimiter;
  FQuoteChar := Source.FQuoteChar;
  FCaseSensitive := Source.FCaseSensitive;
  FDuplicates := Source.FDuplicates;
  FSorted := false;
  for k := 0 to Source.Count - 1 do InsertItem(FCount, Source.FStrings[k], Source.FObjects[k]);
  FSorted := Source.FSorted
end;
function TStringList.Equals(Other: TStringList): Boolean;
var k: Integer;
begin
  Result := Count = Other.Count;
  k := 0;
  while Result and (k < FCount) do
  begin
    Result := FStrings[k] = Other.FStrings[k];
    k := k + 1
  end
end;
procedure TStringList.BeginUpdate;
begin
end;
procedure TStringList.EndUpdate;
begin
end;
function TStringList.GetText: string;
var k: Integer;
begin
  Result := '';
  for k := 0 to FCount - 1 do Result := Result + FStrings[k] + #10
end;
procedure TStringList.SetText(const Value: string);
var i, n: Integer; line: string; c: Char;
begin
  Clear;
  n := Length(Value);
  i := 1;
  line := '';
  while i <= n do
  begin
    c := Value[i];
    if (c = #10) or (c = #13) then
    begin
      Add(line);
      line := '';
      if (c = #13) and (i < n) and (Value[i + 1] = #10) then i := i + 1
    end
    else line := line + c;
    i := i + 1
  end;
  if line <> '' then Add(line)
end;
function TStringList.BuildDelimited(AStrict: Boolean; AQuote, ADelim: Char): string;
var k, j: Integer; s, q: string; need: Boolean;
begin
  Result := '';
  for k := 0 to FCount - 1 do
  begin
    s := FStrings[k];
    need := false;
    for j := 1 to Length(s) do
      if (s[j] = AQuote) or (s[j] = ADelim) or (s[j] = #0) or ((not AStrict) and (s[j] <= ' ')) then need := true;
    if need and (AQuote <> #0) then
    begin
      q := AQuote;
      for j := 1 to Length(s) do
      begin
        if s[j] = AQuote then q := q + AQuote;
        q := q + s[j]
      end;
      Result := Result + q + AQuote
    end
    else Result := Result + s;
    if k < FCount - 1 then Result := Result + ADelim
  end;
  if (Length(Result) = 0) and (FCount = 1) and (AQuote <> #0) then Result := AQuote + AQuote
end;
procedure TStringList.ParseDelimited(const Value: string; AStrict: Boolean; AQuote, ADelim: Char);
var len, i, j: Integer; notFirst: Boolean; piece: string;
begin
  Clear;
  i := 1;
  notFirst := false;
  len := Length(Value);
  while i <= len do
  begin
    if notFirst and (i <= len) and (Value[i] = ADelim) then i := i + 1;
    if not AStrict then while (i <= len) and (Value[i] <= ' ') do i := i + 1;
    if i > len then
    begin
      if notFirst then Add('')
    end
    else if (Value[i] = AQuote) and (AQuote <> #0) then
    begin
      j := i + 1;
      piece := '';
      while (j <= len) and ((Value[j] <> AQuote) or ((j + 1 <= len) and (Value[j + 1] = AQuote))) do
      begin
        if Value[j] = AQuote then j := j + 1;
        piece := piece + Value[j];
        j := j + 1
      end;
      Add(piece);
      i := j + 1
    end
    else
    begin
      j := i;
      while (j <= len) and (AStrict or (Value[j] > ' ')) and (Value[j] <> ADelim) do j := j + 1;
      Add(Copy(Value, i, j - i));
      i := j
    end;
    if not AStrict then while (i <= len) and (Value[i] <= ' ') do i := i + 1;
    notFirst := true
  end
end;
function TStringList.GetDelimitedText: string;
begin
  Result := BuildDelimited(FStrictDelimiter, FQuoteChar, FDelimiter)
end;
procedure TStringList.SetDelimitedText(const Value: string);
begin
  ParseDelimited(Value, FStrictDelimiter, FQuoteChar, FDelimiter)
end;
function TStringList.GetCommaText: string;
begin
  Result := BuildDelimited(false, '"', ',')
end;
procedure TStringList.SetCommaText(const Value: string);
begin
  ParseDelimited(Value, false, '"', ',')
end;
function TStringList.IndexOfName(const Name: string): Integer;
var k, p: Integer; s: string;
begin
  Result := -1;
  k := 0;
  while (Result < 0) and (k < FCount) do
  begin
    s := FStrings[k];
    p := Pos(FNameValueSeparator, s);
    if (p > 0) and (CompareStr2(Copy(s, 1, p - 1), Name) = 0) then Result := k;
    k := k + 1
  end
end;
function TStringList.GetName(Index: Integer): string;
var p: Integer;
begin
  Result := Strings[Index];
  p := Pos(FNameValueSeparator, Result);
  if p > 0 then Result := Copy(Result, 1, p - 1) else Result := ''
end;
function TStringList.GetValueFromIndex(Index: Integer): string;
var s: string; p: Integer;
begin
  s := Strings[Index];
  p := Pos(FNameValueSeparator, s);
  if p > 0 then Result := Copy(s, p + 1, Length(s)) else Result := ''
end;
function TStringList.GetValue(const Name: string): string;
var k: Integer;
begin
  k := IndexOfName(Name);
  if k >= 0 then Result := GetValueFromIndex(k) else Result := ''
end;
procedure TStringList.SetValue(const Name, Value: string);
var k: Integer;
begin
  k := IndexOfName(Name);
  if k >= 0 then FStrings[k] := Name + FNameValueSeparator + Value
  else Add(Name + FNameValueSeparator + Value)
end;
procedure TStringList.LoadFromFile(const FileName: string);
var f: Text; line, all: string;
begin
  Clear;
  OpenForRead(f, FileName);
  while not Eof(f) do
  begin
    ReadLn(f, line);
    Add(line)
  end;
  Close(f)
end;
procedure TStringList.SaveToFile(const FileName: string);
var f: Text; k: Integer;
begin
  OpenForWrite(f, FileName);
  for k := 0 to FCount - 1 do WriteLn(f, FStrings[k]);
  Close(f)
end;
end.
