  .intel_syntax noprefix
  .text
  .globl proc_fib_α
proc_fib_α:
#=======================================================================================================================
    .global proc_fib_α
    .global proc_fib_β
    .global proc_fib_γ
    .global proc_fib_ω
  sub rsp, 1072
  mov [rsp + 1048], rcx
  mov [rsp + 1056], rdx
  mov rdi, rsp
  mov ecx, 1040
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 1032], rsp
proc_fib_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n3_α
 xchain0_n1_β:
 jmp xchain0_n2_α
.Lx2_0:
 .quad 1
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp proc_fib_γ
# IR_COERCE_NUMERIC
 xchain0_n3_α:
 mov eax, dword ptr [rsp + 224]
 cmp eax, 7
 je .Lx5_1
 cmp eax, 6
 jne .Lx5_0
 mov eax, dword ptr [rsp + 192]
 cmp eax, 6
 jne .Lx5_0
.Lx5_1:
 mov rax, qword ptr [rsp + 224]
 mov qword ptr [rsp + 160], rax
 mov rax, qword ptr [rsp + 232]
 mov qword ptr [rsp + 168], rax
 jmp .Lx5_2
.Lx5_0:
 lea rdi, [rsp + 224]
 lea rsi, [rsp + 192]
 lea rdx, [rsp + 160]
 mov rcx, 118
 call rt_coerce_num2_d@PLT
.Lx5_2:
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 416], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 424], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp proc_fib_γ
.Lx6_0:
 .quad 1
# IR_COERCE_NUMERIC
 xchain0_n5_α:
 mov eax, dword ptr [rsp + 192]
 cmp eax, 7
 je .Lx8_1
 cmp eax, 6
 jne .Lx8_0
 mov eax, dword ptr [rsp + 224]
 cmp eax, 6
 jne .Lx8_0
.Lx8_1:
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
 jmp .Lx8_2
.Lx8_0:
 lea rdi, [rsp + 192]
 lea rsi, [rsp + 224]
 lea rdx, [rsp + 128]
 mov rcx, 119
 call rt_coerce_num2_d@PLT
.Lx8_2:
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n2_α
 xchain0_n6_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 384]
 cmp eax, 100
 je .Lx9_0
 mov eax, dword ptr [rsp + 384]
 cmp eax, 6
 jne .Lx9_2
.Lx9_1:
 mov rax, qword ptr [rsp + 392]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [rsp + 352], 6
 mov qword ptr [rsp + 360], rax
 jmp xchain0_n8_α
.Lx9_0:
 mov rdi, qword ptr [rsp + 384]
 mov rsi, qword ptr [rsp + 392]
 mov rdx, qword ptr [rsp + 416]
 mov rcx, qword ptr [rsp + 424]
 mov r8d, 1
 lea r9, [rsp + 352]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx9_3
.Lx9_2:
 mov rdi, qword ptr [rsp + 384]
 mov rsi, qword ptr [rsp + 392]
 mov rdx, qword ptr [rsp + 416]
 mov rcx, qword ptr [rsp + 424]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
.Lx9_3:
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp proc_fib_γ
# IR_CMP_TEST
 xchain0_n7_α:
 lea rdi, [rsp + 160]
 lea rsi, [rsp + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jg xchain0_n2_α
 mov qword ptr [rsp + 96], 0
 mov qword ptr [rsp + 104], 0
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n2_α
 xchain0_n8_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 352]
 mov rdx, qword ptr [rsp + 360]
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
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
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
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp proc_fib_γ
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp proc_fib_γ
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [rsp + 608]
 mov rdx, qword ptr [rsp + 616]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp proc_fib_γ
 xchain0_n11_β:
 jmp proc_fib_γ
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [rsp + 560], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp proc_fib_γ
.Lx17_0:
 .quad 2
 xchain0_n13_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 528]
 cmp eax, 100
 je .Lx18_0
 mov eax, dword ptr [rsp + 528]
 cmp eax, 6
 jne .Lx18_2
.Lx18_1:
 mov rax, qword ptr [rsp + 536]
 mov rcx, 2
 sub rax, rcx
 mov qword ptr [rsp + 496], 6
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n14_α
.Lx18_0:
 mov rdi, qword ptr [rsp + 528]
 mov rsi, qword ptr [rsp + 536]
 mov rdx, qword ptr [rsp + 560]
 mov rcx, qword ptr [rsp + 568]
 mov r8d, 1
 lea r9, [rsp + 496]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx18_3
.Lx18_2:
 mov rdi, qword ptr [rsp + 528]
 mov rsi, qword ptr [rsp + 536]
 mov rdx, qword ptr [rsp + 560]
 mov rcx, qword ptr [rsp + 568]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
.Lx18_3:
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp proc_fib_γ
 xchain0_n14_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 496]
 mov rdx, qword ptr [rsp + 504]
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
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
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
 mov eax, dword ptr [rsp + 304]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [rsp + 448]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [rsp + 304]
 cmp eax, 6
 jne .Lx21_2
 mov eax, dword ptr [rsp + 448]
 cmp eax, 6
 jne .Lx21_2
.Lx21_1:
 mov rax, qword ptr [rsp + 312]
 mov rcx, qword ptr [rsp + 456]
 add rax, rcx
 mov qword ptr [rsp + 272], 6
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n16_α
.Lx21_0:
 mov rdi, qword ptr [rsp + 304]
 mov rsi, qword ptr [rsp + 312]
 mov rdx, qword ptr [rsp + 448]
 mov rcx, qword ptr [rsp + 456]
 mov r8d, 0
 lea r9, [rsp + 272]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx21_3
.Lx21_2:
 mov rdi, qword ptr [rsp + 304]
 mov rsi, qword ptr [rsp + 312]
 mov rdx, qword ptr [rsp + 448]
 mov rcx, qword ptr [rsp + 456]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_fib_γ
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
.Lx21_3:
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp proc_fib_γ
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rsp + 272]
 mov rdx, qword ptr [rsp + 280]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp proc_fib_γ
 xchain0_n16_β:
 jmp proc_fib_γ
proc_fib_res:
add rsp, 8
pop rsp
proc_fib_β:
jmp proc_fib_ω
proc_fib_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 1048]
lea rsp, [rsp + 1072]
jmp rax
proc_fib_ω:
mov rax, [rsp + 1056]
lea rsp, [rsp + 1072]
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
  mov esi, 1040
  call rt_proc_set_frame_bytes@PLT
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
main_α_body:
# IR_LIT_INTEGER
 xchain23_n0_α:
 mov qword ptr [rsp + 704], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rsp + 712], rax
 jmp xchain23_n1_α
 xchain23_n0_β:
 jmp xchain23_n3_α
.Lx24_0:
 .quad 0
 xchain23_n1_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 704]
 mov rdx, qword ptr [rsp + 712]
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
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
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
 mov rsi, qword ptr [rsp + 656]
 mov rdx, qword ptr [rsp + 664]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 jmp xchain23_n3_α
 xchain23_n2_β:
 jmp xchain23_n3_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain23_n3_α:
 mov qword ptr [rsp + 800], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rsp + 808], rax
 jmp xchain23_n4_α
 xchain23_n3_β:
 jmp xchain23_n6_α
.Lx28_0:
 .quad 1
 xchain23_n4_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 800]
 mov rdx, qword ptr [rsp + 808]
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
 mov qword ptr [rsp + 752], rax
 mov qword ptr [rsp + 760], rdx
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
 mov rsi, qword ptr [rsp + 752]
 mov rdx, qword ptr [rsp + 760]
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 736], rax
 mov qword ptr [rsp + 744], rdx
 jmp xchain23_n6_α
 xchain23_n5_β:
 jmp xchain23_n6_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain23_n6_α:
 mov qword ptr [rsp + 896], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rsp + 904], rax
 jmp xchain23_n7_α
 xchain23_n6_β:
 jmp xchain23_n9_α
.Lx32_0:
 .quad 6
 xchain23_n7_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 896]
 mov rdx, qword ptr [rsp + 904]
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
 mov qword ptr [rsp + 848], rax
 mov qword ptr [rsp + 856], rdx
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
 mov rsi, qword ptr [rsp + 848]
 mov rdx, qword ptr [rsp + 856]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
 jmp xchain23_n9_α
 xchain23_n8_β:
 jmp xchain23_n9_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain23_n9_α:
 mov qword ptr [rsp + 992], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rsp + 1000], rax
 jmp xchain23_n10_α
 xchain23_n9_β:
 jmp main_γ
.Lx36_0:
 .quad 10
 xchain23_n10_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 992]
 mov rdx, qword ptr [rsp + 1000]
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
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
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
 mov rsi, qword ptr [rsp + 944]
 mov rdx, qword ptr [rsp + 952]
 mov rdi, qword ptr [rip + .Lx39_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 928], rax
 mov qword ptr [rsp + 936], rdx
 jmp main_γ
 xchain23_n11_β:
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
mov rsp, qword ptr [rsp + 1032]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1032]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
