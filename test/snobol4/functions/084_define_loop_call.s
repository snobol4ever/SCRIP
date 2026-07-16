  .intel_syntax noprefix
  .text
  .globl proc_bump_α
proc_bump_α:
#=======================================================================================================================
    .global proc_bump_α
    .global proc_bump_β
    .global proc_bump_γ
    .global proc_bump_ω
  sub rsp, 896
  mov [rsp + 872], rcx
  mov [rsp + 880], rdx
  mov rdi, rsp
  mov ecx, 864
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 856], rsp
proc_bump_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_bump_γ
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_bump_γ
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
 je proc_bump_γ
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bump_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp proc_bump_γ
 xchain0_n3_β:
 jmp proc_bump_γ
proc_bump_res:
add rsp, 8
pop rsp
proc_bump_β:
jmp proc_bump_ω
proc_bump_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 872]
lea rsp, [rsp + 896]
jmp rax
proc_bump_ω:
mov rax, [rsp + 880]
lea rsp, [rsp + 896]
jmp rax
proc_startup:
  sub rsp, 8
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
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 864
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
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
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 4
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 4
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
  mov qword ptr [rsp + 856], rsp
main_α_body:
# IR_LIT_STRING
 xchain5_n0_α:
 mov qword ptr [rsp + 224], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n2_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string ""
# IR_ASSIGN gva
 xchain5_n1_α:
 mov rax, qword ptr [rsp + 224]
 mov rdx, qword ptr [rsp + 232]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n2_α
# IR_LIT_INTEGER
 xchain5_n2_α:
 mov qword ptr [rsp + 272], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n4_α
.Lx8_0:
 .quad 0
# IR_ASSIGN gva
 xchain5_n3_α:
 mov rax, qword ptr [rsp + 272]
 mov rdx, qword ptr [rsp + 280]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp xchain5_n4_α
# IR_VAR
 xchain5_n4_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp xchain5_n6_α
# IR_LIT_INTEGER
 xchain5_n5_α:
 mov qword ptr [rsp + 464], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain5_n7_α
 xchain5_n5_β:
 jmp xchain5_n6_α
.Lx11_0:
 .quad 2
# IR_VAR
 xchain5_n6_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp xchain5_n8_α
 xchain5_n6_β:
 jmp xchain5_n9_α
# IR_VAR
 xchain5_n7_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp xchain5_n10_α
 xchain5_n7_β:
 jmp xchain5_n6_α
# IR_LIT_INTEGER
 xchain5_n8_α:
 mov qword ptr [rsp + 608], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain5_n11_α
 xchain5_n8_β:
 jmp xchain5_n9_α
.Lx14_0:
 .quad 1
# IR_VAR
 xchain5_n9_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rsp + 768], rax
 mov qword ptr [rsp + 776], rdx
 jmp xchain5_n12_α
 xchain5_n9_β:
 jmp xchain5_n13_α
 xchain5_n10_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 496]
 cmp eax, 100
 je .Lx16_0
 mov eax, dword ptr [rsp + 496]
 cmp eax, 6
 jne .Lx16_2
.Lx16_1:
 mov rax, 2
 mov rcx, qword ptr [rsp + 504]
 imul rax, rcx
 mov qword ptr [rsp + 432], 6
 mov qword ptr [rsp + 440], rax
 jmp xchain5_n14_α
.Lx16_0:
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 mov rdx, qword ptr [rsp + 496]
 mov rcx, qword ptr [rsp + 504]
 mov r8d, 2
 lea r9, [rsp + 432]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx16_3
.Lx16_2:
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 mov rdx, qword ptr [rsp + 496]
 mov rcx, qword ptr [rsp + 504]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n6_α
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
.Lx16_3:
 jmp xchain5_n14_α
 xchain5_n10_β:
 jmp xchain5_n6_α
 xchain5_n11_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 576]
 cmp eax, 100
 je .Lx17_0
 mov eax, dword ptr [rsp + 576]
 cmp eax, 6
 jne .Lx17_2
.Lx17_1:
 mov rax, qword ptr [rsp + 584]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 544], 6
 mov qword ptr [rsp + 552], rax
 jmp xchain5_n15_α
.Lx17_0:
 mov rdi, qword ptr [rsp + 576]
 mov rsi, qword ptr [rsp + 584]
 mov rdx, qword ptr [rsp + 608]
 mov rcx, qword ptr [rsp + 616]
 mov r8d, 0
 lea r9, [rsp + 544]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx17_3
.Lx17_2:
 mov rdi, qword ptr [rsp + 576]
 mov rsi, qword ptr [rsp + 584]
 mov rdx, qword ptr [rsp + 608]
 mov rcx, qword ptr [rsp + 616]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n9_α
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
.Lx17_3:
 jmp xchain5_n15_α
 xchain5_n11_β:
 jmp xchain5_n9_α
# IR_LIT_INTEGER
 xchain5_n12_α:
 mov qword ptr [rsp + 736], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 744], rax
 jmp xchain5_n16_α
 xchain5_n12_β:
 jmp xchain5_n13_α
.Lx18_0:
 .quad 5
# IR_VAR
 xchain5_n13_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 816], rax
 mov qword ptr [rsp + 824], rdx
 jmp xchain5_n17_α
 xchain5_n13_β:
 jmp main_γ
 xchain5_n14_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 432]
 mov rdx, qword ptr [rsp + 440]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx21_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx21_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx21_3]
 lea rdx, [rip + .Lx21_4]
 jmp rax
.Lx21_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx21_2
.Lx21_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx21_2
.Lx21_1:
 call rt_faildescr@PLT
.Lx21_2:
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 cmp eax, 99
 je xchain5_n6_α
 jmp xchain5_n18_α
 xchain5_n14_β:
 jmp xchain5_n6_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "bump"
# IR_ASSIGN gva
 xchain5_n15_α:
 mov rax, qword ptr [rsp + 544]
 mov rdx, qword ptr [rsp + 552]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
 jmp xchain5_n9_α
 xchain5_n15_β:
 jmp xchain5_n9_α
# IR_COERCE_NUMERIC
 xchain5_n16_α:
 mov eax, dword ptr [rsp + 768]
 cmp eax, 7
 je .Lx24_1
 cmp eax, 6
 jne .Lx24_0
 mov eax, dword ptr [rsp + 736]
 cmp eax, 6
 jne .Lx24_0
.Lx24_1:
 mov rax, qword ptr [rsp + 768]
 mov qword ptr [rsp + 704], rax
 mov rax, qword ptr [rsp + 776]
 mov qword ptr [rsp + 712], rax
 jmp .Lx24_2
.Lx24_0:
 lea rdi, [rsp + 768]
 lea rsi, [rsp + 736]
 lea rdx, [rsp + 704]
 mov rcx, 147
 call rt_coerce_num2_d@PLT
.Lx24_2:
 jmp xchain5_n19_α
 xchain5_n16_β:
 jmp xchain5_n13_α
# IR_ASSIGN global
 xchain5_n17_α:
 mov rsi, qword ptr [rsp + 816]
 mov rdx, qword ptr [rsp + 824]
 mov rdi, qword ptr [rip + .Lx25_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 800], rax
 mov qword ptr [rsp + 808], rdx
 jmp main_γ
 xchain5_n17_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "OUTPUT"
 xchain5_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 352]
 mov rsi, qword ptr [rsp + 360]
 mov rdx, qword ptr [rsp + 384]
 mov rcx, qword ptr [rsp + 392]
 call str_concat_d@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain5_n20_α
 xchain5_n18_β:
 jmp xchain5_n6_α
# IR_COERCE_NUMERIC
 xchain5_n19_α:
 mov eax, dword ptr [rsp + 736]
 cmp eax, 7
 je .Lx28_1
 cmp eax, 6
 jne .Lx28_0
 mov eax, dword ptr [rsp + 768]
 cmp eax, 6
 jne .Lx28_0
.Lx28_1:
 mov rax, qword ptr [rsp + 736]
 mov qword ptr [rsp + 672], rax
 mov rax, qword ptr [rsp + 744]
 mov qword ptr [rsp + 680], rax
 jmp .Lx28_2
.Lx28_0:
 lea rdi, [rsp + 736]
 lea rsi, [rsp + 768]
 lea rdx, [rsp + 672]
 mov rcx, 148
 call rt_coerce_num2_d@PLT
.Lx28_2:
 jmp xchain5_n21_α
 xchain5_n19_β:
 jmp xchain5_n13_α
# IR_ASSIGN gva
 xchain5_n20_α:
 mov rax, qword ptr [rsp + 320]
 mov rdx, qword ptr [rsp + 328]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 jmp xchain5_n6_α
 xchain5_n20_β:
 jmp xchain5_n6_α
# IR_CMP_TEST
 xchain5_n21_α:
 lea rdi, [rsp + 704]
 lea rsi, [rsp + 672]
 call rt_cmp_d@PLT
 test eax, eax
 jns xchain5_n13_α
 mov qword ptr [rsp + 640], 0
 mov qword ptr [rsp + 648], 0
 jmp xchain5_n4_α
 xchain5_n21_β:
 jmp xchain5_n13_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 856]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 856]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
