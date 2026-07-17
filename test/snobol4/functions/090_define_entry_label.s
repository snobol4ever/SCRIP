  .intel_syntax noprefix
  .text
  .globl proc_bumpit_α
proc_bumpit_α:
#=======================================================================================================================
    .global proc_bumpit_α
    .global proc_bumpit_β
    .global proc_bumpit_γ
    .global proc_bumpit_ω
  sub rsp, 352
  mov [rsp + 328], rcx
  mov [rsp + 336], rdx
  mov [rsp + 344], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 320
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 312], rsp
proc_bumpit_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_bumpit_γ
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_bumpit_γ
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 144]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [rsp + 144]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [rsp + 152]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 112], 6
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [rsp + 144]
 mov rsi, qword ptr [rsp + 152]
 mov rdx, qword ptr [rsp + 176]
 mov rcx, qword ptr [rsp + 184]
 mov r8d, 0
 lea r9, [rsp + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [rsp + 144]
 mov rsi, qword ptr [rsp + 152]
 mov rdx, qword ptr [rsp + 176]
 mov rcx, qword ptr [rsp + 184]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_bumpit_γ
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bumpit_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp proc_bumpit_γ
 xchain0_n3_β:
 jmp proc_bumpit_γ
proc_bumpit_res:
add rsp, 8
pop rsp
proc_bumpit_β:
jmp proc_bumpit_ω
proc_bumpit_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 328]
mov rbp, [rbp + 344]
lea rsp, [rsp + 352]
jmp rax
proc_bumpit_ω:
mov rax, [rsp + 336]
mov rbp, [rbp + 344]
lea rsp, [rsp + 352]
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
  mov esi, 320
  call rt_proc_set_frame_bytes@PLT
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
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain5_n0_α:
 mov qword ptr [rsp + 272], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp main_γ
.Lx6_0:
 .quad 41
 xchain5_n1_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 272]
 mov rdx, qword ptr [rsp + 280]
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
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
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
 mov rsi, qword ptr [rsp + 224]
 mov rdx, qword ptr [rsp + 232]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp main_γ
 xchain5_n2_β:
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
mov rsp, qword ptr [rsp + 312]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 312]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
