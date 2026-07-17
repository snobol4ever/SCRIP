  .intel_syntax noprefix
  .text
  .globl proc_double_α
proc_double_α:
#=======================================================================================================================
    .global proc_double_α
    .global proc_double_β
    .global proc_double_γ
    .global proc_double_ω
  sub rsp, 448
  mov [rsp + 424], rcx
  mov [rsp + 432], rdx
  mov [rsp + 440], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 416
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 408], rsp
proc_double_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_double_γ
.Lx1_0:
 .quad 2
# IR_VAR
 xchain0_n1_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 176], rax
 mov qword ptr [rbp + 184], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_double_γ
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 176]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [rbp + 176]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, 2
 mov rcx, qword ptr [rbp + 184]
 imul rax, rcx
 mov qword ptr [rbp + 112], 6
 mov qword ptr [rbp + 120], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 mov r8d, 2
 lea r9, [rbp + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_double_γ
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_double_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp proc_double_γ
 xchain0_n3_β:
 jmp proc_double_γ
proc_double_res:
add rsp, 8
pop rbp
proc_double_β:
jmp proc_double_ω
proc_double_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 424]
lea rsp, [rbp + 448]
mov rbp, [rbp + 440]
jmp rax
proc_double_ω:
mov rax, [rbp + 432]
lea rsp, [rbp + 448]
mov rbp, [rbp + 440]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "double"
  .Lstartup_pp0_0: .string "s"
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
  lea rsi, [rip + proc_double_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 416
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "double"
  .Lgvan1: .string "s"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
  call gva_register@PLT
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 408], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain5_n0_α:
 mov qword ptr [rbp + 272], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n3_α
.Lx6_0:
 .quad 5
 xchain5_n1_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 272]
 mov rdx, qword ptr [rbp + 280]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx8_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx8_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx8_3]
 lea rdx, [rip + .Lx8_4]
 jmp rax
.Lx8_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx8_2
.Lx8_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx8_2
.Lx8_1:
 call rt_faildescr@PLT
.Lx8_2:
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 cmp eax, 99
 je xchain5_n3_α
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n3_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "double"
# IR_ASSIGN global
 xchain5_n2_α:
 mov rsi, qword ptr [rbp + 224]
 mov rdx, qword ptr [rbp + 232]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n3_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain5_n3_α:
 mov qword ptr [rbp + 368], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp main_γ
.Lx10_0:
 .quad 21
 xchain5_n4_α:
 mov edi, 0
 mov rsi, qword ptr [rbp + 368]
 mov rdx, qword ptr [rbp + 376]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx12_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx12_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx12_3]
 lea rdx, [rip + .Lx12_4]
 jmp rax
.Lx12_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx12_2
.Lx12_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx12_2
.Lx12_1:
 call rt_faildescr@PLT
.Lx12_2:
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 cmp eax, 99
 je main_γ
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "double"
# IR_ASSIGN global
 xchain5_n5_α:
 mov rsi, qword ptr [rbp + 320]
 mov rdx, qword ptr [rbp + 328]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 jmp main_γ
 xchain5_n5_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 408]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 408]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
