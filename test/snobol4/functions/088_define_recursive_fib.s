  .intel_syntax noprefix
  .text
  .globl proc_fib_α
proc_fib_α:
#=======================================================================================================================
    .global proc_fib_α
    .global proc_fib_β
    .global proc_fib_γ
    .global proc_fib_ω
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
 mov qword ptr [r12 + 624], rax
 pop rsi
proc_fib_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n3_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad 1
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp proc_fib_γ
 xchain0_n3_α:
# BOX IR_CALL LE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+128] -> [zr+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lrkfn5: .string "LE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp proc_fib_γ
.Lx6_0:
 .quad 1
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp proc_fib_γ
 xchain0_n6_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 224]
 cmp eax, 100
 je .Lx8_0
 mov eax, dword ptr [r12 + 240]
 cmp eax, 100
 je .Lx8_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx8_2
 mov eax, dword ptr [r12 + 240]
 cmp eax, 6
 jne .Lx8_2
.Lx8_1:
 mov rax, qword ptr [r12 + 232]
 mov rcx, qword ptr [r12 + 248]
 sub rax, rcx
 mov qword ptr [r12 + 208], 6
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n8_α
.Lx8_0:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 1
 lea r9, [r12 + 208]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx8_3
.Lx8_2:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
.Lx8_3:
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp proc_fib_γ
# IR_ASSIGN gva
 xchain0_n7_α:
 mov rax, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp proc_fib_γ
 xchain0_n7_β:
 jmp proc_fib_γ
 xchain0_n8_α:
  .section .rodata
  .Lcall9_pname: .string "fib"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall9_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je proc_fib_γ
 jmp xchain0_n9_α
xchain0_n8_β:
 jmp proc_fib_γ
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp proc_fib_γ
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp proc_fib_γ
.Lx12_0:
 .quad 2
 xchain0_n11_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 304]
 cmp eax, 100
 je .Lx13_0
 mov eax, dword ptr [r12 + 320]
 cmp eax, 100
 je .Lx13_0
 mov eax, dword ptr [r12 + 304]
 cmp eax, 6
 jne .Lx13_2
 mov eax, dword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx13_2
.Lx13_1:
 mov rax, qword ptr [r12 + 312]
 mov rcx, qword ptr [r12 + 328]
 sub rax, rcx
 mov qword ptr [r12 + 288], 6
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n12_α
.Lx13_0:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8d, 1
 lea r9, [r12 + 288]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx13_3
.Lx13_2:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
.Lx13_3:
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp proc_fib_γ
 xchain0_n12_α:
  .section .rodata
  .Lcall13_pname: .string "fib"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall13_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je proc_fib_γ
 jmp xchain0_n13_α
xchain0_n12_β:
 jmp proc_fib_γ
 xchain0_n13_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 176]
 cmp eax, 100
 je .Lx15_0
 mov eax, dword ptr [r12 + 256]
 cmp eax, 100
 je .Lx15_0
 mov eax, dword ptr [r12 + 176]
 cmp eax, 6
 jne .Lx15_2
 mov eax, dword ptr [r12 + 256]
 cmp eax, 6
 jne .Lx15_2
.Lx15_1:
 mov rax, qword ptr [r12 + 184]
 mov rcx, qword ptr [r12 + 264]
 add rax, rcx
 mov qword ptr [r12 + 160], 6
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n14_α
.Lx15_0:
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 mov r8d, 0
 lea r9, [r12 + 160]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx15_3
.Lx15_2:
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
.Lx15_3:
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp proc_fib_γ
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp proc_fib_γ
 xchain0_n14_β:
 jmp proc_fib_γ
proc_fib_β:
jmp proc_fib_ω
proc_fib_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 624]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_fib_ω:
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
  .Lstartup_pname0: .string "fib"
  .Lstartup_pp0_0: .string "n"
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
  lea rsi, [rip + proc_fib_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "fib"
  .Lgvan1: .string "n"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
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
  mov edx, 2
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
 mov qword ptr [r12 + 624], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain17_n0_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain17_n1_α
 xchain17_n0_β:
 jmp xchain17_n3_α
.Lx18_0:
 .quad 0
 xchain17_n1_α:
  .section .rodata
  .Lcall17_pname: .string "fib"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall17_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain17_n3_α
 jmp xchain17_n2_α
xchain17_n1_β:
 jmp xchain17_n3_α
# IR_ASSIGN global
 xchain17_n2_α:
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain17_n3_α
 xchain17_n2_β:
 jmp xchain17_n3_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain17_n3_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain17_n4_α
 xchain17_n3_β:
 jmp xchain17_n6_α
.Lx21_0:
 .quad 1
 xchain17_n4_α:
  .section .rodata
  .Lcall20_pname: .string "fib"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall20_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain17_n6_α
 jmp xchain17_n5_α
xchain17_n4_β:
 jmp xchain17_n6_α
# IR_ASSIGN global
 xchain17_n5_α:
 mov rsi, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain17_n6_α
 xchain17_n5_β:
 jmp xchain17_n6_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain17_n6_α:
 mov qword ptr [r12 + 544], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain17_n7_α
 xchain17_n6_β:
 jmp xchain17_n9_α
.Lx24_0:
 .quad 6
 xchain17_n7_α:
  .section .rodata
  .Lcall23_pname: .string "fib"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall23_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain17_n9_α
 jmp xchain17_n8_α
xchain17_n7_β:
 jmp xchain17_n9_α
# IR_ASSIGN global
 xchain17_n8_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain17_n9_α
 xchain17_n8_β:
 jmp xchain17_n9_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain17_n9_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain17_n10_α
 xchain17_n9_β:
 jmp main_γ
.Lx27_0:
 .quad 10
 xchain17_n10_α:
  .section .rodata
  .Lcall26_pname: .string "fib"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall26_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je main_γ
 jmp xchain17_n11_α
xchain17_n10_β:
 jmp main_γ
# IR_ASSIGN global
 xchain17_n11_α:
 mov rsi, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp main_γ
 xchain17_n11_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
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
 mov rdi, qword ptr [r12 + 624]
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
