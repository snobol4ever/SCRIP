  .intel_syntax noprefix
  .text
  .globl proc_bumpit_α
proc_bumpit_α:
#=======================================================================================================================
    .global proc_bumpit_α
    .global proc_bumpit_β
    .global proc_bumpit_γ
    .global proc_bumpit_ω
  sub rsp, 256
  mov [rsp + 232], rcx
  mov [rsp + 240], rdx
  mov [rsp + 248], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 208], 0
  mov qword ptr [rsp + 216], rsp
proc_bumpit_α_body:
# bumpit  bumpit = v + 1                                             :(RETURN)
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n1_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 184], rax
 jmp xchain0_n2_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 144]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [rbp + 144]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [rbp + 152]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 112], 6
 mov qword ptr [rbp + 120], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 mov r8d, 0
 lea r9, [rbp + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n3_α
.Lx3_2:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_bumpit_γ
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bumpit_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp proc_bumpit_γ
proc_bumpit_res:
add rsp, 8
pop rbp
proc_bumpit_β:
jmp proc_bumpit_ω
proc_bumpit_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 232]
lea rsp, [rbp + 256]
mov rbp, [rbp + 248]
jmp rax
proc_bumpit_ω:
mov rax, [rbp + 240]
lea rsp, [rbp + 256]
mov rbp, [rbp + 248]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "bumpit"
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
  lea rsi, [rip + proc_bumpit_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 224
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "bumpit"
  .Lgvan1: .string "v"
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
  mov qword ptr [rsp + 312], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         DEFINE('bumpit(v)', .bumpit)                                :(bumpend)
#         OUTPUT = bumpit(41)
# IR_LIT_INTEGER
 xchain5_n0_α:
 mov qword ptr [rbp + 272], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain5_n1_α
.Lx6_0:
 .quad 41
 xchain5_n1_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx8_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx8_5
 mov rax, qword ptr [rbp + 272]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 280]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx8_6]
 lea rdx, [rip + .Lx8_7]
 jmp rax
.Lx8_6:
 mov rdi, qword ptr [1879052288]
 mov rsi, qword ptr [1879052296]
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 32
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx8_2
.Lx8_7:
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 32
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx8_2
.Lx8_5:
 add rsp, 32
 lea r11, [rip + g_gc_pending]
 mov eax, dword ptr [r11 + 0]
 test eax, eax
 jne .Lx8_20
 mov rax, qword ptr [rbp + 272]
 mov rdx, qword ptr [rbp + 280]
 lea r10, [rip + g_call_args]
 mov qword ptr [r10 + 0], rax
 mov qword ptr [r10 + 8], rdx
 jmp .Lx8_21
.Lx8_20:
 mov edi, 0
 mov rsi, qword ptr [rbp + 272]
 mov rdx, qword ptr [rbp + 280]
 call rt_arg_stage@PLT
.Lx8_21:
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
 je main_γ
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "bumpit"
# IR_ASSIGN global
 xchain5_n2_α:
 mov rsi, qword ptr [rbp + 224]
 mov rdx, qword ptr [rbp + 232]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 312]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 312]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
