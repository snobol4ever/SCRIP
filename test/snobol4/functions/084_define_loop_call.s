  .intel_syntax noprefix
  .text
  .globl proc_bump_α
proc_bump_α:
#=======================================================================================================================
    .global proc_bump_α
    .global proc_bump_β
    .global proc_bump_γ
    .global proc_bump_ω
push r12
  mov r12, rdi
proc_bump_α_body:
xchain0_n0_α:
# IR_VAR gva
bb1_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_bump_γ
xchain0_n1_α:
# IR_LIT_INTEGER
bb2_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_bump_γ
.Lx2_0:
 .quad 1
xchain0_n2_α:
bb3_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 48]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 64]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 48]
 cmp eax, 6
 jne .Lx3_2
 mov eax, dword ptr [r12 + 64]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [r12 + 56]
 mov rcx, qword ptr [r12 + 72]
 add rax, rcx
 mov qword ptr [r12 + 32], 6
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 mov r8d, 0
 lea r9, [r12 + 32]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_bump_γ
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bump_γ
xchain0_n3_α:
# IR_ASSIGN gva
bb4_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_bump_γ
 xchain0_n3_β:
 jmp proc_bump_γ
proc_bump_β:
jmp proc_bump_ω
proc_bump_γ:
mov eax, 1
xor edx, edx
pop r12
ret
proc_bump_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "bump"
  .Lstartup_pp0_0: .string "v"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_bump_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "bump"
  .Lgvan1: .string "v"
  .Lgvan2: .string "S"
  .Lgvan3: .string "J"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
main_α_body:
xchain5_n0_α:
# IR_LIT_STRING
bb5_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n2_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string ""
xchain5_n1_α:
# IR_ASSIGN gva
bb6_α:
 mov rax, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n2_α
xchain5_n2_α:
# IR_LIT_INTEGER
bb7_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n4_α
.Lx8_0:
 .quad 0
xchain5_n3_α:
# IR_ASSIGN gva
bb8_α:
 mov rax, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp xchain5_n4_α
xchain5_n4_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp xchain5_n9_α
xchain5_n5_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain5_n6_α
 xchain5_n5_β:
 jmp xchain5_n9_α
.Lx11_0:
 .quad 2
xchain5_n6_α:
# IR_VAR gva
bb11_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain5_n7_α
 xchain5_n6_β:
 jmp xchain5_n9_α
xchain5_n7_α:
bb12_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 240]
 cmp eax, 100
 je .Lx13_0
 mov eax, dword ptr [r12 + 256]
 cmp eax, 100
 je .Lx13_0
 mov eax, dword ptr [r12 + 240]
 cmp eax, 6
 jne .Lx13_2
 mov eax, dword ptr [r12 + 256]
 cmp eax, 6
 jne .Lx13_2
.Lx13_1:
 mov rax, qword ptr [r12 + 248]
 mov rcx, qword ptr [r12 + 264]
 imul rax, rcx
 mov qword ptr [r12 + 224], 6
 mov qword ptr [r12 + 232], rax
 jmp xchain5_n8_α
.Lx13_0:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 mov r8d, 2
 lea r9, [r12 + 224]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx13_3
.Lx13_2:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n9_α
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
.Lx13_3:
 jmp xchain5_n8_α
 xchain5_n7_β:
 jmp xchain5_n9_α
xchain5_n8_α:
bb13_α:
  .section .rodata
  .Lcall13_pname: .string "bump"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall13_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain5_n9_α
 jmp xchain5_n10_α
xchain5_n8_β:
 jmp xchain5_n9_α
xchain5_n9_α:
# IR_VAR gva
bb14_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain5_n11_α
 xchain5_n9_β:
 jmp xchain5_n15_α
xchain5_n10_α:
bb15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 mov rdx, qword ptr [r12 + 192]
 mov rcx, qword ptr [r12 + 200]
 call str_concat_d@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain5_n12_α
 xchain5_n10_β:
 jmp xchain5_n9_α
xchain5_n11_α:
# IR_LIT_INTEGER
bb16_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain5_n13_α
 xchain5_n11_β:
 jmp xchain5_n15_α
.Lx17_0:
 .quad 1
xchain5_n12_α:
# IR_ASSIGN gva
bb17_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain5_n9_α
 xchain5_n12_β:
 jmp xchain5_n9_α
xchain5_n13_α:
bb18_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 304]
 cmp eax, 100
 je .Lx19_0
 mov eax, dword ptr [r12 + 320]
 cmp eax, 100
 je .Lx19_0
 mov eax, dword ptr [r12 + 304]
 cmp eax, 6
 jne .Lx19_2
 mov eax, dword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx19_2
.Lx19_1:
 mov rax, qword ptr [r12 + 312]
 mov rcx, qword ptr [r12 + 328]
 add rax, rcx
 mov qword ptr [r12 + 288], 6
 mov qword ptr [r12 + 296], rax
 jmp xchain5_n14_α
.Lx19_0:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8d, 0
 lea r9, [r12 + 288]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx19_3
.Lx19_2:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n15_α
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
.Lx19_3:
 jmp xchain5_n14_α
 xchain5_n13_β:
 jmp xchain5_n15_α
xchain5_n14_α:
# IR_ASSIGN gva
bb19_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain5_n15_α
 xchain5_n14_β:
 jmp xchain5_n15_α
xchain5_n15_α:
# IR_VAR gva
bb20_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain5_n16_α
 xchain5_n15_β:
 jmp xchain5_n18_α
xchain5_n16_α:
# IR_LIT_INTEGER
bb21_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain5_n17_α
 xchain5_n16_β:
 jmp xchain5_n18_α
.Lx22_0:
 .quad 5
xchain5_n17_α:
bb22_α:
# BOX IR_CALL LT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+384] -> [r12+352]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lrkfn24: .string "LT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn24]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain5_n18_α
 jmp xchain5_n4_α
 xchain5_n17_β:
 jmp xchain5_n18_α
xchain5_n18_α:
# IR_VAR gva
bb23_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain5_n19_α
 xchain5_n18_β:
 jmp main_γ
xchain5_n19_α:
# IR_ASSIGN global
bb24_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp main_γ
 xchain5_n19_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
