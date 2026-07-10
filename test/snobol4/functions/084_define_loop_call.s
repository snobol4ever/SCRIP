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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 496], rax
 pop rsi
proc_bump_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_bump_γ
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_bump_γ
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 96]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 112]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 96]
 cmp eax, 6
 jne .Lx3_2
 mov eax, dword ptr [r12 + 112]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [r12 + 104]
 mov rcx, qword ptr [r12 + 120]
 add rax, rcx
 mov qword ptr [r12 + 80], 6
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 112]
 mov rcx, qword ptr [r12 + 120]
 mov r8d, 0
 lea r9, [r12 + 80]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 112]
 mov rcx, qword ptr [r12 + 120]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_bump_γ
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bump_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp proc_bump_γ
 xchain0_n3_β:
 jmp proc_bump_γ
proc_bump_β:
jmp proc_bump_ω
proc_bump_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 496]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_bump_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
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
  call core_lib_init@PLT
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 496], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain5_n0_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n2_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string ""
# IR_ASSIGN gva
 xchain5_n1_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n2_α
# IR_LIT_INTEGER
 xchain5_n2_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n4_α
.Lx8_0:
 .quad 0
# IR_ASSIGN gva
 xchain5_n3_α:
 mov rax, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp xchain5_n4_α
# IR_VAR
 xchain5_n4_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp xchain5_n6_α
# IR_LIT_INTEGER
 xchain5_n5_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain5_n7_α
 xchain5_n5_β:
 jmp xchain5_n6_α
.Lx11_0:
 .quad 2
# IR_VAR
 xchain5_n6_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain5_n8_α
 xchain5_n6_β:
 jmp xchain5_n9_α
# IR_VAR
 xchain5_n7_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain5_n10_α
 xchain5_n7_β:
 jmp xchain5_n6_α
# IR_LIT_INTEGER
 xchain5_n8_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain5_n11_α
 xchain5_n8_β:
 jmp xchain5_n9_α
.Lx14_0:
 .quad 1
# IR_VAR
 xchain5_n9_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain5_n12_α
 xchain5_n9_β:
 jmp xchain5_n13_α
 xchain5_n10_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 288]
 cmp eax, 100
 je .Lx16_0
 mov eax, dword ptr [r12 + 304]
 cmp eax, 100
 je .Lx16_0
 mov eax, dword ptr [r12 + 288]
 cmp eax, 6
 jne .Lx16_2
 mov eax, dword ptr [r12 + 304]
 cmp eax, 6
 jne .Lx16_2
.Lx16_1:
 mov rax, qword ptr [r12 + 296]
 mov rcx, qword ptr [r12 + 312]
 imul rax, rcx
 mov qword ptr [r12 + 272], 6
 mov qword ptr [r12 + 280], rax
 jmp xchain5_n14_α
.Lx16_0:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 mov r8d, 2
 lea r9, [r12 + 272]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx16_3
.Lx16_2:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n6_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
.Lx16_3:
 jmp xchain5_n14_α
 xchain5_n10_β:
 jmp xchain5_n6_α
 xchain5_n11_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 352]
 cmp eax, 100
 je .Lx17_0
 mov eax, dword ptr [r12 + 368]
 cmp eax, 100
 je .Lx17_0
 mov eax, dword ptr [r12 + 352]
 cmp eax, 6
 jne .Lx17_2
 mov eax, dword ptr [r12 + 368]
 cmp eax, 6
 jne .Lx17_2
.Lx17_1:
 mov rax, qword ptr [r12 + 360]
 mov rcx, qword ptr [r12 + 376]
 add rax, rcx
 mov qword ptr [r12 + 336], 6
 mov qword ptr [r12 + 344], rax
 jmp xchain5_n15_α
.Lx17_0:
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 368]
 mov rcx, qword ptr [r12 + 376]
 mov r8d, 0
 lea r9, [r12 + 336]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx17_3
.Lx17_2:
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 368]
 mov rcx, qword ptr [r12 + 376]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n9_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
.Lx17_3:
 jmp xchain5_n15_α
 xchain5_n11_β:
 jmp xchain5_n9_α
# IR_LIT_INTEGER
 xchain5_n12_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain5_n16_α
 xchain5_n12_β:
 jmp xchain5_n13_α
.Lx18_0:
 .quad 5
# IR_VAR
 xchain5_n13_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain5_n17_α
 xchain5_n13_β:
 jmp main_γ
 xchain5_n14_α:
  .section .rodata
  .Lcall19_pname: .string "bump"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall19_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain5_n6_α
 jmp xchain5_n18_α
xchain5_n14_β:
 jmp xchain5_n6_α
# IR_ASSIGN gva
 xchain5_n15_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain5_n9_α
 xchain5_n15_β:
 jmp xchain5_n9_α
 xchain5_n16_α:
# BOX IR_CALL LT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+432] -> [zr+400]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [zr+448] -> [zr+416]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lrkfn23: .string "LT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn23]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain5_n13_α
 jmp xchain5_n4_α
 xchain5_n16_β:
 jmp xchain5_n13_α
# IR_ASSIGN global
 xchain5_n17_α:
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp main_γ
 xchain5_n17_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
 xchain5_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain5_n19_α
 xchain5_n18_β:
 jmp xchain5_n6_α
# IR_ASSIGN gva
 xchain5_n19_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain5_n6_α
 xchain5_n19_β:
 jmp xchain5_n6_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 496]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
