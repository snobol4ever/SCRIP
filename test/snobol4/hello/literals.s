  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call flat_α
  xor eax, eax
  pop rbp
  ret
flat_α:
#=======================================================================================================================
    .global flat_α
    .global flat_β
    .global flat_γ
    .global flat_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_α_body:
snoch0_n0_α:
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_scalar
bb5_α:
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb6_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
bb8_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S0]
 movabs rsi, 0
 call rt_gvar_assign_int@PLT
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n10_α
snoch0_n9_α:
bb10_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S0]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
# IR_LIT_scalar
bb11_α:
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
bb12_α:
# IR_UNOP_GVAR_SLOT
 mov rax, 1
 neg rax
 mov qword ptr [r12 + 0], rax
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n14_α
 snoch0_n12_β:
 jmp snoch0_n15_α
snoch0_n13_α:
bb14_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 movabs rsi, 18446744073709551615
 call rt_gvar_assign_int@PLT
 jmp snoch0_n12_α
 snoch0_n13_β:
 jmp snoch0_n12_α
snoch0_n14_α:
bb15_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 movabs rsi, 7
 movabs rdx, 4607182418800017408
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n15_α
 snoch0_n14_β:
 jmp snoch0_n15_α
snoch0_n15_α:
# IR_LIT_scalar
bb16_α:
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n17_α
snoch0_n16_α:
bb17_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n17_α
snoch0_n17_α:
# IR_LIT_scalar
bb18_α:
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n19_α
snoch0_n18_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n19_α
 snoch0_n18_β:
 jmp snoch0_n19_α
snoch0_n19_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n21_α
snoch0_n20_α:
bb21_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n21_α
snoch0_n21_α:
# IR_LIT_scalar
bb22_α:
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
bb23_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n23_α
 snoch0_n22_β:
 jmp snoch0_n23_α
snoch0_n23_α:
# IR_LIT_scalar
bb24_α:
 jmp snoch0_n24_α
 snoch0_n23_β:
 jmp snoch0_n25_α
snoch0_n24_α:
bb25_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n25_α
 snoch0_n24_β:
 jmp snoch0_n25_α
snoch0_n25_α:
# IR_LIT_scalar
bb26_α:
 jmp snoch0_n26_α
 snoch0_n25_β:
 jmp snoch0_n27_α
snoch0_n26_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n28_α
 snoch0_n26_β:
 jmp snoch0_n27_α
snoch0_n27_α:
# IR_LIT_scalar
bb28_α:
 jmp snoch0_n29_α
 snoch0_n27_β:
 jmp snoch0_n30_α
snoch0_n28_α:
bb29_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 0
 mov rcx, 0
 add rax, rcx
 mov qword ptr [r12 + 8], rax
 jmp snoch0_n31_α
 snoch0_n28_β:
 jmp snoch0_n27_α
snoch0_n29_α:
# IR_LIT_scalar
bb30_α:
 jmp snoch0_n32_α
 snoch0_n29_β:
 jmp snoch0_n30_α
snoch0_n30_α:
# IR_LIT_scalar
bb31_α:
 jmp snoch0_n33_α
 snoch0_n30_β:
 jmp snoch0_n34_α
snoch0_n31_α:
bb32_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 8]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n27_α
 snoch0_n31_β:
 jmp snoch0_n27_α
snoch0_n32_α:
bb33_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 0
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 16], rax
 jmp snoch0_n35_α
 snoch0_n32_β:
 jmp snoch0_n30_α
snoch0_n33_α:
# IR_LIT_scalar
bb34_α:
 jmp snoch0_n36_α
 snoch0_n33_β:
 jmp snoch0_n34_α
snoch0_n34_α:
# IR_LIT_scalar
bb35_α:
 jmp snoch0_n37_α
 snoch0_n34_β:
 jmp snoch0_n38_α
snoch0_n35_α:
bb36_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 16]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n30_α
 snoch0_n35_β:
 jmp snoch0_n30_α
snoch0_n36_α:
bb37_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 1
 mov rcx, 0
 add rax, rcx
 mov qword ptr [r12 + 24], rax
 jmp snoch0_n39_α
 snoch0_n36_β:
 jmp snoch0_n34_α
snoch0_n37_α:
bb38_α:
# IR_ASSIGN_CONCAT lit_s
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n38_α
 snoch0_n37_β:
 jmp snoch0_n38_α
snoch0_n38_α:
# IR_LIT_scalar
bb39_α:
 jmp snoch0_n40_α
 snoch0_n38_β:
 jmp snoch0_n41_α
snoch0_n39_α:
bb40_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 24]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n34_α
 snoch0_n39_β:
 jmp snoch0_n34_α
snoch0_n40_α:
bb41_α:
# IR_ASSIGN_CONCAT lit_s
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n41_α
 snoch0_n40_β:
 jmp snoch0_n41_α
snoch0_n41_α:
# IR_LIT_scalar
bb42_α:
 jmp snoch0_n42_α
 snoch0_n41_β:
 jmp snoch0_n43_α
snoch0_n42_α:
bb43_α:
# IR_ASSIGN_CONCAT lit_s
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n43_α
 snoch0_n42_β:
 jmp snoch0_n43_α
snoch0_n43_α:
# IR_LIT_scalar
bb44_α:
 jmp snoch0_n44_α
 snoch0_n43_β:
 jmp snoch0_n45_α
snoch0_n44_α:
bb45_α:
# IR_ASSIGN_CONCAT lit_s
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n45_α
 snoch0_n44_β:
 jmp snoch0_n45_α
snoch0_n45_α:
# IR_LIT_scalar
bb46_α:
 jmp snoch0_n46_α
 snoch0_n45_β:
 jmp snoch0_n47_α
snoch0_n46_α:
# IR_LIT_scalar
bb47_α:
 jmp snoch0_n48_α
 snoch0_n46_β:
 jmp snoch0_n47_α
snoch0_n47_α:
# IR_LIT_scalar
bb48_α:
 jmp snoch0_n49_α
 snoch0_n47_β:
 jmp snoch0_n50_α
snoch0_n48_α:
bb49_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 1
 mov rcx, 2
 add rax, rcx
 mov qword ptr [r12 + 32], rax
 jmp snoch0_n51_α
 snoch0_n48_β:
 jmp snoch0_n47_α
snoch0_n49_α:
# IR_LIT_scalar
bb50_α:
 jmp snoch0_n52_α
 snoch0_n49_β:
 jmp snoch0_n50_α
snoch0_n50_α:
# IR_LIT_scalar
bb51_α:
 jmp snoch0_n53_α
 snoch0_n50_β:
 jmp snoch0_n54_α
snoch0_n51_α:
bb52_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 32]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n47_α
 snoch0_n51_β:
 jmp snoch0_n47_α
snoch0_n52_α:
# IR_LIT_scalar
bb53_α:
 jmp snoch0_n55_α
 snoch0_n52_β:
 jmp snoch0_n50_α
snoch0_n53_α:
# IR_LIT_scalar
bb54_α:
 jmp snoch0_n56_α
 snoch0_n53_β:
 jmp snoch0_n54_α
snoch0_n54_α:
# IR_LIT_scalar
bb55_α:
 jmp snoch0_n57_α
 snoch0_n54_β:
 jmp flat_γ
snoch0_n55_α:
bb56_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 2
 mov rcx, 3
 imul rax, rcx
 mov qword ptr [r12 + 40], rax
 jmp snoch0_n58_α
 snoch0_n55_β:
 jmp snoch0_n50_α
snoch0_n56_α:
bb57_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 1
 mov rcx, 2
 add rax, rcx
 mov qword ptr [r12 + 48], rax
 jmp snoch0_n59_α
 snoch0_n56_β:
 jmp snoch0_n54_α
snoch0_n57_α:
# IR_LIT_scalar
bb58_α:
 jmp snoch0_n60_α
 snoch0_n57_β:
 jmp flat_γ
snoch0_n58_α:
bb59_α:
# IR_BINOP_GVAR_ARITH_SLOT
 mov rax, 1
 mov rcx, qword ptr [r12 + 40]
 add rax, rcx
 mov qword ptr [r12 + 64], rax
 movabs rax, 6
 mov qword ptr [r12 + 56], rax
 jmp snoch0_n61_α
 snoch0_n58_β:
 jmp snoch0_n50_α
snoch0_n59_α:
# IR_LIT_scalar
bb60_α:
 jmp snoch0_n62_α
 snoch0_n59_β:
 jmp snoch0_n54_α
snoch0_n60_α:
# IR_LIT_scalar
bb61_α:
 jmp snoch0_n63_α
 snoch0_n60_β:
 jmp flat_γ
snoch0_n61_α:
bb62_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 56]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n50_α
 snoch0_n61_β:
 jmp snoch0_n50_α
snoch0_n62_α:
bb63_α:
# IR_BINOP_GVAR_ARITH_SLOT
 mov rax, qword ptr [r12 + 48]
 mov rcx, 3
 imul rax, rcx
 mov qword ptr [r12 + 72], rax
 movabs rax, 6
 mov qword ptr [r12 + 64], rax
 jmp snoch0_n64_α
 snoch0_n62_β:
 jmp snoch0_n54_α
snoch0_n63_α:
bb64_α:
# IR_BINOP_GVAR_ARITH
 mov rax, 2
 mov rcx, 3
 imul rax, rcx
 mov qword ptr [r12 + 72], rax
 jmp snoch0_n65_α
 snoch0_n63_β:
 jmp flat_γ
snoch0_n64_α:
bb65_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 64]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n54_α
 snoch0_n64_β:
 jmp snoch0_n54_α
snoch0_n65_α:
bb66_α:
# IR_BINOP_GVAR_ARITH_SLOT
 mov rax, 1
 mov rcx, qword ptr [r12 + 72]
 add rax, rcx
 mov qword ptr [r12 + 88], rax
 movabs rax, 6
 mov qword ptr [r12 + 80], rax
 jmp snoch0_n66_α
 snoch0_n65_β:
 jmp flat_γ
snoch0_n66_α:
bb67_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 80]
 call rt_gvar_assign_int@PLT
 jmp flat_γ
 snoch0_n66_β:
 jmp flat_γ
flat_β:
jmp flat_ω
flat_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string "OUTPUT"
.S1: .string ""
.S2: .string "Hello World!"
.S3: .string "1"
.S4: .string "1.0"
.S5: .string "I'm here"
.S6: .string "\"Quote of the day\""
.S7: .string "Z"
.S8: .string "A"
.S9: .string "AZ"
.text
