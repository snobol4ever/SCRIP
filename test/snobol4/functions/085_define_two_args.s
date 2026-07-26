  .intel_syntax noprefix
  .text
  .globl proc_add_α
proc_add_α:
#=======================================================================================================================
    .global proc_add_α
    .global proc_add_β
    .global proc_add_γ
    .global proc_add_ω
  sub rsp, 256
  mov [rsp + 232], rcx
  mov [rsp + 240], rdx
  mov [rsp + 248], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 208], 0
  mov qword ptr [rsp + 216], rsp
proc_add_α_body:
# add     add = a + b                                                :(RETURN)
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n1_α
# IR_VAR
 xchain0_n1_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 176], rax
 mov qword ptr [rbp + 184], rdx
 jmp xchain0_n2_α
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 144]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [rbp + 176]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [rbp + 144]
 cmp eax, 6
 jne .Lx3_2
 mov eax, dword ptr [rbp + 176]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [rbp + 152]
 mov rcx, qword ptr [rbp + 184]
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
 je proc_add_γ
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_add_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp proc_add_γ
proc_add_res:
add rsp, 8
pop rbp
proc_add_β:
jmp proc_add_ω
proc_add_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 232]
lea rsp, [rbp + 256]
mov rbp, [rbp + 248]
jmp rax
proc_add_ω:
mov rax, [rbp + 240]
lea rsp, [rbp + 256]
mov rbp, [rbp + 248]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "add"
  .Lstartup_pp0_0: .string "a"
  .Lstartup_pp0_1: .string "b"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad .Lstartup_pp0_1
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 2
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_add_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 2
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
  .Lgvan0: .string "add"
  .Lgvan1: .string "a"
  .Lgvan2: .string "b"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
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
  mov qword ptr [rsp + 504], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         DEFINE('add(a,b)')                                          :(add_end)
#         OUTPUT = add(3, 4)
# IR_LIT_INTEGER
 xchain5_n0_α:
 mov qword ptr [rbp + 288], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 296], rax
 jmp xchain5_n1_α
.Lx6_0:
 .quad 3
# IR_LIT_INTEGER
 xchain5_n1_α:
 mov qword ptr [rbp + 320], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 328], rax
 jmp xchain5_n2_α
.Lx7_0:
 .quad 4
 xchain5_n2_α:
 sub rsp, 48
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx9_0]
 mov esi, 2
 mov edx, 2
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx9_5
 mov rax, qword ptr [rbp + 288]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 296]
 mov qword ptr [1879052312], rax
 mov rax, qword ptr [rbp + 320]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rbp + 328]
 mov qword ptr [1879052328], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx9_6]
 lea rdx, [rip + .Lx9_7]
 jmp rax
.Lx9_6:
 mov rdi, qword ptr [1879052288]
 mov rsi, qword ptr [1879052296]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052328], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx9_2
.Lx9_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052328], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx9_2
.Lx9_5:
 add rsp, 48
 lea r11, [rip + g_gc_pending]
 mov eax, dword ptr [r11 + 0]
 test eax, eax
 jne .Lx9_20
 mov rax, qword ptr [rbp + 288]
 mov rdx, qword ptr [rbp + 296]
 lea r10, [rip + g_call_args]
 mov qword ptr [r10 + 0], rax
 mov qword ptr [r10 + 8], rdx
 jmp .Lx9_21
.Lx9_20:
 mov edi, 0
 mov rsi, qword ptr [rbp + 288]
 mov rdx, qword ptr [rbp + 296]
 call rt_arg_stage@PLT
.Lx9_21:
 lea r11, [rip + g_gc_pending]
 mov eax, dword ptr [r11 + 0]
 test eax, eax
 jne .Lx9_22
 mov rax, qword ptr [rbp + 320]
 mov rdx, qword ptr [rbp + 328]
 lea r10, [rip + g_call_args]
 mov qword ptr [r10 + 16], rax
 mov qword ptr [r10 + 24], rdx
 jmp .Lx9_23
.Lx9_22:
 mov edi, 1
 mov rsi, qword ptr [rbp + 320]
 mov rdx, qword ptr [rbp + 328]
 call rt_arg_stage@PLT
.Lx9_23:
 mov rdi, qword ptr [rip + .Lx9_0]
 mov esi, 2
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx9_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx9_3]
 lea rdx, [rip + .Lx9_4]
 jmp rax
.Lx9_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx9_2
.Lx9_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx9_2
.Lx9_1:
 call rt_faildescr@PLT
.Lx9_2:
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 cmp eax, 99
 je xchain5_n4_α
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n4_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "add"
# IR_ASSIGN global
 xchain5_n3_α:
 mov rsi, qword ptr [rbp + 224]
 mov rdx, qword ptr [rbp + 232]
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain5_n4_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "OUTPUT"
#         OUTPUT = add(10, 32)
# IR_LIT_INTEGER
 xchain5_n4_α:
 mov qword ptr [rbp + 432], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 440], rax
 jmp xchain5_n5_α
.Lx11_0:
 .quad 10
# IR_LIT_INTEGER
 xchain5_n5_α:
 mov qword ptr [rbp + 464], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 472], rax
 jmp xchain5_n6_α
.Lx12_0:
 .quad 32
 xchain5_n6_α:
 sub rsp, 48
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx14_0]
 mov esi, 2
 mov edx, 2
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx14_5
 mov rax, qword ptr [rbp + 432]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 440]
 mov qword ptr [1879052312], rax
 mov rax, qword ptr [rbp + 464]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rbp + 472]
 mov qword ptr [1879052328], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx14_6]
 lea rdx, [rip + .Lx14_7]
 jmp rax
.Lx14_6:
 mov rdi, qword ptr [1879052288]
 mov rsi, qword ptr [1879052296]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052328], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx14_2
.Lx14_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052328], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx14_2
.Lx14_5:
 add rsp, 48
 lea r11, [rip + g_gc_pending]
 mov eax, dword ptr [r11 + 0]
 test eax, eax
 jne .Lx14_20
 mov rax, qword ptr [rbp + 432]
 mov rdx, qword ptr [rbp + 440]
 lea r10, [rip + g_call_args]
 mov qword ptr [r10 + 0], rax
 mov qword ptr [r10 + 8], rdx
 jmp .Lx14_21
.Lx14_20:
 mov edi, 0
 mov rsi, qword ptr [rbp + 432]
 mov rdx, qword ptr [rbp + 440]
 call rt_arg_stage@PLT
.Lx14_21:
 lea r11, [rip + g_gc_pending]
 mov eax, dword ptr [r11 + 0]
 test eax, eax
 jne .Lx14_22
 mov rax, qword ptr [rbp + 464]
 mov rdx, qword ptr [rbp + 472]
 lea r10, [rip + g_call_args]
 mov qword ptr [r10 + 16], rax
 mov qword ptr [r10 + 24], rdx
 jmp .Lx14_23
.Lx14_22:
 mov edi, 1
 mov rsi, qword ptr [rbp + 464]
 mov rdx, qword ptr [rbp + 472]
 call rt_arg_stage@PLT
.Lx14_23:
 mov rdi, qword ptr [rip + .Lx14_0]
 mov esi, 2
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx14_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx14_3]
 lea rdx, [rip + .Lx14_4]
 jmp rax
.Lx14_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx14_2
.Lx14_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx14_2
.Lx14_1:
 call rt_faildescr@PLT
.Lx14_2:
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 cmp eax, 99
 je main_γ
 jmp xchain5_n7_α
 xchain5_n6_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "add"
# IR_ASSIGN global
 xchain5_n7_α:
 mov rsi, qword ptr [rbp + 368]
 mov rdx, qword ptr [rbp + 376]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 504]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 504]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
