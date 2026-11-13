{$H+}
unit gmp;
interface
type
  mpz_t = array of longint;
procedure mpz_init(var __sh_r: mpz_t);
procedure mpz_clear(var __sh_r: mpz_t);
procedure mpz_init_set_ui(var __sh_r: mpz_t; __sh_v: qword);
procedure mpz_set_ui(var __sh_r: mpz_t; __sh_v: qword);
procedure mpz_add(var __sh_r, __sh_a, __sh_b: mpz_t);
procedure mpz_add_ui(var __sh_r, __sh_a: mpz_t; __sh_v: qword);
procedure mpz_mul_ui(var __sh_r, __sh_a: mpz_t; __sh_v: qword);
procedure mpz_pow_ui(var __sh_r, __sh_a: mpz_t; __sh_e: qword);
procedure mpz_ui_pow_ui(var __sh_r: mpz_t; __sh_b, __sh_e: qword);
procedure mpz_fdiv_q(var __sh_q, __sh_n, __sh_d: mpz_t);
procedure mpz_fdiv_q_ui(var __sh_q, __sh_n: mpz_t; __sh_d: qword);
function mpz_fdiv_ui(var __sh_n: mpz_t; __sh_d: qword): qword;
function mpz_get_ui(var __sh_n: mpz_t): qword;
function mpz_sizeinbase(var __sh_n: mpz_t; __sh_b: longint): qword;
function mpz_get_str(__sh_s: PAnsiChar; __sh_base: longint; var __sh_n: mpz_t): PAnsiChar;
implementation
type
  __sh_gbox = record
    __sh_s: ansistring;
  end;
function __sh_mn(const __sh_a: mpz_t): longint;
begin
  if length(__sh_a) = 0 then __sh_mn := 0 else __sh_mn := length(__sh_a) - 1
end;
function __sh_mcopy(const __sh_a: mpz_t): mpz_t;
var __sh_t: mpz_t; i: longint;
begin
  setlength(__sh_t, length(__sh_a));
  for i := 0 to length(__sh_a) - 1 do __sh_t[i] := __sh_a[i];
  __sh_mcopy := __sh_t
end;
function __sh_mtrim(var __sh_t: mpz_t; n: longint): longint;
begin
  while (n > 0) and (__sh_t[n] = 0) do n := n - 1;
  setlength(__sh_t, n + 1);
  if n = 0 then __sh_t[0] := 0;
  __sh_mtrim := n
end;
function __sh_mneg(const __sh_a: mpz_t): boolean;
begin
  if length(__sh_a) = 0 then __sh_mneg := false else __sh_mneg := __sh_a[0] <> 0
end;
function __sh_mcmp(const __sh_a, __sh_b: mpz_t): longint;
var na, nb, i: longint;
begin
  na := __sh_mn(__sh_a);
  nb := __sh_mn(__sh_b);
  if na <> nb then begin
    if na < nb then __sh_mcmp := -1 else __sh_mcmp := 1;
    exit
  end;
  for i := na downto 1 do
    if __sh_a[i] <> __sh_b[i] then begin
      if __sh_a[i] < __sh_b[i] then __sh_mcmp := -1 else __sh_mcmp := 1;
      exit
    end;
  __sh_mcmp := 0
end;
function __sh_madd(const __sh_a, __sh_b: mpz_t): mpz_t;
var __sh_t: mpz_t; na, nb, n, i: longint; c, s: int64;
begin
  na := __sh_mn(__sh_a);
  nb := __sh_mn(__sh_b);
  if na > nb then n := na else n := nb;
  setlength(__sh_t, n + 2);
  __sh_t[0] := 0;
  c := 0;
  for i := 1 to n do begin
    s := c;
    if i <= na then s := s + __sh_a[i];
    if i <= nb then s := s + __sh_b[i];
    if s >= 1000000000 then begin
      __sh_t[i] := s - 1000000000;
      c := 1
    end else begin
      __sh_t[i] := s;
      c := 0
    end
  end;
  __sh_t[n + 1] := c;
  __sh_mtrim(__sh_t, n + 1);
  __sh_madd := __sh_t
end;
function __sh_msub(const __sh_a, __sh_b: mpz_t): mpz_t;
var __sh_t: mpz_t; na, nb, i: longint; c, s: int64;
begin
  na := __sh_mn(__sh_a);
  nb := __sh_mn(__sh_b);
  setlength(__sh_t, na + 1);
  __sh_t[0] := 0;
  c := 0;
  for i := 1 to na do begin
    s := __sh_a[i] - c;
    if i <= nb then s := s - __sh_b[i];
    if s < 0 then begin
      __sh_t[i] := s + 1000000000;
      c := 1
    end else begin
      __sh_t[i] := s;
      c := 0
    end
  end;
  __sh_mtrim(__sh_t, na);
  __sh_msub := __sh_t
end;
function __sh_mmullimb(const __sh_a: mpz_t; m: int64): mpz_t;
var __sh_t: mpz_t; na, i: longint; c, s: int64;
begin
  na := __sh_mn(__sh_a);
  setlength(__sh_t, na + 2);
  __sh_t[0] := 0;
  c := 0;
  for i := 1 to na do begin
    s := __sh_a[i] * m + c;
    c := s div 1000000000;
    __sh_t[i] := s - c * 1000000000
  end;
  __sh_t[na + 1] := c;
  __sh_mtrim(__sh_t, na + 1);
  __sh_mmullimb := __sh_t
end;
function __sh_mmulv(const __sh_a: mpz_t; v: int64): mpz_t;
var __sh_t: mpz_t; na, nt, i, c, s, v0, v1, v2, a0, a1, a2: int64;
begin
  na := __sh_mn(__sh_a);
  v0 := v mod 1000000000;
  v1 := (v div 1000000000) mod 1000000000;
  v2 := v div 1000000000000000000;
  nt := na + 3;
  setlength(__sh_t, nt + 1);
  __sh_t[0] := 0;
  c := 0;
  a1 := 0;
  a2 := 0;
  if v2 = 0 then begin
    for i := 1 to na do begin
      a0 := __sh_a[i];
      s := a0 * v0 + a1 * v1 + c;
      c := s div 1000000000;
      __sh_t[i] := s - c * 1000000000;
      a1 := a0
    end
  end else begin
    for i := 1 to na do begin
      a0 := __sh_a[i];
      s := a0 * v0 + a1 * v1 + a2 * v2 + c;
      c := s div 1000000000;
      __sh_t[i] := s - c * 1000000000;
      a2 := a1;
      a1 := a0
    end
  end;
  for i := na + 1 to nt do begin
    s := a1 * v1 + a2 * v2 + c;
    c := s div 1000000000;
    __sh_t[i] := s - c * 1000000000;
    a2 := a1;
    a1 := 0
  end;
  __sh_mtrim(__sh_t, nt);
  __sh_mmulv := __sh_t
end;
function __sh_mmul(const __sh_a, __sh_b: mpz_t): mpz_t;
var __sh_t: mpz_t; na, nb, i, j, k: longint; c, s: int64; ai: int64;
begin
  na := __sh_mn(__sh_a);
  nb := __sh_mn(__sh_b);
  setlength(__sh_t, na + nb + 1);
  for i := 0 to na + nb do __sh_t[i] := 0;
  for i := 1 to na do begin
    ai := __sh_a[i];
    c := 0;
    if ai <> 0 then begin
      for j := 1 to nb do begin
        k := i + j - 1;
        s := __sh_t[k] + ai * __sh_b[j] + c;
        c := s div 1000000000;
        __sh_t[k] := s - c * 1000000000
      end;
      k := i + nb;
      while c > 0 do begin
        s := __sh_t[k] + c;
        c := s div 1000000000;
        __sh_t[k] := s - c * 1000000000;
        k := k + 1
      end
    end
  end;
  __sh_mtrim(__sh_t, na + nb);
  __sh_mmul := __sh_t
end;
function __sh_msqr(const __sh_a: mpz_t): mpz_t;
var __sh_t: mpz_t; na, i, j, k: longint; c, v, ai: int64;
begin
  na := __sh_mn(__sh_a);
  setlength(__sh_t, 2 * na + 1);
  for i := 0 to 2 * na do __sh_t[i] := 0;
  for i := 1 to na - 1 do begin
    ai := __sh_a[i];
    c := 0;
    if ai <> 0 then begin
      for j := i + 1 to na do begin
        k := i + j - 1;
        v := __sh_t[k] + ai * __sh_a[j] + c;
        c := v div 1000000000;
        __sh_t[k] := v - c * 1000000000
      end;
      __sh_t[i + na] := c
    end
  end;
  c := 0;
  for k := 1 to 2 * na do begin
    v := 2 * __sh_t[k] + c;
    if v >= 1000000000 then begin
      __sh_t[k] := v - 1000000000;
      c := 1
    end else begin
      __sh_t[k] := v;
      c := 0
    end
  end;
  c := 0;
  for i := 1 to na do begin
    ai := __sh_a[i];
    v := __sh_t[2 * i - 1] + ai * ai + c;
    c := v div 1000000000;
    __sh_t[2 * i - 1] := v - c * 1000000000;
    v := __sh_t[2 * i] + c;
    c := v div 1000000000;
    __sh_t[2 * i] := v - c * 1000000000
  end;
  __sh_mtrim(__sh_t, 2 * na);
  __sh_msqr := __sh_t
end;
function __sh_mfromu(__sh_v: qword): mpz_t;
var __sh_t: mpz_t; __sh_c: mpz_t; w: int64; n: longint; __sh_hi: boolean;
begin
  __sh_hi := (__sh_v shr 63) <> 0;
  w := __sh_v and $7FFFFFFFFFFFFFFF;
  setlength(__sh_t, 4);
  n := 0;
  while w > 0 do begin
    n := n + 1;
    __sh_t[n] := w mod 1000000000;
    w := w div 1000000000
  end;
  __sh_t[0] := 0;
  __sh_mtrim(__sh_t, n);
  if __sh_hi then begin
    setlength(__sh_c, 4);
    __sh_c[0] := 0;
    __sh_c[1] := 854775808;
    __sh_c[2] := 223372036;
    __sh_c[3] := 9;
    __sh_t := __sh_madd(__sh_t, __sh_c)
  end;
  __sh_mfromu := __sh_t
end;
function __sh_mtou(const __sh_a: mpz_t): qword;
var i: longint; acc: qword;
begin
  acc := 0;
  for i := __sh_mn(__sh_a) downto 1 do acc := acc * 1000000000 + __sh_a[i];
  __sh_mtou := acc
end;
procedure __sh_mdivsmall(var __sh_a: mpz_t; d: int64; var rem: int64);
var i: longint; r, cur: int64;
begin
  r := 0;
  for i := __sh_mn(__sh_a) downto 1 do begin
    cur := r * 1000000000 + __sh_a[i];
    __sh_a[i] := cur div d;
    r := cur - (cur div d) * d
  end;
  __sh_mtrim(__sh_a, __sh_mn(__sh_a));
  rem := r
end;
procedure __sh_mdivmod(const __sh_n, __sh_d: mpz_t; var __sh_q, __sh_r: mpz_t);
var __sh_u, __sh_v: mpz_t; nn, nd, j, i: longint; f, qh, rh, bor, c, p, rem: int64; ub, vt: int64;
begin
  nn := __sh_mn(__sh_n);
  nd := __sh_mn(__sh_d);
  if __sh_mcmp(__sh_n, __sh_d) < 0 then begin
    __sh_r := __sh_mcopy(__sh_n);
    setlength(__sh_q, 1);
    __sh_q[0] := 0;
    exit
  end;
  if nd = 1 then begin
    __sh_q := __sh_mcopy(__sh_n);
    __sh_mdivsmall(__sh_q, __sh_d[1], rem);
    setlength(__sh_r, 2);
    __sh_r[0] := 0;
    __sh_r[1] := rem;
    __sh_mtrim(__sh_r, 1);
    exit
  end;
  f := 1000000000 div (__sh_d[nd] + 1);
  setlength(__sh_u, nn + 2);
  setlength(__sh_v, nd + 1);
  c := 0;
  for i := 1 to nn do begin
    p := __sh_n[i] * f + c;
    c := p div 1000000000;
    __sh_u[i] := p - c * 1000000000
  end;
  __sh_u[nn + 1] := c;
  c := 0;
  for i := 1 to nd do begin
    p := __sh_d[i] * f + c;
    c := p div 1000000000;
    __sh_v[i] := p - c * 1000000000
  end;
  setlength(__sh_q, nn - nd + 2);
  for i := 0 to nn - nd + 1 do __sh_q[i] := 0;
  for j := nn - nd downto 0 do begin
    ub := __sh_u[j + nd + 1] * 1000000000 + __sh_u[j + nd];
    vt := __sh_v[nd];
    qh := ub div vt;
    rh := ub - qh * vt;
    while (qh >= 1000000000) or (qh * __sh_v[nd - 1] > rh * 1000000000 + __sh_u[j + nd - 1]) do begin
      qh := qh - 1;
      rh := rh + vt;
      if rh >= 1000000000 then break
    end;
    bor := 0;
    c := 0;
    for i := 1 to nd do begin
      p := qh * __sh_v[i] + c;
      c := p div 1000000000;
      p := p - c * 1000000000;
      rem := __sh_u[j + i] - p - bor;
      if rem < 0 then begin
        rem := rem + 1000000000;
        bor := 1
      end else bor := 0;
      __sh_u[j + i] := rem
    end;
    rem := __sh_u[j + nd + 1] - c - bor;
    if rem < 0 then begin
      rem := rem + 1000000000;
      bor := 1
    end else bor := 0;
    __sh_u[j + nd + 1] := rem;
    if bor <> 0 then begin
      qh := qh - 1;
      c := 0;
      for i := 1 to nd do begin
        p := __sh_u[j + i] + __sh_v[i] + c;
        if p >= 1000000000 then begin
          p := p - 1000000000;
          c := 1
        end else c := 0;
        __sh_u[j + i] := p
      end;
      __sh_u[j + nd + 1] := (__sh_u[j + nd + 1] + c) mod 1000000000
    end;
    __sh_q[j + 1] := qh
  end;
  __sh_mtrim(__sh_q, nn - nd + 1);
  setlength(__sh_u, nd + 1);
  __sh_u[0] := 0;
  __sh_mtrim(__sh_u, nd);
  __sh_mdivsmall(__sh_u, f, rem);
  __sh_r := __sh_u
end;
procedure __sh_msetsign(var __sh_t: mpz_t; __sh_neg: boolean);
begin
  if length(__sh_t) = 0 then exit;
  if __sh_neg and (length(__sh_t) > 1) then __sh_t[0] := 1 else __sh_t[0] := 0
end;
procedure mpz_init(var __sh_r: mpz_t);
begin
  setlength(__sh_r, 1);
  __sh_r[0] := 0
end;
procedure mpz_clear(var __sh_r: mpz_t);
begin
  setlength(__sh_r, 0)
end;
procedure mpz_set_ui(var __sh_r: mpz_t; __sh_v: qword);
begin
  __sh_r := __sh_mfromu(__sh_v)
end;
procedure mpz_init_set_ui(var __sh_r: mpz_t; __sh_v: qword);
begin
  __sh_r := __sh_mfromu(__sh_v)
end;
procedure mpz_add(var __sh_r, __sh_a, __sh_b: mpz_t);
var __sh_t: mpz_t; c: longint;
begin
  if __sh_mneg(__sh_a) = __sh_mneg(__sh_b) then begin
    __sh_t := __sh_madd(__sh_a, __sh_b);
    __sh_msetsign(__sh_t, __sh_mneg(__sh_a))
  end else begin
    c := __sh_mcmp(__sh_a, __sh_b);
    if c = 0 then begin
      setlength(__sh_t, 1);
      __sh_t[0] := 0
    end else if c > 0 then begin
      __sh_t := __sh_msub(__sh_a, __sh_b);
      __sh_msetsign(__sh_t, __sh_mneg(__sh_a))
    end else begin
      __sh_t := __sh_msub(__sh_b, __sh_a);
      __sh_msetsign(__sh_t, __sh_mneg(__sh_b))
    end
  end;
  __sh_r := __sh_t
end;
procedure mpz_add_ui(var __sh_r, __sh_a: mpz_t; __sh_v: qword);
var __sh_u: mpz_t;
begin
  __sh_u := __sh_mfromu(__sh_v);
  mpz_add(__sh_r, __sh_a, __sh_u)
end;
procedure mpz_mul_ui(var __sh_r, __sh_a: mpz_t; __sh_v: qword);
var __sh_t, __sh_u: mpz_t; __sh_neg: boolean;
begin
  __sh_neg := __sh_mneg(__sh_a);
  if (__sh_v shr 63) = 0 then begin
    if __sh_v < 1000000000 then __sh_t := __sh_mmullimb(__sh_a, __sh_v)
    else __sh_t := __sh_mmulv(__sh_a, __sh_v)
  end else begin
    __sh_u := __sh_mfromu(__sh_v);
    __sh_t := __sh_mmul(__sh_a, __sh_u)
  end;
  __sh_msetsign(__sh_t, __sh_neg);
  __sh_r := __sh_t
end;
procedure mpz_pow_ui(var __sh_r, __sh_a: mpz_t; __sh_e: qword);
var __sh_res, __sh_b: mpz_t; __sh_neg: boolean; e: qword;
begin
  __sh_neg := __sh_mneg(__sh_a) and ((__sh_e and 1) = 1);
  __sh_b := __sh_mcopy(__sh_a);
  setlength(__sh_res, 2);
  __sh_res[0] := 0;
  __sh_res[1] := 1;
  e := __sh_e;
  while e > 0 do begin
    if (e and 1) = 1 then __sh_res := __sh_mmul(__sh_res, __sh_b);
    e := e shr 1;
    if e > 0 then __sh_b := __sh_msqr(__sh_b)
  end;
  __sh_msetsign(__sh_res, __sh_neg);
  __sh_r := __sh_res
end;
procedure mpz_ui_pow_ui(var __sh_r: mpz_t; __sh_b, __sh_e: qword);
var __sh_x: mpz_t; k, q, rm, i, n: longint; p: int64; __sh_t: mpz_t;
begin
  if __sh_b = 10 then begin
    q := __sh_e div 9;
    rm := __sh_e mod 9;
    p := 1;
    for i := 1 to rm do p := p * 10;
    setlength(__sh_t, q + 2);
    for i := 0 to q do __sh_t[i] := 0;
    __sh_t[q + 1] := p;
    __sh_r := __sh_t;
    exit
  end;
  __sh_x := __sh_mfromu(__sh_b);
  mpz_pow_ui(__sh_r, __sh_x, __sh_e)
end;
procedure __sh_mfloor(const __sh_n, __sh_d: mpz_t; var __sh_q, __sh_r: mpz_t);
var __sh_one: mpz_t; __sh_nneg, __sh_dneg: boolean;
begin
  __sh_nneg := __sh_mneg(__sh_n);
  __sh_dneg := __sh_mneg(__sh_d);
  __sh_mdivmod(__sh_n, __sh_d, __sh_q, __sh_r);
  if (__sh_nneg <> __sh_dneg) and (__sh_mn(__sh_r) > 0) then begin
    setlength(__sh_one, 2);
    __sh_one[0] := 0;
    __sh_one[1] := 1;
    __sh_q := __sh_madd(__sh_q, __sh_one);
    __sh_r := __sh_msub(__sh_d, __sh_r);
    __sh_msetsign(__sh_r, __sh_dneg)
  end else __sh_msetsign(__sh_r, __sh_nneg);
  __sh_msetsign(__sh_q, __sh_nneg <> __sh_dneg)
end;
procedure mpz_fdiv_q(var __sh_q, __sh_n, __sh_d: mpz_t);
var __sh_qq, __sh_rr: mpz_t;
begin
  __sh_mfloor(__sh_n, __sh_d, __sh_qq, __sh_rr);
  __sh_q := __sh_qq
end;
procedure mpz_fdiv_q_ui(var __sh_q, __sh_n: mpz_t; __sh_d: qword);
var __sh_dd, __sh_qq, __sh_rr: mpz_t;
begin
  __sh_dd := __sh_mfromu(__sh_d);
  __sh_mfloor(__sh_n, __sh_dd, __sh_qq, __sh_rr);
  __sh_q := __sh_qq
end;
function mpz_fdiv_ui(var __sh_n: mpz_t; __sh_d: qword): qword;
var __sh_dd, __sh_qq, __sh_rr: mpz_t;
begin
  __sh_dd := __sh_mfromu(__sh_d);
  __sh_mfloor(__sh_n, __sh_dd, __sh_qq, __sh_rr);
  mpz_fdiv_ui := __sh_mtou(__sh_rr)
end;
function mpz_get_ui(var __sh_n: mpz_t): qword;
begin
  mpz_get_ui := __sh_mtou(__sh_n)
end;
function __sh_mdigits(const __sh_a: mpz_t): longint;
var n, d: longint; top: int64;
begin
  n := __sh_mn(__sh_a);
  if n = 0 then begin __sh_mdigits := 1; exit end;
  top := __sh_a[n];
  d := 0;
  while top > 0 do begin
    d := d + 1;
    top := top div 10
  end;
  __sh_mdigits := d + 9 * (n - 1)
end;
function __sh_mbits(const __sh_a: mpz_t): longint;
var n, k: longint; lg, fr: double; __sh_p, __sh_x: mpz_t;
begin
  n := __sh_mn(__sh_a);
  if n = 0 then begin __sh_mbits := 0; exit end;
  if n = 1 then lg := ln(__sh_a[1]) / ln(2)
  else if n = 2 then lg := ln(__sh_a[2] * 1e9 + __sh_a[1]) / ln(2)
  else lg := ln((__sh_a[n] * 1e9 + __sh_a[n - 1]) * 1e9 + __sh_a[n - 2]) / ln(2) + 9 * (n - 3) * (ln(10) / ln(2));
  k := trunc(lg + 0.5);
  fr := lg - k;
  if fr < 0 then fr := -fr;
  if fr < 1e-6 then begin
    mpz_ui_pow_ui(__sh_p, 2, k);
    __sh_x := __sh_a;
    if __sh_mcmp(__sh_x, __sh_p) < 0 then __sh_mbits := k else __sh_mbits := k + 1;
    exit
  end;
  __sh_mbits := trunc(lg) + 1
end;
function mpz_sizeinbase(var __sh_n: mpz_t; __sh_b: longint): qword;
var bits, k: longint; lb: double;
begin
  bits := __sh_mbits(__sh_n);
  if bits = 0 then begin mpz_sizeinbase := 1; exit end;
  k := 0;
  case __sh_b of
    2: k := 1;
    4: k := 2;
    8: k := 3;
    16: k := 4;
    32: k := 5;
  end;
  if k > 0 then mpz_sizeinbase := (bits + k - 1) div k
  else begin
    lb := ln(2) / ln(__sh_b);
    mpz_sizeinbase := trunc(bits * lb) + 1
  end
end;
function __sh_mpad9(v: longint): ansistring;
var __sh_p: ansistring; j: longint;
begin
  __sh_p := '000000000';
  for j := 9 downto 1 do begin
    __sh_p[j] := chr(48 + v mod 10);
    v := v div 10
  end;
  __sh_mpad9 := __sh_p
end;
function __sh_mchars(const __sh_a: mpz_t; base: longint): ansistring;
var __sh_w: mpz_t; __sh_s: ansistring; i, n, k, j, digs: longint; chunk, rem: int64; __sh_neg: boolean; d: int64; __sh_ch: char;
begin
  __sh_neg := __sh_mneg(__sh_a);
  n := __sh_mn(__sh_a);
  if n = 0 then begin __sh_mchars := '0'; exit end;
  __sh_s := '';
  if base = 10 then begin
    __sh_s := __sh_mpad9(__sh_a[n]);
    k := 1;
    while (k < 9) and (__sh_s[k] = '0') do k := k + 1;
    __sh_s := copy(__sh_s, k, 10 - k);
    for i := n - 1 downto 1 do __sh_s := __sh_s + __sh_mpad9(__sh_a[i])
  end else begin
    __sh_w := __sh_mcopy(__sh_a);
    digs := 0;
    chunk := base;
    k := 1;
    while chunk * base < 2000000000 do begin
      chunk := chunk * base;
      k := k + 1
    end;
    setlength(__sh_s, 0);
    __sh_s := '';
    while __sh_mn(__sh_w) > 0 do begin
      __sh_mdivsmall(__sh_w, chunk, rem);
      for j := 1 to k do begin
        d := rem mod base;
        rem := rem div base;
        if d < 10 then __sh_ch := chr(48 + d) else __sh_ch := chr(87 + d);
        __sh_s := __sh_s + __sh_ch;
        if (__sh_mn(__sh_w) = 0) and (rem = 0) then break
      end
    end;
    n := length(__sh_s);
    while (n > 1) and (__sh_s[n] = '0') do n := n - 1;
    setlength(__sh_s, n);
    n := length(__sh_s);
    for i := 1 to n div 2 do begin
      __sh_ch := __sh_s[i];
      __sh_s[i] := __sh_s[n + 1 - i];
      __sh_s[n + 1 - i] := __sh_ch
    end
  end;
  if __sh_neg then __sh_s := '-' + __sh_s;
  __sh_mchars := __sh_s
end;
function mpz_get_str(__sh_s: PAnsiChar; __sh_base: longint; var __sh_n: mpz_t): PAnsiChar;
var __sh_txt: ansistring; __sh_bx: ^__sh_gbox; i, n: longint;
begin
  __sh_txt := __sh_mchars(__sh_n, __sh_base);
  n := length(__sh_txt);
  if __sh_s = nil then begin
    new(__sh_bx);
    __sh_bx^.__sh_s := __sh_txt;
    mpz_get_str := PAnsiChar(__sh_bx^.__sh_s)
  end else begin
    for i := 1 to n do __sh_s[i - 1] := __sh_txt[i];
    __sh_s[n] := #0;
    mpz_get_str := __sh_s
  end
end;
end.
