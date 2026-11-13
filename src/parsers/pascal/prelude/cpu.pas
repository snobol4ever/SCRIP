unit cpu;
interface
uses sysutils;
function InterlockedCompareExchange128Support: boolean;
function AESSupport: boolean;
function AVXSupport: boolean;
function AVX2Support: boolean;
function FMASupport: boolean;
function POPCNTSupport: boolean;
var is_sse3_cpu: boolean;
function InterlockedCompareExchange128(var Target: Int128Rec; NewValue: Int128Rec; Comperand: Int128Rec): Int128Rec;
implementation
function InterlockedCompareExchange128Support: boolean;
begin
  InterlockedCompareExchange128Support := (__pas_cpuid(1, 0, 2) and $2000) <> 0
end;
function AESSupport: boolean;
begin
  AESSupport := (__pas_cpuid(1, 0, 2) and $2000000) <> 0
end;
function POPCNTSupport: boolean;
begin
  POPCNTSupport := (__pas_cpuid(1, 0, 2) and $800000) <> 0
end;
function AVXSupport: boolean;
begin
  AVXSupport := ((__pas_cpuid(1, 0, 2) and $8000000) <> 0) and ((__pas_cpuid(0, 0, 4) and 6) = 6) and ((__pas_cpuid(1, 0, 2) and $10000000) <> 0)
end;
function FMASupport: boolean;
begin
  FMASupport := AVXSupport and ((__pas_cpuid(1, 0, 2) and $1000) <> 0)
end;
function AVX2Support: boolean;
begin
  AVX2Support := AVXSupport and ((__pas_cpuid(7, 0, 1) and $20) <> 0)
end;
function InterlockedCompareExchange128(var Target: Int128Rec; NewValue: Int128Rec; Comperand: Int128Rec): Int128Rec;
begin
  InterlockedCompareExchange128 := Target;
  if (Target.Lo = Comperand.Lo) and (Target.Hi = Comperand.Hi) then Target := NewValue
end;
begin
  is_sse3_cpu := (__pas_cpuid(1, 0, 2) and 1) <> 0
end.
