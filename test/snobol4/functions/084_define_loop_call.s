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
bb13_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 0]
 mov rsi, qword ptr [r12 + 8]
 mov rdx, qword ptr [r12 + 16]
 mov rcx, qword ptr [r12 + 24]
 call str_concat_d@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp snoch7_n11_α
 snoch7_n8_β:
 jmp snoch7_n7_α
snoch7_n9_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch7_n12_α
 snoch7_n9_β:
 jmp snoch7_n10_α
snoch7_n10_α:
# IR_VAR
bb15_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch7_n13_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "J"
xgvarg22_done:
# IR_LIT_I
bb16_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg25_done
 xgvarg25_β:
 jmp snoch7_n13_α
.Lx26_0:
 .quad 5
xgvarg25_done:
bb17_α:
# BOX IR_CALL LT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+96] -> [r12+144]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+160]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn28: .string "LT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch7_n13_α
 jmp snoch7_n5_α
snoch7_n10_β:
 jmp snoch7_n13_α
snoch7_n11_α:
bb18_α:
# IR_ASSIGN
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp snoch7_n7_α
 snoch7_n11_β:
 jmp snoch7_n7_α
snoch7_n12_α:
bb19_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 176], rax
 jmp snoch7_n14_α
 snoch7_n12_β:
 jmp snoch7_n10_α
snoch7_n13_α:
# IR_VAR
bb20_α:
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 184], rax
 mov qword ptr [r12 + 192], rdx
 jmp snoch7_n15_α
 snoch7_n13_β:
 jmp flat_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "S"
snoch7_n14_α:
bb21_α:
# IR_ASSIGN
 lea rdi, [rip + .S5]
 mov rsi, qword ptr [r12 + 176]
 call rt_gvar_assign_int@PLT
 jmp snoch7_n10_α
 snoch7_n14_β:
 jmp snoch7_n10_α
snoch7_n15_α:
bb22_α:
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
