  .intel_syntax noprefix
  .text
  .globl proc_fib_α
proc_fib_α:
#=======================================================================================================================
    .global proc_fib_α
    .global proc_fib_β
    .global proc_fib_γ
    .global proc_fib_ω
  sub rsp, 688
  mov [rsp + 664], rcx
  mov [rsp + 672], rdx
  mov [rsp + 680], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 640], 0
  mov qword ptr [rsp + 648], rsp
proc_fib_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain0_n1_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 jmp xchain0_n4_α
# IR_COERCE_NUMERIC
 xchain0_n3_α:
 mov eax, dword ptr [rbp + 224]
 cmp eax, 7
 je .Lx5_1
 cmp eax, 6
 jne .Lx5_0
 mov eax, dword ptr [rbp + 192]
 cmp eax, 6
 jne .Lx5_0
.Lx5_1:
 mov rax, qword ptr [rbp + 224]
 mov qword ptr [rbp + 160], rax
 mov rax, qword ptr [rbp + 232]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n5_α
.Lx5_0:
 lea rdi, [rbp + 224]
 lea rsi, [rbp + 192]
 lea rdx, [rbp + 160]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rbp + 416], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 424], rax
 jmp xchain0_n6_α
.Lx6_0:
 .quad 1
# IR_COERCE_NUMERIC
 xchain0_n5_α:
 mov eax, dword ptr [rbp + 192]
 cmp eax, 7
 je .Lx8_1
 cmp eax, 6
 jne .Lx8_0
 mov eax, dword ptr [rbp + 224]
 cmp eax, 6
 jne .Lx8_0
.Lx8_1:
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 136], rax
 jmp xchain0_n7_α
.Lx8_0:
 lea rdi, [rbp + 192]
 lea rsi, [rbp + 224]
 lea rdx, [rbp + 128]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
 jmp xchain0_n7_α
 xchain0_n6_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 384]
 cmp eax, 100
 je .Lx9_0
 mov eax, dword ptr [rbp + 384]
 cmp eax, 6
 jne .Lx9_2
.Lx9_1:
 mov rax, qword ptr [rbp + 392]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [rbp + 352], 6
 mov qword ptr [rbp + 360], rax
 jmp xchain0_n8_α
.Lx9_0:
 mov rdi, qword ptr [rbp + 384]
 mov rsi, qword ptr [rbp + 392]
 mov rdx, qword ptr [rbp + 416]
 mov rcx, qword ptr [rbp + 424]
 mov r8d, 1
 lea r9, [rbp + 352]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n8_α
.Lx9_2:
 mov rdi, qword ptr [rbp + 384]
 mov rsi, qword ptr [rbp + 392]
 mov rdx, qword ptr [rbp + 416]
 mov rcx, qword ptr [rbp + 424]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp proc_fib_γ
# IR_CMP_TEST
 xchain0_n7_α:
 lea rdi, [rbp + 160]
 lea rsi, [rbp + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jg xchain0_n2_α
 mov qword ptr [rbp + 96], 0
 mov qword ptr [rbp + 104], 0
 jmp xchain0_n9_α
 xchain0_n8_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx13_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx13_5
 mov rax, qword ptr [rbp + 352]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 360]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx13_6]
 lea rdx, [rip + .Lx13_7]
 jmp rax
.Lx13_6:
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
 jmp .Lx13_2
.Lx13_7:
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
 jmp .Lx13_2
.Lx13_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 352]
 mov rdx, qword ptr [rbp + 360]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx13_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx13_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx13_3]
 lea rdx, [rip + .Lx13_4]
 jmp rax
.Lx13_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx13_2
.Lx13_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx13_2
.Lx13_1:
 call rt_faildescr@PLT
.Lx13_2:
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 cmp eax, 99
 je proc_fib_γ
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp proc_fib_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "fib"
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 608], rax
 mov qword ptr [rbp + 616], rdx
 jmp xchain0_n11_α
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain0_n12_α
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [rbp + 608]
 mov rdx, qword ptr [rbp + 616]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp proc_fib_γ
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [rbp + 560], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain0_n13_α
.Lx17_0:
 .quad 2
 xchain0_n13_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 528]
 cmp eax, 100
 je .Lx18_0
 mov eax, dword ptr [rbp + 528]
 cmp eax, 6
 jne .Lx18_2
.Lx18_1:
 mov rax, qword ptr [rbp + 536]
 mov rcx, 2
 sub rax, rcx
 mov qword ptr [rbp + 496], 6
 mov qword ptr [rbp + 504], rax
 jmp xchain0_n14_α
.Lx18_0:
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 560]
 mov rcx, qword ptr [rbp + 568]
 mov r8d, 1
 lea r9, [rbp + 496]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n14_α
.Lx18_2:
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 560]
 mov rcx, qword ptr [rbp + 568]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp proc_fib_γ
 xchain0_n14_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx20_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx20_5
 mov rax, qword ptr [rbp + 496]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 504]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx20_6]
 lea rdx, [rip + .Lx20_7]
 jmp rax
.Lx20_6:
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
 jmp .Lx20_2
.Lx20_7:
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
 jmp .Lx20_2
.Lx20_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 496]
 mov rdx, qword ptr [rbp + 504]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx20_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx20_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx20_3]
 lea rdx, [rip + .Lx20_4]
 jmp rax
.Lx20_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx20_2
.Lx20_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx20_2
.Lx20_1:
 call rt_faildescr@PLT
.Lx20_2:
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 cmp eax, 99
 je proc_fib_γ
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp proc_fib_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "fib"
 xchain0_n15_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 304]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [rbp + 448]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [rbp + 304]
 cmp eax, 6
 jne .Lx21_2
 mov eax, dword ptr [rbp + 448]
 cmp eax, 6
 jne .Lx21_2
.Lx21_1:
 mov rax, qword ptr [rbp + 312]
 mov rcx, qword ptr [rbp + 456]
 add rax, rcx
 mov qword ptr [rbp + 272], 6
 mov qword ptr [rbp + 280], rax
 jmp xchain0_n16_α
.Lx21_0:
 mov rdi, qword ptr [rbp + 304]
 mov rsi, qword ptr [rbp + 312]
 mov rdx, qword ptr [rbp + 448]
 mov rcx, qword ptr [rbp + 456]
 mov r8d, 0
 lea r9, [rbp + 272]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n16_α
.Lx21_2:
 mov rdi, qword ptr [rbp + 304]
 mov rsi, qword ptr [rbp + 312]
 mov rdx, qword ptr [rbp + 448]
 mov rcx, qword ptr [rbp + 456]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp proc_fib_γ
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rbp + 272]
 mov rdx, qword ptr [rbp + 280]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 256], rax
 mov qword ptr [rbp + 264], rdx
 jmp proc_fib_γ
proc_fib_res:
add rsp, 8
pop rbp
proc_fib_β:
jmp proc_fib_ω
proc_fib_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 664]
lea rsp, [rbp + 688]
mov rbp, [rbp + 680]
jmp rax
proc_fib_ω:
mov rax, [rbp + 672]
lea rsp, [rbp + 688]
mov rbp, [rbp + 680]
jmp rax
proc_startup:
  sub rsp, 8
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
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 656
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "fib"
  .Lgvan1: .string "n"
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
  mov qword ptr [rsp + 1032], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain23_n0_α:
 mov qword ptr [rbp + 704], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rbp + 712], rax
 jmp xchain23_n1_α
.Lx24_0:
 .quad 0
 xchain23_n1_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx26_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx26_5
 mov rax, qword ptr [rbp + 704]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 712]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx26_6]
 lea rdx, [rip + .Lx26_7]
 jmp rax
.Lx26_6:
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
 jmp .Lx26_2
.Lx26_7:
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
 jmp .Lx26_2
.Lx26_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 704]
 mov rdx, qword ptr [rbp + 712]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx26_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx26_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx26_3]
 lea rdx, [rip + .Lx26_4]
 jmp rax
.Lx26_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx26_2
.Lx26_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx26_2
.Lx26_1:
 call rt_faildescr@PLT
.Lx26_2:
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 cmp eax, 99
 je xchain23_n3_α
 jmp xchain23_n2_α
 xchain23_n1_β:
 jmp xchain23_n3_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "fib"
# IR_ASSIGN global
 xchain23_n2_α:
 mov rsi, qword ptr [rbp + 656]
 mov rdx, qword ptr [rbp + 664]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 640], rax
 mov qword ptr [rbp + 648], rdx
 jmp xchain23_n3_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain23_n3_α:
 mov qword ptr [rbp + 800], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 808], rax
 jmp xchain23_n4_α
.Lx28_0:
 .quad 1
 xchain23_n4_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx30_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx30_5
 mov rax, qword ptr [rbp + 800]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 808]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx30_6]
 lea rdx, [rip + .Lx30_7]
 jmp rax
.Lx30_6:
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
 jmp .Lx30_2
.Lx30_7:
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
 jmp .Lx30_2
.Lx30_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 800]
 mov rdx, qword ptr [rbp + 808]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx30_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx30_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx30_3]
 lea rdx, [rip + .Lx30_4]
 jmp rax
.Lx30_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx30_2
.Lx30_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx30_2
.Lx30_1:
 call rt_faildescr@PLT
.Lx30_2:
 mov qword ptr [rbp + 752], rax
 mov qword ptr [rbp + 760], rdx
 cmp eax, 99
 je xchain23_n6_α
 jmp xchain23_n5_α
 xchain23_n4_β:
 jmp xchain23_n6_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "fib"
# IR_ASSIGN global
 xchain23_n5_α:
 mov rsi, qword ptr [rbp + 752]
 mov rdx, qword ptr [rbp + 760]
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 736], rax
 mov qword ptr [rbp + 744], rdx
 jmp xchain23_n6_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain23_n6_α:
 mov qword ptr [rbp + 896], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain23_n7_α
.Lx32_0:
 .quad 6
 xchain23_n7_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx34_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx34_5
 mov rax, qword ptr [rbp + 896]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 904]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx34_6]
 lea rdx, [rip + .Lx34_7]
 jmp rax
.Lx34_6:
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
 jmp .Lx34_2
.Lx34_7:
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
 jmp .Lx34_2
.Lx34_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 896]
 mov rdx, qword ptr [rbp + 904]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx34_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx34_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx34_3]
 lea rdx, [rip + .Lx34_4]
 jmp rax
.Lx34_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx34_2
.Lx34_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx34_2
.Lx34_1:
 call rt_faildescr@PLT
.Lx34_2:
 mov qword ptr [rbp + 848], rax
 mov qword ptr [rbp + 856], rdx
 cmp eax, 99
 je xchain23_n9_α
 jmp xchain23_n8_α
 xchain23_n7_β:
 jmp xchain23_n9_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "fib"
# IR_ASSIGN global
 xchain23_n8_α:
 mov rsi, qword ptr [rbp + 848]
 mov rdx, qword ptr [rbp + 856]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 832], rax
 mov qword ptr [rbp + 840], rdx
 jmp xchain23_n9_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain23_n9_α:
 mov qword ptr [rbp + 992], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rbp + 1000], rax
 jmp xchain23_n10_α
.Lx36_0:
 .quad 10
 xchain23_n10_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx38_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx38_5
 mov rax, qword ptr [rbp + 992]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 1000]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx38_6]
 lea rdx, [rip + .Lx38_7]
 jmp rax
.Lx38_6:
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
 jmp .Lx38_2
.Lx38_7:
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
 jmp .Lx38_2
.Lx38_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 992]
 mov rdx, qword ptr [rbp + 1000]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx38_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx38_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx38_3]
 lea rdx, [rip + .Lx38_4]
 jmp rax
.Lx38_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx38_2
.Lx38_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx38_2
.Lx38_1:
 call rt_faildescr@PLT
.Lx38_2:
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 cmp eax, 99
 je main_γ
 jmp xchain23_n11_α
 xchain23_n10_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "fib"
# IR_ASSIGN global
 xchain23_n11_α:
 mov rsi, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov rdi, qword ptr [rip + .Lx39_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp main_γ
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1032]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1032]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
