{$H+}
unit sha1;
interface
type
  TSHA1Digest = array[0..19] of byte;
function SHA1String(const __sh_s: string): TSHA1Digest;
function SHA1Print(const __sh_d: TSHA1Digest): string;
function SHA1Match(const __sh_a, __sh_b: TSHA1Digest): boolean;
implementation
function __sh_rotl(x: int64; n: longint): int64;
begin
  __sh_rotl := ((x shl n) or (x shr (32 - n))) and $FFFFFFFF
end;
function SHA1String(const __sh_s: string): TSHA1Digest;
var
  __sh_w: array[0..79] of int64;
  __sh_m: array of byte;
  __sh_r: TSHA1Digest;
  n, total, blk, i, t, nb, k: longint;
  h0, h1, h2, h3, h4, a, b, c, d, e, f, tmp: int64;
  bits: int64;
begin
  n := length(__sh_s);
  total := ((n + 8) div 64 + 1) * 64;
  setlength(__sh_m, total);
  for i := 0 to total - 1 do __sh_m[i] := 0;
  for i := 1 to n do __sh_m[i - 1] := ord(__sh_s[i]);
  __sh_m[n] := 128;
  bits := n;
  bits := bits * 8;
  for i := 0 to 7 do begin
    __sh_m[total - 1 - i] := bits and 255;
    bits := bits shr 8
  end;
  h0 := $67452301;
  h1 := $EFCDAB89;
  h2 := $98BADCFE;
  h3 := $10325476;
  h4 := $C3D2E1F0;
  nb := total div 64;
  for blk := 0 to nb - 1 do begin
    for t := 0 to 15 do begin
      k := blk * 64 + t * 4;
      __sh_w[t] := ((__sh_m[k] * 256 + __sh_m[k + 1]) * 256 + __sh_m[k + 2]) * 256 + __sh_m[k + 3]
    end;
    for t := 16 to 79 do
      __sh_w[t] := __sh_rotl(__sh_w[t - 3] xor __sh_w[t - 8] xor __sh_w[t - 14] xor __sh_w[t - 16], 1);
    a := h0; b := h1; c := h2; d := h3; e := h4;
    for t := 0 to 79 do begin
      if t < 20 then begin
        f := (b and c) or ((b xor $FFFFFFFF) and d);
        k := $5A827999
      end else if t < 40 then begin
        f := b xor c xor d;
        k := $6ED9EBA1
      end else if t < 60 then begin
        f := (b and c) or (b and d) or (c and d);
        k := $8F1BBCDC
      end else begin
        f := b xor c xor d;
        k := $CA62C1D6
      end;
      tmp := (__sh_rotl(a, 5) + f + e + k + __sh_w[t]) and $FFFFFFFF;
      e := d;
      d := c;
      c := __sh_rotl(b, 30);
      b := a;
      a := tmp
    end;
    h0 := (h0 + a) and $FFFFFFFF;
    h1 := (h1 + b) and $FFFFFFFF;
    h2 := (h2 + c) and $FFFFFFFF;
    h3 := (h3 + d) and $FFFFFFFF;
    h4 := (h4 + e) and $FFFFFFFF
  end;
  for i := 0 to 3 do begin
    __sh_r[i] := (h0 shr (24 - 8 * i)) and 255;
    __sh_r[4 + i] := (h1 shr (24 - 8 * i)) and 255;
    __sh_r[8 + i] := (h2 shr (24 - 8 * i)) and 255;
    __sh_r[12 + i] := (h3 shr (24 - 8 * i)) and 255;
    __sh_r[16 + i] := (h4 shr (24 - 8 * i)) and 255
  end;
  SHA1String := __sh_r
end;
function SHA1Print(const __sh_d: TSHA1Digest): string;
var __sh_o: string; i: longint; v: longint;
begin
  __sh_o := '';
  for i := 0 to 19 do begin
    v := __sh_d[i] div 16;
    if v < 10 then __sh_o := __sh_o + chr(48 + v) else __sh_o := __sh_o + chr(87 + v);
    v := __sh_d[i] mod 16;
    if v < 10 then __sh_o := __sh_o + chr(48 + v) else __sh_o := __sh_o + chr(87 + v)
  end;
  SHA1Print := __sh_o
end;
function SHA1Match(const __sh_a, __sh_b: TSHA1Digest): boolean;
var i: longint; __sh_ok: boolean;
begin
  __sh_ok := true;
  for i := 0 to 19 do if __sh_a[i] <> __sh_b[i] then __sh_ok := false;
  SHA1Match := __sh_ok
end;
end.
