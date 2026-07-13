  .intel_syntax noprefix
  .text
  .globl proc_EXPR$0_α
proc_EXPR$0_α:
#=======================================================================================================================
    .global proc_EXPR$0_α
    .global proc_EXPR$0_β
    .global proc_EXPR$0_γ
    .global proc_EXPR$0_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 136], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 128], rax
 pop rsi
proc_EXPR$0_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_EXPR$0_ω
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_EXPR$0_ω
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "def"
 xchain0_n2_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 64]
 mov rsi, qword ptr [r12 + 72]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 call str_concat_d@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_EXPR$0_ω
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$0_γ
 xchain0_n3_β:
 jmp proc_EXPR$0_ω
proc_EXPR$0_β:
jmp proc_EXPR$0_ω
proc_EXPR$0_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 136]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 128]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_EXPR$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 136]
pop r12
ret
  .globl proc_EXPR$1_α
proc_EXPR$1_α:
#=======================================================================================================================
    .global proc_EXPR$1_α
    .global proc_EXPR$1_β
    .global proc_EXPR$1_γ
    .global proc_EXPR$1_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 72], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 64], rax
 pop rsi
proc_EXPR$1_α_body:
# IR_VAR
 xchain5_n0_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp proc_EXPR$1_ω
# IR_ASSIGN gva
 xchain5_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 96], rax
 mov qword ptr [rbx + 104], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$1_γ
 xchain5_n1_β:
 jmp proc_EXPR$1_ω
proc_EXPR$1_β:
jmp proc_EXPR$1_ω
proc_EXPR$1_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 72]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 64]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_EXPR$1_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 72]
pop r12
ret
  .globl proc_EXPR$2_α
proc_EXPR$2_α:
#=======================================================================================================================
    .global proc_EXPR$2_α
    .global proc_EXPR$2_β
    .global proc_EXPR$2_γ
    .global proc_EXPR$2_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 168], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 160], rax
 pop rsi
proc_EXPR$2_α_body:
# IR_LIT_INTEGER
 xchain8_n0_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain8_n1_α
 xchain8_n0_β:
 jmp proc_EXPR$2_ω
.Lx9_0:
 .quad 1
# IR_LIT_INTEGER
 xchain8_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain8_n2_α
 xchain8_n1_β:
 jmp proc_EXPR$2_ω
.Lx10_0:
 .quad 2
 xchain8_n2_α:
# BOX CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+96] -> [zr+48]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = producer-box slot [zr+128] -> [zr+64]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn9: .string "ident"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$2_ω
 jmp xchain8_n3_α
 xchain8_n2_β:
 jmp proc_EXPR$2_ω
# IR_ASSIGN gva
 xchain8_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$2_γ
 xchain8_n3_β:
 jmp proc_EXPR$2_ω
proc_EXPR$2_β:
jmp proc_EXPR$2_ω
proc_EXPR$2_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 168]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 160]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_EXPR$2_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 168]
pop r12
ret
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "EXPR$0"
  .align 8
  .Lstartup_pnames0:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_EXPR$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 144
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname1: .string "EXPR$1"
  .align 8
  .Lstartup_pnames1:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + .Lstartup_pnames1]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + proc_EXPR$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  .section .rodata
  .Lstartup_pname2: .string "EXPR$2"
  .align 8
  .Lstartup_pnames2:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + .Lstartup_pnames2]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + proc_EXPR$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 176
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "expr"
  .Lgvan1: .string "output"
  .Lgvan2: .string "q"
  .Lgvan3: .string "sexp"
  .Lgvan4: .string "fexp"
  .Lgvan5: .string "EXPR$0"
  .Lgvan6: .string "EXPR$1"
  .Lgvan7: .string "EXPR$2"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .quad .Lgvan7
  .section .bss
  .align 16
__gva: .space 128, 0
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
  mov edx, 8
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
  mov qword ptr [r12 + 1064], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1056], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain13_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain13_n1_α
 xchain13_n0_β:
 jmp xchain13_n3_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "EXPR$0"
 xchain13_n1_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn16: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain13_n3_α
 jmp xchain13_n2_α
 xchain13_n1_β:
 jmp xchain13_n3_α
# IR_ASSIGN gva
 xchain13_n2_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain13_n3_α
 xchain13_n2_β:
 jmp xchain13_n3_α
# IR_VAR
 xchain13_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain13_n4_α
 xchain13_n3_β:
 jmp xchain13_n5_α
 xchain13_n4_α:
# BOX CALL eval(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+304] -> [zr+272]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn15: .string "eval"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je xchain13_n5_α
 jmp xchain13_n6_α
 xchain13_n4_β:
 jmp xchain13_n5_α
# IR_LIT_STRING
 xchain13_n5_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain13_n7_α
 xchain13_n5_β:
 jmp xchain13_n9_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "qqq"
# IR_LIT_STRING
 xchain13_n6_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain13_n8_α
 xchain13_n6_β:
 jmp xchain13_n5_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "abcdef"
# IR_ASSIGN gva
 xchain13_n7_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain13_n9_α
 xchain13_n7_β:
 jmp xchain13_n9_α
 xchain13_n8_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+256] -> [zr+208]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [zr+336] -> [zr+224]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain13_n5_α
 jmp xchain13_n10_α
 xchain13_n8_β:
 jmp xchain13_n5_α
# IR_LIT_STRING
 xchain13_n9_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain13_n11_α
 xchain13_n9_β:
 jmp xchain13_n14_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "EXPR$1"
# IR_LIT_STRING
 xchain13_n10_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain13_n12_α
 xchain13_n10_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 1016/001: eval concat expr"
 xchain13_n11_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+528] -> [zr+496]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lrkfn27: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn27]
 lea rsi, [r12 + 496]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je xchain13_n14_α
 jmp xchain13_n13_α
 xchain13_n11_β:
 jmp xchain13_n14_α
# IR_ASSIGN gva
 xchain13_n12_α:
 mov rax, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp main_γ
 xchain13_n12_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain13_n13_α:
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain13_n14_α
 xchain13_n13_β:
 jmp xchain13_n14_α
# IR_VAR
 xchain13_n14_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain13_n15_α
 xchain13_n14_β:
 jmp xchain13_n16_α
 xchain13_n15_α:
# BOX CALL eval(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn26: .string "eval"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je xchain13_n16_α
 jmp xchain13_n17_α
 xchain13_n15_β:
 jmp xchain13_n16_α
# IR_LIT_STRING
 xchain13_n16_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain13_n18_α
 xchain13_n16_β:
 jmp xchain13_n21_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "EXPR$2"
# IR_LIT_STRING
 xchain13_n17_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain13_n19_α
 xchain13_n17_β:
 jmp xchain13_n16_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "qqq"
 xchain13_n18_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+848] -> [zr+816]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lrkfn35: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn35]
 lea rsi, [r12 + 816]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain13_n21_α
 jmp xchain13_n20_α
 xchain13_n18_β:
 jmp xchain13_n21_α
 xchain13_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+576]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 584], rax
# marshal arg1 = producer-box slot [zr+704] -> [zr+592]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn30: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 cmp eax, 99
 je xchain13_n16_α
 jmp xchain13_n22_α
 xchain13_n19_β:
 jmp xchain13_n16_α
# IR_ASSIGN gva
 xchain13_n20_α:
 mov rax, qword ptr [r12 + 800]
 mov rdx, qword ptr [r12 + 808]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain13_n21_α
 xchain13_n20_β:
 jmp xchain13_n21_α
# IR_VAR
 xchain13_n21_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain13_n23_α
 xchain13_n21_β:
 jmp xchain13_n24_α
# IR_LIT_STRING
 xchain13_n22_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain13_n25_α
 xchain13_n22_β:
 jmp main_γ
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "FAIL 1016/002: eval var ref"
 xchain13_n23_α:
# BOX CALL eval(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+928] -> [zr+896]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 904], rax
  .section .rodata
  .Lbynamefn34: .string "eval"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 896]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain13_n24_α
 jmp xchain13_n26_α
 xchain13_n23_β:
 jmp xchain13_n24_α
# IR_LIT_STRING
 xchain13_n24_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain13_n27_α
 xchain13_n24_β:
 jmp main_γ
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "PASS 1016_eval (3/3)"
# IR_ASSIGN gva
 xchain13_n25_α:
 mov rax, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp main_γ
 xchain13_n25_β:
 jmp main_γ
# IR_LIT_STRING
 xchain13_n26_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain13_n28_α
 xchain13_n26_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 1016/003: eval failing expr should fail"
# IR_ASSIGN gva
 xchain13_n27_α:
 mov rax, qword ptr [r12 + 1024]
 mov rdx, qword ptr [r12 + 1032]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp main_γ
 xchain13_n27_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain13_n28_α:
 mov rax, qword ptr [r12 + 976]
 mov rdx, qword ptr [r12 + 984]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp main_γ
 xchain13_n28_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 1064]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1056]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1064]
pop r12
ret
