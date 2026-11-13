{$mode objfpc}{$H+}
unit RegExpr;
interface
type
  TRegExpr = class
  private
    FExpr: string;
    FInput: string;
    FPat: string;
    FPp: integer;
    FAk, FAv, FAa, FAb, FAmin, FAmax, FAg: array of integer;
    FAn: integer;
    FNk, FNv, FNx, FNy: array of integer;
    FNn: integer;
    FEntry: integer;
    FNgroups: integer;
    FNslots: integer;
    FCls: array of string;
    FNcls: integer;
    FMk: array of integer;
    FCap: array of integer;
    FMend: integer;
    FLast: integer;
    FMatched: boolean;
    function ANew(k, v, a, b: integer): integer;
    function NNew(k, v, x, y: integer): integer;
    function PAlt: integer;
    function PSeq: integer;
    function PRep: integer;
    function PAtom: integer;
    function PClass: integer;
    function PEsc(var cl: string; inclass: boolean): integer;
    function Emit(a, nxt: integer): integer;
    function Run(n, i: integer): boolean;
    function IsWord(i: integer): boolean;
    function TryAt(start: integer): boolean;
    procedure SetExpr(const AValue: string);
    function GetMatch(Idx: integer): string;
    function GetMatchPos(Idx: integer): integer;
    function GetMatchLen(Idx: integer): integer;
    function GetCount: integer;
    function Subst(const rep: string): string;
  public
    constructor Create; overload;
    constructor Create(const AExpression: string); overload;
    function Exec(const AInputString: string): boolean;
    function ExecNext: boolean;
    function Replace(const AInputStr, AReplaceStr: string; AUseSubstitution: boolean): string;
    property Expression: string read FExpr write SetExpr;
    property InputString: string read FInput;
    property Match[Idx: integer]: string read GetMatch;
    property MatchPos[Idx: integer]: integer read GetMatchPos;
    property MatchLen[Idx: integer]: integer read GetMatchLen;
    property SubExprMatchCount: integer read GetCount;
  end;
function ExecRegExpr(const ARegExpr, AInputStr: string): boolean;
function ReplaceRegExpr(const ARegExpr, AInputStr, AReplaceStr: string; AUseSubstitution: boolean): string;
implementation
const
  KCHR = 1; KANY = 2; KCLS = 3; KSEQ = 4; KALT = 5; KGRP = 6; KREP = 7; KBOL = 8; KEOL = 9; KWB = 10; KNWB = 11; KEMP = 12;
  NCHR = 1; NANY = 2; NCLS = 3; NSPL = 4; NSAV = 5; NBOL = 6; NEOL = 7; NWB = 8; NNWB = 9; NMAT = 10; NMRK = 11; NPRG = 12;
constructor TRegExpr.Create;
begin
  inherited Create;
  FExpr := '';
  FNn := 0;
  FEntry := -1;
  FMatched := false;
end;
constructor TRegExpr.Create(const AExpression: string);
begin
  inherited Create;
  FMatched := false;
  SetExpr(AExpression);
end;
function TRegExpr.ANew(k, v, a, b: integer): integer;
begin
  if FAn >= length(FAk) then
  begin
    setlength(FAk, FAn * 2 + 16);
    setlength(FAv, FAn * 2 + 16);
    setlength(FAa, FAn * 2 + 16);
    setlength(FAb, FAn * 2 + 16);
    setlength(FAmin, FAn * 2 + 16);
    setlength(FAmax, FAn * 2 + 16);
    setlength(FAg, FAn * 2 + 16);
  end;
  FAk[FAn] := k;
  FAv[FAn] := v;
  FAa[FAn] := a;
  FAb[FAn] := b;
  FAmin[FAn] := 0;
  FAmax[FAn] := 0;
  FAg[FAn] := 1;
  ANew := FAn;
  FAn := FAn + 1;
end;
function TRegExpr.NNew(k, v, x, y: integer): integer;
begin
  if FNn >= length(FNk) then
  begin
    setlength(FNk, FNn * 2 + 16);
    setlength(FNv, FNn * 2 + 16);
    setlength(FNx, FNn * 2 + 16);
    setlength(FNy, FNn * 2 + 16);
  end;
  FNk[FNn] := k;
  FNv[FNn] := v;
  FNx[FNn] := x;
  FNy[FNn] := y;
  NNew := FNn;
  FNn := FNn + 1;
end;
function TRegExpr.PEsc(var cl: string; inclass: boolean): integer;
var c: char; i: integer; h: integer;
  procedure AddRange(lo, hi: integer; neg: boolean);
  var j: integer;
  begin
    for j := 0 to 255 do
      if ((j >= lo) and (j <= hi)) <> neg then cl[j + 1] := '1';
  end;
begin
  PEsc := -1;
  FPp := FPp + 1;
  if FPp > length(FPat) then begin PEsc := 92; exit; end;
  c := FPat[FPp];
  FPp := FPp + 1;
  case c of
    'd': begin AddRange(48, 57, false); PEsc := -2; end;
    'D': begin AddRange(48, 57, true); PEsc := -2; end;
    'w': begin AddRange(48, 57, false); AddRange(65, 90, false); AddRange(97, 122, false); AddRange(95, 95, false); PEsc := -2; end;
    'W': begin
      for i := 0 to 255 do
        if not (((i >= 48) and (i <= 57)) or ((i >= 65) and (i <= 90)) or ((i >= 97) and (i <= 122)) or (i = 95)) then cl[i + 1] := '1';
      PEsc := -2;
    end;
    's': begin AddRange(9, 10, false); AddRange(12, 13, false); AddRange(32, 32, false); PEsc := -2; end;
    'S': begin
      for i := 0 to 255 do
        if not ((i = 9) or (i = 10) or (i = 12) or (i = 13) or (i = 32)) then cl[i + 1] := '1';
      PEsc := -2;
    end;
    'n': PEsc := 10;
    'r': PEsc := 13;
    't': PEsc := 9;
    'f': PEsc := 12;
    'a': PEsc := 7;
    'e': PEsc := 27;
    'x': begin
      h := 0;
      i := 0;
      while (i < 2) and (FPp <= length(FPat)) and (pos(upcase(FPat[FPp]), '0123456789ABCDEF') > 0) do
      begin
        h := h * 16 + pos(upcase(FPat[FPp]), '0123456789ABCDEF') - 1;
        FPp := FPp + 1;
        i := i + 1;
      end;
      PEsc := h;
    end;
    'b': if inclass then PEsc := 8 else PEsc := -3;
    'B': if inclass then PEsc := 66 else PEsc := -4;
  else
    PEsc := ord(c);
  end;
end;
function TRegExpr.PClass: integer;
var cl: string; neg: boolean; lo, hi, e, i: integer; scratch: string;
begin
  cl := '';
  for i := 1 to 256 do cl := cl + '0';
  FPp := FPp + 1;
  neg := false;
  if (FPp <= length(FPat)) and (FPat[FPp] = '^') then begin neg := true; FPp := FPp + 1; end;
  i := 0;
  while (FPp <= length(FPat)) and ((FPat[FPp] <> ']') or (i = 0)) do
  begin
    i := i + 1;
    if FPat[FPp] = '\' then
    begin
      e := PEsc(cl, true);
      if e = -2 then continue;
      lo := e;
    end
    else
    begin
      lo := ord(FPat[FPp]);
      FPp := FPp + 1;
    end;
    hi := lo;
    if (FPp + 1 <= length(FPat)) and (FPat[FPp] = '-') and (FPat[FPp + 1] <> ']') then
    begin
      FPp := FPp + 1;
      if FPat[FPp] = '\' then
      begin
        scratch := cl;
        e := PEsc(scratch, true);
        hi := e;
      end
      else
      begin
        hi := ord(FPat[FPp]);
        FPp := FPp + 1;
      end;
    end;
    while lo <= hi do
    begin
      cl[lo + 1] := '1';
      lo := lo + 1;
    end;
  end;
  if FPp <= length(FPat) then FPp := FPp + 1;
  if neg then
    for i := 1 to 256 do
      if cl[i] = '1' then cl[i] := '0' else cl[i] := '1';
  if FNcls >= length(FCls) then setlength(FCls, FNcls * 2 + 4);
  FCls[FNcls] := cl;
  FNcls := FNcls + 1;
  PClass := ANew(KCLS, FNcls - 1, 0, 0);
end;
function TRegExpr.PAtom: integer;
var c: char; k, inner, e: integer; cl: string; i: integer;
begin
  c := FPat[FPp];
  case c of
    '(': begin
      FPp := FPp + 1;
      if (FPp + 1 <= length(FPat)) and (FPat[FPp] = '?') and (FPat[FPp + 1] = ':') then
      begin
        FPp := FPp + 2;
        inner := PAlt;
        if (FPp <= length(FPat)) and (FPat[FPp] = ')') then FPp := FPp + 1;
        PAtom := inner;
      end
      else
      begin
        FNgroups := FNgroups + 1;
        k := FNgroups;
        inner := PAlt;
        if (FPp <= length(FPat)) and (FPat[FPp] = ')') then FPp := FPp + 1;
        PAtom := ANew(KGRP, k, inner, 0);
      end;
    end;
    '[': PAtom := PClass;
    '.': begin FPp := FPp + 1; PAtom := ANew(KANY, 0, 0, 0); end;
    '^': begin FPp := FPp + 1; PAtom := ANew(KBOL, 0, 0, 0); end;
    '$': begin FPp := FPp + 1; PAtom := ANew(KEOL, 0, 0, 0); end;
    '\': begin
      cl := '';
      for i := 1 to 256 do cl := cl + '0';
      e := PEsc(cl, false);
      if e = -2 then
      begin
        if FNcls >= length(FCls) then setlength(FCls, FNcls * 2 + 4);
        FCls[FNcls] := cl;
        FNcls := FNcls + 1;
        PAtom := ANew(KCLS, FNcls - 1, 0, 0);
      end
      else if e = -3 then PAtom := ANew(KWB, 0, 0, 0)
      else if e = -4 then PAtom := ANew(KNWB, 0, 0, 0)
      else PAtom := ANew(KCHR, e, 0, 0);
    end;
  else
    begin
      FPp := FPp + 1;
      PAtom := ANew(KCHR, ord(c), 0, 0);
    end;
  end;
end;
function TRegExpr.PRep: integer;
var a, r, mn, mx, save, j: integer; c: char; ok: boolean;
begin
  a := PAtom;
  while FPp <= length(FPat) do
  begin
    c := FPat[FPp];
    mn := 0;
    mx := -1;
    if c = '*' then begin mn := 0; mx := -1; FPp := FPp + 1; end
    else if c = '+' then begin mn := 1; mx := -1; FPp := FPp + 1; end
    else if c = '?' then begin mn := 0; mx := 1; FPp := FPp + 1; end
    else if c = '{' then
    begin
      save := FPp;
      FPp := FPp + 1;
      ok := false;
      mn := 0;
      j := 0;
      while (FPp <= length(FPat)) and (FPat[FPp] >= '0') and (FPat[FPp] <= '9') do
      begin
        mn := mn * 10 + ord(FPat[FPp]) - 48;
        FPp := FPp + 1;
        j := j + 1;
      end;
      if j > 0 then
      begin
        if (FPp <= length(FPat)) and (FPat[FPp] = '}') then begin mx := mn; ok := true; FPp := FPp + 1; end
        else if (FPp <= length(FPat)) and (FPat[FPp] = ',') then
        begin
          FPp := FPp + 1;
          if (FPp <= length(FPat)) and (FPat[FPp] = '}') then begin mx := -1; ok := true; FPp := FPp + 1; end
          else
          begin
            mx := 0;
            j := 0;
            while (FPp <= length(FPat)) and (FPat[FPp] >= '0') and (FPat[FPp] <= '9') do
            begin
              mx := mx * 10 + ord(FPat[FPp]) - 48;
              FPp := FPp + 1;
              j := j + 1;
            end;
            if (j > 0) and (FPp <= length(FPat)) and (FPat[FPp] = '}') then begin ok := true; FPp := FPp + 1; end;
          end;
        end;
      end;
      if not ok then begin FPp := save; break; end;
    end
    else break;
    r := ANew(KREP, 0, a, 0);
    FAmin[r] := mn;
    FAmax[r] := mx;
    if (FPp <= length(FPat)) and (FPat[FPp] = '?') then begin FAg[r] := 0; FPp := FPp + 1; end;
    a := r;
  end;
  PRep := a;
end;
function TRegExpr.PSeq: integer;
var a, b: integer;
begin
  a := -1;
  while (FPp <= length(FPat)) and (FPat[FPp] <> '|') and (FPat[FPp] <> ')') do
  begin
    b := PRep;
    if a < 0 then a := b else a := ANew(KSEQ, 0, a, b);
  end;
  if a < 0 then a := ANew(KEMP, 0, 0, 0);
  PSeq := a;
end;
function TRegExpr.PAlt: integer;
var a, b: integer;
begin
  a := PSeq;
  while (FPp <= length(FPat)) and (FPat[FPp] = '|') do
  begin
    FPp := FPp + 1;
    b := PSeq;
    a := ANew(KALT, 0, a, b);
  end;
  PAlt := a;
end;
function TRegExpr.Emit(a, nxt: integer): integer;
var e, e1, l, pg, mk, body, cur, i, slot: integer;
begin
  case FAk[a] of
    KCHR: Emit := NNew(NCHR, FAv[a], nxt, 0);
    KANY: Emit := NNew(NANY, 0, nxt, 0);
    KCLS: Emit := NNew(NCLS, FAv[a], nxt, 0);
    KBOL: Emit := NNew(NBOL, 0, nxt, 0);
    KEOL: Emit := NNew(NEOL, 0, nxt, 0);
    KWB: Emit := NNew(NWB, 0, nxt, 0);
    KNWB: Emit := NNew(NNWB, 0, nxt, 0);
    KEMP: Emit := nxt;
    KSEQ: begin
      e := Emit(FAb[a], nxt);
      Emit := Emit(FAa[a], e);
    end;
    KALT: begin
      e := Emit(FAa[a], nxt);
      e1 := Emit(FAb[a], nxt);
      Emit := NNew(NSPL, 0, e, e1);
    end;
    KGRP: begin
      e1 := NNew(NSAV, 2 * FAv[a] + 1, nxt, 0);
      body := Emit(FAa[a], e1);
      Emit := NNew(NSAV, 2 * FAv[a], body, 0);
    end;
    KREP: begin
      cur := nxt;
      if FAmax[a] < 0 then
      begin
        slot := FNslots;
        FNslots := FNslots + 1;
        l := NNew(NSPL, 0, 0, 0);
        pg := NNew(NPRG, slot, l, 0);
        body := Emit(FAa[a], pg);
        mk := NNew(NMRK, slot, body, 0);
        if FAg[a] = 1 then begin FNx[l] := mk; FNy[l] := nxt; end
        else begin FNx[l] := nxt; FNy[l] := mk; end;
        cur := l;
      end
      else
        for i := 1 to FAmax[a] - FAmin[a] do
        begin
          body := Emit(FAa[a], cur);
          if FAg[a] = 1 then cur := NNew(NSPL, 0, body, nxt)
          else cur := NNew(NSPL, 0, nxt, body);
        end;
      for i := 1 to FAmin[a] do cur := Emit(FAa[a], cur);
      Emit := cur;
    end;
  else
    Emit := nxt;
  end;
end;
procedure TRegExpr.SetExpr(const AValue: string);
var a, m: integer;
begin
  FExpr := AValue;
  FPat := AValue;
  FPp := 1;
  FAn := 0;
  FNn := 0;
  FNgroups := 0;
  FNslots := 0;
  FNcls := 0;
  FMatched := false;
  setlength(FAk, 0); setlength(FAv, 0); setlength(FAa, 0); setlength(FAb, 0);
  setlength(FAmin, 0); setlength(FAmax, 0); setlength(FAg, 0);
  setlength(FNk, 0); setlength(FNv, 0); setlength(FNx, 0); setlength(FNy, 0);
  setlength(FCls, 0);
  a := PAlt;
  while FPp <= length(FPat) do
  begin
    FPp := FPp + 1;
    if FPp <= length(FPat) then a := ANew(KSEQ, 0, a, PAlt);
  end;
  m := NNew(NMAT, 0, 0, 0);
  FEntry := Emit(a, m);
  setlength(FMk, FNslots + 1);
  setlength(FCap, 2 * FNgroups + 2);
end;
function TRegExpr.IsWord(i: integer): boolean;
var c: char;
begin
  IsWord := false;
  if (i < 1) or (i > length(FInput)) then exit;
  c := FInput[i];
  IsWord := ((c >= '0') and (c <= '9')) or ((c >= 'A') and (c <= 'Z')) or ((c >= 'a') and (c <= 'z')) or (c = '_');
end;
function TRegExpr.Run(n, i: integer): boolean;
var old, k: integer;
begin
  Run := false;
  case FNk[n] of
    NCHR: if (i <= length(FInput)) and (ord(FInput[i]) = FNv[n]) then Run := Run(FNx[n], i + 1);
    NANY: if i <= length(FInput) then Run := Run(FNx[n], i + 1);
    NCLS: if (i <= length(FInput)) and (FCls[FNv[n]][ord(FInput[i]) + 1] = '1') then Run := Run(FNx[n], i + 1);
    NSPL: begin
      if Run(FNx[n], i) then Run := true
      else Run := Run(FNy[n], i);
    end;
    NSAV: begin
      old := FCap[FNv[n]];
      FCap[FNv[n]] := i;
      if Run(FNx[n], i) then Run := true
      else FCap[FNv[n]] := old;
    end;
    NBOL: if i = 1 then Run := Run(FNx[n], i);
    NEOL: if i = length(FInput) + 1 then Run := Run(FNx[n], i);
    NWB: if IsWord(i - 1) <> IsWord(i) then Run := Run(FNx[n], i);
    NNWB: if IsWord(i - 1) = IsWord(i) then Run := Run(FNx[n], i);
    NMAT: begin FMend := i; Run := true; end;
    NMRK: begin
      k := FMk[FNv[n]];
      FMk[FNv[n]] := i;
      if Run(FNx[n], i) then Run := true
      else FMk[FNv[n]] := k;
    end;
    NPRG: if FMk[FNv[n]] <> i then Run := Run(FNx[n], i);
  end;
end;
function TRegExpr.TryAt(start: integer): boolean;
var j: integer;
begin
  for j := 0 to length(FCap) - 1 do FCap[j] := -1;
  for j := 0 to length(FMk) - 1 do FMk[j] := -1;
  TryAt := false;
  if Run(FEntry, start) then
  begin
    FCap[0] := start;
    FCap[1] := FMend;
    TryAt := true;
  end;
end;
function TRegExpr.Exec(const AInputString: string): boolean;
var s: integer;
begin
  FInput := AInputString;
  Exec := false;
  FMatched := false;
  if FEntry < 0 then exit;
  s := 1;
  while s <= length(FInput) + 1 do
  begin
    if TryAt(s) then
    begin
      FMatched := true;
      FLast := FMend;
      if FMend = s then FLast := s + 1;
      Exec := true;
      exit;
    end;
    s := s + 1;
  end;
end;
function TRegExpr.ExecNext: boolean;
var s: integer;
begin
  ExecNext := false;
  if (FEntry < 0) or not FMatched then exit;
  s := FLast;
  FMatched := false;
  while s <= length(FInput) + 1 do
  begin
    if TryAt(s) then
    begin
      FMatched := true;
      FLast := FMend;
      if FMend = s then FLast := s + 1;
      ExecNext := true;
      exit;
    end;
    s := s + 1;
  end;
end;
function TRegExpr.GetMatch(Idx: integer): string;
begin
  GetMatch := '';
  if (not FMatched) or (Idx < 0) or (2 * Idx + 1 >= length(FCap)) then exit;
  if (FCap[2 * Idx] < 1) or (FCap[2 * Idx + 1] < FCap[2 * Idx]) then exit;
  GetMatch := copy(FInput, FCap[2 * Idx], FCap[2 * Idx + 1] - FCap[2 * Idx]);
end;
function TRegExpr.GetMatchPos(Idx: integer): integer;
begin
  GetMatchPos := -1;
  if (not FMatched) or (Idx < 0) or (2 * Idx + 1 >= length(FCap)) then exit;
  if (FCap[2 * Idx] < 1) or (FCap[2 * Idx + 1] < FCap[2 * Idx]) then exit;
  GetMatchPos := FCap[2 * Idx];
end;
function TRegExpr.GetMatchLen(Idx: integer): integer;
begin
  GetMatchLen := -1;
  if (not FMatched) or (Idx < 0) or (2 * Idx + 1 >= length(FCap)) then exit;
  if (FCap[2 * Idx] < 1) or (FCap[2 * Idx + 1] < FCap[2 * Idx]) then exit;
  GetMatchLen := FCap[2 * Idx + 1] - FCap[2 * Idx];
end;
function TRegExpr.GetCount: integer;
begin
  GetCount := -1;
  if FMatched then GetCount := FNgroups;
end;
function TRegExpr.Subst(const rep: string): string;
var i, g: integer; r: string; c: char;
begin
  r := '';
  i := 1;
  while i <= length(rep) do
  begin
    c := rep[i];
    if (c = '$') and (i < length(rep)) and (rep[i + 1] >= '0') and (rep[i + 1] <= '9') then
    begin
      g := ord(rep[i + 1]) - 48;
      r := r + GetMatch(g);
      i := i + 2;
    end
    else if (c = '$') and (i + 3 <= length(rep)) and (rep[i + 1] = '{') and (rep[i + 2] >= '0') and (rep[i + 2] <= '9') and (rep[i + 3] = '}') then
    begin
      g := ord(rep[i + 2]) - 48;
      r := r + GetMatch(g);
      i := i + 4;
    end
    else if (c = '$') and (i < length(rep)) and (rep[i + 1] = '&') then
    begin
      r := r + GetMatch(0);
      i := i + 2;
    end
    else if (c = '\') and (i < length(rep)) then
    begin
      i := i + 1;
      case rep[i] of
        'n': r := r + #10;
        'r': r := r + #13;
        't': r := r + #9;
      else
        r := r + rep[i];
      end;
      i := i + 1;
    end
    else
    begin
      r := r + c;
      i := i + 1;
    end;
  end;
  Subst := r;
end;
function TRegExpr.Replace(const AInputStr, AReplaceStr: string; AUseSubstitution: boolean): string;
var res, input: string; pos0, ms, me: integer; more: boolean;
begin
  res := '';
  input := AInputStr;
  pos0 := 1;
  more := Exec(input);
  while more do
  begin
    ms := FCap[0];
    me := FCap[1];
    res := res + copy(input, pos0, ms - pos0);
    if AUseSubstitution then res := res + Subst(AReplaceStr)
    else res := res + AReplaceStr;
    pos0 := me;
    if me = ms then
    begin
      if ms <= length(input) then res := res + copy(input, ms, 1);
      pos0 := ms + 1;
    end;
    more := ExecNext;
  end;
  if pos0 <= length(input) then res := res + copy(input, pos0, length(input) - pos0 + 1);
  Replace := res;
end;
function ExecRegExpr(const ARegExpr, AInputStr: string): boolean;
var r: TRegExpr;
begin
  r := TRegExpr.Create(ARegExpr);
  ExecRegExpr := r.Exec(AInputStr);
  r.Free;
end;
function ReplaceRegExpr(const ARegExpr, AInputStr, AReplaceStr: string; AUseSubstitution: boolean): string;
var r: TRegExpr;
begin
  r := TRegExpr.Create(ARegExpr);
  ReplaceRegExpr := r.Replace(AInputStr, AReplaceStr, AUseSubstitution);
  r.Free;
end;
end.
