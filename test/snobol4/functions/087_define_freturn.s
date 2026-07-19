  .intel_syntax noprefix
  .text
  .globl proc_ispos_α
proc_ispos_α:
#=======================================================================================================================
    .global proc_ispos_α
    .global proc_ispos_β
    .global proc_ispos_γ
    .global proc_ispos_ω
  sub rsp, 688
  mov [rsp + 664], rcx
  mov [rsp + 672], rdx
  mov [rsp + 680], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 656
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 648], rsp
proc_ispos_α_body:
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
 jmp xchain0_n2_α
.Lx2_0:
 .quad 0
# IR_COERCE_NUMERIC
 xchain0_n2_α:
 mov eax, dword ptr [rbp + 224]
 cmp eax, 7
 je .Lx4_1
 cmp eax, 6
 jne .Lx4_0
 mov eax, dword ptr [rbp + 192]
 cmp eax, 6
 jne .Lx4_0
.Lx4_1:
 mov rax, qword ptr [rbp + 224]
 mov qword ptr [rbp + 160], rax
 mov rax, qword ptr [rbp + 232]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n3_α
.Lx4_0:
 lea rdi, [rbp + 224]
 lea rsi, [rbp + 192]
 lea rdx, [rbp + 160]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
 jmp xchain0_n3_α
# IR_COERCE_NUMERIC
 xchain0_n3_α:
 mov eax, dword ptr [rbp + 192]
 cmp eax, 7
 je .Lx6_1
 cmp eax, 6
 jne .Lx6_0
 mov eax, dword ptr [rbp + 224]
 cmp eax, 6
 jne .Lx6_0
.Lx6_1:
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 136], rax
 jmp xchain0_n4_α
.Lx6_0:
 lea rdi, [rbp + 192]
 lea rsi, [rbp + 224]
 lea rdx, [rbp + 128]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
 jmp xchain0_n4_α
# IR_CMP_TEST
 xchain0_n4_α:
 lea rdi, [rbp + 160]
 lea rsi, [rbp + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jle proc_ispos_ω
 mov qword ptr [rbp + 96], 0
 mov qword ptr [rbp + 104], 0
 jmp proc_ispos_γ
proc_ispos_res:
add rsp, 8
pop rbp
proc_ispos_β:
jmp proc_ispos_ω
proc_ispos_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 664]
lea rsp, [rbp + 688]
mov rbp, [rbp + 680]
jmp rax
proc_ispos_ω:
mov rax, [rbp + 672]
lea rsp, [rbp + 688]
mov rbp, [rbp + 680]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "ispos"
  .Lstartup_pp0_0: .string "x"
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
  lea rsi, [rip + proc_ispos_α]
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
  .Lgvan0: .string "ispos"
  .Lgvan1: .string "x"
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
  mov qword ptr [rsp + 648], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain9_n0_α:
 mov qword ptr [rbp + 304], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 312], rax
 jmp xchain9_n1_α
.Lx10_0:
 .quad 5
 xchain9_n1_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx12_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx12_5
 mov rax, qword ptr [rbp + 304]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 312]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx12_6]
 lea rdx, [rip + .Lx12_7]
 jmp rax
.Lx12_6:
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
 jmp .Lx12_2
.Lx12_7:
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
 jmp .Lx12_2
.Lx12_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 304]
 mov rdx, qword ptr [rbp + 312]
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
 mov qword ptr [rbp + 256], rax
 mov qword ptr [rbp + 264], rdx
 cmp eax, 99
 je xchain9_n3_α
 jmp xchain9_n2_α
 xchain9_n1_β:
 jmp xchain9_n3_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "ispos"
# IR_LIT_STRING
 xchain9_n2_α:
 mov qword ptr [rbp + 352], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rbp + 360], rax
 jmp xchain9_n4_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "positive"
# IR_LIT_STRING
 xchain9_n3_α:
 mov qword ptr [rbp + 400], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 408], rax
 jmp xchain9_n5_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "wrong"
# IR_ASSIGN global
 xchain9_n4_α:
 mov rsi, qword ptr [rbp + 352]
 mov rdx, qword ptr [rbp + 360]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain9_n6_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain9_n5_α:
 mov rsi, qword ptr [rbp + 400]
 mov rdx, qword ptr [rbp + 408]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 jmp xchain9_n6_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain9_n6_α:
 mov qword ptr [rbp + 512], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain9_n7_α
.Lx17_0:
 .quad 3
# IR_UNOP
 xchain9_n7_α:
 mov rdi, qword ptr [rbp + 512]
 mov rsi, qword ptr [rbp + 520]
 call rt_num_neg@PLT
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp xchain9_n8_α
 xchain9_n8_α:
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
 mov rax, qword ptr [rbp + 480]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 488]
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
 mov rsi, qword ptr [rbp + 480]
 mov rdx, qword ptr [rbp + 488]
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
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 cmp eax, 99
 je xchain9_n9_α
 jmp xchain9_n10_α
 xchain9_n8_β:
 jmp xchain9_n9_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "ispos"
# IR_LIT_STRING
 xchain9_n9_α:
 mov qword ptr [rbp + 608], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rbp + 616], rax
 jmp xchain9_n11_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "not positive"
# IR_LIT_STRING
 xchain9_n10_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain9_n12_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "wrong"
# IR_ASSIGN global
 xchain9_n11_α:
 mov rsi, qword ptr [rbp + 608]
 mov rdx, qword ptr [rbp + 616]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain9_n12_α:
 mov rsi, qword ptr [rbp + 560]
 mov rdx, qword ptr [rbp + 568]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 648]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 648]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
