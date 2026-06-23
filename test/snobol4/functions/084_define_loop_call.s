  .intel_syntax noprefix
  .text
bump_α:
#=======================================================================================================================
    .global bump_α
    .global bump_β
    .global bump_γ
    .global bump_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
bump_α_body:
snoch0_n0_α:
# IR_VAR
bb1_α:
 mov rdi, qword ptr [rip + .Lx2_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "v"
snoch0_n1_α:
# IR_LIT_scalar
bb2_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
snoch0_n2_β:
jmp bump_γ
jmp bump_γ
snoch0_n3_α:
bb3_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S0]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 16], rax
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n2_α
snoch0_n4_α:
bb4_α:
# IR_ASSIGN
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 16]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n4_β:
 jmp snoch0_n2_α
bump_β:
jmp bump_ω
bump_γ:
mov eax, 1
xor edx, edx
pop r12
ret
bump_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "bump"
  .Lpp0_0: .string "v"
  .Lpnames0:
  .quad .Lpp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
proc_startup:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + .Lpnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + bump_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 24
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .globl main
main:
  push rbp
  mov rbp, rsp
  call proc_startup
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
snoch7_n0_α:
bb5_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S2]
 call rt_proc_define@PLT
 jmp snoch7_n1_α
 snoch7_n0_β:
 jmp snoch7_n1_α
snoch7_n1_α:
# IR_LIT_scalar
bb6_α:
 jmp snoch7_n2_α
 snoch7_n1_β:
 jmp snoch7_n3_α
snoch7_n2_α:
bb7_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S3]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp snoch7_n3_α
 snoch7_n2_β:
 jmp snoch7_n3_α
snoch7_n3_α:
# IR_LIT_scalar
bb8_α:
 jmp snoch7_n4_α
 snoch7_n3_β:
 jmp snoch7_n5_α
snoch7_n4_α:
bb9_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S5]
 movabs rsi, 0
 call rt_gvar_assign_int@PLT
 jmp snoch7_n5_α
 snoch7_n4_β:
 jmp snoch7_n5_α
snoch7_n5_α:
# IR_VAR
bb10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch7_n6_α
 snoch7_n5_β:
 jmp snoch7_n7_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "S"
snoch7_n6_α:
xgvarg15_done:
bb11_α:
# BOX IR_CALL bump(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+32]
 movabs rax, 2
 mov qword ptr [r12 + 48], rax
   lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov rcx, rax
 mov rax, qword ptr [r12 + 48]
 imul rax, rcx
 mov qword ptr [r12 + 32], 6
 mov qword ptr [r12 + 40], rax
  .section .rodata
  .Lprocfn17: .string "bump"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn17]
 lea rsi, [r12 + 32]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je snoch7_n7_α
 jmp snoch7_n8_α
snoch7_n6_β:
 jmp snoch7_n7_α
snoch7_n7_α:
# IR_VAR
bb12_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp snoch7_n9_α
 snoch7_n7_β:
 jmp snoch7_n10_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "J"
snoch7_n8_α:
# IR_VAR
bb13_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xgvcat20_0d
 xgvcat20_0b:
 jmp snoch7_n7_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "S"
xgvcat20_0d:
xgvarg24_done:
bb14_α:
# BOX IR_CALL bump(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+112]
 movabs rax, 2
 mov qword ptr [r12 + 128], rax
   lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov rcx, rax
 mov rax, qword ptr [r12 + 128]
 imul rax, rcx
 mov qword ptr [r12 + 112], 6
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lprocfn26: .string "bump"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn26]
 lea rsi, [r12 + 112]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch7_n7_α
 jmp xgvcat23_1d
xgvcat23_1b:
 jmp snoch7_n7_α
xgvcat23_1d:
bb15_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 0]
 mov rsi, qword ptr [r12 + 8]
 mov rdx, qword ptr [r12 + 16]
 mov rcx, qword ptr [r12 + 24]
 call str_concat_d@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp snoch7_n11_α
 snoch7_n8_β:
 jmp snoch7_n7_α
snoch7_n9_α:
# IR_LIT_scalar
bb16_α:
 jmp snoch7_n12_α
 snoch7_n9_β:
 jmp snoch7_n10_α
snoch7_n10_α:
# IR_VAR
bb17_α:
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xgvarg29_done
 xgvarg29_β:
 jmp snoch7_n13_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "J"
xgvarg29_done:
# IR_LIT_I
bb18_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg32_done
 xgvarg32_β:
 jmp snoch7_n13_α
.Lx33_0:
 .quad 5
xgvarg32_done:
bb19_α:
# BOX IR_CALL LT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+208]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+224]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn35: .string "LT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je snoch7_n13_α
 jmp snoch7_n5_α
snoch7_n10_β:
 jmp snoch7_n13_α
snoch7_n11_α:
bb20_α:
# IR_ASSIGN
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 call rt_gvar_assign_descr@PLT
 jmp snoch7_n7_α
 snoch7_n11_β:
 jmp snoch7_n7_α
snoch7_n12_α:
bb21_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 240], rax
 jmp snoch7_n14_α
 snoch7_n12_β:
 jmp snoch7_n10_α
snoch7_n13_α:
# IR_VAR
bb22_α:
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 248], rax
 mov qword ptr [r12 + 256], rdx
 jmp snoch7_n15_α
 snoch7_n13_β:
 jmp flat_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "S"
snoch7_n14_α:
bb23_α:
# IR_ASSIGN
 lea rdi, [rip + .S5]
 mov rsi, qword ptr [r12 + 240]
 call rt_gvar_assign_int@PLT
 jmp snoch7_n10_α
 snoch7_n14_β:
 jmp snoch7_n10_α
snoch7_n15_α:
bb24_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp flat_γ
 snoch7_n15_β:
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
.S0: .string "v"
.S1: .string "bump"
.S2: .string "bump(v)"
.S3: .string "S"
.S4: .string ""
.S5: .string "J"
.S6: .string "OUTPUT"
.text
