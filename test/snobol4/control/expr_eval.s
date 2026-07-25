  .intel_syntax noprefix
  .text
  .globl proc_Push_α
proc_Push_α:
#=======================================================================================================================
    .global proc_Push_α
    .global proc_Push_β
    .global proc_Push_γ
    .global proc_Push_ω
  sub rsp, 752
  mov [rsp + 728], rcx
  mov [rsp + 736], rdx
  mov [rsp + 744], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 704], 0
  mov qword ptr [rsp + 712], rsp
proc_Push_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n1_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 136], rax
 jmp xchain0_n3_α
.Lx2_0:
 .quad 0
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain0_n4_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n3_α:
 mov rdi, qword ptr [rbp + 96]
 mov rsi, qword ptr [rbp + 104]
 mov rdx, qword ptr [rbp + 128]
 mov rcx, qword ptr [rbp + 136]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 jmp xchain0_n8_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rbp + 464], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 472], rax
 jmp xchain0_n11_α
.Lx8_0:
 .quad 0
 xchain0_n8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+624] -> [zr+592]
 mov rax, qword ptr [rbp + 624]
 mov qword ptr [rbp + 592], rax
 mov rax, qword ptr [rbp + 632]
 mov qword ptr [rbp + 600], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn10]
 lea rsi, [rbp + 592]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n12_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rbp + 16], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 24], rax
 jmp xchain0_n13_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rbp + 256], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 264], rax
 jmp xchain0_n14_α
.Lx12_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [rbp + 432]
 mov rsi, qword ptr [rbp + 440]
 mov rdx, qword ptr [rbp + 464]
 mov rcx, qword ptr [rbp + 472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain0_n15_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain0_n16_α
 xchain0_n13_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [rbp + 16]
 mov qword ptr [rbp + 64], rax
 mov rax, qword ptr [rbp + 24]
 mov qword ptr [rbp + 72], rax
  .section .rodata
  .Lrkfn16: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn16]
 lea rsi, [rbp + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 48], rax
 mov qword ptr [rbp + 56], rdx
 cmp eax, 99
 je proc_Push_ω
 jmp proc_Push_γ
 xchain0_n13_β:
 jmp proc_Push_ω
# IR_SUBSCRIPT x[i] variable
 xchain0_n14_α:
 mov rdi, qword ptr [rbp + 224]
 mov rsi, qword ptr [rbp + 232]
 mov rdx, qword ptr [rbp + 256]
 mov rcx, qword ptr [rbp + 264]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 jmp xchain0_n17_α
# IR_DEREF variable -> value
 xchain0_n15_α:
 mov rdi, qword ptr [rbp + 496]
 mov rsi, qword ptr [rbp + 504]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain0_n18_α
# IR_ASSIGN_VAR
 xchain0_n16_α:
 mov rdi, qword ptr [rbp + 576]
 mov rsi, qword ptr [rbp + 584]
 mov rdx, qword ptr [rbp + 656]
 mov rcx, qword ptr [rbp + 664]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [rbp + 688], rax
 mov qword ptr [rbp + 696], rdx
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n17_α:
 mov rdi, qword ptr [rbp + 288]
 mov rsi, qword ptr [rbp + 296]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp xchain0_n19_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [rbp + 400]
 mov rsi, qword ptr [rbp + 408]
 mov rdx, qword ptr [rbp + 528]
 mov rcx, qword ptr [rbp + 536]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp xchain0_n20_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rbp + 336], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n21_α
.Lx22_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rbp + 544]
 mov rdx, qword ptr [rbp + 552]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 jmp xchain0_n5_α
 xchain0_n21_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 320]
 cmp eax, 100
 je .Lx24_0
 mov eax, dword ptr [rbp + 320]
 cmp eax, 6
 jne .Lx24_2
.Lx24_1:
 mov rax, qword ptr [rbp + 328]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 192], 6
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n22_α
.Lx24_0:
 mov rdi, qword ptr [rbp + 320]
 mov rsi, qword ptr [rbp + 328]
 mov rdx, qword ptr [rbp + 336]
 mov rcx, qword ptr [rbp + 344]
 mov r8d, 0
 lea r9, [rbp + 192]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n22_α
.Lx24_2:
 mov rdi, qword ptr [rbp + 320]
 mov rsi, qword ptr [rbp + 328]
 mov rdx, qword ptr [rbp + 336]
 mov rcx, qword ptr [rbp + 344]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n2_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [rbp + 160]
 mov rsi, qword ptr [rbp + 168]
 mov rdx, qword ptr [rbp + 192]
 mov rcx, qword ptr [rbp + 200]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n2_α
proc_Push_res:
add rsp, 8
pop rbp
proc_Push_β:
jmp proc_Push_ω
proc_Push_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 728]
lea rsp, [rbp + 752]
mov rbp, [rbp + 744]
jmp rax
proc_Push_ω:
mov rax, [rbp + 736]
lea rsp, [rbp + 752]
mov rbp, [rbp + 744]
jmp rax
  .globl proc_Pop_α
proc_Pop_α:
#=======================================================================================================================
    .global proc_Pop_α
    .global proc_Pop_β
    .global proc_Pop_γ
    .global proc_Pop_ω
  sub rsp, 640
  mov [rsp + 616], rcx
  mov [rsp + 624], rdx
  mov [rsp + 632], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 592], 0
  mov qword ptr [rsp + 600], rsp
proc_Pop_α_body:
# IR_VAR
 xchain26_n0_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 jmp xchain26_n1_α
# IR_VAR
 xchain26_n1_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain26_n3_α
# IR_VAR
 xchain26_n2_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 jmp xchain26_n4_α
# IR_LIT_INTEGER
 xchain26_n3_α:
 mov qword ptr [rbp + 176], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 184], rax
 jmp xchain26_n5_α
.Lx30_0:
 .quad 0
# IR_LIT_INTEGER
 xchain26_n4_α:
 mov qword ptr [rbp + 336], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain26_n6_α
.Lx31_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain26_n5_α:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain26_n7_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n6_α:
 mov rdi, qword ptr [rbp + 304]
 mov rsi, qword ptr [rbp + 312]
 mov rdx, qword ptr [rbp + 336]
 mov rcx, qword ptr [rbp + 344]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain26_n8_α
# IR_DEREF variable -> value
 xchain26_n7_α:
 mov rdi, qword ptr [rbp + 208]
 mov rsi, qword ptr [rbp + 216]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 jmp xchain26_n9_α
# IR_VAR
 xchain26_n8_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain26_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n9_α:
 mov rdi, qword ptr [rbp + 112]
 mov rsi, qword ptr [rbp + 120]
 mov rdx, qword ptr [rbp + 240]
 mov rcx, qword ptr [rbp + 248]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 256], rax
 mov qword ptr [rbp + 264], rdx
 jmp xchain26_n11_α
# IR_LIT_INTEGER
 xchain26_n10_α:
 mov qword ptr [rbp + 464], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [rbp + 472], rax
 jmp xchain26_n12_α
.Lx37_0:
 .quad 0
# IR_DEREF variable -> value
 xchain26_n11_α:
 mov rdi, qword ptr [rbp + 256]
 mov rsi, qword ptr [rbp + 264]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 jmp xchain26_n13_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n12_α:
 mov rdi, qword ptr [rbp + 432]
 mov rsi, qword ptr [rbp + 440]
 mov rdx, qword ptr [rbp + 464]
 mov rcx, qword ptr [rbp + 472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain26_n14_α
# IR_ASSIGN gva
 xchain26_n13_α:
 mov rax, qword ptr [rbp + 288]
 mov rdx, qword ptr [rbp + 296]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain26_n2_α
# IR_DEREF variable -> value
 xchain26_n14_α:
 mov rdi, qword ptr [rbp + 496]
 mov rsi, qword ptr [rbp + 504]
 call rt_deref@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain26_n15_α
# IR_LIT_INTEGER
 xchain26_n15_α:
 mov qword ptr [rbp + 544], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [rbp + 552], rax
 jmp xchain26_n16_α
.Lx42_0:
 .quad 1
 xchain26_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 528]
 cmp eax, 100
 je .Lx43_0
 mov eax, dword ptr [rbp + 528]
 cmp eax, 6
 jne .Lx43_2
.Lx43_1:
 mov rax, qword ptr [rbp + 536]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [rbp + 400], 6
 mov qword ptr [rbp + 408], rax
 jmp xchain26_n17_α
.Lx43_0:
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 544]
 mov rcx, qword ptr [rbp + 552]
 mov r8d, 1
 lea r9, [rbp + 400]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain26_n17_α
.Lx43_2:
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 544]
 mov rcx, qword ptr [rbp + 552]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain26_n17_α
 xchain26_n16_β:
 jmp proc_Pop_γ
# IR_ASSIGN_VAR
 xchain26_n17_α:
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 mov rdx, qword ptr [rbp + 400]
 mov rcx, qword ptr [rbp + 408]
 call rt_assign_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp proc_Pop_γ
proc_Pop_res:
add rsp, 8
pop rbp
proc_Pop_β:
jmp proc_Pop_ω
proc_Pop_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 616]
lea rsp, [rbp + 640]
mov rbp, [rbp + 632]
jmp rax
proc_Pop_ω:
mov rax, [rbp + 624]
lea rsp, [rbp + 640]
mov rbp, [rbp + 632]
jmp rax
  .globl proc_Unary_α
proc_Unary_α:
#=======================================================================================================================
    .global proc_Unary_α
    .global proc_Unary_β
    .global proc_Unary_γ
    .global proc_Unary_ω
  sub rsp, 608
  mov [rsp + 584], rcx
  mov [rsp + 592], rdx
  mov [rsp + 600], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 560], 0
  mov qword ptr [rsp + 568], rsp
proc_Unary_α_body:
 xchain45_n0_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx47_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx47_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx47_6]
 lea rdx, [rip + .Lx47_7]
 jmp rax
.Lx47_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx47_2
.Lx47_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx47_2
.Lx47_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx47_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx47_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx47_3]
 lea rdx, [rip + .Lx47_4]
 jmp rax
.Lx47_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx47_2
.Lx47_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx47_2
.Lx47_1:
 call rt_faildescr@PLT
.Lx47_2:
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain45_n2_α
 jmp xchain45_n1_α
 xchain45_n0_β:
 jmp xchain45_n2_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain45_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain45_n2_α
 xchain45_n2_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx50_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx50_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx50_6]
 lea rdx, [rip + .Lx50_7]
 jmp rax
.Lx50_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx50_2
.Lx50_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx50_2
.Lx50_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx50_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx50_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx50_3]
 lea rdx, [rip + .Lx50_4]
 jmp rax
.Lx50_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx50_2
.Lx50_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx50_2
.Lx50_1:
 call rt_faildescr@PLT
.Lx50_2:
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 cmp eax, 99
 je xchain45_n4_α
 jmp xchain45_n3_α
 xchain45_n2_β:
 jmp xchain45_n4_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain45_n3_α:
 mov rax, qword ptr [rbp + 160]
 mov rdx, qword ptr [rbp + 168]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain45_n4_α
# IR_LIT_STRING
 xchain45_n4_α:
 mov qword ptr [rbp + 192], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain45_n5_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string ""
 xchain45_n5_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+192] -> [zr+240]
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 240], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 248], rax
  .section .rodata
  .Lrkfn54: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn54]
 lea rsi, [rbp + 240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n6_α
 xchain45_n5_β:
 jmp xchain45_n7_α
 xchain45_n6_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx56_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx56_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx56_6]
 lea rdx, [rip + .Lx56_7]
 jmp rax
.Lx56_6:
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
 jmp .Lx56_2
.Lx56_7:
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
 jmp .Lx56_2
.Lx56_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx56_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx56_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx56_3]
 lea rdx, [rip + .Lx56_4]
 jmp rax
.Lx56_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx56_2
.Lx56_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx56_2
.Lx56_1:
 call rt_faildescr@PLT
.Lx56_2:
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n8_α
 xchain45_n6_β:
 jmp xchain45_n7_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "Push"
# IR_LIT_STRING
 xchain45_n7_α:
 mov qword ptr [rbp + 528], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rbp + 536], rax
 jmp xchain45_n9_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "dummy"
# IR_VAR
 xchain45_n8_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 jmp xchain45_n10_α
 xchain45_n9_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+528] -> [zr+496]
 mov rax, qword ptr [rbp + 528]
 mov qword ptr [rbp + 496], rax
 mov rax, qword ptr [rbp + 536]
 mov qword ptr [rbp + 504], rax
  .section .rodata
  .Lrkfn60: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn60]
 lea rsi, [rbp + 496]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 cmp eax, 99
 je xchain45_n12_α
 jmp xchain45_n11_α
 xchain45_n9_β:
 jmp xchain45_n12_α
# IR_VAR
 xchain45_n10_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain45_n13_α
# IR_ASSIGN gva
 xchain45_n11_α:
 mov rax, qword ptr [rbp + 480]
 mov rdx, qword ptr [rbp + 488]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain45_n12_α
# IR_LIT_STRING
 xchain45_n12_α:
 mov qword ptr [rbp + 16], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rbp + 24], rax
 jmp xchain45_n14_α
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string ""
 xchain45_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 384]
 mov rsi, qword ptr [rbp + 392]
 mov rdx, qword ptr [rbp + 416]
 mov rcx, qword ptr [rbp + 424]
 call str_concat_d@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain45_n15_α
 xchain45_n14_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [rbp + 16]
 mov qword ptr [rbp + 64], rax
 mov rax, qword ptr [rbp + 24]
 mov qword ptr [rbp + 72], rax
  .section .rodata
  .Lrkfn66: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn66]
 lea rsi, [rbp + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 48], rax
 mov qword ptr [rbp + 56], rdx
 cmp eax, 99
 je proc_Unary_ω
 jmp proc_Unary_γ
 xchain45_n14_β:
 jmp proc_Unary_ω
 xchain45_n15_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+352] -> [zr+320]
 mov rax, qword ptr [rbp + 352]
 mov qword ptr [rbp + 320], rax
 mov rax, qword ptr [rbp + 360]
 mov qword ptr [rbp + 328], rax
  .section .rodata
  .Lrkfn68: .string "EVAL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn68]
 lea rsi, [rbp + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n16_α
 xchain45_n15_β:
 jmp xchain45_n7_α
# IR_ASSIGN_VAR
 xchain45_n16_α:
 mov rdi, qword ptr [rbp + 272]
 mov rsi, qword ptr [rbp + 280]
 mov rdx, qword ptr [rbp + 304]
 mov rcx, qword ptr [rbp + 312]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain45_n7_α
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain45_n7_α
proc_Unary_res:
add rsp, 8
pop rbp
proc_Unary_β:
jmp proc_Unary_ω
proc_Unary_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 584]
lea rsp, [rbp + 608]
mov rbp, [rbp + 600]
jmp rax
proc_Unary_ω:
mov rax, [rbp + 592]
lea rsp, [rbp + 608]
mov rbp, [rbp + 600]
jmp rax
  .globl proc_Binary_α
proc_Binary_α:
#=======================================================================================================================
    .global proc_Binary_α
    .global proc_Binary_β
    .global proc_Binary_γ
    .global proc_Binary_ω
  sub rsp, 848
  mov [rsp + 824], rcx
  mov [rsp + 832], rdx
  mov [rsp + 840], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 800], 0
  mov qword ptr [rsp + 808], rsp
proc_Binary_α_body:
 xchain70_n0_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx72_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx72_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx72_6]
 lea rdx, [rip + .Lx72_7]
 jmp rax
.Lx72_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx72_2
.Lx72_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx72_2
.Lx72_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx72_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx72_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx72_3]
 lea rdx, [rip + .Lx72_4]
 jmp rax
.Lx72_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx72_2
.Lx72_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx72_2
.Lx72_1:
 call rt_faildescr@PLT
.Lx72_2:
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain70_n2_α
 jmp xchain70_n1_α
 xchain70_n0_β:
 jmp xchain70_n2_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052416], rax
 mov qword ptr [1879052424], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain70_n2_α
 xchain70_n2_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx75_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx75_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx75_6]
 lea rdx, [rip + .Lx75_7]
 jmp rax
.Lx75_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx75_2
.Lx75_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx75_2
.Lx75_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx75_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx75_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx75_3]
 lea rdx, [rip + .Lx75_4]
 jmp rax
.Lx75_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx75_2
.Lx75_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx75_2
.Lx75_1:
 call rt_faildescr@PLT
.Lx75_2:
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 cmp eax, 99
 je xchain70_n4_α
 jmp xchain70_n3_α
 xchain70_n2_β:
 jmp xchain70_n4_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n3_α:
 mov rax, qword ptr [rbp + 160]
 mov rdx, qword ptr [rbp + 168]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain70_n4_α
 xchain70_n4_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx78_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx78_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx78_6]
 lea rdx, [rip + .Lx78_7]
 jmp rax
.Lx78_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx78_2
.Lx78_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx78_2
.Lx78_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx78_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx78_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx78_3]
 lea rdx, [rip + .Lx78_4]
 jmp rax
.Lx78_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx78_2
.Lx78_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx78_2
.Lx78_1:
 call rt_faildescr@PLT
.Lx78_2:
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 cmp eax, 99
 je xchain70_n6_α
 jmp xchain70_n5_α
 xchain70_n4_β:
 jmp xchain70_n6_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "Pop"
# IR_ASSIGN gva
 xchain70_n5_α:
 mov rax, qword ptr [rbp + 208]
 mov rdx, qword ptr [rbp + 216]
 mov qword ptr [1879052400], rax
 mov qword ptr [1879052408], rdx
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain70_n6_α
# IR_LIT_STRING
 xchain70_n6_α:
 mov qword ptr [rbp + 240], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain70_n7_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string ""
 xchain70_n7_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+240] -> [zr+288]
 mov rax, qword ptr [rbp + 240]
 mov qword ptr [rbp + 288], rax
 mov rax, qword ptr [rbp + 248]
 mov qword ptr [rbp + 296], rax
  .section .rodata
  .Lrkfn82: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn82]
 lea rsi, [rbp + 288]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n8_α
 xchain70_n7_β:
 jmp xchain70_n9_α
 xchain70_n8_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx84_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx84_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx84_6]
 lea rdx, [rip + .Lx84_7]
 jmp rax
.Lx84_6:
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
 jmp .Lx84_2
.Lx84_7:
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
 jmp .Lx84_2
.Lx84_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx84_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx84_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx84_3]
 lea rdx, [rip + .Lx84_4]
 jmp rax
.Lx84_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx84_2
.Lx84_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx84_2
.Lx84_1:
 call rt_faildescr@PLT
.Lx84_2:
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n10_α
 xchain70_n8_β:
 jmp xchain70_n9_α
.Lx84_0:
 .quad .Lx84_0_s
.Lx84_0_s:
 .string "Push"
# IR_LIT_STRING
 xchain70_n9_α:
 mov qword ptr [rbp + 768], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain70_n11_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "dummy"
# IR_VAR
 xchain70_n10_α:
 mov rax, qword ptr [1879052400]
 mov rdx, qword ptr [1879052408]
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain70_n12_α
 xchain70_n11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+768] -> [zr+736]
 mov rax, qword ptr [rbp + 768]
 mov qword ptr [rbp + 736], rax
 mov rax, qword ptr [rbp + 776]
 mov qword ptr [rbp + 744], rax
  .section .rodata
  .Lrkfn88: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn88]
 lea rsi, [rbp + 736]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 720], rax
 mov qword ptr [rbp + 728], rdx
 cmp eax, 99
 je xchain70_n14_α
 jmp xchain70_n13_α
 xchain70_n11_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n12_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain70_n15_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string " "
# IR_ASSIGN gva
 xchain70_n13_α:
 mov rax, qword ptr [rbp + 720]
 mov rdx, qword ptr [rbp + 728]
 mov qword ptr [1879052384], rax
 mov qword ptr [1879052392], rdx
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n14_α:
 mov qword ptr [rbp + 16], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [rbp + 24], rax
 jmp xchain70_n16_α
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string ""
 xchain70_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 560]
 mov rcx, qword ptr [rbp + 568]
 call str_concat_d@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain70_n17_α
 xchain70_n16_α:
# BOX IR_CALL SNO$NRET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+16] -> [zr+64]
 mov rax, qword ptr [rbp + 16]
 mov qword ptr [rbp + 64], rax
 mov rax, qword ptr [rbp + 24]
 mov qword ptr [rbp + 72], rax
  .section .rodata
  .Lrkfn94: .string "SNO$NRET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn94]
 lea rsi, [rbp + 64]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 48], rax
 mov qword ptr [rbp + 56], rdx
 cmp eax, 99
 je proc_Binary_ω
 jmp proc_Binary_γ
 xchain70_n16_β:
 jmp proc_Binary_ω
# IR_VAR
 xchain70_n17_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain70_n18_α
 xchain70_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 496]
 mov rsi, qword ptr [rbp + 504]
 mov rdx, qword ptr [rbp + 592]
 mov rcx, qword ptr [rbp + 600]
 call str_concat_d@PLT
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain70_n19_α
# IR_LIT_STRING
 xchain70_n19_α:
 mov qword ptr [rbp + 624], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain70_n20_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string " "
 xchain70_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 464]
 mov rsi, qword ptr [rbp + 472]
 mov rdx, qword ptr [rbp + 624]
 mov rcx, qword ptr [rbp + 632]
 call str_concat_d@PLT
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain70_n21_α
# IR_VAR
 xchain70_n21_α:
 mov rax, qword ptr [1879052416]
 mov rdx, qword ptr [1879052424]
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain70_n22_α
 xchain70_n22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 432]
 mov rsi, qword ptr [rbp + 440]
 mov rdx, qword ptr [rbp + 656]
 mov rcx, qword ptr [rbp + 664]
 call str_concat_d@PLT
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain70_n23_α
 xchain70_n23_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [rbp + 400]
 mov qword ptr [rbp + 368], rax
 mov rax, qword ptr [rbp + 408]
 mov qword ptr [rbp + 376], rax
  .section .rodata
  .Lrkfn102: .string "EVAL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn102]
 lea rsi, [rbp + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n24_α
 xchain70_n23_β:
 jmp xchain70_n9_α
# IR_ASSIGN_VAR
 xchain70_n24_α:
 mov rdi, qword ptr [rbp + 320]
 mov rsi, qword ptr [rbp + 328]
 mov rdx, qword ptr [rbp + 352]
 mov rcx, qword ptr [rbp + 360]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain70_n9_α
 mov qword ptr [rbp + 688], rax
 mov qword ptr [rbp + 696], rdx
 jmp xchain70_n9_α
proc_Binary_res:
add rsp, 8
pop rbp
proc_Binary_β:
jmp proc_Binary_ω
proc_Binary_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 824]
lea rsp, [rbp + 848]
mov rbp, [rbp + 840]
jmp rax
proc_Binary_ω:
mov rax, [rbp + 832]
lea rsp, [rbp + 848]
mov rbp, [rbp + 840]
jmp rax
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 96
  mov [rsp + 72], rcx
  mov [rsp + 80], rdx
  mov [rsp + 88], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 32], 0
  mov qword ptr [rsp + 40], 0
  mov qword ptr [rsp + 48], 0
  mov qword ptr [rsp + 56], rsp
mov qword ptr [rbp + 64], r8
mov dword ptr [rbp + 56], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain104_n0_β]
mov qword ptr [rbp + 32], rax
# IR_MATCH_SPAN
 xchain104_n0_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 movsxd rcx, r14d
.Lx106_0:
 cmp ecx, r15d
 jge .Lx106_1
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx106_1
 add ecx, 1
 cmp ecx, r15d
 jge .Lx106_1
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx106_1
 add ecx, 1
 cmp ecx, r15d
 jge .Lx106_1
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx106_1
 add ecx, 1
 cmp ecx, r15d
 jge .Lx106_1
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx106_1
 add ecx, 1
 jmp .Lx106_0
.Lx106_1:
 cmp ecx, r14d
 jg .Lx106_240
 add rsp, 16
 jmp proc_PAT$0_scanfail
.Lx106_240:
 mov dword ptr [rsp + 4], r14d
 mov r14d, ecx
 jmp proc_PAT$0_scanhit
 xchain104_n0_β:
 xchain104_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$0_scanfail
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 64], 1
jne 7f
mov ecx, dword ptr [rbp + 56]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 64], 1
jne 8f
mov eax, dword ptr [rbp + 56]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 56], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 32]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 72]
mov rbp, [rbp + 88]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 80]
lea rsp, [rbp + 96]
mov rbp, [rbp + 88]
jmp rax
  .globl proc_PAT$1_α
proc_PAT$1_α:
#=======================================================================================================================
    .global proc_PAT$1_α
    .global proc_PAT$1_β
    .global proc_PAT$1_γ
    .global proc_PAT$1_ω
  sub rsp, 160
  mov [rsp + 136], rcx
  mov [rsp + 144], rdx
  mov [rsp + 152], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 96], 0
  mov qword ptr [rsp + 104], 0
  mov qword ptr [rsp + 112], 0
  mov qword ptr [rsp + 120], rsp
mov qword ptr [rbp + 128], r8
mov dword ptr [rbp + 120], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
lea rax, [rip + xchain107_n0_β]
mov qword ptr [rbp + 96], rax
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain107_n0_α:
 mov dword ptr [rbp + 16], r14d
 jmp xchain107_n1_α
xchain107_n0_as:
 jmp proc_PAT$1_γ
 xchain107_n0_β:
 jmp xchain107_n3_β
xchain107_n0_af:
 jmp proc_PAT$1_ω
# IR_MATCH_ANY
 xchain107_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge proc_PAT$1_ω
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C1]
 cmp byte ptr [rdi+rsi], 0
 je proc_PAT$1_ω
 add r14d, 1
 jmp xchain107_n2_α
 xchain107_n1_β:
 sub r14d, 1
 jmp proc_PAT$1_ω
# IR_MATCH_ALT_NARY
 xchain107_n2_α:
 mov dword ptr [rbp + 48], r14d
 mov dword ptr [rbp + 52], 0
 jmp xchain107_n4_α
xchain107_n2_as:
 jmp xchain107_n3_α
 xchain107_n2_β:
 mov eax, dword ptr [rbp + 52]
 cmp eax, 0
 je xchain107_n4_β
 jmp xchain107_n5_β
xchain107_n2_af:
 add dword ptr [rbp + 52], 1
 mov r14d, dword ptr [rbp + 48]
 mov eax, dword ptr [rbp + 52]
 cmp eax, 1
 je xchain107_n5_α
 jmp xchain107_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain107_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx114_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx114_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx114_10
.Lx114_9:
 xor eax, eax
.Lx114_10:
 test rax, rax
 jz .Lx114_0
 mov r8d, 0
 lea rcx, [rip + .Lx114_4]
 lea rdx, [rip + .Lx114_5]
 jmp rax
.Lx114_4:
 jmp proc_PAT$1_γ
.Lx114_5:
 jmp xchain107_n2_β
.Lx114_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx114_2:
 test rax, rax
 je .Lx114_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx114_7]
 lea rdx, [rip + .Lx114_8]
 jmp rax
.Lx114_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx114_2
.Lx114_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx114_2
.Lx114_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain107_n2_β
 mov r14d, eax
 lea rax, [rip + .Lx114_6]
 sub rsp, 8
 push rax
 jmp proc_PAT$1_γ
.Lx114_6:
 add rsp, 16
 jmp xchain107_n2_β
 xchain107_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain107_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain107_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 43
 je .Lx116_0
 cmp esi, 45
 je .Lx116_0
 jmp xchain107_n2_af
.Lx116_0:
 add r14d, 1
 jmp xchain107_n2_as
 xchain107_n4_β:
 sub r14d, 1
 jmp xchain107_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain107_n5_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx117_0
 mov r8d, 0
 lea rcx, [rip + .Lx117_4]
 lea rdx, [rip + .Lx117_5]
 jmp rax
.Lx117_4:
 jmp xchain107_n2_as
.Lx117_5:
 jmp xchain107_n2_af
.Lx117_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx117_2:
 test rax, rax
 je .Lx117_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx117_7]
 lea rdx, [rip + .Lx117_8]
 jmp rax
.Lx117_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx117_2
.Lx117_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx117_2
.Lx117_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain107_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx117_6]
 sub rsp, 8
 push rax
 jmp xchain107_n2_as
.Lx117_6:
 add rsp, 16
 jmp xchain107_n2_af
 xchain107_n5_β:
 jmp qword ptr [rsp]
proc_PAT$1_scanhit:
cmp qword ptr [rbp + 128], 1
jne 7f
mov ecx, dword ptr [rbp + 120]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$1_γ
proc_PAT$1_scanfail:
cmp qword ptr [rbp + 128], 1
jne 8f
mov eax, dword ptr [rbp + 120]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 120], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$1_attempt
8:
jmp proc_PAT$1_ω
proc_PAT$1_res:
add rsp, 8
pop rbp
proc_PAT$1_β:
jmp qword ptr [rbp + 96]
proc_PAT$1_γ:
push rbp
lea rax, [rip + proc_PAT$1_res]
push rax
mov rax, [rbp + 136]
mov rbp, [rbp + 152]
jmp rax
proc_PAT$1_ω:
mov rax, [rbp + 144]
lea rsp, [rbp + 160]
mov rbp, [rbp + 152]
jmp rax
  .globl proc_PAT$2_α
proc_PAT$2_α:
#=======================================================================================================================
    .global proc_PAT$2_α
    .global proc_PAT$2_β
    .global proc_PAT$2_γ
    .global proc_PAT$2_ω
  sub rsp, 288
  mov [rsp + 264], rcx
  mov [rsp + 272], rdx
  mov [rsp + 280], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 224], 0
  mov qword ptr [rsp + 232], 0
  mov qword ptr [rsp + 240], 0
  mov qword ptr [rsp + 248], rsp
mov qword ptr [rbp + 256], r8
mov dword ptr [rbp + 248], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
lea rax, [rip + xchain118_n0_β]
mov qword ptr [rbp + 224], rax
# IR_MATCH_ALT_NARY
 xchain118_n0_α:
 mov dword ptr [rbp + 16], r14d
 mov dword ptr [rbp + 20], 0
 jmp xchain118_n1_α
xchain118_n0_as:
 jmp proc_PAT$2_γ
 xchain118_n0_β:
 mov eax, dword ptr [rbp + 20]
 cmp eax, 0
 je xchain118_n1_β
 jmp xchain118_n2_β
xchain118_n0_af:
 add dword ptr [rbp + 20], 1
 mov r14d, dword ptr [rbp + 16]
 mov eax, dword ptr [rbp + 20]
 cmp eax, 1
 je xchain118_n2_α
 jmp proc_PAT$2_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain118_n1_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain118_n3_α
xchain118_n1_as:
 jmp xchain118_n0_as
 xchain118_n1_β:
 jmp xchain118_n6_β
xchain118_n1_af:
 jmp xchain118_n0_af
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain118_n2_α:
 mov dword ptr [rbp + 176], r14d
 jmp xchain118_n7_α
xchain118_n2_as:
 jmp xchain118_n0_as
 xchain118_n2_β:
 jmp xchain118_n8_β
xchain118_n2_af:
 jmp xchain118_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx125_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx125_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx125_10
.Lx125_9:
 xor eax, eax
.Lx125_10:
 test rax, rax
 jz .Lx125_0
 mov r8d, 0
 lea rcx, [rip + .Lx125_4]
 lea rdx, [rip + .Lx125_5]
 jmp rax
.Lx125_4:
 jmp xchain118_n4_α
.Lx125_5:
 jmp xchain118_n0_af
.Lx125_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx125_2:
 test rax, rax
 je .Lx125_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx125_7]
 lea rdx, [rip + .Lx125_8]
 jmp rax
.Lx125_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx125_2
.Lx125_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx125_2
.Lx125_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx125_6]
 sub rsp, 8
 push rax
 jmp xchain118_n4_α
.Lx125_6:
 add rsp, 16
 jmp xchain118_n0_af
 xchain118_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain118_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain118_n3_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 46
 jne xchain118_n3_β
 add r14d, 1
 jmp xchain118_n5_α
 xchain118_n4_β:
 sub r14d, 1
 jmp xchain118_n3_β
# IR_MATCH_ALT_NARY
 xchain118_n5_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain118_n9_α
xchain118_n5_as:
 jmp xchain118_n6_α
 xchain118_n5_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain118_n9_β
 jmp xchain118_n10_β
xchain118_n5_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain118_n10_α
 jmp xchain118_n4_β
# IR_MATCH_ALT_NARY
 xchain118_n6_α:
 mov dword ptr [rbp + 128], r14d
 mov dword ptr [rbp + 132], 0
 jmp xchain118_n11_α
xchain118_n6_as:
 jmp xchain118_n0_as
 xchain118_n6_β:
 mov eax, dword ptr [rbp + 132]
 cmp eax, 0
 je xchain118_n11_β
 jmp xchain118_n12_β
xchain118_n6_af:
 add dword ptr [rbp + 132], 1
 mov r14d, dword ptr [rbp + 128]
 mov eax, dword ptr [rbp + 132]
 cmp eax, 1
 je xchain118_n12_α
 jmp xchain118_n5_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n7_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx132_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx132_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx132_10
.Lx132_9:
 xor eax, eax
.Lx132_10:
 test rax, rax
 jz .Lx132_0
 mov r8d, 0
 lea rcx, [rip + .Lx132_4]
 lea rdx, [rip + .Lx132_5]
 jmp rax
.Lx132_4:
 jmp xchain118_n8_α
.Lx132_5:
 jmp xchain118_n0_af
.Lx132_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx132_2:
 test rax, rax
 je .Lx132_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx132_7]
 lea rdx, [rip + .Lx132_8]
 jmp rax
.Lx132_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx132_2
.Lx132_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx132_2
.Lx132_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx132_6]
 sub rsp, 8
 push rax
 jmp xchain118_n8_α
.Lx132_6:
 add rsp, 16
 jmp xchain118_n0_af
 xchain118_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n8_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx133_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx133_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx133_10
.Lx133_9:
 xor eax, eax
.Lx133_10:
 test rax, rax
 jz .Lx133_0
 mov r8d, 0
 lea rcx, [rip + .Lx133_4]
 lea rdx, [rip + .Lx133_5]
 jmp rax
.Lx133_4:
 jmp xchain118_n0_as
.Lx133_5:
 jmp xchain118_n7_β
.Lx133_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx133_2:
 test rax, rax
 je .Lx133_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx133_7]
 lea rdx, [rip + .Lx133_8]
 jmp rax
.Lx133_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx133_2
.Lx133_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx133_2
.Lx133_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx133_6]
 sub rsp, 8
 push rax
 jmp xchain118_n0_as
.Lx133_6:
 add rsp, 16
 jmp xchain118_n7_β
 xchain118_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n9_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx134_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx134_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx134_10
.Lx134_9:
 xor eax, eax
.Lx134_10:
 test rax, rax
 jz .Lx134_0
 mov r8d, 0
 lea rcx, [rip + .Lx134_4]
 lea rdx, [rip + .Lx134_5]
 jmp rax
.Lx134_4:
 jmp xchain118_n5_as
.Lx134_5:
 jmp xchain118_n5_af
.Lx134_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx134_2:
 test rax, rax
 je .Lx134_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx134_7]
 lea rdx, [rip + .Lx134_8]
 jmp rax
.Lx134_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx134_2
.Lx134_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx134_2
.Lx134_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx134_6]
 sub rsp, 8
 push rax
 jmp xchain118_n5_as
.Lx134_6:
 add rsp, 16
 jmp xchain118_n5_af
 xchain118_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n10_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx135_0
 mov r8d, 0
 lea rcx, [rip + .Lx135_4]
 lea rdx, [rip + .Lx135_5]
 jmp rax
.Lx135_4:
 jmp xchain118_n5_as
.Lx135_5:
 jmp xchain118_n5_af
.Lx135_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx135_2:
 test rax, rax
 je .Lx135_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx135_7]
 lea rdx, [rip + .Lx135_8]
 jmp rax
.Lx135_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx135_2
.Lx135_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx135_2
.Lx135_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx135_6]
 sub rsp, 8
 push rax
 jmp xchain118_n5_as
.Lx135_6:
 add rsp, 16
 jmp xchain118_n5_af
 xchain118_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n11_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx136_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx136_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx136_10
.Lx136_9:
 xor eax, eax
.Lx136_10:
 test rax, rax
 jz .Lx136_0
 mov r8d, 0
 lea rcx, [rip + .Lx136_4]
 lea rdx, [rip + .Lx136_5]
 jmp rax
.Lx136_4:
 jmp xchain118_n6_as
.Lx136_5:
 jmp xchain118_n6_af
.Lx136_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx136_2:
 test rax, rax
 je .Lx136_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx136_7]
 lea rdx, [rip + .Lx136_8]
 jmp rax
.Lx136_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx136_2
.Lx136_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx136_2
.Lx136_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx136_6]
 sub rsp, 8
 push rax
 jmp xchain118_n6_as
.Lx136_6:
 add rsp, 16
 jmp xchain118_n6_af
 xchain118_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain118_n12_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx137_0
 mov r8d, 0
 lea rcx, [rip + .Lx137_4]
 lea rdx, [rip + .Lx137_5]
 jmp rax
.Lx137_4:
 jmp xchain118_n6_as
.Lx137_5:
 jmp xchain118_n6_af
.Lx137_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx137_2:
 test rax, rax
 je .Lx137_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx137_7]
 lea rdx, [rip + .Lx137_8]
 jmp rax
.Lx137_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx137_2
.Lx137_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx137_2
.Lx137_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain118_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx137_6]
 sub rsp, 8
 push rax
 jmp xchain118_n6_as
.Lx137_6:
 add rsp, 16
 jmp xchain118_n6_af
 xchain118_n12_β:
 jmp qword ptr [rsp]
proc_PAT$2_scanhit:
cmp qword ptr [rbp + 256], 1
jne 7f
mov ecx, dword ptr [rbp + 248]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$2_γ
proc_PAT$2_scanfail:
cmp qword ptr [rbp + 256], 1
jne 8f
mov eax, dword ptr [rbp + 248]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 248], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$2_attempt
8:
jmp proc_PAT$2_ω
proc_PAT$2_res:
add rsp, 8
pop rbp
proc_PAT$2_β:
jmp qword ptr [rbp + 224]
proc_PAT$2_γ:
push rbp
lea rax, [rip + proc_PAT$2_res]
push rax
mov rax, [rbp + 264]
mov rbp, [rbp + 280]
jmp rax
proc_PAT$2_ω:
mov rax, [rbp + 272]
lea rsp, [rbp + 288]
mov rbp, [rbp + 280]
jmp rax
  .globl proc_PAT$3_α
proc_PAT$3_α:
#=======================================================================================================================
    .global proc_PAT$3_α
    .global proc_PAT$3_β
    .global proc_PAT$3_γ
    .global proc_PAT$3_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 48], 0
  mov qword ptr [rsp + 56], 0
  mov qword ptr [rsp + 80], 0
  mov qword ptr [rsp + 88], 0
  mov qword ptr [rsp + 96], 0
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$3_attempt:
proc_PAT$3_α_body:
lea rax, [rip + xchain138_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain138_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain138_n1_α
 xchain138_n0_β:
 add rsp, 16
 jmp proc_PAT$3_scanfail
# IR_MATCH_ANY
 xchain138_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx142_240
 add rsp, 16
 jmp proc_PAT$3_scanfail
.Lx142_240:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 43
 je .Lx142_0
 cmp esi, 45
 je .Lx142_0
 add rsp, 16
 jmp proc_PAT$3_scanfail
.Lx142_0:
 add r14d, 1
 jmp xchain138_n2_α
 xchain138_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$3_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain138_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S3]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$3_scanhit
 xchain138_n2_β:
 sub r12, 24
 jmp xchain138_n1_β
proc_PAT$3_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$3_γ
proc_PAT$3_scanfail:
cmp qword ptr [rbp + 112], 1
jne 8f
mov eax, dword ptr [rbp + 104]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 104], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$3_attempt
8:
jmp proc_PAT$3_ω
proc_PAT$3_res:
add rsp, 8
pop rbp
proc_PAT$3_β:
jmp qword ptr [rbp + 80]
proc_PAT$3_γ:
push rbp
lea rax, [rip + proc_PAT$3_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$3_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$4_α
proc_PAT$4_α:
#=======================================================================================================================
    .global proc_PAT$4_α
    .global proc_PAT$4_β
    .global proc_PAT$4_γ
    .global proc_PAT$4_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 48], 0
  mov qword ptr [rsp + 56], 0
  mov qword ptr [rsp + 80], 0
  mov qword ptr [rsp + 88], 0
  mov qword ptr [rsp + 96], 0
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$4_attempt:
proc_PAT$4_α_body:
lea rax, [rip + xchain145_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain145_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain145_n1_α
 xchain145_n0_β:
 add rsp, 16
 jmp proc_PAT$4_scanfail
# IR_MATCH_ANY
 xchain145_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx149_240
 add rsp, 16
 jmp proc_PAT$4_scanfail
.Lx149_240:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 42
 je .Lx149_0
 cmp esi, 47
 je .Lx149_0
 add rsp, 16
 jmp proc_PAT$4_scanfail
.Lx149_0:
 add r14d, 1
 jmp xchain145_n2_α
 xchain145_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$4_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain145_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S3]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$4_scanhit
 xchain145_n2_β:
 sub r12, 24
 jmp xchain145_n1_β
proc_PAT$4_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$4_γ
proc_PAT$4_scanfail:
cmp qword ptr [rbp + 112], 1
jne 8f
mov eax, dword ptr [rbp + 104]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 104], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$4_attempt
8:
jmp proc_PAT$4_ω
proc_PAT$4_res:
add rsp, 8
pop rbp
proc_PAT$4_β:
jmp qword ptr [rbp + 80]
proc_PAT$4_γ:
push rbp
lea rax, [rip + proc_PAT$4_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$4_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$5_α
proc_PAT$5_α:
#=======================================================================================================================
    .global proc_PAT$5_α
    .global proc_PAT$5_β
    .global proc_PAT$5_γ
    .global proc_PAT$5_ω
  sub rsp, 192
  mov [rsp + 168], rcx
  mov [rsp + 176], rdx
  mov [rsp + 184], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 48], 0
  mov qword ptr [rsp + 56], 0
  mov qword ptr [rsp + 128], 0
  mov qword ptr [rsp + 136], 0
  mov qword ptr [rsp + 144], 0
  mov qword ptr [rsp + 152], rsp
mov qword ptr [rbp + 160], r8
mov dword ptr [rbp + 152], r14d
proc_PAT$5_attempt:
proc_PAT$5_α_body:
lea rax, [rip + xchain152_n2_β]
mov qword ptr [rbp + 128], rax
# IR_MATCH_CAPTURE_SAVE push
 xchain152_n0_α:
 lea rdi, [rbp + 48]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain152_n1_α
 xchain152_n0_β:
 lea rdi, [rbp + 48]
 call rt_cap_pop@PLT
 jmp proc_PAT$5_scanfail
# IR_MATCH_ALT_NARY
 xchain152_n1_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain152_n3_α
xchain152_n1_as:
 jmp xchain152_n2_α
 xchain152_n1_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain152_n3_β
 jmp xchain152_n4_β
xchain152_n1_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain152_n4_α
 jmp xchain152_n0_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain152_n2_α:
 lea rdi, [rbp + 48]
 call rt_cap_top@PLT
 lea rcx, [rip + .S3]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$5_scanhit
 xchain152_n2_β:
 sub r12, 24
 jmp xchain152_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain152_n3_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 cmp eax, 3
 jne .Lx159_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx159_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx159_10
.Lx159_9:
 xor eax, eax
.Lx159_10:
 test rax, rax
 jz .Lx159_0
 mov r8d, 0
 lea rcx, [rip + .Lx159_4]
 lea rdx, [rip + .Lx159_5]
 jmp rax
.Lx159_4:
 jmp xchain152_n1_as
.Lx159_5:
 jmp xchain152_n1_af
.Lx159_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S4]
 xor esi, esi
 call rt_defer_open@PLT
.Lx159_2:
 test rax, rax
 je .Lx159_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx159_7]
 lea rdx, [rip + .Lx159_8]
 jmp rax
.Lx159_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx159_2
.Lx159_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx159_2
.Lx159_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain152_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx159_6]
 sub rsp, 8
 push rax
 jmp xchain152_n1_as
.Lx159_6:
 add rsp, 16
 jmp xchain152_n1_af
 xchain152_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain152_n4_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx160_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx160_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx160_10
.Lx160_9:
 xor eax, eax
.Lx160_10:
 test rax, rax
 jz .Lx160_0
 mov r8d, 0
 lea rcx, [rip + .Lx160_4]
 lea rdx, [rip + .Lx160_5]
 jmp rax
.Lx160_4:
 jmp xchain152_n1_as
.Lx160_5:
 jmp xchain152_n1_af
.Lx160_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx160_2:
 test rax, rax
 je .Lx160_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx160_7]
 lea rdx, [rip + .Lx160_8]
 jmp rax
.Lx160_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx160_2
.Lx160_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx160_2
.Lx160_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain152_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx160_6]
 sub rsp, 8
 push rax
 jmp xchain152_n1_as
.Lx160_6:
 add rsp, 16
 jmp xchain152_n1_af
 xchain152_n4_β:
 jmp qword ptr [rsp]
proc_PAT$5_scanhit:
cmp qword ptr [rbp + 160], 1
jne 7f
mov ecx, dword ptr [rbp + 152]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$5_γ
proc_PAT$5_scanfail:
cmp qword ptr [rbp + 160], 1
jne 8f
mov eax, dword ptr [rbp + 152]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 152], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$5_attempt
8:
jmp proc_PAT$5_ω
proc_PAT$5_res:
add rsp, 8
pop rbp
proc_PAT$5_β:
jmp qword ptr [rbp + 128]
proc_PAT$5_γ:
push rbp
lea rax, [rip + proc_PAT$5_res]
push rax
mov rax, [rbp + 168]
mov rbp, [rbp + 184]
jmp rax
proc_PAT$5_ω:
mov rax, [rbp + 176]
lea rsp, [rbp + 192]
mov rbp, [rbp + 184]
jmp rax
  .globl proc_PAT$6_α
proc_PAT$6_α:
#=======================================================================================================================
    .global proc_PAT$6_α
    .global proc_PAT$6_β
    .global proc_PAT$6_γ
    .global proc_PAT$6_ω
  sub rsp, 160
  mov [rsp + 136], rcx
  mov [rsp + 144], rdx
  mov [rsp + 152], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 96], 0
  mov qword ptr [rsp + 104], 0
  mov qword ptr [rsp + 112], 0
  mov qword ptr [rsp + 120], rsp
mov qword ptr [rbp + 128], r8
mov dword ptr [rbp + 120], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
lea rax, [rip + xchain161_n0_β]
mov qword ptr [rbp + 96], rax
# IR_MATCH_ALT_NARY
 xchain161_n0_α:
 mov dword ptr [rbp + 16], r14d
 mov dword ptr [rbp + 20], 0
 jmp xchain161_n1_α
xchain161_n0_as:
 jmp proc_PAT$6_γ
 xchain161_n0_β:
 mov eax, dword ptr [rbp + 20]
 cmp eax, 0
 je xchain161_n1_β
 jmp xchain161_n2_β
xchain161_n0_af:
 add dword ptr [rbp + 20], 1
 mov r14d, dword ptr [rbp + 16]
 mov eax, dword ptr [rbp + 20]
 cmp eax, 1
 je xchain161_n2_α
 jmp proc_PAT$6_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain161_n1_α:
 mov rax, qword ptr [1879052544]
 mov rdx, qword ptr [1879052552]
 cmp eax, 3
 jne .Lx164_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx164_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx164_10
.Lx164_9:
 xor eax, eax
.Lx164_10:
 test rax, rax
 jz .Lx164_0
 mov r8d, 0
 lea rcx, [rip + .Lx164_4]
 lea rdx, [rip + .Lx164_5]
 jmp rax
.Lx164_4:
 jmp xchain161_n0_as
.Lx164_5:
 jmp xchain161_n0_af
.Lx164_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S5]
 xor esi, esi
 call rt_defer_open@PLT
.Lx164_2:
 test rax, rax
 je .Lx164_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx164_7]
 lea rdx, [rip + .Lx164_8]
 jmp rax
.Lx164_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx164_2
.Lx164_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx164_2
.Lx164_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain161_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx164_6]
 sub rsp, 8
 push rax
 jmp xchain161_n0_as
.Lx164_6:
 add rsp, 16
 jmp xchain161_n0_af
 xchain161_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain161_n2_α:
 mov dword ptr [rbp + 48], r14d
 jmp xchain161_n3_α
xchain161_n2_as:
 jmp xchain161_n0_as
 xchain161_n2_β:
 jmp xchain161_n5_β
xchain161_n2_af:
 jmp xchain161_n0_af
# IR_MATCH_LIT
 xchain161_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain161_n0_af
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 40
 jne xchain161_n0_af
 add r14d, 1
 jmp xchain161_n4_α
 xchain161_n3_β:
 sub r14d, 1
 jmp xchain161_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain161_n4_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx169_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx169_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx169_10
.Lx169_9:
 xor eax, eax
.Lx169_10:
 test rax, rax
 jz .Lx169_0
 mov r8d, 0
 lea rcx, [rip + .Lx169_4]
 lea rdx, [rip + .Lx169_5]
 jmp rax
.Lx169_4:
 jmp xchain161_n5_α
.Lx169_5:
 jmp xchain161_n3_β
.Lx169_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx169_2:
 test rax, rax
 je .Lx169_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx169_7]
 lea rdx, [rip + .Lx169_8]
 jmp rax
.Lx169_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx169_2
.Lx169_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx169_2
.Lx169_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain161_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx169_6]
 sub rsp, 8
 push rax
 jmp xchain161_n5_α
.Lx169_6:
 add rsp, 16
 jmp xchain161_n3_β
 xchain161_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain161_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain161_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 41
 jne xchain161_n4_β
 add r14d, 1
 jmp xchain161_n0_as
 xchain161_n5_β:
 sub r14d, 1
 jmp xchain161_n4_β
proc_PAT$6_scanhit:
cmp qword ptr [rbp + 128], 1
jne 7f
mov ecx, dword ptr [rbp + 120]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$6_γ
proc_PAT$6_scanfail:
cmp qword ptr [rbp + 128], 1
jne 8f
mov eax, dword ptr [rbp + 120]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 120], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$6_attempt
8:
jmp proc_PAT$6_ω
proc_PAT$6_res:
add rsp, 8
pop rbp
proc_PAT$6_β:
jmp qword ptr [rbp + 96]
proc_PAT$6_γ:
push rbp
lea rax, [rip + proc_PAT$6_res]
push rax
mov rax, [rbp + 136]
mov rbp, [rbp + 152]
jmp rax
proc_PAT$6_ω:
mov rax, [rbp + 144]
lea rsp, [rbp + 160]
mov rbp, [rbp + 152]
jmp rax
  .globl proc_PAT$7_α
proc_PAT$7_α:
#=======================================================================================================================
    .global proc_PAT$7_α
    .global proc_PAT$7_β
    .global proc_PAT$7_γ
    .global proc_PAT$7_ω
  sub rsp, 224
  mov [rsp + 200], rcx
  mov [rsp + 208], rdx
  mov [rsp + 216], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 96], 0
  mov qword ptr [rsp + 104], 0
  mov qword ptr [rsp + 160], 0
  mov qword ptr [rsp + 168], 0
  mov qword ptr [rsp + 176], 0
  mov qword ptr [rsp + 184], rsp
mov qword ptr [rbp + 192], r8
mov dword ptr [rbp + 184], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
lea rax, [rip + xchain172_n0_β]
mov qword ptr [rbp + 160], rax
# IR_MATCH_ALT_NARY
 xchain172_n0_α:
 mov dword ptr [rbp + 16], r14d
 mov dword ptr [rbp + 20], 0
 jmp xchain172_n1_α
xchain172_n0_as:
 jmp proc_PAT$7_γ
 xchain172_n0_β:
 mov eax, dword ptr [rbp + 20]
 cmp eax, 0
 je xchain172_n1_β
 jmp xchain172_n2_β
xchain172_n0_af:
 add dword ptr [rbp + 20], 1
 mov r14d, dword ptr [rbp + 16]
 mov eax, dword ptr [rbp + 20]
 cmp eax, 1
 je xchain172_n2_α
 jmp proc_PAT$7_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain172_n1_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain172_n3_α
xchain172_n1_as:
 jmp xchain172_n0_as
 xchain172_n1_β:
 jmp xchain172_n5_β
xchain172_n1_af:
 jmp xchain172_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain172_n2_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 cmp eax, 3
 jne .Lx177_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx177_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx177_10
.Lx177_9:
 xor eax, eax
.Lx177_10:
 test rax, rax
 jz .Lx177_0
 mov r8d, 0
 lea rcx, [rip + .Lx177_4]
 lea rdx, [rip + .Lx177_5]
 jmp rax
.Lx177_4:
 jmp xchain172_n0_as
.Lx177_5:
 jmp xchain172_n0_af
.Lx177_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx177_2:
 test rax, rax
 je .Lx177_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx177_7]
 lea rdx, [rip + .Lx177_8]
 jmp rax
.Lx177_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx177_2
.Lx177_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx177_2
.Lx177_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain172_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx177_6]
 sub rsp, 8
 push rax
 jmp xchain172_n0_as
.Lx177_6:
 add rsp, 16
 jmp xchain172_n0_af
 xchain172_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain172_n3_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx178_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx178_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx178_10
.Lx178_9:
 xor eax, eax
.Lx178_10:
 test rax, rax
 jz .Lx178_0
 mov r8d, 0
 lea rcx, [rip + .Lx178_4]
 lea rdx, [rip + .Lx178_5]
 jmp rax
.Lx178_4:
 jmp xchain172_n4_α
.Lx178_5:
 jmp xchain172_n0_af
.Lx178_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx178_2:
 test rax, rax
 je .Lx178_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx178_7]
 lea rdx, [rip + .Lx178_8]
 jmp rax
.Lx178_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx178_2
.Lx178_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx178_2
.Lx178_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain172_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx178_6]
 sub rsp, 8
 push rax
 jmp xchain172_n4_α
.Lx178_6:
 add rsp, 16
 jmp xchain172_n0_af
 xchain172_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain172_n4_α:
 lea rdi, [rbp + 96]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain172_n6_α
 xchain172_n4_β:
 lea rdi, [rbp + 96]
 call rt_cap_pop@PLT
 jmp xchain172_n3_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain172_n5_α:
 lea rdi, [rbp + 96]
 call rt_cap_top@PLT
 lea rcx, [rip + .S9]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain172_n0_as
 xchain172_n5_β:
 sub r12, 24
 jmp xchain172_n6_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain172_n6_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx183_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx183_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx183_10
.Lx183_9:
 xor eax, eax
.Lx183_10:
 test rax, rax
 jz .Lx183_0
 mov r8d, 0
 lea rcx, [rip + .Lx183_4]
 lea rdx, [rip + .Lx183_5]
 jmp rax
.Lx183_4:
 jmp xchain172_n5_α
.Lx183_5:
 jmp xchain172_n4_β
.Lx183_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx183_2:
 test rax, rax
 je .Lx183_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx183_7]
 lea rdx, [rip + .Lx183_8]
 jmp rax
.Lx183_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx183_2
.Lx183_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx183_2
.Lx183_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain172_n4_β
 mov r14d, eax
 lea rax, [rip + .Lx183_6]
 sub rsp, 8
 push rax
 jmp xchain172_n5_α
.Lx183_6:
 add rsp, 16
 jmp xchain172_n4_β
 xchain172_n6_β:
 jmp qword ptr [rsp]
proc_PAT$7_scanhit:
cmp qword ptr [rbp + 192], 1
jne 7f
mov ecx, dword ptr [rbp + 184]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$7_γ
proc_PAT$7_scanfail:
cmp qword ptr [rbp + 192], 1
jne 8f
mov eax, dword ptr [rbp + 184]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 184], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$7_attempt
8:
jmp proc_PAT$7_ω
proc_PAT$7_res:
add rsp, 8
pop rbp
proc_PAT$7_β:
jmp qword ptr [rbp + 160]
proc_PAT$7_γ:
push rbp
lea rax, [rip + proc_PAT$7_res]
push rax
mov rax, [rbp + 200]
mov rbp, [rbp + 216]
jmp rax
proc_PAT$7_ω:
mov rax, [rbp + 208]
lea rsp, [rbp + 224]
mov rbp, [rbp + 216]
jmp rax
  .globl proc_PAT$8_α
proc_PAT$8_α:
#=======================================================================================================================
    .global proc_PAT$8_α
    .global proc_PAT$8_β
    .global proc_PAT$8_γ
    .global proc_PAT$8_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 112], 0
  mov qword ptr [rsp + 120], 0
  mov qword ptr [rsp + 176], 0
  mov qword ptr [rsp + 184], 0
  mov qword ptr [rsp + 192], 0
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
lea rax, [rip + xchain184_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain184_n0_α:
 mov dword ptr [rbp + 16], r14d
 mov dword ptr [rbp + 20], 0
 jmp xchain184_n1_α
xchain184_n0_as:
 jmp proc_PAT$8_γ
 xchain184_n0_β:
 mov eax, dword ptr [rbp + 20]
 cmp eax, 0
 je xchain184_n1_β
 jmp xchain184_n2_β
xchain184_n0_af:
 add dword ptr [rbp + 20], 1
 mov r14d, dword ptr [rbp + 16]
 mov eax, dword ptr [rbp + 20]
 cmp eax, 1
 je xchain184_n2_α
 jmp proc_PAT$8_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain184_n1_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain184_n3_α
xchain184_n1_as:
 jmp xchain184_n0_as
 xchain184_n1_β:
 jmp xchain184_n6_β
xchain184_n1_af:
 jmp xchain184_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain184_n2_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx189_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx189_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx189_10
.Lx189_9:
 xor eax, eax
.Lx189_10:
 test rax, rax
 jz .Lx189_0
 mov r8d, 0
 lea rcx, [rip + .Lx189_4]
 lea rdx, [rip + .Lx189_5]
 jmp rax
.Lx189_4:
 jmp xchain184_n0_as
.Lx189_5:
 jmp xchain184_n0_af
.Lx189_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx189_2:
 test rax, rax
 je .Lx189_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx189_7]
 lea rdx, [rip + .Lx189_8]
 jmp rax
.Lx189_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx189_2
.Lx189_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx189_2
.Lx189_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain184_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx189_6]
 sub rsp, 8
 push rax
 jmp xchain184_n0_as
.Lx189_6:
 add rsp, 16
 jmp xchain184_n0_af
 xchain184_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain184_n3_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx190_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx190_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx190_10
.Lx190_9:
 xor eax, eax
.Lx190_10:
 test rax, rax
 jz .Lx190_0
 mov r8d, 0
 lea rcx, [rip + .Lx190_4]
 lea rdx, [rip + .Lx190_5]
 jmp rax
.Lx190_4:
 jmp xchain184_n4_α
.Lx190_5:
 jmp xchain184_n0_af
.Lx190_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx190_2:
 test rax, rax
 je .Lx190_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx190_7]
 lea rdx, [rip + .Lx190_8]
 jmp rax
.Lx190_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx190_2
.Lx190_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx190_2
.Lx190_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain184_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx190_6]
 sub rsp, 8
 push rax
 jmp xchain184_n4_α
.Lx190_6:
 add rsp, 16
 jmp xchain184_n0_af
 xchain184_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain184_n4_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 cmp eax, 3
 jne .Lx191_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx191_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx191_10
.Lx191_9:
 xor eax, eax
.Lx191_10:
 test rax, rax
 jz .Lx191_0
 mov r8d, 0
 lea rcx, [rip + .Lx191_4]
 lea rdx, [rip + .Lx191_5]
 jmp rax
.Lx191_4:
 jmp xchain184_n5_α
.Lx191_5:
 jmp xchain184_n3_β
.Lx191_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S11]
 xor esi, esi
 call rt_defer_open@PLT
.Lx191_2:
 test rax, rax
 je .Lx191_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx191_7]
 lea rdx, [rip + .Lx191_8]
 jmp rax
.Lx191_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx191_2
.Lx191_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx191_2
.Lx191_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain184_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx191_6]
 sub rsp, 8
 push rax
 jmp xchain184_n5_α
.Lx191_6:
 add rsp, 16
 jmp xchain184_n3_β
 xchain184_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain184_n5_α:
 lea rdi, [rbp + 112]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain184_n7_α
 xchain184_n5_β:
 lea rdi, [rbp + 112]
 call rt_cap_pop@PLT
 jmp xchain184_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain184_n6_α:
 lea rdi, [rbp + 112]
 call rt_cap_top@PLT
 lea rcx, [rip + .S12]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain184_n0_as
 xchain184_n6_β:
 sub r12, 24
 jmp xchain184_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain184_n7_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx196_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx196_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx196_10
.Lx196_9:
 xor eax, eax
.Lx196_10:
 test rax, rax
 jz .Lx196_0
 mov r8d, 0
 lea rcx, [rip + .Lx196_4]
 lea rdx, [rip + .Lx196_5]
 jmp rax
.Lx196_4:
 jmp xchain184_n6_α
.Lx196_5:
 jmp xchain184_n5_β
.Lx196_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx196_2:
 test rax, rax
 je .Lx196_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx196_7]
 lea rdx, [rip + .Lx196_8]
 jmp rax
.Lx196_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx196_2
.Lx196_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx196_2
.Lx196_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain184_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx196_6]
 sub rsp, 8
 push rax
 jmp xchain184_n6_α
.Lx196_6:
 add rsp, 16
 jmp xchain184_n5_β
 xchain184_n7_β:
 jmp qword ptr [rsp]
proc_PAT$8_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$8_γ
proc_PAT$8_scanfail:
cmp qword ptr [rbp + 208], 1
jne 8f
mov eax, dword ptr [rbp + 200]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 200], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$8_attempt
8:
jmp proc_PAT$8_ω
proc_PAT$8_res:
add rsp, 8
pop rbp
proc_PAT$8_β:
jmp qword ptr [rbp + 176]
proc_PAT$8_γ:
push rbp
lea rax, [rip + proc_PAT$8_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$8_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$9_α
proc_PAT$9_α:
#=======================================================================================================================
    .global proc_PAT$9_α
    .global proc_PAT$9_β
    .global proc_PAT$9_γ
    .global proc_PAT$9_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 112], 0
  mov qword ptr [rsp + 120], 0
  mov qword ptr [rsp + 176], 0
  mov qword ptr [rsp + 184], 0
  mov qword ptr [rsp + 192], 0
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
lea rax, [rip + xchain197_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain197_n0_α:
 mov dword ptr [rbp + 16], r14d
 mov dword ptr [rbp + 20], 0
 jmp xchain197_n1_α
xchain197_n0_as:
 jmp proc_PAT$9_γ
 xchain197_n0_β:
 mov eax, dword ptr [rbp + 20]
 cmp eax, 0
 je xchain197_n1_β
 jmp xchain197_n2_β
xchain197_n0_af:
 add dword ptr [rbp + 20], 1
 mov r14d, dword ptr [rbp + 16]
 mov eax, dword ptr [rbp + 20]
 cmp eax, 1
 je xchain197_n2_α
 jmp proc_PAT$9_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain197_n1_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain197_n3_α
xchain197_n1_as:
 jmp xchain197_n0_as
 xchain197_n1_β:
 jmp xchain197_n6_β
xchain197_n1_af:
 jmp xchain197_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain197_n2_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx202_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx202_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx202_10
.Lx202_9:
 xor eax, eax
.Lx202_10:
 test rax, rax
 jz .Lx202_0
 mov r8d, 0
 lea rcx, [rip + .Lx202_4]
 lea rdx, [rip + .Lx202_5]
 jmp rax
.Lx202_4:
 jmp xchain197_n0_as
.Lx202_5:
 jmp xchain197_n0_af
.Lx202_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx202_2:
 test rax, rax
 je .Lx202_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx202_7]
 lea rdx, [rip + .Lx202_8]
 jmp rax
.Lx202_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx202_2
.Lx202_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx202_2
.Lx202_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain197_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx202_6]
 sub rsp, 8
 push rax
 jmp xchain197_n0_as
.Lx202_6:
 add rsp, 16
 jmp xchain197_n0_af
 xchain197_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain197_n3_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx203_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx203_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx203_10
.Lx203_9:
 xor eax, eax
.Lx203_10:
 test rax, rax
 jz .Lx203_0
 mov r8d, 0
 lea rcx, [rip + .Lx203_4]
 lea rdx, [rip + .Lx203_5]
 jmp rax
.Lx203_4:
 jmp xchain197_n4_α
.Lx203_5:
 jmp xchain197_n0_af
.Lx203_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx203_2:
 test rax, rax
 je .Lx203_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx203_7]
 lea rdx, [rip + .Lx203_8]
 jmp rax
.Lx203_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx203_2
.Lx203_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx203_2
.Lx203_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain197_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx203_6]
 sub rsp, 8
 push rax
 jmp xchain197_n4_α
.Lx203_6:
 add rsp, 16
 jmp xchain197_n0_af
 xchain197_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain197_n4_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx204_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx204_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx204_10
.Lx204_9:
 xor eax, eax
.Lx204_10:
 test rax, rax
 jz .Lx204_0
 mov r8d, 0
 lea rcx, [rip + .Lx204_4]
 lea rdx, [rip + .Lx204_5]
 jmp rax
.Lx204_4:
 jmp xchain197_n5_α
.Lx204_5:
 jmp xchain197_n3_β
.Lx204_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx204_2:
 test rax, rax
 je .Lx204_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx204_7]
 lea rdx, [rip + .Lx204_8]
 jmp rax
.Lx204_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx204_2
.Lx204_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx204_2
.Lx204_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain197_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx204_6]
 sub rsp, 8
 push rax
 jmp xchain197_n5_α
.Lx204_6:
 add rsp, 16
 jmp xchain197_n3_β
 xchain197_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain197_n5_α:
 lea rdi, [rbp + 112]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain197_n7_α
 xchain197_n5_β:
 lea rdi, [rbp + 112]
 call rt_cap_pop@PLT
 jmp xchain197_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain197_n6_α:
 lea rdi, [rbp + 112]
 call rt_cap_top@PLT
 lea rcx, [rip + .S12]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain197_n0_as
 xchain197_n6_β:
 sub r12, 24
 jmp xchain197_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain197_n7_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx209_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx209_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx209_10
.Lx209_9:
 xor eax, eax
.Lx209_10:
 test rax, rax
 jz .Lx209_0
 mov r8d, 0
 lea rcx, [rip + .Lx209_4]
 lea rdx, [rip + .Lx209_5]
 jmp rax
.Lx209_4:
 jmp xchain197_n6_α
.Lx209_5:
 jmp xchain197_n5_β
.Lx209_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx209_2:
 test rax, rax
 je .Lx209_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx209_7]
 lea rdx, [rip + .Lx209_8]
 jmp rax
.Lx209_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx209_2
.Lx209_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx209_2
.Lx209_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain197_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx209_6]
 sub rsp, 8
 push rax
 jmp xchain197_n6_α
.Lx209_6:
 add rsp, 16
 jmp xchain197_n5_β
 xchain197_n7_β:
 jmp qword ptr [rsp]
proc_PAT$9_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$9_γ
proc_PAT$9_scanfail:
cmp qword ptr [rbp + 208], 1
jne 8f
mov eax, dword ptr [rbp + 200]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 200], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$9_attempt
8:
jmp proc_PAT$9_ω
proc_PAT$9_res:
add rsp, 8
pop rbp
proc_PAT$9_β:
jmp qword ptr [rbp + 176]
proc_PAT$9_γ:
push rbp
lea rax, [rip + proc_PAT$9_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$9_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "Push"
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
  lea rsi, [rip + proc_Push_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 720
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname1: .string "Pop"
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
  lea rsi, [rip + proc_Pop_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 608
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname2: .string "Unary"
  .Lstartup_pp2_0: .string "arg"
  .Lstartup_pp2_1: .string "op"
  .align 8
  .Lstartup_pnames2:
  .quad .Lstartup_pp2_0
  .quad .Lstartup_pp2_1
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + .Lstartup_pnames2]
  mov edx, 2
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + proc_Unary_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 2
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 576
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname3: .string "Binary"
  .Lstartup_pp3_0: .string "op"
  .Lstartup_pp3_1: .string "left"
  .Lstartup_pp3_2: .string "right"
  .align 8
  .Lstartup_pnames3:
  .quad .Lstartup_pp3_0
  .quad .Lstartup_pp3_1
  .quad .Lstartup_pp3_2
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname3]
  lea rsi, [rip + .Lstartup_pnames3]
  mov edx, 3
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname3]
  lea rsi, [rip + proc_Binary_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 3
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 816
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname4: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 64
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_zstatic@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname5: .string "PAT$1"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + proc_PAT$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 128
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname6: .string "PAT$2"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + proc_PAT$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 256
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname7: .string "PAT$3"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + proc_PAT$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_zstatic@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname8: .string "PAT$4"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + proc_PAT$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_zstatic@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname9: .string "PAT$5"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + proc_PAT$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 160
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname10: .string "PAT$6"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + proc_PAT$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 128
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname11: .string "PAT$7"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + proc_PAT$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 192
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname12: .string "PAT$8"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + proc_PAT$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname13: .string "PAT$9"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + proc_PAT$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "Push"
  .Lgvan1: .string "x"
  .Lgvan2: .string "Pop"
  .Lgvan3: .string "Unary"
  .Lgvan4: .string "arg"
  .Lgvan5: .string "op"
  .Lgvan6: .string "Binary"
  .Lgvan7: .string "left"
  .Lgvan8: .string "right"
  .Lgvan9: .string "stk"
  .Lgvan10: .string "dummy"
  .Lgvan11: .string "integer"
  .Lgvan12: .string "exponent"
  .Lgvan13: .string "real"
  .Lgvan14: .string "addop"
  .Lgvan15: .string "mulop"
  .Lgvan16: .string "constant"
  .Lgvan17: .string "primary"
  .Lgvan18: .string "factor"
  .Lgvan19: .string "term"
  .Lgvan20: .string "expr"
  .Lgvan21: .string "line"
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
  .quad .Lgvan8
  .quad .Lgvan9
  .quad .Lgvan10
  .quad .Lgvan11
  .quad .Lgvan12
  .quad .Lgvan13
  .quad .Lgvan14
  .quad .Lgvan15
  .quad .Lgvan16
  .quad .Lgvan17
  .quad .Lgvan18
  .quad .Lgvan19
  .quad .Lgvan20
  .quad .Lgvan21
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 22
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 22
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
  mov qword ptr [rsp + 3864], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
 xchain210_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn212: .string "TABLE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn212]
 lea rsi, [rbp + 128]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain210_n2_α
 jmp xchain210_n1_α
 xchain210_n0_β:
 jmp xchain210_n2_α
# IR_ASSIGN gva
 xchain210_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052432], rax
 mov qword ptr [1879052440], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain210_n2_α
# IR_LIT_STRING
 xchain210_n2_α:
 mov qword ptr [rbp + 2480], 1
 mov rax, qword ptr [rip + .Lx214_0]
 mov qword ptr [rbp + 2488], rax
 jmp xchain210_n3_α
.Lx214_0:
 .quad .Lx214_0_s
.Lx214_0_s:
 .string "PAT$0"
 xchain210_n3_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2480] -> [zr+2448]
 mov rax, qword ptr [rbp + 2480]
 mov qword ptr [rbp + 2448], rax
 mov rax, qword ptr [rbp + 2488]
 mov qword ptr [rbp + 2456], rax
  .section .rodata
  .Lrkfn216: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn216]
 lea rsi, [rbp + 2448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2432], rax
 mov qword ptr [rbp + 2440], rdx
 cmp eax, 99
 je xchain210_n5_α
 jmp xchain210_n4_α
 xchain210_n3_β:
 jmp xchain210_n5_α
# IR_ASSIGN gva
 xchain210_n4_α:
 mov rax, qword ptr [rbp + 2432]
 mov rdx, qword ptr [rbp + 2440]
 mov qword ptr [1879052464], rax
 mov qword ptr [1879052472], rdx
 mov qword ptr [rbp + 2416], rax
 mov qword ptr [rbp + 2424], rdx
 jmp xchain210_n5_α
# IR_LIT_STRING
 xchain210_n5_α:
 mov qword ptr [rbp + 2576], 1
 mov rax, qword ptr [rip + .Lx218_0]
 mov qword ptr [rbp + 2584], rax
 jmp xchain210_n6_α
.Lx218_0:
 .quad .Lx218_0_s
.Lx218_0_s:
 .string "PAT$1"
 xchain210_n6_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2576] -> [zr+2544]
 mov rax, qword ptr [rbp + 2576]
 mov qword ptr [rbp + 2544], rax
 mov rax, qword ptr [rbp + 2584]
 mov qword ptr [rbp + 2552], rax
  .section .rodata
  .Lrkfn220: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn220]
 lea rsi, [rbp + 2544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2528], rax
 mov qword ptr [rbp + 2536], rdx
 cmp eax, 99
 je xchain210_n8_α
 jmp xchain210_n7_α
 xchain210_n6_β:
 jmp xchain210_n8_α
# IR_ASSIGN gva
 xchain210_n7_α:
 mov rax, qword ptr [rbp + 2528]
 mov rdx, qword ptr [rbp + 2536]
 mov qword ptr [1879052480], rax
 mov qword ptr [1879052488], rdx
 mov qword ptr [rbp + 2512], rax
 mov qword ptr [rbp + 2520], rdx
 jmp xchain210_n8_α
# IR_LIT_STRING
 xchain210_n8_α:
 mov qword ptr [rbp + 2672], 1
 mov rax, qword ptr [rip + .Lx222_0]
 mov qword ptr [rbp + 2680], rax
 jmp xchain210_n9_α
.Lx222_0:
 .quad .Lx222_0_s
.Lx222_0_s:
 .string "PAT$2"
 xchain210_n9_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2672] -> [zr+2640]
 mov rax, qword ptr [rbp + 2672]
 mov qword ptr [rbp + 2640], rax
 mov rax, qword ptr [rbp + 2680]
 mov qword ptr [rbp + 2648], rax
  .section .rodata
  .Lrkfn224: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn224]
 lea rsi, [rbp + 2640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2624], rax
 mov qword ptr [rbp + 2632], rdx
 cmp eax, 99
 je xchain210_n11_α
 jmp xchain210_n10_α
 xchain210_n9_β:
 jmp xchain210_n11_α
# IR_ASSIGN gva
 xchain210_n10_α:
 mov rax, qword ptr [rbp + 2624]
 mov rdx, qword ptr [rbp + 2632]
 mov qword ptr [1879052496], rax
 mov qword ptr [1879052504], rdx
 mov qword ptr [rbp + 2608], rax
 mov qword ptr [rbp + 2616], rdx
 jmp xchain210_n11_α
# IR_LIT_STRING
 xchain210_n11_α:
 mov qword ptr [rbp + 2768], 1
 mov rax, qword ptr [rip + .Lx226_0]
 mov qword ptr [rbp + 2776], rax
 jmp xchain210_n12_α
.Lx226_0:
 .quad .Lx226_0_s
.Lx226_0_s:
 .string "PAT$3"
 xchain210_n12_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2768] -> [zr+2736]
 mov rax, qword ptr [rbp + 2768]
 mov qword ptr [rbp + 2736], rax
 mov rax, qword ptr [rbp + 2776]
 mov qword ptr [rbp + 2744], rax
  .section .rodata
  .Lrkfn228: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn228]
 lea rsi, [rbp + 2736]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2720], rax
 mov qword ptr [rbp + 2728], rdx
 cmp eax, 99
 je xchain210_n14_α
 jmp xchain210_n13_α
 xchain210_n12_β:
 jmp xchain210_n14_α
# IR_ASSIGN gva
 xchain210_n13_α:
 mov rax, qword ptr [rbp + 2720]
 mov rdx, qword ptr [rbp + 2728]
 mov qword ptr [1879052512], rax
 mov qword ptr [1879052520], rdx
 mov qword ptr [rbp + 2704], rax
 mov qword ptr [rbp + 2712], rdx
 jmp xchain210_n14_α
# IR_LIT_STRING
 xchain210_n14_α:
 mov qword ptr [rbp + 2864], 1
 mov rax, qword ptr [rip + .Lx230_0]
 mov qword ptr [rbp + 2872], rax
 jmp xchain210_n15_α
.Lx230_0:
 .quad .Lx230_0_s
.Lx230_0_s:
 .string "PAT$4"
 xchain210_n15_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2864] -> [zr+2832]
 mov rax, qword ptr [rbp + 2864]
 mov qword ptr [rbp + 2832], rax
 mov rax, qword ptr [rbp + 2872]
 mov qword ptr [rbp + 2840], rax
  .section .rodata
  .Lrkfn232: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn232]
 lea rsi, [rbp + 2832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2816], rax
 mov qword ptr [rbp + 2824], rdx
 cmp eax, 99
 je xchain210_n17_α
 jmp xchain210_n16_α
 xchain210_n15_β:
 jmp xchain210_n17_α
# IR_ASSIGN gva
 xchain210_n16_α:
 mov rax, qword ptr [rbp + 2816]
 mov rdx, qword ptr [rbp + 2824]
 mov qword ptr [1879052528], rax
 mov qword ptr [1879052536], rdx
 mov qword ptr [rbp + 2800], rax
 mov qword ptr [rbp + 2808], rdx
 jmp xchain210_n17_α
# IR_LIT_STRING
 xchain210_n17_α:
 mov qword ptr [rbp + 2960], 1
 mov rax, qword ptr [rip + .Lx234_0]
 mov qword ptr [rbp + 2968], rax
 jmp xchain210_n18_α
.Lx234_0:
 .quad .Lx234_0_s
.Lx234_0_s:
 .string "PAT$5"
 xchain210_n18_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2960] -> [zr+2928]
 mov rax, qword ptr [rbp + 2960]
 mov qword ptr [rbp + 2928], rax
 mov rax, qword ptr [rbp + 2968]
 mov qword ptr [rbp + 2936], rax
  .section .rodata
  .Lrkfn236: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn236]
 lea rsi, [rbp + 2928]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2912], rax
 mov qword ptr [rbp + 2920], rdx
 cmp eax, 99
 je xchain210_n20_α
 jmp xchain210_n19_α
 xchain210_n18_β:
 jmp xchain210_n20_α
# IR_ASSIGN gva
 xchain210_n19_α:
 mov rax, qword ptr [rbp + 2912]
 mov rdx, qword ptr [rbp + 2920]
 mov qword ptr [1879052544], rax
 mov qword ptr [1879052552], rdx
 mov qword ptr [rbp + 2896], rax
 mov qword ptr [rbp + 2904], rdx
 jmp xchain210_n20_α
# IR_LIT_STRING
 xchain210_n20_α:
 mov qword ptr [rbp + 3056], 1
 mov rax, qword ptr [rip + .Lx238_0]
 mov qword ptr [rbp + 3064], rax
 jmp xchain210_n21_α
.Lx238_0:
 .quad .Lx238_0_s
.Lx238_0_s:
 .string "PAT$6"
 xchain210_n21_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3056] -> [zr+3024]
 mov rax, qword ptr [rbp + 3056]
 mov qword ptr [rbp + 3024], rax
 mov rax, qword ptr [rbp + 3064]
 mov qword ptr [rbp + 3032], rax
  .section .rodata
  .Lrkfn240: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn240]
 lea rsi, [rbp + 3024]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3008], rax
 mov qword ptr [rbp + 3016], rdx
 cmp eax, 99
 je xchain210_n23_α
 jmp xchain210_n22_α
 xchain210_n21_β:
 jmp xchain210_n23_α
# IR_ASSIGN gva
 xchain210_n22_α:
 mov rax, qword ptr [rbp + 3008]
 mov rdx, qword ptr [rbp + 3016]
 mov qword ptr [1879052560], rax
 mov qword ptr [1879052568], rdx
 mov qword ptr [rbp + 2992], rax
 mov qword ptr [rbp + 3000], rdx
 jmp xchain210_n23_α
# IR_LIT_STRING
 xchain210_n23_α:
 mov qword ptr [rbp + 3152], 1
 mov rax, qword ptr [rip + .Lx242_0]
 mov qword ptr [rbp + 3160], rax
 jmp xchain210_n24_α
.Lx242_0:
 .quad .Lx242_0_s
.Lx242_0_s:
 .string "PAT$7"
 xchain210_n24_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3152] -> [zr+3120]
 mov rax, qword ptr [rbp + 3152]
 mov qword ptr [rbp + 3120], rax
 mov rax, qword ptr [rbp + 3160]
 mov qword ptr [rbp + 3128], rax
  .section .rodata
  .Lrkfn244: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn244]
 lea rsi, [rbp + 3120]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3104], rax
 mov qword ptr [rbp + 3112], rdx
 cmp eax, 99
 je xchain210_n26_α
 jmp xchain210_n25_α
 xchain210_n24_β:
 jmp xchain210_n26_α
# IR_ASSIGN gva
 xchain210_n25_α:
 mov rax, qword ptr [rbp + 3104]
 mov rdx, qword ptr [rbp + 3112]
 mov qword ptr [1879052576], rax
 mov qword ptr [1879052584], rdx
 mov qword ptr [rbp + 3088], rax
 mov qword ptr [rbp + 3096], rdx
 jmp xchain210_n26_α
# IR_LIT_STRING
 xchain210_n26_α:
 mov qword ptr [rbp + 3248], 1
 mov rax, qword ptr [rip + .Lx246_0]
 mov qword ptr [rbp + 3256], rax
 jmp xchain210_n27_α
.Lx246_0:
 .quad .Lx246_0_s
.Lx246_0_s:
 .string "PAT$8"
 xchain210_n27_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3248] -> [zr+3216]
 mov rax, qword ptr [rbp + 3248]
 mov qword ptr [rbp + 3216], rax
 mov rax, qword ptr [rbp + 3256]
 mov qword ptr [rbp + 3224], rax
  .section .rodata
  .Lrkfn248: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn248]
 lea rsi, [rbp + 3216]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3200], rax
 mov qword ptr [rbp + 3208], rdx
 cmp eax, 99
 je xchain210_n29_α
 jmp xchain210_n28_α
 xchain210_n27_β:
 jmp xchain210_n29_α
# IR_ASSIGN gva
 xchain210_n28_α:
 mov rax, qword ptr [rbp + 3200]
 mov rdx, qword ptr [rbp + 3208]
 mov qword ptr [1879052592], rax
 mov qword ptr [1879052600], rdx
 mov qword ptr [rbp + 3184], rax
 mov qword ptr [rbp + 3192], rdx
 jmp xchain210_n29_α
# IR_LIT_STRING
 xchain210_n29_α:
 mov qword ptr [rbp + 3344], 1
 mov rax, qword ptr [rip + .Lx250_0]
 mov qword ptr [rbp + 3352], rax
 jmp xchain210_n30_α
.Lx250_0:
 .quad .Lx250_0_s
.Lx250_0_s:
 .string "PAT$9"
 xchain210_n30_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3344] -> [zr+3312]
 mov rax, qword ptr [rbp + 3344]
 mov qword ptr [rbp + 3312], rax
 mov rax, qword ptr [rbp + 3352]
 mov qword ptr [rbp + 3320], rax
  .section .rodata
  .Lrkfn252: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn252]
 lea rsi, [rbp + 3312]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3296], rax
 mov qword ptr [rbp + 3304], rdx
 cmp eax, 99
 je xchain210_n32_α
 jmp xchain210_n31_α
 xchain210_n30_β:
 jmp xchain210_n32_α
# IR_ASSIGN gva
 xchain210_n31_α:
 mov rax, qword ptr [rbp + 3296]
 mov rdx, qword ptr [rbp + 3304]
 mov qword ptr [1879052608], rax
 mov qword ptr [1879052616], rdx
 mov qword ptr [rbp + 3280], rax
 mov qword ptr [rbp + 3288], rdx
 jmp xchain210_n32_α
# IR_LIT_STRING
 xchain210_n32_α:
 mov qword ptr [rbp + 3440], 1
 mov rax, qword ptr [rip + .Lx254_0]
 mov qword ptr [rbp + 3448], rax
 jmp xchain210_n33_α
.Lx254_0:
 .quad .Lx254_0_s
.Lx254_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain210_n33_α:
 mov qword ptr [rbp + 3472], 6
 mov rax, qword ptr [rip + .Lx255_0]
 mov qword ptr [rbp + 3480], rax
 jmp xchain210_n34_α
.Lx255_0:
 .quad 1
 xchain210_n34_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3440] -> [zr+3392]
 mov rax, qword ptr [rbp + 3440]
 mov qword ptr [rbp + 3392], rax
 mov rax, qword ptr [rbp + 3448]
 mov qword ptr [rbp + 3400], rax
# marshal arg1 = producer-box slot [zr+3472] -> [zr+3408]
 mov rax, qword ptr [rbp + 3472]
 mov qword ptr [rbp + 3408], rax
 mov rax, qword ptr [rbp + 3480]
 mov qword ptr [rbp + 3416], rax
  .section .rodata
  .Lrkfn257: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn257]
 lea rsi, [rbp + 3392]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3376], rax
 mov qword ptr [rbp + 3384], rdx
 cmp eax, 99
 je xchain210_n35_α
 jmp xchain210_n35_α
 xchain210_n34_β:
 jmp xchain210_n35_α
# IR_VAR
 xchain210_n35_α:
 mov rdi, qword ptr [rip + .Lx258_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 3520], rax
 mov qword ptr [rbp + 3528], rdx
 jmp xchain210_n36_α
.Lx258_0:
 .quad .Lx258_0_s
.Lx258_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain210_n36_α:
 mov rax, qword ptr [rbp + 3520]
 mov rdx, qword ptr [rbp + 3528]
 mov qword ptr [1879052624], rax
 mov qword ptr [1879052632], rdx
 mov qword ptr [rbp + 3504], rax
 mov qword ptr [rbp + 3512], rdx
 jmp xchain210_n37_α
# IR_VAR
 xchain210_n37_α:
 mov rax, qword ptr [1879052624]
 mov rdx, qword ptr [1879052632]
 mov qword ptr [rbp + 3728], rax
 mov qword ptr [rbp + 3736], rdx
 jmp xchain210_n38_α
# IR_MATCH_HEAD
 xchain210_n38_α:
 mov qword ptr [rbp + 3608], rbp
 mov rdi, qword ptr [rbp + 3728]
 mov rsi, qword ptr [rbp + 3736]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 3600], r12
 mov qword ptr [rbp + 3584], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 3576], rax
 mov dword ptr [rbp + 3568], 0
.Lx262_0:
 mov r14d, dword ptr [rbp + 3568]
 jmp xchain210_n40_α
 xchain210_n38_β:
 add dword ptr [rbp + 3568], 1
 mov eax, dword ptr [rbp + 3568]
 cmp eax, r15d
 jg .Lx262_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx262_1
 jmp .Lx262_0
.Lx262_1:
 mov rax, qword ptr [rbp + 3576]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 3584]
 mov r12, qword ptr [rbp + 3600]
 mov rbp, qword ptr [rbp + 3608]
 jmp xchain210_n39_α
# IR_LIT_STRING
 xchain210_n39_α:
 mov qword ptr [rbp + 3824], 1
 mov rax, qword ptr [rip + .Lx263_0]
 mov qword ptr [rbp + 3832], rax
 jmp xchain210_n41_α
.Lx263_0:
 .quad .Lx263_0_s
.Lx263_0_s:
 .string "Bad input, try again"
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain210_n40_α:
 mov dword ptr [rbp + 3616], r14d
 jmp xchain210_n43_α
xchain210_n40_as:
 jmp xchain210_n42_α
 xchain210_n40_β:
 jmp xchain210_n47_β
xchain210_n40_af:
 jmp xchain210_n38_β
# IR_ASSIGN global
 xchain210_n41_α:
 mov rsi, qword ptr [rbp + 3824]
 mov rdx, qword ptr [rbp + 3832]
 mov rdi, qword ptr [rip + .Lx266_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 3808], rax
 mov qword ptr [rbp + 3816], rdx
 jmp xchain210_n35_α
.Lx266_0:
 .quad .Lx266_0_s
.Lx266_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain210_n42_α:
 mov rax, qword ptr [rbp + 3576]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 3584]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 3632]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx268_1:
 test rax, rax
 je .Lx268_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx268_3]
 lea rdx, [rip + .Lx268_4]
 jmp rax
.Lx268_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx268_1
.Lx268_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx268_1
.Lx268_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 3600]
 mov rbp, qword ptr [rbp + 3608]
 jmp xchain210_n48_α
# IR_LIT_INTEGER
 xchain210_n43_α:
 mov qword ptr [rbp + 3648], 6
 mov rax, qword ptr [rip + .Lx269_0]
 mov qword ptr [rbp + 3656], rax
 jmp xchain210_n44_α
 xchain210_n43_β:
 jmp xchain210_n38_β
.Lx269_0:
 .quad 0
# IR_MATCH_POS
 xchain210_n44_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain210_n38_β
 jmp xchain210_n45_α
 xchain210_n44_β:
 jmp xchain210_n38_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain210_n45_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx271_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx271_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx271_10
.Lx271_9:
 xor eax, eax
.Lx271_10:
 test rax, rax
 jz .Lx271_0
 mov r8d, 0
 lea rcx, [rip + .Lx271_4]
 lea rdx, [rip + .Lx271_5]
 jmp rax
.Lx271_4:
 jmp xchain210_n46_α
.Lx271_5:
 jmp xchain210_n38_β
.Lx271_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx271_2:
 test rax, rax
 je .Lx271_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx271_7]
 lea rdx, [rip + .Lx271_8]
 jmp rax
.Lx271_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx271_2
.Lx271_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx271_2
.Lx271_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain210_n38_β
 mov r14d, eax
 lea rax, [rip + .Lx271_6]
 sub rsp, 8
 push rax
 jmp xchain210_n46_α
.Lx271_6:
 add rsp, 16
 jmp xchain210_n38_β
 xchain210_n45_β:
 jmp qword ptr [rsp]
# IR_LIT_INTEGER
 xchain210_n46_α:
 mov qword ptr [rbp + 3696], 6
 mov rax, qword ptr [rip + .Lx272_0]
 mov qword ptr [rbp + 3704], rax
 jmp xchain210_n47_α
 xchain210_n46_β:
 jmp xchain210_n45_β
.Lx272_0:
 .quad 0
# IR_MATCH_RPOS
 xchain210_n47_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain210_n45_β
 jmp xchain210_n42_α
 xchain210_n47_β:
 jmp xchain210_n45_β
 xchain210_n48_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx275_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx275_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx275_6]
 lea rdx, [rip + .Lx275_7]
 jmp rax
.Lx275_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx275_2
.Lx275_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx275_2
.Lx275_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx275_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx275_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx275_3]
 lea rdx, [rip + .Lx275_4]
 jmp rax
.Lx275_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx275_2
.Lx275_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx275_2
.Lx275_1:
 call rt_faildescr@PLT
.Lx275_2:
 mov qword ptr [rbp + 3776], rax
 mov qword ptr [rbp + 3784], rdx
 cmp eax, 99
 je xchain210_n35_α
 jmp xchain210_n49_α
 xchain210_n48_β:
 jmp xchain210_n35_α
.Lx275_0:
 .quad .Lx275_0_s
.Lx275_0_s:
 .string "Pop"
# IR_ASSIGN global
 xchain210_n49_α:
 mov rsi, qword ptr [rbp + 3776]
 mov rdx, qword ptr [rbp + 3784]
 mov rdi, qword ptr [rip + .Lx276_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 3760], rax
 mov qword ptr [rbp + 3768], rdx
 jmp xchain210_n35_α
.Lx276_0:
 .quad .Lx276_0_s
.Lx276_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 3864]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 3864]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "integer"
.S1: .string "epsilon"
.S2: .string "exponent"
.S3: .string "*Push"
.S4: .string "real"
.S5: .string "constant"
.S6: .string "expr"
.S7: .string "primary"
.S8: .string "addop"
.S9: .string "*Unary"
.S10: .string "factor"
.S11: .string "mulop"
.S12: .string "*Binary"
.S13: .string "term"
.text
.section .rodata
.C0:
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.C1:
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.text
