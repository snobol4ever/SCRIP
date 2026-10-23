unit sysutils;
interface
type
  TDateTime = double;
  Exception = class
  private
    FMessage: string;
  public
    constructor Create(const msg: string);
    function ToString: string; override;
    property Message: string read FMessage write FMessage;
  end;
  EAbort = class(Exception);
  EConvertError = class(Exception);
  EInvalidCast = class(Exception);
  ENotImplemented = class(Exception);
  EVariantError = class(Exception);
  EInvalidOpException = class(Exception);
  EArgumentException = class(Exception);
  EArgumentOutOfRangeException = class(EArgumentException);
  EArgumentNilException = class(EArgumentException);
  EInOutError = class(Exception);
  EExternal = class(Exception);
  EIntError = class(EExternal);
  EDivByZero = class(EIntError);
  ERangeError = class(EIntError);
  EIntOverflow = class(EIntError);
  EMathError = class(EExternal);
  EInvalidOp = class(EMathError);
  EZeroDivide = class(EMathError);
  EOverflow = class(EMathError);
  EUnderflow = class(EMathError);
  EAccessViolation = class(EExternal);
  EHeapMemoryError = class(Exception);
  EInvalidPointer = class(EHeapMemoryError);
  EOutOfMemory = class(EHeapMemoryError);
function IntToStr(n: integer): string;
function StrToInt(s: string): integer;
function UpperCase(s: string): string;
function LowerCase(s: string): string;
function TrimLeft(s: string): string;
function TrimRight(s: string): string;
function Trim(s: string): string;
function StrToIntDef(s: string; d: integer): integer;
function TryStrToInt(s: string; var n: integer): boolean;
function FileExists(s: string): boolean;
function Now: double;
function Date: double;
function Time: double;
function GetTickCount64: int64;
function GetTickCount: int64;
function CompareStr(a, b: string): integer;
function CompareText(a, b: string): integer;
function AnsiCompareStr(a, b: string): integer;
function AnsiCompareText(a, b: string): integer;
function SameText(a, b: string): boolean;
function QuotedStr(s: string): string;
function IntToHex(v: int64; digits: integer): string;
function FloatToStr(d: double): string;
function FormatFloat(fmt: string; d: double): string;
function StrToFloat(s: string): double;
function TryStrToFloat(s: string; var d: double): boolean;
function StrToFloatDef(s: string; def: double): double;
function BoolToStr(b: boolean; useBoolStrs: boolean = false): string;
function IsLeapYear(year: integer): boolean;
function EncodeDate(year, month, day: integer): double;
procedure DecodeDate(dt: double; var year, month, day: word);
function EncodeTime(hour, minute, sec, msec: integer): double;
procedure DecodeTime(dt: double; var hour, minute, sec, msec: word);
function DayOfWeek(dt: double): integer;
function FormatDateTime(fmt: string; dt: double): string;
implementation
constructor Exception.Create(const msg: string);
begin
  FMessage := msg
end;
function Exception.ToString: string;
begin
  ToString := ClassName + ': ' + FMessage
end;
function IntToStr(n: integer): string;
var r: string; neg: boolean;
begin
  setlength(r, 0);
  neg := n < 0;
  if neg then n := -n;
  repeat
    r := chr(ord('0') + n mod 10) + r;
    n := n div 10
  until n = 0;
  if neg then r := '-' + r;
  IntToStr := r
end;
function StrToInt(s: string): integer;
var i, v: integer; neg, ok: boolean;
begin
  i := 1;
  v := 0;
  neg := false;
  ok := false;
  while (i <= length(s)) and (s[i] = ' ') do i := i + 1;
  if (i <= length(s)) and ((s[i] = '-') or (s[i] = '+')) then begin neg := s[i] = '-'; i := i + 1 end;
  if (i <= length(s)) and (s[i] = '$') then
  begin
    i := i + 1;
    while (i <= length(s)) and (pos(upcase(s[i]), '0123456789ABCDEF') > 0) do begin v := v * 16 + pos(upcase(s[i]), '0123456789ABCDEF') - 1; i := i + 1; ok := true end
  end
  else
    while (i <= length(s)) and (s[i] >= '0') and (s[i] <= '9') do begin v := v * 10 + (ord(s[i]) - ord('0')); i := i + 1; ok := true end;
  if not ok or (i <= length(s)) then raise EConvertError.Create('"' + s + '" is an invalid integer');
  if neg then v := -v;
  StrToInt := v
end;
function UpperCase(s: string): string;
var r: string; i: integer;
begin
  r := s;
  for i := 1 to length(r) do
    if (r[i] >= 'a') and (r[i] <= 'z') then r[i] := chr(ord(r[i]) - 32);
  UpperCase := r
end;
function LowerCase(s: string): string;
var r: string; i: integer;
begin
  r := s;
  for i := 1 to length(r) do
    if (r[i] >= 'A') and (r[i] <= 'Z') then r[i] := chr(ord(r[i]) + 32);
  LowerCase := r
end;
function TrimLeft(s: string): string;
var r: string; i: integer;
begin
  i := 1;
  while (i <= length(s)) and (s[i] <= ' ') do i := i + 1;
  setlength(r, 0);
  while i <= length(s) do begin r := r + s[i]; i := i + 1 end;
  TrimLeft := r
end;
function TrimRight(s: string): string;
var r: string; i, j: integer;
begin
  j := length(s);
  while (j >= 1) and (s[j] <= ' ') do j := j - 1;
  setlength(r, 0);
  for i := 1 to j do r := r + s[i];
  TrimRight := r
end;
function Trim(s: string): string;
begin
  Trim := TrimRight(TrimLeft(s))
end;
function StrToIntDef(s: string; d: integer): integer;
var i, v: integer; neg, ok: boolean;
begin
  i := 1;
  v := 0;
  neg := false;
  ok := false;
  while (i <= length(s)) and (s[i] = ' ') do i := i + 1;
  if (i <= length(s)) and ((s[i] = '-') or (s[i] = '+')) then begin neg := s[i] = '-'; i := i + 1 end;
  if (i <= length(s)) and (s[i] = '$') then
  begin
    i := i + 1;
    while (i <= length(s)) and (pos(upcase(s[i]), '0123456789ABCDEF') > 0) do begin v := v * 16 + pos(upcase(s[i]), '0123456789ABCDEF') - 1; i := i + 1; ok := true end
  end
  else
    while (i <= length(s)) and (s[i] >= '0') and (s[i] <= '9') do begin v := v * 10 + (ord(s[i]) - ord('0')); i := i + 1; ok := true end;
  if not ok or (i <= length(s)) then StrToIntDef := d
  else if neg then StrToIntDef := -v
  else StrToIntDef := v
end;
function TryStrToInt(s: string; var n: integer): boolean;
var i, v: integer; neg, ok: boolean;
begin
  i := 1;
  v := 0;
  neg := false;
  ok := false;
  while (i <= length(s)) and (s[i] = ' ') do i := i + 1;
  if (i <= length(s)) and ((s[i] = '-') or (s[i] = '+')) then begin neg := s[i] = '-'; i := i + 1 end;
  if (i <= length(s)) and (s[i] = '$') then
  begin
    i := i + 1;
    while (i <= length(s)) and (pos(upcase(s[i]), '0123456789ABCDEF') > 0) do begin v := v * 16 + pos(upcase(s[i]), '0123456789ABCDEF') - 1; i := i + 1; ok := true end
  end
  else
    while (i <= length(s)) and (s[i] >= '0') and (s[i] <= '9') do begin v := v * 10 + (ord(s[i]) - ord('0')); i := i + 1; ok := true end;
  if not ok or (i <= length(s)) then TryStrToInt := false
  else
  begin
    if neg then n := -v else n := v;
    TryStrToInt := true
  end
end;
function FileExists(s: string): boolean;
begin
  FileExists := __pas_fileexists(s)
end;
function Now: double;
begin
  Now := __pas_now()
end;
function Date: double;
begin
  Date := trunc(__pas_now())
end;
function Time: double;
var d: double;
begin
  d := __pas_now();
  Time := d - trunc(d)
end;
function GetTickCount64: int64;
begin
  GetTickCount64 := __pas_ticks()
end;
function GetTickCount: int64;
begin
  GetTickCount := __pas_ticks() mod 4294967296
end;
function CompareStr(a, b: string): integer;
var i, n: integer;
begin
  n := length(a);
  if length(b) < n then n := length(b);
  i := 1;
  while (i <= n) and (a[i] = b[i]) do i := i + 1;
  if i <= n then CompareStr := ord(a[i]) - ord(b[i])
  else CompareStr := length(a) - length(b)
end;
function AnsiCompareStr(a, b: string): integer;
begin
  AnsiCompareStr := CompareStr(a, b)
end;
function AnsiCompareText(a, b: string): integer;
begin
  AnsiCompareText := CompareText(a, b)
end;
function CompareText(a, b: string): integer;
var x, y: string; i, n: integer;
begin
  x := UpperCase(a);
  y := UpperCase(b);
  n := length(x);
  if length(y) < n then n := length(y);
  i := 1;
  while (i <= n) and (x[i] = y[i]) do i := i + 1;
  if i <= n then CompareText := ord(x[i]) - ord(y[i])
  else CompareText := length(x) - length(y)
end;
function SameText(a, b: string): boolean;
begin
  SameText := CompareText(a, b) = 0
end;
function QuotedStr(s: string): string;
var r: string; i: integer;
begin
  r := '''';
  for i := 1 to length(s) do if s[i] = '''' then r := r + '''''' else r := r + s[i];
  QuotedStr := r + ''''
end;
function IntToHex(v: int64; digits: integer): string;
var r: string; k: integer; n: int64; neg: boolean;
begin
  r := '';
  neg := v < 0;
  if neg then
  begin
    n := v + 1;
    n := -n;
    for k := 1 to 16 do
    begin
      r := copy('FEDCBA9876543210', (n mod 16) + 1, 1) + r;
      n := n div 16
    end
  end
  else
  begin
    n := v;
    repeat
      r := copy('0123456789ABCDEF', (n mod 16) + 1, 1) + r;
      n := n div 16
    until n = 0
  end;
  while length(r) < digits do r := '0' + r;
  IntToHex := r
end;
procedure FDigits(d: double; nsig: integer; var digs: string; var pt: integer);
var s, m: string; i, e, ep, esign: integer; c: char;
begin
  str(d, s);
  m := '';
  ep := pos('E', s);
  for i := 1 to ep - 1 do
  begin
    c := s[i];
    if (c >= '0') and (c <= '9') then m := m + c
  end;
  e := 0;
  esign := 1;
  for i := ep + 1 to length(s) do
  begin
    c := s[i];
    if c = '-' then esign := -1
    else if (c >= '0') and (c <= '9') then e := e * 10 + (ord(c) - ord('0'))
  end;
  e := e * esign;
  pt := e + 1;
  while length(m) < nsig + 1 do m := m + '0';
  if m[nsig + 1] >= '5' then
  begin
    i := nsig;
    while (i >= 1) and (m[i] = '9') do
    begin
      m[i] := '0';
      i := i - 1
    end;
    if i >= 1 then m[i] := chr(ord(m[i]) + 1)
    else
    begin
      m := '1' + m;
      pt := pt + 1
    end
  end;
  digs := copy(m, 1, nsig)
end;
function FloatToStr(d: double): string;
var digs, r: string; pt, e, n, i: integer; neg: boolean;
begin
  if d = 0 then FloatToStr := '0'
  else
  begin
    neg := d < 0;
    if neg then d := -d;
    FDigits(d, 15, digs, pt);
    n := length(digs);
    while (n > 1) and (digs[n] = '0') do n := n - 1;
    digs := copy(digs, 1, n);
    if (pt > 15) or (pt < -4) then
    begin
      r := copy(digs, 1, 1);
      if n > 1 then r := r + '.' + copy(digs, 2, n - 1);
      e := pt - 1;
      if e < 0 then r := r + 'E-' + IntToStr(-e) else r := r + 'E' + IntToStr(e)
    end
    else if pt <= 0 then
    begin
      r := '0.';
      for i := 1 to -pt do r := r + '0';
      r := r + digs
    end
    else if n <= pt then
    begin
      r := digs;
      for i := n + 1 to pt do r := r + '0'
    end
    else r := copy(digs, 1, pt) + '.' + copy(digs, pt + 1, n - pt);
    if neg then r := '-' + r;
    FloatToStr := r
  end
end;
function FormatFloat(fmt: string; d: double): string;
var i, nfmt, intZ, fracZ, fracMax, keep, n, pt, k, cnt: integer;
    pre, suf, ip, fp, digs, r: string; c: char; inFrac, thou, started, ended, neg, pct, tiny: boolean;
begin
  nfmt := length(fmt);
  pre := ''; suf := ''; intZ := 0; fracZ := 0; fracMax := 0; inFrac := false; thou := false; started := false; ended := false; pct := false;
  for i := 1 to nfmt do
  begin
    c := fmt[i];
    if (not ended) and ((c = '0') or (c = '#') or (c = ',') or ((c = '.') and (i < nfmt) and ((fmt[i + 1] = '0') or (fmt[i + 1] = '#')))) then
    begin
      started := true;
      if c = '.' then inFrac := true
      else if c = ',' then thou := true
      else if inFrac then
      begin
        fracMax := fracMax + 1;
        if c = '0' then fracZ := fracMax
      end
      else if c = '0' then intZ := intZ + 1
    end
    else
    begin
      if started then ended := true;
      if started then suf := suf + c else pre := pre + c
    end
  end;
  neg := d < 0;
  if neg then d := -d;
  tiny := false;
  if d = 0 then
  begin
    digs := '';
    pt := 0
  end
  else FDigits(d, 15, digs, pt);
  if pt > 17 then FormatFloat := FloatToStr(d)
  else
  begin
    keep := pt + fracMax;
    if keep < 0 then
    begin
      digs := '';
      pt := 0;
      tiny := true
    end
    else if keep < length(digs) then
    begin
      n := keep;
      if digs[n + 1] >= '5' then
      begin
        k := n;
        while (k >= 1) and (digs[k] = '9') do
        begin
          digs[k] := '0';
          k := k - 1
        end;
        if k >= 1 then digs[k] := chr(ord(digs[k]) + 1)
        else
        begin
          digs := '1' + copy(digs, 1, n);
          pt := pt + 1;
          n := n + 1
        end
      end;
      digs := copy(digs, 1, n);
      if (n > 0) and (pos('1', digs) + pos('2', digs) + pos('3', digs) + pos('4', digs) + pos('5', digs) + pos('6', digs) + pos('7', digs) + pos('8', digs) + pos('9', digs) = 0) then tiny := true;
      if n = 0 then tiny := true
    end;
    ip := '';
    fp := '';
    for k := 1 to pt do
      if k <= length(digs) then ip := ip + digs[k] else ip := ip + '0';
    for k := 1 to fracMax do
    begin
      cnt := pt + k;
      if cnt < 1 then fp := fp + '0'
      else if cnt <= length(digs) then fp := fp + digs[cnt]
      else fp := fp + '0'
    end;
    while length(ip) < intZ do ip := '0' + ip;
    n := length(fp);
    if not tiny then
      while (n > fracZ) and (fp[n] = '0') do n := n - 1;
    fp := copy(fp, 1, n);
    if thou and (length(ip) > 3) then
    begin
      r := '';
      for k := 1 to length(ip) do
      begin
        r := r + ip[k];
        cnt := length(ip) - k;
        if (cnt > 0) and (cnt mod 3 = 0) then r := r + ','
      end;
      ip := r
    end;
    r := ip;
    if length(fp) > 0 then r := r + '.' + fp;
    if neg then r := '-' + r;
    FormatFloat := pre + r + suf
  end
end;
function ParseFloat(s: string; var d: double): boolean;
var i, n, fracd, ex, k: integer; neg, eneg, anyd, ok: boolean; mant, p: double;
begin
  n := length(s);
  i := 1;
  ok := true;
  while (i <= n) and (s[i] = ' ') do i := i + 1;
  while (n >= i) and (s[n] = ' ') do n := n - 1;
  neg := false;
  if (i <= n) and ((s[i] = '-') or (s[i] = '+')) then
  begin
    neg := s[i] = '-';
    i := i + 1
  end;
  mant := 0.0;
  fracd := 0;
  anyd := false;
  while (i <= n) and (s[i] >= '0') and (s[i] <= '9') do
  begin
    mant := mant * 10.0 + (ord(s[i]) - ord('0'));
    anyd := true;
    i := i + 1
  end;
  if (i <= n) and (s[i] = '.') then
  begin
    i := i + 1;
    while (i <= n) and (s[i] >= '0') and (s[i] <= '9') do
    begin
      mant := mant * 10.0 + (ord(s[i]) - ord('0'));
      fracd := fracd + 1;
      anyd := true;
      i := i + 1
    end
  end;
  ex := 0;
  if anyd and (i <= n) and ((s[i] = 'e') or (s[i] = 'E')) then
  begin
    i := i + 1;
    eneg := false;
    if (i <= n) and ((s[i] = '-') or (s[i] = '+')) then
    begin
      eneg := s[i] = '-';
      i := i + 1
    end;
    k := 0;
    while (i <= n) and (s[i] >= '0') and (s[i] <= '9') do
    begin
      ex := ex * 10 + (ord(s[i]) - ord('0'));
      k := k + 1;
      i := i + 1
    end;
    if k = 0 then ok := false;
    if eneg then ex := -ex
  end;
  if (not anyd) or (i <= n) then ok := false;
  if ok then
  begin
    ex := ex - fracd;
    p := 1.0;
    for k := 1 to abs(ex) do p := p * 10.0;
    if ex < 0 then mant := mant / p else mant := mant * p;
    if neg then mant := -mant;
    d := mant
  end;
  ParseFloat := ok
end;
function StrToFloat(s: string): double;
var d: double;
begin
  d := 0.0;
  if not ParseFloat(s, d) then raise EConvertError.Create('"' + s + '" is an invalid float');
  StrToFloat := d
end;
function TryStrToFloat(s: string; var d: double): boolean;
var v: double; ok: boolean;
begin
  v := 0.0;
  ok := ParseFloat(s, v);
  if ok then d := v;
  TryStrToFloat := ok
end;
function StrToFloatDef(s: string; def: double): double;
var v: double;
begin
  v := 0.0;
  if ParseFloat(s, v) then StrToFloatDef := v else StrToFloatDef := def
end;
function BoolToStr(b: boolean; useBoolStrs: boolean = false): string;
begin
  if useBoolStrs then
  begin
    if b then BoolToStr := 'True' else BoolToStr := 'False'
  end
  else
  begin
    if b then BoolToStr := '-1' else BoolToStr := '0'
  end
end;
function IsLeapYear(year: integer): boolean;
begin
  IsLeapYear := (year mod 4 = 0) and ((year mod 100 <> 0) or (year mod 400 = 0))
end;
function EncodeDate(year, month, day: integer): double;
var a, y, m: integer; n: int64;
begin
  a := (14 - month) div 12;
  y := year + 4800 - a;
  m := month + 12 * a - 3;
  n := day + (153 * m + 2) div 5 + 365 * y + y div 4 - y div 100 + y div 400 - 32045;
  EncodeDate := n - 2415019
end;
procedure DecodeDate(dt: double; var year, month, day: word);
var n, a, b, c, d, m: int64;
begin
  n := trunc(dt);
  if (dt < 0) and (dt <> n) then n := n - 1;
  n := n + 2415019 + 32044;
  a := (4 * n + 3) div 146097;
  b := n - (146097 * a) div 4;
  c := (4 * b + 3) div 1461;
  d := b - (1461 * c) div 4;
  m := (5 * d + 2) div 153;
  day := d - (153 * m + 2) div 5 + 1;
  month := m + 3 - 12 * (m div 10);
  year := 100 * a + c - 4800 + m div 10
end;
function EncodeTime(hour, minute, sec, msec: integer): double;
begin
  EncodeTime := (hour * 3600000.0 + minute * 60000.0 + sec * 1000.0 + msec) / 86400000.0
end;
procedure DecodeTime(dt: double; var hour, minute, sec, msec: word);
var ms: int64; f: double;
begin
  f := dt - trunc(dt);
  if f < 0 then f := f + 1;
  ms := trunc(f * 86400000.0 + 0.5);
  hour := ms div 3600000;
  minute := (ms div 60000) mod 60;
  sec := (ms div 1000) mod 60;
  msec := ms mod 1000
end;
function DayOfWeek(dt: double): integer;
var n: int64;
begin
  n := trunc(dt);
  if (dt < 0) and (dt <> n) then n := n - 1;
  DayOfWeek := (((n mod 7) + 7 + 6) mod 7) + 1
end;
function Two(n: integer): string;
begin
  if n < 10 then Two := '0' + IntToStr(n) else Two := IntToStr(n)
end;
function FormatDateTime(fmt: string; dt: double): string;
const
  Mon: array[1..12] of string = ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec');
  MonL: array[1..12] of string = ('January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
  Dow: array[1..7] of string = ('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');
  DowL: array[1..7] of string = ('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday');
var y, mo, dd, h, mi, s, ms: word; i, j, run, n: integer; r: string; c: char; lastH: boolean; q: char;
begin
  DecodeDate(dt, y, mo, dd);
  DecodeTime(dt, h, mi, s, ms);
  r := '';
  lastH := false;
  i := 1;
  n := length(fmt);
  while i <= n do
  begin
    c := upcase(fmt[i]);
    if (c = '''') or (c = '"') then
    begin
      q := fmt[i];
      i := i + 1;
      while (i <= n) and (fmt[i] <> q) do
      begin
        r := r + fmt[i];
        i := i + 1
      end;
      i := i + 1
    end
    else if (c = 'Y') or (c = 'M') or (c = 'D') or (c = 'H') or (c = 'N') or (c = 'S') or (c = 'Z') then
    begin
      run := 0;
      j := i;
      while (j <= n) and (upcase(fmt[j]) = c) do
      begin
        run := run + 1;
        j := j + 1
      end;
      i := j;
      if c = 'Y' then
      begin
        if run <= 2 then r := r + Two(y mod 100) else r := r + IntToStr(y);
        lastH := false
      end
      else if c = 'M' then
      begin
        if lastH or ((i <= n) and (upcase(fmt[i]) = 'S')) then
        begin
          if run = 1 then r := r + IntToStr(mi) else r := r + Two(mi)
        end
        else if run = 1 then r := r + IntToStr(mo)
        else if run = 2 then r := r + Two(mo)
        else if run = 3 then r := r + Mon[mo]
        else r := r + MonL[mo];
        lastH := false
      end
      else if c = 'D' then
      begin
        if run = 1 then r := r + IntToStr(dd)
        else if run = 2 then r := r + Two(dd)
        else if run = 3 then r := r + Dow[DayOfWeek(dt)]
        else r := r + DowL[DayOfWeek(dt)];
        lastH := false
      end
      else if c = 'H' then
      begin
        if run = 1 then r := r + IntToStr(h) else r := r + Two(h);
        lastH := true
      end
      else if c = 'N' then
      begin
        if run = 1 then r := r + IntToStr(mi) else r := r + Two(mi);
        lastH := false
      end
      else if c = 'S' then
      begin
        if run = 1 then r := r + IntToStr(s) else r := r + Two(s);
        lastH := false
      end
      else
      begin
        if run = 1 then r := r + IntToStr(ms)
        else if ms < 10 then r := r + '00' + IntToStr(ms)
        else if ms < 100 then r := r + '0' + IntToStr(ms)
        else r := r + IntToStr(ms);
        lastH := false
      end
    end
    else
    begin
      if fmt[i] = '/' then r := r + '-' else r := r + fmt[i];
      i := i + 1
    end
  end;
  FormatDateTime := r
end;
end.
