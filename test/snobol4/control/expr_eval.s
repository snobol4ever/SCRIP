  .intel_syntax noprefix
  .text
  .globl proc_Push_α
proc_Push_α:
#=======================================================================================================================
    .global proc_Push_α
    .global proc_Push_β
    .global proc_Push_γ
    .global proc_Push_ω
  sub rsp, 3952
  mov [rsp + 3928], rcx
  mov [rsp + 3936], rdx
  mov [rsp + 3944], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 3920
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 3912], rsp
proc_Push_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 144], rax
 mov qword ptr [rbp + 152], rdx
 jmp xchain0_n1_α
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 184], rax
 jmp xchain0_n3_α
.Lx2_0:
 .quad 0
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain0_n4_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n3_α:
 mov rdi, qword ptr [rbp + 144]
 mov rsi, qword ptr [rbp + 152]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 480], rax
 mov qword ptr [rbp + 488], rdx
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp xchain0_n8_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rbp + 512], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n11_α
.Lx8_0:
 .quad 0
 xchain0_n8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [rbp + 672]
 mov qword ptr [rbp + 640], rax
 mov rax, qword ptr [rbp + 680]
 mov qword ptr [rbp + 648], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn10]
 lea rsi, [rbp + 640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
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
 mov qword ptr [rbp + 304], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 312], rax
 jmp xchain0_n14_α
.Lx12_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [rbp + 480]
 mov rsi, qword ptr [rbp + 488]
 mov rdx, qword ptr [rbp + 512]
 mov rcx, qword ptr [rbp + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp xchain0_n15_α
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
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
 mov rdi, qword ptr [rbp + 272]
 mov rsi, qword ptr [rbp + 280]
 mov rdx, qword ptr [rbp + 304]
 mov rcx, qword ptr [rbp + 312]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain0_n17_α
# IR_DEREF variable -> value
 xchain0_n15_α:
 mov rdi, qword ptr [rbp + 544]
 mov rsi, qword ptr [rbp + 552]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp xchain0_n18_α
# IR_ASSIGN_VAR
 xchain0_n16_α:
 mov rdi, qword ptr [rbp + 624]
 mov rsi, qword ptr [rbp + 632]
 mov rdx, qword ptr [rbp + 704]
 mov rcx, qword ptr [rbp + 712]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [rbp + 736], rax
 mov qword ptr [rbp + 744], rdx
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n17_α:
 mov rdi, qword ptr [rbp + 336]
 mov rsi, qword ptr [rbp + 344]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n19_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [rbp + 448]
 mov rsi, qword ptr [rbp + 456]
 mov rdx, qword ptr [rbp + 576]
 mov rcx, qword ptr [rbp + 584]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain0_n20_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rbp + 384], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 392], rax
 jmp xchain0_n21_α
.Lx22_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain0_n5_α
 xchain0_n21_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 368]
 cmp eax, 100
 je .Lx24_0
 mov eax, dword ptr [rbp + 368]
 cmp eax, 6
 jne .Lx24_2
.Lx24_1:
 mov rax, qword ptr [rbp + 376]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rbp + 240], 6
 mov qword ptr [rbp + 248], rax
 jmp xchain0_n22_α
.Lx24_0:
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 mov rdx, qword ptr [rbp + 384]
 mov rcx, qword ptr [rbp + 392]
 mov r8d, 0
 lea r9, [rbp + 240]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain0_n22_α
.Lx24_2:
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 mov rdx, qword ptr [rbp + 384]
 mov rcx, qword ptr [rbp + 392]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n2_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [rbp + 208]
 mov rsi, qword ptr [rbp + 216]
 mov rdx, qword ptr [rbp + 240]
 mov rcx, qword ptr [rbp + 248]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n2_α
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 jmp xchain0_n2_α
proc_Push_res:
add rsp, 8
pop rbp
proc_Push_β:
jmp proc_Push_ω
proc_Push_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 3928]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
proc_Push_ω:
mov rax, [rbp + 3936]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
  .globl proc_Pop_α
proc_Pop_α:
#=======================================================================================================================
    .global proc_Pop_α
    .global proc_Pop_β
    .global proc_Pop_γ
    .global proc_Pop_ω
  sub rsp, 3952
  mov [rsp + 3928], rcx
  mov [rsp + 3936], rdx
  mov [rsp + 3944], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 3920
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 3912], rsp
proc_Pop_α_body:
# IR_VAR
 xchain26_n0_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 768], rax
 mov qword ptr [rbp + 776], rdx
 jmp xchain26_n1_α
# IR_VAR
 xchain26_n1_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 jmp xchain26_n3_α
# IR_VAR
 xchain26_n2_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 960], rax
 mov qword ptr [rbp + 968], rdx
 jmp xchain26_n4_α
# IR_LIT_INTEGER
 xchain26_n3_α:
 mov qword ptr [rbp + 832], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 840], rax
 jmp xchain26_n5_α
.Lx30_0:
 .quad 0
# IR_LIT_INTEGER
 xchain26_n4_α:
 mov qword ptr [rbp + 992], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rbp + 1000], rax
 jmp xchain26_n6_α
.Lx31_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain26_n5_α:
 mov rdi, qword ptr [rbp + 800]
 mov rsi, qword ptr [rbp + 808]
 mov rdx, qword ptr [rbp + 832]
 mov rcx, qword ptr [rbp + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 864], rax
 mov qword ptr [rbp + 872], rdx
 jmp xchain26_n7_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n6_α:
 mov rdi, qword ptr [rbp + 960]
 mov rsi, qword ptr [rbp + 968]
 mov rdx, qword ptr [rbp + 992]
 mov rcx, qword ptr [rbp + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1024], rax
 mov qword ptr [rbp + 1032], rdx
 jmp xchain26_n8_α
# IR_DEREF variable -> value
 xchain26_n7_α:
 mov rdi, qword ptr [rbp + 864]
 mov rsi, qword ptr [rbp + 872]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 896], rax
 mov qword ptr [rbp + 904], rdx
 jmp xchain26_n9_α
# IR_VAR
 xchain26_n8_α:
 mov rax, qword ptr [1879052432]
 mov rdx, qword ptr [1879052440]
 mov qword ptr [rbp + 1088], rax
 mov qword ptr [rbp + 1096], rdx
 jmp xchain26_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n9_α:
 mov rdi, qword ptr [rbp + 768]
 mov rsi, qword ptr [rbp + 776]
 mov rdx, qword ptr [rbp + 896]
 mov rcx, qword ptr [rbp + 904]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 912], rax
 mov qword ptr [rbp + 920], rdx
 jmp xchain26_n11_α
# IR_LIT_INTEGER
 xchain26_n10_α:
 mov qword ptr [rbp + 1120], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [rbp + 1128], rax
 jmp xchain26_n12_α
.Lx37_0:
 .quad 0
# IR_DEREF variable -> value
 xchain26_n11_α:
 mov rdi, qword ptr [rbp + 912]
 mov rsi, qword ptr [rbp + 920]
 call rt_deref@PLT
 cmp eax, 99
 je xchain26_n2_α
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 jmp xchain26_n13_α
# IR_SUBSCRIPT x[i] variable
 xchain26_n12_α:
 mov rdi, qword ptr [rbp + 1088]
 mov rsi, qword ptr [rbp + 1096]
 mov rdx, qword ptr [rbp + 1120]
 mov rcx, qword ptr [rbp + 1128]
 call rt_subscript_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1152], rax
 mov qword ptr [rbp + 1160], rdx
 jmp xchain26_n14_α
# IR_ASSIGN gva
 xchain26_n13_α:
 mov rax, qword ptr [rbp + 944]
 mov rdx, qword ptr [rbp + 952]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 752], rax
 mov qword ptr [rbp + 760], rdx
 jmp xchain26_n2_α
# IR_DEREF variable -> value
 xchain26_n14_α:
 mov rdi, qword ptr [rbp + 1152]
 mov rsi, qword ptr [rbp + 1160]
 call rt_deref@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 jmp xchain26_n15_α
# IR_LIT_INTEGER
 xchain26_n15_α:
 mov qword ptr [rbp + 1200], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [rbp + 1208], rax
 jmp xchain26_n16_α
.Lx42_0:
 .quad 1
 xchain26_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rbp + 1184]
 cmp eax, 100
 je .Lx43_0
 mov eax, dword ptr [rbp + 1184]
 cmp eax, 6
 jne .Lx43_2
.Lx43_1:
 mov rax, qword ptr [rbp + 1192]
 mov rcx, 1
 sub rax, rcx
 mov qword ptr [rbp + 1056], 6
 mov qword ptr [rbp + 1064], rax
 jmp xchain26_n17_α
.Lx43_0:
 mov rdi, qword ptr [rbp + 1184]
 mov rsi, qword ptr [rbp + 1192]
 mov rdx, qword ptr [rbp + 1200]
 mov rcx, qword ptr [rbp + 1208]
 mov r8d, 1
 lea r9, [rbp + 1056]
 call rt_binop_overload@PLT
 test eax, eax
 jne xchain26_n17_α
.Lx43_2:
 mov rdi, qword ptr [rbp + 1184]
 mov rsi, qword ptr [rbp + 1192]
 mov rdx, qword ptr [rbp + 1200]
 mov rcx, qword ptr [rbp + 1208]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1056], rax
 mov qword ptr [rbp + 1064], rdx
 jmp xchain26_n17_α
 xchain26_n16_β:
 jmp proc_Pop_γ
# IR_ASSIGN_VAR
 xchain26_n17_α:
 mov rdi, qword ptr [rbp + 1024]
 mov rsi, qword ptr [rbp + 1032]
 mov rdx, qword ptr [rbp + 1056]
 mov rcx, qword ptr [rbp + 1064]
 call rt_assign_var@PLT
 cmp eax, 99
 je proc_Pop_γ
 mov qword ptr [rbp + 1232], rax
 mov qword ptr [rbp + 1240], rdx
 jmp proc_Pop_γ
proc_Pop_res:
add rsp, 8
pop rbp
proc_Pop_β:
jmp proc_Pop_ω
proc_Pop_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 3928]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
proc_Pop_ω:
mov rax, [rbp + 3936]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
  .globl proc_Unary_α
proc_Unary_α:
#=======================================================================================================================
    .global proc_Unary_α
    .global proc_Unary_β
    .global proc_Unary_γ
    .global proc_Unary_ω
  sub rsp, 3952
  mov [rsp + 3928], rcx
  mov [rsp + 3936], rdx
  mov [rsp + 3944], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 3920
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 3912], rsp
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
 mov qword ptr [rbp + 1264], rax
 mov qword ptr [rbp + 1272], rdx
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
 mov rax, qword ptr [rbp + 1264]
 mov rdx, qword ptr [rbp + 1272]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 1248], rax
 mov qword ptr [rbp + 1256], rdx
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
 mov qword ptr [rbp + 1312], rax
 mov qword ptr [rbp + 1320], rdx
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
 mov rax, qword ptr [rbp + 1312]
 mov rdx, qword ptr [rbp + 1320]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 1296], rax
 mov qword ptr [rbp + 1304], rdx
 jmp xchain45_n4_α
# IR_LIT_STRING
 xchain45_n4_α:
 mov qword ptr [rbp + 1344], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [rbp + 1352], rax
 jmp xchain45_n5_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string ""
 xchain45_n5_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1344] -> [zr+1392]
 mov rax, qword ptr [rbp + 1344]
 mov qword ptr [rbp + 1392], rax
 mov rax, qword ptr [rbp + 1352]
 mov qword ptr [rbp + 1400], rax
  .section .rodata
  .Lrkfn54: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn54]
 lea rsi, [rbp + 1392]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1376], rax
 mov qword ptr [rbp + 1384], rdx
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
 mov qword ptr [rbp + 1424], rax
 mov qword ptr [rbp + 1432], rdx
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
 mov qword ptr [rbp + 1680], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rbp + 1688], rax
 jmp xchain45_n9_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "dummy"
# IR_VAR
 xchain45_n8_α:
 mov rax, qword ptr [1879052368]
 mov rdx, qword ptr [1879052376]
 mov qword ptr [rbp + 1536], rax
 mov qword ptr [rbp + 1544], rdx
 jmp xchain45_n10_α
 xchain45_n9_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1648]
 mov rax, qword ptr [rbp + 1680]
 mov qword ptr [rbp + 1648], rax
 mov rax, qword ptr [rbp + 1688]
 mov qword ptr [rbp + 1656], rax
  .section .rodata
  .Lrkfn60: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn60]
 lea rsi, [rbp + 1648]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1632], rax
 mov qword ptr [rbp + 1640], rdx
 cmp eax, 99
 je xchain45_n12_α
 jmp xchain45_n11_α
 xchain45_n9_β:
 jmp xchain45_n12_α
# IR_VAR
 xchain45_n10_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 1568], rax
 mov qword ptr [rbp + 1576], rdx
 jmp xchain45_n13_α
# IR_ASSIGN gva
 xchain45_n11_α:
 mov rax, qword ptr [rbp + 1632]
 mov rdx, qword ptr [rbp + 1640]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 1616], rax
 mov qword ptr [rbp + 1624], rdx
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
 mov rdi, qword ptr [rbp + 1536]
 mov rsi, qword ptr [rbp + 1544]
 mov rdx, qword ptr [rbp + 1568]
 mov rcx, qword ptr [rbp + 1576]
 call str_concat_d@PLT
 mov qword ptr [rbp + 1504], rax
 mov qword ptr [rbp + 1512], rdx
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
# marshal arg0 = producer-box slot [zr+1504] -> [zr+1472]
 mov rax, qword ptr [rbp + 1504]
 mov qword ptr [rbp + 1472], rax
 mov rax, qword ptr [rbp + 1512]
 mov qword ptr [rbp + 1480], rax
  .section .rodata
  .Lrkfn68: .string "EVAL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn68]
 lea rsi, [rbp + 1472]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1456], rax
 mov qword ptr [rbp + 1464], rdx
 cmp eax, 99
 je xchain45_n7_α
 jmp xchain45_n16_α
 xchain45_n15_β:
 jmp xchain45_n7_α
# IR_ASSIGN_VAR
 xchain45_n16_α:
 mov rdi, qword ptr [rbp + 1424]
 mov rsi, qword ptr [rbp + 1432]
 mov rdx, qword ptr [rbp + 1456]
 mov rcx, qword ptr [rbp + 1464]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain45_n7_α
 mov qword ptr [rbp + 1600], rax
 mov qword ptr [rbp + 1608], rdx
 jmp xchain45_n7_α
proc_Unary_res:
add rsp, 8
pop rbp
proc_Unary_β:
jmp proc_Unary_ω
proc_Unary_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 3928]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
proc_Unary_ω:
mov rax, [rbp + 3936]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
  .globl proc_Binary_α
proc_Binary_α:
#=======================================================================================================================
    .global proc_Binary_α
    .global proc_Binary_β
    .global proc_Binary_γ
    .global proc_Binary_ω
  sub rsp, 3952
  mov [rsp + 3928], rcx
  mov [rsp + 3936], rdx
  mov [rsp + 3944], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 3920
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 3912], rsp
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
 mov qword ptr [rbp + 1728], rax
 mov qword ptr [rbp + 1736], rdx
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
 mov rax, qword ptr [rbp + 1728]
 mov rdx, qword ptr [rbp + 1736]
 mov qword ptr [1879052416], rax
 mov qword ptr [1879052424], rdx
 mov qword ptr [rbp + 1712], rax
 mov qword ptr [rbp + 1720], rdx
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
 mov qword ptr [rbp + 1776], rax
 mov qword ptr [rbp + 1784], rdx
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
 mov rax, qword ptr [rbp + 1776]
 mov rdx, qword ptr [rbp + 1784]
 mov qword ptr [1879052368], rax
 mov qword ptr [1879052376], rdx
 mov qword ptr [rbp + 1760], rax
 mov qword ptr [rbp + 1768], rdx
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
 mov qword ptr [rbp + 1824], rax
 mov qword ptr [rbp + 1832], rdx
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
 mov rax, qword ptr [rbp + 1824]
 mov rdx, qword ptr [rbp + 1832]
 mov qword ptr [1879052400], rax
 mov qword ptr [1879052408], rdx
 mov qword ptr [rbp + 1808], rax
 mov qword ptr [rbp + 1816], rdx
 jmp xchain70_n6_α
# IR_LIT_STRING
 xchain70_n6_α:
 mov qword ptr [rbp + 1856], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rbp + 1864], rax
 jmp xchain70_n7_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string ""
 xchain70_n7_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1856] -> [zr+1904]
 mov rax, qword ptr [rbp + 1856]
 mov qword ptr [rbp + 1904], rax
 mov rax, qword ptr [rbp + 1864]
 mov qword ptr [rbp + 1912], rax
  .section .rodata
  .Lrkfn82: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn82]
 lea rsi, [rbp + 1904]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1888], rax
 mov qword ptr [rbp + 1896], rdx
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
 mov qword ptr [rbp + 1936], rax
 mov qword ptr [rbp + 1944], rdx
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
 mov qword ptr [rbp + 2384], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [rbp + 2392], rax
 jmp xchain70_n11_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "dummy"
# IR_VAR
 xchain70_n10_α:
 mov rax, qword ptr [1879052400]
 mov rdx, qword ptr [1879052408]
 mov qword ptr [rbp + 2144], rax
 mov qword ptr [rbp + 2152], rdx
 jmp xchain70_n12_α
 xchain70_n11_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2384] -> [zr+2352]
 mov rax, qword ptr [rbp + 2384]
 mov qword ptr [rbp + 2352], rax
 mov rax, qword ptr [rbp + 2392]
 mov qword ptr [rbp + 2360], rax
  .section .rodata
  .Lrkfn88: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn88]
 lea rsi, [rbp + 2352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2336], rax
 mov qword ptr [rbp + 2344], rdx
 cmp eax, 99
 je xchain70_n14_α
 jmp xchain70_n13_α
 xchain70_n11_β:
 jmp xchain70_n14_α
# IR_LIT_STRING
 xchain70_n12_α:
 mov qword ptr [rbp + 2176], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [rbp + 2184], rax
 jmp xchain70_n15_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string " "
# IR_ASSIGN gva
 xchain70_n13_α:
 mov rax, qword ptr [rbp + 2336]
 mov rdx, qword ptr [rbp + 2344]
 mov qword ptr [1879052384], rax
 mov qword ptr [1879052392], rdx
 mov qword ptr [rbp + 2320], rax
 mov qword ptr [rbp + 2328], rdx
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
 mov rdi, qword ptr [rbp + 2144]
 mov rsi, qword ptr [rbp + 2152]
 mov rdx, qword ptr [rbp + 2176]
 mov rcx, qword ptr [rbp + 2184]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2112], rax
 mov qword ptr [rbp + 2120], rdx
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
 mov qword ptr [rbp + 2208], rax
 mov qword ptr [rbp + 2216], rdx
 jmp xchain70_n18_α
 xchain70_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2112]
 mov rsi, qword ptr [rbp + 2120]
 mov rdx, qword ptr [rbp + 2208]
 mov rcx, qword ptr [rbp + 2216]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2080], rax
 mov qword ptr [rbp + 2088], rdx
 jmp xchain70_n19_α
# IR_LIT_STRING
 xchain70_n19_α:
 mov qword ptr [rbp + 2240], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [rbp + 2248], rax
 jmp xchain70_n20_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string " "
 xchain70_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2080]
 mov rsi, qword ptr [rbp + 2088]
 mov rdx, qword ptr [rbp + 2240]
 mov rcx, qword ptr [rbp + 2248]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2048], rax
 mov qword ptr [rbp + 2056], rdx
 jmp xchain70_n21_α
# IR_VAR
 xchain70_n21_α:
 mov rax, qword ptr [1879052416]
 mov rdx, qword ptr [1879052424]
 mov qword ptr [rbp + 2272], rax
 mov qword ptr [rbp + 2280], rdx
 jmp xchain70_n22_α
 xchain70_n22_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 2048]
 mov rsi, qword ptr [rbp + 2056]
 mov rdx, qword ptr [rbp + 2272]
 mov rcx, qword ptr [rbp + 2280]
 call str_concat_d@PLT
 mov qword ptr [rbp + 2016], rax
 mov qword ptr [rbp + 2024], rdx
 jmp xchain70_n23_α
 xchain70_n23_α:
# BOX IR_CALL EVAL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2016] -> [zr+1984]
 mov rax, qword ptr [rbp + 2016]
 mov qword ptr [rbp + 1984], rax
 mov rax, qword ptr [rbp + 2024]
 mov qword ptr [rbp + 1992], rax
  .section .rodata
  .Lrkfn102: .string "EVAL"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn102]
 lea rsi, [rbp + 1984]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1968], rax
 mov qword ptr [rbp + 1976], rdx
 cmp eax, 99
 je xchain70_n9_α
 jmp xchain70_n24_α
 xchain70_n23_β:
 jmp xchain70_n9_α
# IR_ASSIGN_VAR
 xchain70_n24_α:
 mov rdi, qword ptr [rbp + 1936]
 mov rsi, qword ptr [rbp + 1944]
 mov rdx, qword ptr [rbp + 1968]
 mov rcx, qword ptr [rbp + 1976]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain70_n9_α
 mov qword ptr [rbp + 2304], rax
 mov qword ptr [rbp + 2312], rdx
 jmp xchain70_n9_α
proc_Binary_res:
add rsp, 8
pop rbp
proc_Binary_β:
jmp proc_Binary_ω
proc_Binary_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 3928]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
proc_Binary_ω:
mov rax, [rbp + 3936]
lea rsp, [rbp + 3952]
mov rbp, [rbp + 3944]
jmp rax
  .globl proc_EXPR$0_α
proc_EXPR$0_α:
#=======================================================================================================================
    .global proc_EXPR$0_α
    .global proc_EXPR$0_β
    .global proc_EXPR$0_γ
    .global proc_EXPR$0_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$0_α_body:
 xchain104_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx106_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx106_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx106_6]
 lea rdx, [rip + .Lx106_7]
 jmp rax
.Lx106_6:
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
 jmp .Lx106_2
.Lx106_7:
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
 jmp .Lx106_2
.Lx106_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx106_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx106_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx106_3]
 lea rdx, [rip + .Lx106_4]
 jmp rax
.Lx106_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx106_2
.Lx106_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx106_2
.Lx106_1:
 call rt_faildescr@PLT
.Lx106_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$0_ω
 jmp xchain104_n1_α
 xchain104_n0_β:
 jmp proc_EXPR$0_ω
.Lx106_0:
 .quad .Lx106_0_s
.Lx106_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain104_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052640], rax
 mov qword ptr [1879052648], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$0_γ
proc_EXPR$0_res:
add rsp, 8
pop rbp
proc_EXPR$0_β:
jmp proc_EXPR$0_ω
proc_EXPR$0_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$0_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$1_α
proc_EXPR$1_α:
#=======================================================================================================================
    .global proc_EXPR$1_α
    .global proc_EXPR$1_β
    .global proc_EXPR$1_γ
    .global proc_EXPR$1_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$1_α_body:
 xchain108_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx110_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx110_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx110_6]
 lea rdx, [rip + .Lx110_7]
 jmp rax
.Lx110_6:
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
 jmp .Lx110_2
.Lx110_7:
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
 jmp .Lx110_2
.Lx110_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx110_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx110_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx110_3]
 lea rdx, [rip + .Lx110_4]
 jmp rax
.Lx110_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx110_2
.Lx110_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx110_2
.Lx110_1:
 call rt_faildescr@PLT
.Lx110_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$1_ω
 jmp xchain108_n1_α
 xchain108_n0_β:
 jmp proc_EXPR$1_ω
.Lx110_0:
 .quad .Lx110_0_s
.Lx110_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain108_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052656], rax
 mov qword ptr [1879052664], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$1_γ
proc_EXPR$1_res:
add rsp, 8
pop rbp
proc_EXPR$1_β:
jmp proc_EXPR$1_ω
proc_EXPR$1_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$1_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$2_α
proc_EXPR$2_α:
#=======================================================================================================================
    .global proc_EXPR$2_α
    .global proc_EXPR$2_β
    .global proc_EXPR$2_γ
    .global proc_EXPR$2_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$2_α_body:
 xchain112_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx114_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx114_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx114_6]
 lea rdx, [rip + .Lx114_7]
 jmp rax
.Lx114_6:
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
 jmp .Lx114_2
.Lx114_7:
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
 jmp .Lx114_2
.Lx114_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx114_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx114_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx114_3]
 lea rdx, [rip + .Lx114_4]
 jmp rax
.Lx114_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx114_2
.Lx114_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx114_2
.Lx114_1:
 call rt_faildescr@PLT
.Lx114_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$2_ω
 jmp xchain112_n1_α
 xchain112_n0_β:
 jmp proc_EXPR$2_ω
.Lx114_0:
 .quad .Lx114_0_s
.Lx114_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain112_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052672], rax
 mov qword ptr [1879052680], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$2_γ
proc_EXPR$2_res:
add rsp, 8
pop rbp
proc_EXPR$2_β:
jmp proc_EXPR$2_ω
proc_EXPR$2_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$2_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$3_α
proc_EXPR$3_α:
#=======================================================================================================================
    .global proc_EXPR$3_α
    .global proc_EXPR$3_β
    .global proc_EXPR$3_γ
    .global proc_EXPR$3_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$3_α_body:
 xchain116_n0_α:
 sub rsp, 48
 mov rax, qword ptr [1879052352]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052360]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052336]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052344]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx118_0]
 mov esi, 2
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx118_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx118_6]
 lea rdx, [rip + .Lx118_7]
 jmp rax
.Lx118_6:
 mov rdi, qword ptr [1879052336]
 mov rsi, qword ptr [1879052344]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx118_2
.Lx118_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx118_2
.Lx118_5:
 add rsp, 48
 mov rdi, qword ptr [rip + .Lx118_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx118_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx118_3]
 lea rdx, [rip + .Lx118_4]
 jmp rax
.Lx118_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx118_2
.Lx118_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx118_2
.Lx118_1:
 call rt_faildescr@PLT
.Lx118_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$3_ω
 jmp xchain116_n1_α
 xchain116_n0_β:
 jmp proc_EXPR$3_ω
.Lx118_0:
 .quad .Lx118_0_s
.Lx118_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain116_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052688], rax
 mov qword ptr [1879052696], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$3_γ
proc_EXPR$3_res:
add rsp, 8
pop rbp
proc_EXPR$3_β:
jmp proc_EXPR$3_ω
proc_EXPR$3_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$3_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$4_α
proc_EXPR$4_α:
#=======================================================================================================================
    .global proc_EXPR$4_α
    .global proc_EXPR$4_β
    .global proc_EXPR$4_γ
    .global proc_EXPR$4_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$4_α_body:
 xchain120_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx122_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx122_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx122_6]
 lea rdx, [rip + .Lx122_7]
 jmp rax
.Lx122_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx122_2
.Lx122_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx122_2
.Lx122_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx122_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx122_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx122_3]
 lea rdx, [rip + .Lx122_4]
 jmp rax
.Lx122_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx122_2
.Lx122_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx122_2
.Lx122_1:
 call rt_faildescr@PLT
.Lx122_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$4_ω
 jmp xchain120_n1_α
 xchain120_n0_β:
 jmp proc_EXPR$4_ω
.Lx122_0:
 .quad .Lx122_0_s
.Lx122_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain120_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052704], rax
 mov qword ptr [1879052712], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$4_γ
proc_EXPR$4_res:
add rsp, 8
pop rbp
proc_EXPR$4_β:
jmp proc_EXPR$4_ω
proc_EXPR$4_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$4_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$5_α
proc_EXPR$5_α:
#=======================================================================================================================
    .global proc_EXPR$5_α
    .global proc_EXPR$5_β
    .global proc_EXPR$5_γ
    .global proc_EXPR$5_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$5_α_body:
 xchain124_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx126_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx126_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx126_6]
 lea rdx, [rip + .Lx126_7]
 jmp rax
.Lx126_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx126_2
.Lx126_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx126_2
.Lx126_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx126_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx126_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx126_3]
 lea rdx, [rip + .Lx126_4]
 jmp rax
.Lx126_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx126_2
.Lx126_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx126_2
.Lx126_1:
 call rt_faildescr@PLT
.Lx126_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$5_ω
 jmp xchain124_n1_α
 xchain124_n0_β:
 jmp proc_EXPR$5_ω
.Lx126_0:
 .quad .Lx126_0_s
.Lx126_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain124_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052720], rax
 mov qword ptr [1879052728], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$5_γ
proc_EXPR$5_res:
add rsp, 8
pop rbp
proc_EXPR$5_β:
jmp proc_EXPR$5_ω
proc_EXPR$5_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$5_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$6_α
proc_EXPR$6_α:
#=======================================================================================================================
    .global proc_EXPR$6_α
    .global proc_EXPR$6_β
    .global proc_EXPR$6_γ
    .global proc_EXPR$6_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$6_α_body:
 xchain128_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx130_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx130_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx130_6]
 lea rdx, [rip + .Lx130_7]
 jmp rax
.Lx130_6:
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
 jmp .Lx130_2
.Lx130_7:
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
 jmp .Lx130_2
.Lx130_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx130_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx130_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx130_3]
 lea rdx, [rip + .Lx130_4]
 jmp rax
.Lx130_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx130_2
.Lx130_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx130_2
.Lx130_1:
 call rt_faildescr@PLT
.Lx130_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$6_ω
 jmp xchain128_n1_α
 xchain128_n0_β:
 jmp proc_EXPR$6_ω
.Lx130_0:
 .quad .Lx130_0_s
.Lx130_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain128_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052736], rax
 mov qword ptr [1879052744], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$6_γ
proc_EXPR$6_res:
add rsp, 8
pop rbp
proc_EXPR$6_β:
jmp proc_EXPR$6_ω
proc_EXPR$6_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$6_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$7_α
proc_EXPR$7_α:
#=======================================================================================================================
    .global proc_EXPR$7_α
    .global proc_EXPR$7_β
    .global proc_EXPR$7_γ
    .global proc_EXPR$7_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$7_α_body:
 xchain132_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx134_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx134_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx134_6]
 lea rdx, [rip + .Lx134_7]
 jmp rax
.Lx134_6:
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
 jmp .Lx134_2
.Lx134_7:
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
 jmp .Lx134_2
.Lx134_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx134_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx134_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx134_3]
 lea rdx, [rip + .Lx134_4]
 jmp rax
.Lx134_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx134_2
.Lx134_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx134_2
.Lx134_1:
 call rt_faildescr@PLT
.Lx134_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$7_ω
 jmp xchain132_n1_α
 xchain132_n0_β:
 jmp proc_EXPR$7_ω
.Lx134_0:
 .quad .Lx134_0_s
.Lx134_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain132_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052752], rax
 mov qword ptr [1879052760], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$7_γ
proc_EXPR$7_res:
add rsp, 8
pop rbp
proc_EXPR$7_β:
jmp proc_EXPR$7_ω
proc_EXPR$7_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$7_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$8_α
proc_EXPR$8_α:
#=======================================================================================================================
    .global proc_EXPR$8_α
    .global proc_EXPR$8_β
    .global proc_EXPR$8_γ
    .global proc_EXPR$8_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$8_α_body:
 xchain136_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx138_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx138_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx138_6]
 lea rdx, [rip + .Lx138_7]
 jmp rax
.Lx138_6:
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
 jmp .Lx138_2
.Lx138_7:
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
 jmp .Lx138_2
.Lx138_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx138_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx138_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx138_3]
 lea rdx, [rip + .Lx138_4]
 jmp rax
.Lx138_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx138_2
.Lx138_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx138_2
.Lx138_1:
 call rt_faildescr@PLT
.Lx138_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$8_ω
 jmp xchain136_n1_α
 xchain136_n0_β:
 jmp proc_EXPR$8_ω
.Lx138_0:
 .quad .Lx138_0_s
.Lx138_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain136_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052768], rax
 mov qword ptr [1879052776], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$8_γ
proc_EXPR$8_res:
add rsp, 8
pop rbp
proc_EXPR$8_β:
jmp proc_EXPR$8_ω
proc_EXPR$8_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$8_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$9_α
proc_EXPR$9_α:
#=======================================================================================================================
    .global proc_EXPR$9_α
    .global proc_EXPR$9_β
    .global proc_EXPR$9_γ
    .global proc_EXPR$9_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$9_α_body:
 xchain140_n0_α:
 sub rsp, 48
 mov rax, qword ptr [1879052352]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052360]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052336]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052344]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx142_0]
 mov esi, 2
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx142_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx142_6]
 lea rdx, [rip + .Lx142_7]
 jmp rax
.Lx142_6:
 mov rdi, qword ptr [1879052336]
 mov rsi, qword ptr [1879052344]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx142_2
.Lx142_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx142_2
.Lx142_5:
 add rsp, 48
 mov rdi, qword ptr [rip + .Lx142_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx142_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx142_3]
 lea rdx, [rip + .Lx142_4]
 jmp rax
.Lx142_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx142_2
.Lx142_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx142_2
.Lx142_1:
 call rt_faildescr@PLT
.Lx142_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$9_ω
 jmp xchain140_n1_α
 xchain140_n0_β:
 jmp proc_EXPR$9_ω
.Lx142_0:
 .quad .Lx142_0_s
.Lx142_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain140_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052784], rax
 mov qword ptr [1879052792], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$9_γ
proc_EXPR$9_res:
add rsp, 8
pop rbp
proc_EXPR$9_β:
jmp proc_EXPR$9_ω
proc_EXPR$9_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$9_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$10_α
proc_EXPR$10_α:
#=======================================================================================================================
    .global proc_EXPR$10_α
    .global proc_EXPR$10_β
    .global proc_EXPR$10_γ
    .global proc_EXPR$10_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$10_α_body:
 xchain144_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx146_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx146_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx146_6]
 lea rdx, [rip + .Lx146_7]
 jmp rax
.Lx146_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx146_2
.Lx146_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx146_2
.Lx146_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx146_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx146_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx146_3]
 lea rdx, [rip + .Lx146_4]
 jmp rax
.Lx146_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx146_2
.Lx146_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx146_2
.Lx146_1:
 call rt_faildescr@PLT
.Lx146_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$10_ω
 jmp xchain144_n1_α
 xchain144_n0_β:
 jmp proc_EXPR$10_ω
.Lx146_0:
 .quad .Lx146_0_s
.Lx146_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain144_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052800], rax
 mov qword ptr [1879052808], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$10_γ
proc_EXPR$10_res:
add rsp, 8
pop rbp
proc_EXPR$10_β:
jmp proc_EXPR$10_ω
proc_EXPR$10_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$10_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$11_α
proc_EXPR$11_α:
#=======================================================================================================================
    .global proc_EXPR$11_α
    .global proc_EXPR$11_β
    .global proc_EXPR$11_γ
    .global proc_EXPR$11_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$11_α_body:
 xchain148_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx150_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx150_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx150_6]
 lea rdx, [rip + .Lx150_7]
 jmp rax
.Lx150_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx150_2
.Lx150_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx150_2
.Lx150_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx150_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx150_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx150_3]
 lea rdx, [rip + .Lx150_4]
 jmp rax
.Lx150_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx150_2
.Lx150_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx150_2
.Lx150_1:
 call rt_faildescr@PLT
.Lx150_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$11_ω
 jmp xchain148_n1_α
 xchain148_n0_β:
 jmp proc_EXPR$11_ω
.Lx150_0:
 .quad .Lx150_0_s
.Lx150_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain148_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052816], rax
 mov qword ptr [1879052824], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$11_γ
proc_EXPR$11_res:
add rsp, 8
pop rbp
proc_EXPR$11_β:
jmp proc_EXPR$11_ω
proc_EXPR$11_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$11_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$12_α
proc_EXPR$12_α:
#=======================================================================================================================
    .global proc_EXPR$12_α
    .global proc_EXPR$12_β
    .global proc_EXPR$12_γ
    .global proc_EXPR$12_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$12_α_body:
 xchain152_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx154_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx154_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx154_6]
 lea rdx, [rip + .Lx154_7]
 jmp rax
.Lx154_6:
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
 jmp .Lx154_2
.Lx154_7:
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
 jmp .Lx154_2
.Lx154_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx154_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx154_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx154_3]
 lea rdx, [rip + .Lx154_4]
 jmp rax
.Lx154_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx154_2
.Lx154_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx154_2
.Lx154_1:
 call rt_faildescr@PLT
.Lx154_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$12_ω
 jmp xchain152_n1_α
 xchain152_n0_β:
 jmp proc_EXPR$12_ω
.Lx154_0:
 .quad .Lx154_0_s
.Lx154_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain152_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052832], rax
 mov qword ptr [1879052840], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$12_γ
proc_EXPR$12_res:
add rsp, 8
pop rbp
proc_EXPR$12_β:
jmp proc_EXPR$12_ω
proc_EXPR$12_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$12_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$13_α
proc_EXPR$13_α:
#=======================================================================================================================
    .global proc_EXPR$13_α
    .global proc_EXPR$13_β
    .global proc_EXPR$13_γ
    .global proc_EXPR$13_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$13_α_body:
 xchain156_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx158_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx158_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx158_6]
 lea rdx, [rip + .Lx158_7]
 jmp rax
.Lx158_6:
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
 jmp .Lx158_2
.Lx158_7:
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
 jmp .Lx158_2
.Lx158_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx158_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx158_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx158_3]
 lea rdx, [rip + .Lx158_4]
 jmp rax
.Lx158_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx158_2
.Lx158_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx158_2
.Lx158_1:
 call rt_faildescr@PLT
.Lx158_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$13_ω
 jmp xchain156_n1_α
 xchain156_n0_β:
 jmp proc_EXPR$13_ω
.Lx158_0:
 .quad .Lx158_0_s
.Lx158_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain156_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052848], rax
 mov qword ptr [1879052856], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$13_γ
proc_EXPR$13_res:
add rsp, 8
pop rbp
proc_EXPR$13_β:
jmp proc_EXPR$13_ω
proc_EXPR$13_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$13_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$14_α
proc_EXPR$14_α:
#=======================================================================================================================
    .global proc_EXPR$14_α
    .global proc_EXPR$14_β
    .global proc_EXPR$14_γ
    .global proc_EXPR$14_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$14_α_body:
 xchain160_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx162_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx162_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx162_6]
 lea rdx, [rip + .Lx162_7]
 jmp rax
.Lx162_6:
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
 jmp .Lx162_2
.Lx162_7:
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
 jmp .Lx162_2
.Lx162_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx162_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx162_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx162_3]
 lea rdx, [rip + .Lx162_4]
 jmp rax
.Lx162_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx162_2
.Lx162_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx162_2
.Lx162_1:
 call rt_faildescr@PLT
.Lx162_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$14_ω
 jmp xchain160_n1_α
 xchain160_n0_β:
 jmp proc_EXPR$14_ω
.Lx162_0:
 .quad .Lx162_0_s
.Lx162_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain160_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052864], rax
 mov qword ptr [1879052872], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$14_γ
proc_EXPR$14_res:
add rsp, 8
pop rbp
proc_EXPR$14_β:
jmp proc_EXPR$14_ω
proc_EXPR$14_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$14_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$15_α
proc_EXPR$15_α:
#=======================================================================================================================
    .global proc_EXPR$15_α
    .global proc_EXPR$15_β
    .global proc_EXPR$15_γ
    .global proc_EXPR$15_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$15_α_body:
 xchain164_n0_α:
 sub rsp, 48
 mov rax, qword ptr [1879052352]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052360]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052336]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052344]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx166_0]
 mov esi, 2
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx166_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx166_6]
 lea rdx, [rip + .Lx166_7]
 jmp rax
.Lx166_6:
 mov rdi, qword ptr [1879052336]
 mov rsi, qword ptr [1879052344]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx166_2
.Lx166_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx166_2
.Lx166_5:
 add rsp, 48
 mov rdi, qword ptr [rip + .Lx166_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx166_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx166_3]
 lea rdx, [rip + .Lx166_4]
 jmp rax
.Lx166_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx166_2
.Lx166_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx166_2
.Lx166_1:
 call rt_faildescr@PLT
.Lx166_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$15_ω
 jmp xchain164_n1_α
 xchain164_n0_β:
 jmp proc_EXPR$15_ω
.Lx166_0:
 .quad .Lx166_0_s
.Lx166_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain164_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052880], rax
 mov qword ptr [1879052888], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$15_γ
proc_EXPR$15_res:
add rsp, 8
pop rbp
proc_EXPR$15_β:
jmp proc_EXPR$15_ω
proc_EXPR$15_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$15_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$16_α
proc_EXPR$16_α:
#=======================================================================================================================
    .global proc_EXPR$16_α
    .global proc_EXPR$16_β
    .global proc_EXPR$16_γ
    .global proc_EXPR$16_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$16_α_body:
 xchain168_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx170_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx170_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx170_6]
 lea rdx, [rip + .Lx170_7]
 jmp rax
.Lx170_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx170_2
.Lx170_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx170_2
.Lx170_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx170_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx170_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx170_3]
 lea rdx, [rip + .Lx170_4]
 jmp rax
.Lx170_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx170_2
.Lx170_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx170_2
.Lx170_1:
 call rt_faildescr@PLT
.Lx170_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$16_ω
 jmp xchain168_n1_α
 xchain168_n0_β:
 jmp proc_EXPR$16_ω
.Lx170_0:
 .quad .Lx170_0_s
.Lx170_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain168_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052896], rax
 mov qword ptr [1879052904], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$16_γ
proc_EXPR$16_res:
add rsp, 8
pop rbp
proc_EXPR$16_β:
jmp proc_EXPR$16_ω
proc_EXPR$16_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$16_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$17_α
proc_EXPR$17_α:
#=======================================================================================================================
    .global proc_EXPR$17_α
    .global proc_EXPR$17_β
    .global proc_EXPR$17_γ
    .global proc_EXPR$17_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$17_α_body:
 xchain172_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx174_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx174_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx174_6]
 lea rdx, [rip + .Lx174_7]
 jmp rax
.Lx174_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx174_2
.Lx174_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx174_2
.Lx174_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx174_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx174_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx174_3]
 lea rdx, [rip + .Lx174_4]
 jmp rax
.Lx174_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx174_2
.Lx174_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx174_2
.Lx174_1:
 call rt_faildescr@PLT
.Lx174_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$17_ω
 jmp xchain172_n1_α
 xchain172_n0_β:
 jmp proc_EXPR$17_ω
.Lx174_0:
 .quad .Lx174_0_s
.Lx174_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain172_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052912], rax
 mov qword ptr [1879052920], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$17_γ
proc_EXPR$17_res:
add rsp, 8
pop rbp
proc_EXPR$17_β:
jmp proc_EXPR$17_ω
proc_EXPR$17_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$17_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$18_α
proc_EXPR$18_α:
#=======================================================================================================================
    .global proc_EXPR$18_α
    .global proc_EXPR$18_β
    .global proc_EXPR$18_γ
    .global proc_EXPR$18_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$18_α_body:
 xchain176_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx178_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx178_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx178_6]
 lea rdx, [rip + .Lx178_7]
 jmp rax
.Lx178_6:
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
 jmp .Lx178_2
.Lx178_7:
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
 jmp .Lx178_2
.Lx178_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx178_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx178_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx178_3]
 lea rdx, [rip + .Lx178_4]
 jmp rax
.Lx178_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx178_2
.Lx178_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx178_2
.Lx178_1:
 call rt_faildescr@PLT
.Lx178_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$18_ω
 jmp xchain176_n1_α
 xchain176_n0_β:
 jmp proc_EXPR$18_ω
.Lx178_0:
 .quad .Lx178_0_s
.Lx178_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain176_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052928], rax
 mov qword ptr [1879052936], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$18_γ
proc_EXPR$18_res:
add rsp, 8
pop rbp
proc_EXPR$18_β:
jmp proc_EXPR$18_ω
proc_EXPR$18_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$18_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$19_α
proc_EXPR$19_α:
#=======================================================================================================================
    .global proc_EXPR$19_α
    .global proc_EXPR$19_β
    .global proc_EXPR$19_γ
    .global proc_EXPR$19_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$19_α_body:
 xchain180_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx182_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx182_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx182_6]
 lea rdx, [rip + .Lx182_7]
 jmp rax
.Lx182_6:
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
 jmp .Lx182_2
.Lx182_7:
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
 jmp .Lx182_2
.Lx182_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx182_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx182_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx182_3]
 lea rdx, [rip + .Lx182_4]
 jmp rax
.Lx182_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx182_2
.Lx182_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx182_2
.Lx182_1:
 call rt_faildescr@PLT
.Lx182_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$19_ω
 jmp xchain180_n1_α
 xchain180_n0_β:
 jmp proc_EXPR$19_ω
.Lx182_0:
 .quad .Lx182_0_s
.Lx182_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain180_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052944], rax
 mov qword ptr [1879052952], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$19_γ
proc_EXPR$19_res:
add rsp, 8
pop rbp
proc_EXPR$19_β:
jmp proc_EXPR$19_ω
proc_EXPR$19_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$19_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$20_α
proc_EXPR$20_α:
#=======================================================================================================================
    .global proc_EXPR$20_α
    .global proc_EXPR$20_β
    .global proc_EXPR$20_γ
    .global proc_EXPR$20_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$20_α_body:
 xchain184_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx186_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx186_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx186_6]
 lea rdx, [rip + .Lx186_7]
 jmp rax
.Lx186_6:
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
 jmp .Lx186_2
.Lx186_7:
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
 jmp .Lx186_2
.Lx186_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx186_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx186_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx186_3]
 lea rdx, [rip + .Lx186_4]
 jmp rax
.Lx186_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx186_2
.Lx186_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx186_2
.Lx186_1:
 call rt_faildescr@PLT
.Lx186_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$20_ω
 jmp xchain184_n1_α
 xchain184_n0_β:
 jmp proc_EXPR$20_ω
.Lx186_0:
 .quad .Lx186_0_s
.Lx186_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain184_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052960], rax
 mov qword ptr [1879052968], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$20_γ
proc_EXPR$20_res:
add rsp, 8
pop rbp
proc_EXPR$20_β:
jmp proc_EXPR$20_ω
proc_EXPR$20_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$20_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$21_α
proc_EXPR$21_α:
#=======================================================================================================================
    .global proc_EXPR$21_α
    .global proc_EXPR$21_β
    .global proc_EXPR$21_γ
    .global proc_EXPR$21_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$21_α_body:
 xchain188_n0_α:
 sub rsp, 48
 mov rax, qword ptr [1879052352]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052360]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052336]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052344]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx190_0]
 mov esi, 2
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx190_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx190_6]
 lea rdx, [rip + .Lx190_7]
 jmp rax
.Lx190_6:
 mov rdi, qword ptr [1879052336]
 mov rsi, qword ptr [1879052344]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx190_2
.Lx190_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx190_2
.Lx190_5:
 add rsp, 48
 mov rdi, qword ptr [rip + .Lx190_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx190_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx190_3]
 lea rdx, [rip + .Lx190_4]
 jmp rax
.Lx190_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx190_2
.Lx190_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx190_2
.Lx190_1:
 call rt_faildescr@PLT
.Lx190_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$21_ω
 jmp xchain188_n1_α
 xchain188_n0_β:
 jmp proc_EXPR$21_ω
.Lx190_0:
 .quad .Lx190_0_s
.Lx190_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain188_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052976], rax
 mov qword ptr [1879052984], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$21_γ
proc_EXPR$21_res:
add rsp, 8
pop rbp
proc_EXPR$21_β:
jmp proc_EXPR$21_ω
proc_EXPR$21_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$21_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$22_α
proc_EXPR$22_α:
#=======================================================================================================================
    .global proc_EXPR$22_α
    .global proc_EXPR$22_β
    .global proc_EXPR$22_γ
    .global proc_EXPR$22_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$22_α_body:
 xchain192_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx194_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx194_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx194_6]
 lea rdx, [rip + .Lx194_7]
 jmp rax
.Lx194_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx194_2
.Lx194_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx194_2
.Lx194_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx194_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx194_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx194_3]
 lea rdx, [rip + .Lx194_4]
 jmp rax
.Lx194_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx194_2
.Lx194_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx194_2
.Lx194_1:
 call rt_faildescr@PLT
.Lx194_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$22_ω
 jmp xchain192_n1_α
 xchain192_n0_β:
 jmp proc_EXPR$22_ω
.Lx194_0:
 .quad .Lx194_0_s
.Lx194_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain192_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879052992], rax
 mov qword ptr [1879053000], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$22_γ
proc_EXPR$22_res:
add rsp, 8
pop rbp
proc_EXPR$22_β:
jmp proc_EXPR$22_ω
proc_EXPR$22_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$22_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$23_α
proc_EXPR$23_α:
#=======================================================================================================================
    .global proc_EXPR$23_α
    .global proc_EXPR$23_β
    .global proc_EXPR$23_γ
    .global proc_EXPR$23_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$23_α_body:
 xchain196_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx198_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx198_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx198_6]
 lea rdx, [rip + .Lx198_7]
 jmp rax
.Lx198_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx198_2
.Lx198_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx198_2
.Lx198_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx198_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx198_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx198_3]
 lea rdx, [rip + .Lx198_4]
 jmp rax
.Lx198_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx198_2
.Lx198_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx198_2
.Lx198_1:
 call rt_faildescr@PLT
.Lx198_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$23_ω
 jmp xchain196_n1_α
 xchain196_n0_β:
 jmp proc_EXPR$23_ω
.Lx198_0:
 .quad .Lx198_0_s
.Lx198_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain196_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053008], rax
 mov qword ptr [1879053016], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$23_γ
proc_EXPR$23_res:
add rsp, 8
pop rbp
proc_EXPR$23_β:
jmp proc_EXPR$23_ω
proc_EXPR$23_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$23_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$24_α
proc_EXPR$24_α:
#=======================================================================================================================
    .global proc_EXPR$24_α
    .global proc_EXPR$24_β
    .global proc_EXPR$24_γ
    .global proc_EXPR$24_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$24_α_body:
 xchain200_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx202_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx202_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx202_6]
 lea rdx, [rip + .Lx202_7]
 jmp rax
.Lx202_6:
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
 jmp .Lx202_2
.Lx202_7:
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
 jmp .Lx202_2
.Lx202_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx202_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx202_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx202_3]
 lea rdx, [rip + .Lx202_4]
 jmp rax
.Lx202_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx202_2
.Lx202_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx202_2
.Lx202_1:
 call rt_faildescr@PLT
.Lx202_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$24_ω
 jmp xchain200_n1_α
 xchain200_n0_β:
 jmp proc_EXPR$24_ω
.Lx202_0:
 .quad .Lx202_0_s
.Lx202_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain200_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053024], rax
 mov qword ptr [1879053032], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$24_γ
proc_EXPR$24_res:
add rsp, 8
pop rbp
proc_EXPR$24_β:
jmp proc_EXPR$24_ω
proc_EXPR$24_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$24_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$25_α
proc_EXPR$25_α:
#=======================================================================================================================
    .global proc_EXPR$25_α
    .global proc_EXPR$25_β
    .global proc_EXPR$25_γ
    .global proc_EXPR$25_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$25_α_body:
 xchain204_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx206_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx206_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx206_6]
 lea rdx, [rip + .Lx206_7]
 jmp rax
.Lx206_6:
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
 jmp .Lx206_2
.Lx206_7:
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
 jmp .Lx206_2
.Lx206_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx206_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx206_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx206_3]
 lea rdx, [rip + .Lx206_4]
 jmp rax
.Lx206_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx206_2
.Lx206_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx206_2
.Lx206_1:
 call rt_faildescr@PLT
.Lx206_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$25_ω
 jmp xchain204_n1_α
 xchain204_n0_β:
 jmp proc_EXPR$25_ω
.Lx206_0:
 .quad .Lx206_0_s
.Lx206_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain204_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053040], rax
 mov qword ptr [1879053048], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$25_γ
proc_EXPR$25_res:
add rsp, 8
pop rbp
proc_EXPR$25_β:
jmp proc_EXPR$25_ω
proc_EXPR$25_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$25_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$26_α
proc_EXPR$26_α:
#=======================================================================================================================
    .global proc_EXPR$26_α
    .global proc_EXPR$26_β
    .global proc_EXPR$26_γ
    .global proc_EXPR$26_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$26_α_body:
 xchain208_n0_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx210_0]
 mov esi, 1
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx210_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx210_6]
 lea rdx, [rip + .Lx210_7]
 jmp rax
.Lx210_6:
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
 jmp .Lx210_2
.Lx210_7:
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
 jmp .Lx210_2
.Lx210_5:
 add rsp, 32
 mov rdi, qword ptr [rip + .Lx210_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx210_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx210_3]
 lea rdx, [rip + .Lx210_4]
 jmp rax
.Lx210_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx210_2
.Lx210_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx210_2
.Lx210_1:
 call rt_faildescr@PLT
.Lx210_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$26_ω
 jmp xchain208_n1_α
 xchain208_n0_β:
 jmp proc_EXPR$26_ω
.Lx210_0:
 .quad .Lx210_0_s
.Lx210_0_s:
 .string "Push"
# IR_ASSIGN gva
 xchain208_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053056], rax
 mov qword ptr [1879053064], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$26_γ
proc_EXPR$26_res:
add rsp, 8
pop rbp
proc_EXPR$26_β:
jmp proc_EXPR$26_ω
proc_EXPR$26_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$26_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$27_α
proc_EXPR$27_α:
#=======================================================================================================================
    .global proc_EXPR$27_α
    .global proc_EXPR$27_β
    .global proc_EXPR$27_γ
    .global proc_EXPR$27_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$27_α_body:
 xchain212_n0_α:
 sub rsp, 48
 mov rax, qword ptr [1879052352]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052360]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052336]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052344]
 mov qword ptr [rsp + 40], rax
 mov rdi, qword ptr [rip + .Lx214_0]
 mov esi, 2
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx214_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx214_6]
 lea rdx, [rip + .Lx214_7]
 jmp rax
.Lx214_6:
 mov rdi, qword ptr [1879052336]
 mov rsi, qword ptr [1879052344]
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx214_2
.Lx214_7:
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052336], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052344], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052376], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052352], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052360], rax
 add rsp, 48
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx214_2
.Lx214_5:
 add rsp, 48
 mov rdi, qword ptr [rip + .Lx214_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx214_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx214_3]
 lea rdx, [rip + .Lx214_4]
 jmp rax
.Lx214_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx214_2
.Lx214_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx214_2
.Lx214_1:
 call rt_faildescr@PLT
.Lx214_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$27_ω
 jmp xchain212_n1_α
 xchain212_n0_β:
 jmp proc_EXPR$27_ω
.Lx214_0:
 .quad .Lx214_0_s
.Lx214_0_s:
 .string "Unary"
# IR_ASSIGN gva
 xchain212_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053072], rax
 mov qword ptr [1879053080], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$27_γ
proc_EXPR$27_res:
add rsp, 8
pop rbp
proc_EXPR$27_β:
jmp proc_EXPR$27_ω
proc_EXPR$27_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$27_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$28_α
proc_EXPR$28_α:
#=======================================================================================================================
    .global proc_EXPR$28_α
    .global proc_EXPR$28_β
    .global proc_EXPR$28_γ
    .global proc_EXPR$28_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$28_α_body:
 xchain216_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx218_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx218_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx218_6]
 lea rdx, [rip + .Lx218_7]
 jmp rax
.Lx218_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx218_2
.Lx218_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx218_2
.Lx218_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx218_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx218_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx218_3]
 lea rdx, [rip + .Lx218_4]
 jmp rax
.Lx218_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx218_2
.Lx218_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx218_2
.Lx218_1:
 call rt_faildescr@PLT
.Lx218_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$28_ω
 jmp xchain216_n1_α
 xchain216_n0_β:
 jmp proc_EXPR$28_ω
.Lx218_0:
 .quad .Lx218_0_s
.Lx218_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain216_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053088], rax
 mov qword ptr [1879053096], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$28_γ
proc_EXPR$28_res:
add rsp, 8
pop rbp
proc_EXPR$28_β:
jmp proc_EXPR$28_ω
proc_EXPR$28_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$28_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_EXPR$29_α
proc_EXPR$29_α:
#=======================================================================================================================
    .global proc_EXPR$29_α
    .global proc_EXPR$29_β
    .global proc_EXPR$29_γ
    .global proc_EXPR$29_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
proc_EXPR$29_α_body:
 xchain220_n0_α:
 sub rsp, 64
 mov rax, qword ptr [1879052368]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052376]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052400]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052408]
 mov qword ptr [rsp + 24], rax
 mov rax, qword ptr [1879052416]
 mov qword ptr [rsp + 32], rax
 mov rax, qword ptr [1879052424]
 mov qword ptr [rsp + 40], rax
 mov rax, qword ptr [1879052384]
 mov qword ptr [rsp + 48], rax
 mov rax, qword ptr [1879052392]
 mov qword ptr [rsp + 56], rax
 mov rdi, qword ptr [rip + .Lx222_0]
 mov esi, 3
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx222_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx222_6]
 lea rdx, [rip + .Lx222_7]
 jmp rax
.Lx222_6:
 mov rdi, qword ptr [1879052384]
 mov rsi, qword ptr [1879052392]
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx222_2
.Lx222_7:
 mov rax, qword ptr [rsp + 48]
 mov qword ptr [1879052384], rax
 mov rax, qword ptr [rsp + 56]
 mov qword ptr [1879052392], rax
 mov rax, qword ptr [rsp + 32]
 mov qword ptr [1879052416], rax
 mov rax, qword ptr [rsp + 40]
 mov qword ptr [1879052424], rax
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052400], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052408], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052368], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052376], rax
 add rsp, 64
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx222_2
.Lx222_5:
 add rsp, 64
 mov rdi, qword ptr [rip + .Lx222_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx222_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx222_3]
 lea rdx, [rip + .Lx222_4]
 jmp rax
.Lx222_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx222_2
.Lx222_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx222_2
.Lx222_1:
 call rt_faildescr@PLT
.Lx222_2:
 mov qword ptr [rbp + 32], rax
 mov qword ptr [rbp + 40], rdx
 cmp eax, 99
 je proc_EXPR$29_ω
 jmp xchain220_n1_α
 xchain220_n0_β:
 jmp proc_EXPR$29_ω
.Lx222_0:
 .quad .Lx222_0_s
.Lx222_0_s:
 .string "Binary"
# IR_ASSIGN gva
 xchain220_n1_α:
 mov rax, qword ptr [rbp + 32]
 mov rdx, qword ptr [rbp + 40]
 mov qword ptr [1879053104], rax
 mov qword ptr [1879053112], rdx
 mov qword ptr [rbp + 16], rax
 mov qword ptr [rbp + 24], rdx
 jmp proc_EXPR$29_γ
proc_EXPR$29_res:
add rsp, 8
pop rbp
proc_EXPR$29_β:
jmp proc_EXPR$29_ω
proc_EXPR$29_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 88]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
proc_EXPR$29_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
mov qword ptr [rbp + 80], r8
mov dword ptr [rbp + 72], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain224_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain224_n0_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx226_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx226_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx226_1
 add dword ptr [rsp + 0], 1
 jmp .Lx226_0
.Lx226_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx226_240
 add rsp, 16
 jmp proc_PAT$0_scanfail
.Lx226_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$0_scanhit
 xchain224_n0_β:
 xchain224_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$0_scanfail
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 80], 1
jne 7f
mov ecx, dword ptr [rbp + 72]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 80], 1
jne 8f
mov eax, dword ptr [rbp + 72]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 72], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 48]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$1_α
proc_PAT$1_α:
#=======================================================================================================================
    .global proc_PAT$1_α
    .global proc_PAT$1_β
    .global proc_PAT$1_γ
    .global proc_PAT$1_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
lea rax, [rip + xchain227_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain227_n0_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain227_n1_α
xchain227_n0_as:
 jmp proc_PAT$1_γ
 xchain227_n0_β:
 jmp xchain227_n3_β
xchain227_n0_af:
 jmp proc_PAT$1_ω
# IR_MATCH_ANY
 xchain227_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain227_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C1]
 cmp byte ptr [rdi+rsi], 0
 je xchain227_n0_af
 add r14d, 1
 jmp xchain227_n2_α
 xchain227_n1_β:
 sub r14d, 1
 jmp xchain227_n0_af
# IR_MATCH_ALT_NARY
 xchain227_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain227_n4_α
xchain227_n2_as:
 jmp xchain227_n3_α
 xchain227_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain227_n4_β
 jmp xchain227_n5_β
xchain227_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain227_n5_α
 jmp xchain227_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain227_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx233_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx233_10
.Lx233_9:
 xor eax, eax
.Lx233_10:
 test rax, rax
 jz .Lx233_0
 mov r8d, 0
 lea rcx, [rip + .Lx233_4]
 lea rdx, [rip + .Lx233_5]
 jmp rax
.Lx233_4:
 jmp xchain227_n0_as
.Lx233_5:
 jmp xchain227_n2_β
.Lx233_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx233_2:
 test rax, rax
 je .Lx233_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx233_7]
 lea rdx, [rip + .Lx233_8]
 jmp rax
.Lx233_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx233_2
.Lx233_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx233_2
.Lx233_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain227_n2_β
 mov r14d, eax
 lea rax, [rip + .Lx233_6]
 sub rsp, 8
 push rax
 jmp xchain227_n0_as
.Lx233_6:
 add rsp, 16
 jmp xchain227_n2_β
 xchain227_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain227_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain227_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 je xchain227_n2_af
 add r14d, 1
 jmp xchain227_n2_as
 xchain227_n4_β:
 sub r14d, 1
 jmp xchain227_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain227_n5_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx235_0
 mov r8d, 0
 lea rcx, [rip + .Lx235_4]
 lea rdx, [rip + .Lx235_5]
 jmp rax
.Lx235_4:
 jmp xchain227_n2_as
.Lx235_5:
 jmp xchain227_n2_af
.Lx235_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx235_2:
 test rax, rax
 je .Lx235_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx235_7]
 lea rdx, [rip + .Lx235_8]
 jmp rax
.Lx235_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx235_2
.Lx235_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx235_2
.Lx235_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain227_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx235_6]
 sub rsp, 8
 push rax
 jmp xchain227_n2_as
.Lx235_6:
 add rsp, 16
 jmp xchain227_n2_af
 xchain227_n5_β:
 jmp qword ptr [rsp]
proc_PAT$1_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$1_γ
proc_PAT$1_scanfail:
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
jmp proc_PAT$1_attempt
8:
jmp proc_PAT$1_ω
proc_PAT$1_res:
add rsp, 8
pop rbp
proc_PAT$1_β:
jmp qword ptr [rbp + 176]
proc_PAT$1_γ:
push rbp
lea rax, [rip + proc_PAT$1_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$1_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$2_α
proc_PAT$2_α:
#=======================================================================================================================
    .global proc_PAT$2_α
    .global proc_PAT$2_β
    .global proc_PAT$2_γ
    .global proc_PAT$2_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
mov qword ptr [rbp + 448], r8
mov dword ptr [rbp + 440], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
lea rax, [rip + xchain236_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain236_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain236_n1_α
xchain236_n0_as:
 jmp proc_PAT$2_γ
 xchain236_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain236_n1_β
 jmp xchain236_n2_β
xchain236_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain236_n2_α
 jmp proc_PAT$2_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain236_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain236_n3_α
xchain236_n1_as:
 jmp xchain236_n0_as
 xchain236_n1_β:
 jmp xchain236_n6_β
xchain236_n1_af:
 jmp xchain236_n0_af
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain236_n2_α:
 mov dword ptr [rbp + 336], r14d
 jmp xchain236_n7_α
xchain236_n2_as:
 jmp xchain236_n0_as
 xchain236_n2_β:
 jmp xchain236_n8_β
xchain236_n2_af:
 jmp xchain236_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx243_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx243_10
.Lx243_9:
 xor eax, eax
.Lx243_10:
 test rax, rax
 jz .Lx243_0
 mov r8d, 0
 lea rcx, [rip + .Lx243_4]
 lea rdx, [rip + .Lx243_5]
 jmp rax
.Lx243_4:
 jmp xchain236_n4_α
.Lx243_5:
 jmp xchain236_n1_af
.Lx243_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx243_2:
 test rax, rax
 je .Lx243_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx243_7]
 lea rdx, [rip + .Lx243_8]
 jmp rax
.Lx243_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx243_2
.Lx243_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx243_2
.Lx243_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx243_6]
 sub rsp, 8
 push rax
 jmp xchain236_n4_α
.Lx243_6:
 add rsp, 16
 jmp xchain236_n1_af
 xchain236_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain236_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain236_n3_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 46
 jne xchain236_n3_β
 add r14d, 1
 jmp xchain236_n5_α
 xchain236_n4_β:
 sub r14d, 1
 jmp xchain236_n3_β
# IR_MATCH_ALT_NARY
 xchain236_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain236_n9_α
xchain236_n5_as:
 jmp xchain236_n6_α
 xchain236_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain236_n9_β
 jmp xchain236_n10_β
xchain236_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain236_n10_α
 jmp xchain236_n4_β
# IR_MATCH_ALT_NARY
 xchain236_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain236_n11_α
xchain236_n6_as:
 jmp xchain236_n1_as
 xchain236_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain236_n11_β
 jmp xchain236_n12_β
xchain236_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain236_n12_α
 jmp xchain236_n5_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n7_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx250_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx250_10
.Lx250_9:
 xor eax, eax
.Lx250_10:
 test rax, rax
 jz .Lx250_0
 mov r8d, 0
 lea rcx, [rip + .Lx250_4]
 lea rdx, [rip + .Lx250_5]
 jmp rax
.Lx250_4:
 jmp xchain236_n8_α
.Lx250_5:
 jmp xchain236_n2_af
.Lx250_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx250_2:
 test rax, rax
 je .Lx250_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx250_7]
 lea rdx, [rip + .Lx250_8]
 jmp rax
.Lx250_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx250_2
.Lx250_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx250_2
.Lx250_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx250_6]
 sub rsp, 8
 push rax
 jmp xchain236_n8_α
.Lx250_6:
 add rsp, 16
 jmp xchain236_n2_af
 xchain236_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n8_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx251_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx251_10
.Lx251_9:
 xor eax, eax
.Lx251_10:
 test rax, rax
 jz .Lx251_0
 mov r8d, 0
 lea rcx, [rip + .Lx251_4]
 lea rdx, [rip + .Lx251_5]
 jmp rax
.Lx251_4:
 jmp xchain236_n2_as
.Lx251_5:
 jmp xchain236_n7_β
.Lx251_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx251_2:
 test rax, rax
 je .Lx251_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx251_7]
 lea rdx, [rip + .Lx251_8]
 jmp rax
.Lx251_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx251_2
.Lx251_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx251_2
.Lx251_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx251_6]
 sub rsp, 8
 push rax
 jmp xchain236_n2_as
.Lx251_6:
 add rsp, 16
 jmp xchain236_n7_β
 xchain236_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n9_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx252_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx252_10
.Lx252_9:
 xor eax, eax
.Lx252_10:
 test rax, rax
 jz .Lx252_0
 mov r8d, 0
 lea rcx, [rip + .Lx252_4]
 lea rdx, [rip + .Lx252_5]
 jmp rax
.Lx252_4:
 jmp xchain236_n5_as
.Lx252_5:
 jmp xchain236_n5_af
.Lx252_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx252_2:
 test rax, rax
 je .Lx252_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx252_7]
 lea rdx, [rip + .Lx252_8]
 jmp rax
.Lx252_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx252_2
.Lx252_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx252_2
.Lx252_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx252_6]
 sub rsp, 8
 push rax
 jmp xchain236_n5_as
.Lx252_6:
 add rsp, 16
 jmp xchain236_n5_af
 xchain236_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n10_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx253_0
 mov r8d, 0
 lea rcx, [rip + .Lx253_4]
 lea rdx, [rip + .Lx253_5]
 jmp rax
.Lx253_4:
 jmp xchain236_n5_as
.Lx253_5:
 jmp xchain236_n5_af
.Lx253_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx253_2:
 test rax, rax
 je .Lx253_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx253_7]
 lea rdx, [rip + .Lx253_8]
 jmp rax
.Lx253_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx253_2
.Lx253_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx253_2
.Lx253_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx253_6]
 sub rsp, 8
 push rax
 jmp xchain236_n5_as
.Lx253_6:
 add rsp, 16
 jmp xchain236_n5_af
 xchain236_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n11_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx254_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx254_10
.Lx254_9:
 xor eax, eax
.Lx254_10:
 test rax, rax
 jz .Lx254_0
 mov r8d, 0
 lea rcx, [rip + .Lx254_4]
 lea rdx, [rip + .Lx254_5]
 jmp rax
.Lx254_4:
 jmp xchain236_n6_as
.Lx254_5:
 jmp xchain236_n6_af
.Lx254_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx254_2:
 test rax, rax
 je .Lx254_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx254_7]
 lea rdx, [rip + .Lx254_8]
 jmp rax
.Lx254_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx254_2
.Lx254_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx254_2
.Lx254_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx254_6]
 sub rsp, 8
 push rax
 jmp xchain236_n6_as
.Lx254_6:
 add rsp, 16
 jmp xchain236_n6_af
 xchain236_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain236_n12_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx255_0
 mov r8d, 0
 lea rcx, [rip + .Lx255_4]
 lea rdx, [rip + .Lx255_5]
 jmp rax
.Lx255_4:
 jmp xchain236_n6_as
.Lx255_5:
 jmp xchain236_n6_af
.Lx255_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx255_2:
 test rax, rax
 je .Lx255_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx255_7]
 lea rdx, [rip + .Lx255_8]
 jmp rax
.Lx255_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx255_2
.Lx255_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx255_2
.Lx255_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain236_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx255_6]
 sub rsp, 8
 push rax
 jmp xchain236_n6_as
.Lx255_6:
 add rsp, 16
 jmp xchain236_n6_af
 xchain236_n12_β:
 jmp qword ptr [rsp]
proc_PAT$2_scanhit:
cmp qword ptr [rbp + 448], 1
jne 7f
mov ecx, dword ptr [rbp + 440]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$2_γ
proc_PAT$2_scanfail:
cmp qword ptr [rbp + 448], 1
jne 8f
mov eax, dword ptr [rbp + 440]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 440], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$2_attempt
8:
jmp proc_PAT$2_ω
proc_PAT$2_res:
add rsp, 8
pop rbp
proc_PAT$2_β:
jmp qword ptr [rbp + 416]
proc_PAT$2_γ:
push rbp
lea rax, [rip + proc_PAT$2_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$2_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
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
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$3_attempt:
proc_PAT$3_α_body:
lea rax, [rip + xchain256_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain256_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain256_n1_α
 xchain256_n0_β:
 add rsp, 16
 jmp proc_PAT$3_scanfail
# IR_MATCH_ANY
 xchain256_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx259_239
 add rsp, 16
 jmp proc_PAT$3_scanfail
.Lx259_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx259_240
 add rsp, 16
 jmp proc_PAT$3_scanfail
.Lx259_240:
 add r14d, 1
 jmp xchain256_n2_α
 xchain256_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$3_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain256_n2_α:
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
 xchain256_n2_β:
 sub r12, 24
 jmp xchain256_n1_β
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
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$4_attempt:
proc_PAT$4_α_body:
lea rax, [rip + xchain262_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain262_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain262_n1_α
 xchain262_n0_β:
 add rsp, 16
 jmp proc_PAT$4_scanfail
# IR_MATCH_ANY
 xchain262_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx265_239
 add rsp, 16
 jmp proc_PAT$4_scanfail
.Lx265_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C3]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx265_240
 add rsp, 16
 jmp proc_PAT$4_scanfail
.Lx265_240:
 add r14d, 1
 jmp xchain262_n2_α
 xchain262_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$4_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain262_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S4]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$4_scanhit
 xchain262_n2_β:
 sub r12, 24
 jmp xchain262_n1_β
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
  sub rsp, 224
  mov [rsp + 200], rcx
  mov [rsp + 208], rdx
  mov [rsp + 216], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 192
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 184], rsp
mov qword ptr [rbp + 192], r8
mov dword ptr [rbp + 184], r14d
proc_PAT$5_attempt:
proc_PAT$5_α_body:
lea rax, [rip + xchain268_n2_β]
mov qword ptr [rbp + 160], rax
# IR_MATCH_CAPTURE_SAVE push
 xchain268_n0_α:
 lea rdi, [rbp + 48]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain268_n1_α
 xchain268_n0_β:
 lea rdi, [rbp + 48]
 call rt_cap_pop@PLT
 jmp proc_PAT$5_scanfail
# IR_MATCH_ALT_NARY
 xchain268_n1_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain268_n3_α
xchain268_n1_as:
 jmp xchain268_n2_α
 xchain268_n1_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain268_n3_β
 jmp xchain268_n4_β
xchain268_n1_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain268_n4_α
 jmp xchain268_n0_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain268_n2_α:
 lea rdi, [rbp + 48]
 call rt_cap_top@PLT
 lea rcx, [rip + .S5]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$5_scanhit
 xchain268_n2_β:
 sub r12, 24
 jmp xchain268_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain268_n3_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 cmp eax, 3
 jne .Lx275_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx275_10
.Lx275_9:
 xor eax, eax
.Lx275_10:
 test rax, rax
 jz .Lx275_0
 mov r8d, 0
 lea rcx, [rip + .Lx275_4]
 lea rdx, [rip + .Lx275_5]
 jmp rax
.Lx275_4:
 jmp xchain268_n1_as
.Lx275_5:
 jmp xchain268_n1_af
.Lx275_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx275_2:
 test rax, rax
 je .Lx275_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx275_7]
 lea rdx, [rip + .Lx275_8]
 jmp rax
.Lx275_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx275_2
.Lx275_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx275_2
.Lx275_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain268_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx275_6]
 sub rsp, 8
 push rax
 jmp xchain268_n1_as
.Lx275_6:
 add rsp, 16
 jmp xchain268_n1_af
 xchain268_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain268_n4_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx276_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx276_10
.Lx276_9:
 xor eax, eax
.Lx276_10:
 test rax, rax
 jz .Lx276_0
 mov r8d, 0
 lea rcx, [rip + .Lx276_4]
 lea rdx, [rip + .Lx276_5]
 jmp rax
.Lx276_4:
 jmp xchain268_n1_as
.Lx276_5:
 jmp xchain268_n1_af
.Lx276_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx276_2:
 test rax, rax
 je .Lx276_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx276_7]
 lea rdx, [rip + .Lx276_8]
 jmp rax
.Lx276_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx276_2
.Lx276_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx276_2
.Lx276_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain268_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx276_6]
 sub rsp, 8
 push rax
 jmp xchain268_n1_as
.Lx276_6:
 add rsp, 16
 jmp xchain268_n1_af
 xchain268_n4_β:
 jmp qword ptr [rsp]
proc_PAT$5_scanhit:
cmp qword ptr [rbp + 192], 1
jne 7f
mov ecx, dword ptr [rbp + 184]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$5_γ
proc_PAT$5_scanfail:
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
jmp proc_PAT$5_attempt
8:
jmp proc_PAT$5_ω
proc_PAT$5_res:
add rsp, 8
pop rbp
proc_PAT$5_β:
jmp qword ptr [rbp + 160]
proc_PAT$5_γ:
push rbp
lea rax, [rip + proc_PAT$5_res]
push rax
mov rax, [rbp + 200]
mov rbp, [rbp + 216]
jmp rax
proc_PAT$5_ω:
mov rax, [rbp + 208]
lea rsp, [rbp + 224]
mov rbp, [rbp + 216]
jmp rax
  .globl proc_PAT$6_α
proc_PAT$6_α:
#=======================================================================================================================
    .global proc_PAT$6_α
    .global proc_PAT$6_β
    .global proc_PAT$6_γ
    .global proc_PAT$6_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
lea rax, [rip + xchain277_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain277_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain277_n1_α
xchain277_n0_as:
 jmp proc_PAT$6_γ
 xchain277_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain277_n1_β
 jmp xchain277_n2_β
xchain277_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain277_n2_α
 jmp proc_PAT$6_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain277_n1_α:
 mov rax, qword ptr [1879052544]
 mov rdx, qword ptr [1879052552]
 cmp eax, 3
 jne .Lx280_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx280_10
.Lx280_9:
 xor eax, eax
.Lx280_10:
 test rax, rax
 jz .Lx280_0
 mov r8d, 0
 lea rcx, [rip + .Lx280_4]
 lea rdx, [rip + .Lx280_5]
 jmp rax
.Lx280_4:
 jmp xchain277_n0_as
.Lx280_5:
 jmp xchain277_n0_af
.Lx280_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx280_2:
 test rax, rax
 je .Lx280_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx280_7]
 lea rdx, [rip + .Lx280_8]
 jmp rax
.Lx280_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx280_2
.Lx280_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx280_2
.Lx280_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain277_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx280_6]
 sub rsp, 8
 push rax
 jmp xchain277_n0_as
.Lx280_6:
 add rsp, 16
 jmp xchain277_n0_af
 xchain277_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain277_n2_α:
 mov dword ptr [rbp + 96], r14d
 jmp xchain277_n3_α
xchain277_n2_as:
 jmp xchain277_n0_as
 xchain277_n2_β:
 jmp xchain277_n5_β
xchain277_n2_af:
 jmp xchain277_n0_af
# IR_MATCH_LIT
 xchain277_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain277_n2_af
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 40
 jne xchain277_n2_af
 add r14d, 1
 jmp xchain277_n4_α
 xchain277_n3_β:
 sub r14d, 1
 jmp xchain277_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain277_n4_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx285_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx285_10
.Lx285_9:
 xor eax, eax
.Lx285_10:
 test rax, rax
 jz .Lx285_0
 mov r8d, 0
 lea rcx, [rip + .Lx285_4]
 lea rdx, [rip + .Lx285_5]
 jmp rax
.Lx285_4:
 jmp xchain277_n5_α
.Lx285_5:
 jmp xchain277_n3_β
.Lx285_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx285_2:
 test rax, rax
 je .Lx285_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx285_7]
 lea rdx, [rip + .Lx285_8]
 jmp rax
.Lx285_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx285_2
.Lx285_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx285_2
.Lx285_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain277_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx285_6]
 sub rsp, 8
 push rax
 jmp xchain277_n5_α
.Lx285_6:
 add rsp, 16
 jmp xchain277_n3_β
 xchain277_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain277_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain277_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 41
 jne xchain277_n4_β
 add r14d, 1
 jmp xchain277_n2_as
 xchain277_n5_β:
 sub r14d, 1
 jmp xchain277_n4_β
proc_PAT$6_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$6_γ
proc_PAT$6_scanfail:
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
jmp proc_PAT$6_attempt
8:
jmp proc_PAT$6_ω
proc_PAT$6_res:
add rsp, 8
pop rbp
proc_PAT$6_β:
jmp qword ptr [rbp + 176]
proc_PAT$6_γ:
push rbp
lea rax, [rip + proc_PAT$6_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$6_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$7_α
proc_PAT$7_α:
#=======================================================================================================================
    .global proc_PAT$7_α
    .global proc_PAT$7_β
    .global proc_PAT$7_γ
    .global proc_PAT$7_ω
  sub rsp, 288
  mov [rsp + 264], rcx
  mov [rsp + 272], rdx
  mov [rsp + 280], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 256
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 248], rsp
mov qword ptr [rbp + 256], r8
mov dword ptr [rbp + 248], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
lea rax, [rip + xchain288_n0_β]
mov qword ptr [rbp + 224], rax
# IR_MATCH_ALT_NARY
 xchain288_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain288_n1_α
xchain288_n0_as:
 jmp proc_PAT$7_γ
 xchain288_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain288_n1_β
 jmp xchain288_n2_β
xchain288_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain288_n2_α
 jmp proc_PAT$7_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain288_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain288_n3_α
xchain288_n1_as:
 jmp xchain288_n0_as
 xchain288_n1_β:
 jmp xchain288_n5_β
xchain288_n1_af:
 jmp xchain288_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain288_n2_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 cmp eax, 3
 jne .Lx293_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx293_10
.Lx293_9:
 xor eax, eax
.Lx293_10:
 test rax, rax
 jz .Lx293_0
 mov r8d, 0
 lea rcx, [rip + .Lx293_4]
 lea rdx, [rip + .Lx293_5]
 jmp rax
.Lx293_4:
 jmp xchain288_n0_as
.Lx293_5:
 jmp xchain288_n0_af
.Lx293_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx293_2:
 test rax, rax
 je .Lx293_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx293_7]
 lea rdx, [rip + .Lx293_8]
 jmp rax
.Lx293_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx293_2
.Lx293_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx293_2
.Lx293_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain288_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx293_6]
 sub rsp, 8
 push rax
 jmp xchain288_n0_as
.Lx293_6:
 add rsp, 16
 jmp xchain288_n0_af
 xchain288_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain288_n3_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx294_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx294_10
.Lx294_9:
 xor eax, eax
.Lx294_10:
 test rax, rax
 jz .Lx294_0
 mov r8d, 0
 lea rcx, [rip + .Lx294_4]
 lea rdx, [rip + .Lx294_5]
 jmp rax
.Lx294_4:
 jmp xchain288_n4_α
.Lx294_5:
 jmp xchain288_n1_af
.Lx294_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx294_2:
 test rax, rax
 je .Lx294_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx294_7]
 lea rdx, [rip + .Lx294_8]
 jmp rax
.Lx294_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx294_2
.Lx294_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx294_2
.Lx294_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain288_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx294_6]
 sub rsp, 8
 push rax
 jmp xchain288_n4_α
.Lx294_6:
 add rsp, 16
 jmp xchain288_n1_af
 xchain288_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain288_n4_α:
 lea rdi, [rbp + 144]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain288_n6_α
 xchain288_n4_β:
 lea rdi, [rbp + 144]
 call rt_cap_pop@PLT
 jmp xchain288_n3_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain288_n5_α:
 lea rdi, [rbp + 144]
 call rt_cap_top@PLT
 lea rcx, [rip + .S11]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain288_n1_as
 xchain288_n5_β:
 sub r12, 24
 jmp xchain288_n6_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain288_n6_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx299_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx299_10
.Lx299_9:
 xor eax, eax
.Lx299_10:
 test rax, rax
 jz .Lx299_0
 mov r8d, 0
 lea rcx, [rip + .Lx299_4]
 lea rdx, [rip + .Lx299_5]
 jmp rax
.Lx299_4:
 jmp xchain288_n5_α
.Lx299_5:
 jmp xchain288_n4_β
.Lx299_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx299_2:
 test rax, rax
 je .Lx299_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx299_7]
 lea rdx, [rip + .Lx299_8]
 jmp rax
.Lx299_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx299_2
.Lx299_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx299_2
.Lx299_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain288_n4_β
 mov r14d, eax
 lea rax, [rip + .Lx299_6]
 sub rsp, 8
 push rax
 jmp xchain288_n5_α
.Lx299_6:
 add rsp, 16
 jmp xchain288_n4_β
 xchain288_n6_β:
 jmp qword ptr [rsp]
proc_PAT$7_scanhit:
cmp qword ptr [rbp + 256], 1
jne 7f
mov ecx, dword ptr [rbp + 248]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$7_γ
proc_PAT$7_scanfail:
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
jmp proc_PAT$7_attempt
8:
jmp proc_PAT$7_ω
proc_PAT$7_res:
add rsp, 8
pop rbp
proc_PAT$7_β:
jmp qword ptr [rbp + 224]
proc_PAT$7_γ:
push rbp
lea rax, [rip + proc_PAT$7_res]
push rax
mov rax, [rbp + 264]
mov rbp, [rbp + 280]
jmp rax
proc_PAT$7_ω:
mov rax, [rbp + 272]
lea rsp, [rbp + 288]
mov rbp, [rbp + 280]
jmp rax
  .globl proc_PAT$8_α
proc_PAT$8_α:
#=======================================================================================================================
    .global proc_PAT$8_α
    .global proc_PAT$8_β
    .global proc_PAT$8_γ
    .global proc_PAT$8_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
lea rax, [rip + xchain300_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain300_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain300_n1_α
xchain300_n0_as:
 jmp proc_PAT$8_γ
 xchain300_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain300_n1_β
 jmp xchain300_n2_β
xchain300_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain300_n2_α
 jmp proc_PAT$8_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain300_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain300_n3_α
xchain300_n1_as:
 jmp xchain300_n0_as
 xchain300_n1_β:
 jmp xchain300_n6_β
xchain300_n1_af:
 jmp xchain300_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain300_n2_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx305_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx305_10
.Lx305_9:
 xor eax, eax
.Lx305_10:
 test rax, rax
 jz .Lx305_0
 mov r8d, 0
 lea rcx, [rip + .Lx305_4]
 lea rdx, [rip + .Lx305_5]
 jmp rax
.Lx305_4:
 jmp xchain300_n0_as
.Lx305_5:
 jmp xchain300_n0_af
.Lx305_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx305_2:
 test rax, rax
 je .Lx305_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx305_7]
 lea rdx, [rip + .Lx305_8]
 jmp rax
.Lx305_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx305_2
.Lx305_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx305_2
.Lx305_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain300_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx305_6]
 sub rsp, 8
 push rax
 jmp xchain300_n0_as
.Lx305_6:
 add rsp, 16
 jmp xchain300_n0_af
 xchain300_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain300_n3_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx306_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx306_10
.Lx306_9:
 xor eax, eax
.Lx306_10:
 test rax, rax
 jz .Lx306_0
 mov r8d, 0
 lea rcx, [rip + .Lx306_4]
 lea rdx, [rip + .Lx306_5]
 jmp rax
.Lx306_4:
 jmp xchain300_n4_α
.Lx306_5:
 jmp xchain300_n1_af
.Lx306_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx306_2:
 test rax, rax
 je .Lx306_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx306_7]
 lea rdx, [rip + .Lx306_8]
 jmp rax
.Lx306_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx306_2
.Lx306_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx306_2
.Lx306_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain300_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx306_6]
 sub rsp, 8
 push rax
 jmp xchain300_n4_α
.Lx306_6:
 add rsp, 16
 jmp xchain300_n1_af
 xchain300_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain300_n4_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 cmp eax, 3
 jne .Lx307_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx307_10
.Lx307_9:
 xor eax, eax
.Lx307_10:
 test rax, rax
 jz .Lx307_0
 mov r8d, 0
 lea rcx, [rip + .Lx307_4]
 lea rdx, [rip + .Lx307_5]
 jmp rax
.Lx307_4:
 jmp xchain300_n5_α
.Lx307_5:
 jmp xchain300_n3_β
.Lx307_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx307_2:
 test rax, rax
 je .Lx307_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx307_7]
 lea rdx, [rip + .Lx307_8]
 jmp rax
.Lx307_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx307_2
.Lx307_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx307_2
.Lx307_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain300_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx307_6]
 sub rsp, 8
 push rax
 jmp xchain300_n5_α
.Lx307_6:
 add rsp, 16
 jmp xchain300_n3_β
 xchain300_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain300_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain300_n7_α
 xchain300_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain300_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain300_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S14]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain300_n1_as
 xchain300_n6_β:
 sub r12, 24
 jmp xchain300_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain300_n7_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx312_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx312_10
.Lx312_9:
 xor eax, eax
.Lx312_10:
 test rax, rax
 jz .Lx312_0
 mov r8d, 0
 lea rcx, [rip + .Lx312_4]
 lea rdx, [rip + .Lx312_5]
 jmp rax
.Lx312_4:
 jmp xchain300_n6_α
.Lx312_5:
 jmp xchain300_n5_β
.Lx312_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx312_2:
 test rax, rax
 je .Lx312_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx312_7]
 lea rdx, [rip + .Lx312_8]
 jmp rax
.Lx312_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx312_2
.Lx312_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx312_2
.Lx312_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain300_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx312_6]
 sub rsp, 8
 push rax
 jmp xchain300_n6_α
.Lx312_6:
 add rsp, 16
 jmp xchain300_n5_β
 xchain300_n7_β:
 jmp qword ptr [rsp]
proc_PAT$8_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$8_γ
proc_PAT$8_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$8_attempt
8:
jmp proc_PAT$8_ω
proc_PAT$8_res:
add rsp, 8
pop rbp
proc_PAT$8_β:
jmp qword ptr [rbp + 256]
proc_PAT$8_γ:
push rbp
lea rax, [rip + proc_PAT$8_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$8_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$9_α
proc_PAT$9_α:
#=======================================================================================================================
    .global proc_PAT$9_α
    .global proc_PAT$9_β
    .global proc_PAT$9_γ
    .global proc_PAT$9_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
lea rax, [rip + xchain313_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain313_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain313_n1_α
xchain313_n0_as:
 jmp proc_PAT$9_γ
 xchain313_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain313_n1_β
 jmp xchain313_n2_β
xchain313_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain313_n2_α
 jmp proc_PAT$9_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain313_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain313_n3_α
xchain313_n1_as:
 jmp xchain313_n0_as
 xchain313_n1_β:
 jmp xchain313_n6_β
xchain313_n1_af:
 jmp xchain313_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain313_n2_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx318_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx318_10
.Lx318_9:
 xor eax, eax
.Lx318_10:
 test rax, rax
 jz .Lx318_0
 mov r8d, 0
 lea rcx, [rip + .Lx318_4]
 lea rdx, [rip + .Lx318_5]
 jmp rax
.Lx318_4:
 jmp xchain313_n0_as
.Lx318_5:
 jmp xchain313_n0_af
.Lx318_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx318_2:
 test rax, rax
 je .Lx318_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx318_7]
 lea rdx, [rip + .Lx318_8]
 jmp rax
.Lx318_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx318_2
.Lx318_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx318_2
.Lx318_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain313_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx318_6]
 sub rsp, 8
 push rax
 jmp xchain313_n0_as
.Lx318_6:
 add rsp, 16
 jmp xchain313_n0_af
 xchain313_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain313_n3_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx319_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx319_10
.Lx319_9:
 xor eax, eax
.Lx319_10:
 test rax, rax
 jz .Lx319_0
 mov r8d, 0
 lea rcx, [rip + .Lx319_4]
 lea rdx, [rip + .Lx319_5]
 jmp rax
.Lx319_4:
 jmp xchain313_n4_α
.Lx319_5:
 jmp xchain313_n1_af
.Lx319_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx319_2:
 test rax, rax
 je .Lx319_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx319_7]
 lea rdx, [rip + .Lx319_8]
 jmp rax
.Lx319_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx319_2
.Lx319_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx319_2
.Lx319_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain313_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx319_6]
 sub rsp, 8
 push rax
 jmp xchain313_n4_α
.Lx319_6:
 add rsp, 16
 jmp xchain313_n1_af
 xchain313_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain313_n4_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx320_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx320_10
.Lx320_9:
 xor eax, eax
.Lx320_10:
 test rax, rax
 jz .Lx320_0
 mov r8d, 0
 lea rcx, [rip + .Lx320_4]
 lea rdx, [rip + .Lx320_5]
 jmp rax
.Lx320_4:
 jmp xchain313_n5_α
.Lx320_5:
 jmp xchain313_n3_β
.Lx320_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx320_2:
 test rax, rax
 je .Lx320_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx320_7]
 lea rdx, [rip + .Lx320_8]
 jmp rax
.Lx320_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx320_2
.Lx320_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx320_2
.Lx320_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain313_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx320_6]
 sub rsp, 8
 push rax
 jmp xchain313_n5_α
.Lx320_6:
 add rsp, 16
 jmp xchain313_n3_β
 xchain313_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain313_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain313_n7_α
 xchain313_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain313_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain313_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S16]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain313_n1_as
 xchain313_n6_β:
 sub r12, 24
 jmp xchain313_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain313_n7_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx325_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx325_10
.Lx325_9:
 xor eax, eax
.Lx325_10:
 test rax, rax
 jz .Lx325_0
 mov r8d, 0
 lea rcx, [rip + .Lx325_4]
 lea rdx, [rip + .Lx325_5]
 jmp rax
.Lx325_4:
 jmp xchain313_n6_α
.Lx325_5:
 jmp xchain313_n5_β
.Lx325_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx325_2:
 test rax, rax
 je .Lx325_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx325_7]
 lea rdx, [rip + .Lx325_8]
 jmp rax
.Lx325_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx325_2
.Lx325_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx325_2
.Lx325_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain313_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx325_6]
 sub rsp, 8
 push rax
 jmp xchain313_n6_α
.Lx325_6:
 add rsp, 16
 jmp xchain313_n5_β
 xchain313_n7_β:
 jmp qword ptr [rsp]
proc_PAT$9_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$9_γ
proc_PAT$9_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$9_attempt
8:
jmp proc_PAT$9_ω
proc_PAT$9_res:
add rsp, 8
pop rbp
proc_PAT$9_β:
jmp qword ptr [rbp + 256]
proc_PAT$9_γ:
push rbp
lea rax, [rip + proc_PAT$9_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$9_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$10_α
proc_PAT$10_α:
#=======================================================================================================================
    .global proc_PAT$10_α
    .global proc_PAT$10_β
    .global proc_PAT$10_γ
    .global proc_PAT$10_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
mov qword ptr [rbp + 80], r8
mov dword ptr [rbp + 72], r14d
proc_PAT$10_attempt:
proc_PAT$10_α_body:
lea rax, [rip + xchain326_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain326_n0_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx328_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx328_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx328_1
 add dword ptr [rsp + 0], 1
 jmp .Lx328_0
.Lx328_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx328_240
 add rsp, 16
 jmp proc_PAT$10_scanfail
.Lx328_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$10_scanhit
 xchain326_n0_β:
 xchain326_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$10_scanfail
proc_PAT$10_scanhit:
cmp qword ptr [rbp + 80], 1
jne 7f
mov ecx, dword ptr [rbp + 72]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$10_γ
proc_PAT$10_scanfail:
cmp qword ptr [rbp + 80], 1
jne 8f
mov eax, dword ptr [rbp + 72]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 72], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$10_attempt
8:
jmp proc_PAT$10_ω
proc_PAT$10_res:
add rsp, 8
pop rbp
proc_PAT$10_β:
jmp qword ptr [rbp + 48]
proc_PAT$10_γ:
push rbp
lea rax, [rip + proc_PAT$10_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$10_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$11_α
proc_PAT$11_α:
#=======================================================================================================================
    .global proc_PAT$11_α
    .global proc_PAT$11_β
    .global proc_PAT$11_γ
    .global proc_PAT$11_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$11_attempt:
proc_PAT$11_α_body:
lea rax, [rip + xchain329_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain329_n0_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain329_n1_α
xchain329_n0_as:
 jmp proc_PAT$11_γ
 xchain329_n0_β:
 jmp xchain329_n3_β
xchain329_n0_af:
 jmp proc_PAT$11_ω
# IR_MATCH_ANY
 xchain329_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain329_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C1]
 cmp byte ptr [rdi+rsi], 0
 je xchain329_n0_af
 add r14d, 1
 jmp xchain329_n2_α
 xchain329_n1_β:
 sub r14d, 1
 jmp xchain329_n0_af
# IR_MATCH_ALT_NARY
 xchain329_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain329_n4_α
xchain329_n2_as:
 jmp xchain329_n3_α
 xchain329_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain329_n4_β
 jmp xchain329_n5_β
xchain329_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain329_n5_α
 jmp xchain329_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain329_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx335_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx335_10
.Lx335_9:
 xor eax, eax
.Lx335_10:
 test rax, rax
 jz .Lx335_0
 mov r8d, 0
 lea rcx, [rip + .Lx335_4]
 lea rdx, [rip + .Lx335_5]
 jmp rax
.Lx335_4:
 jmp xchain329_n0_as
.Lx335_5:
 jmp xchain329_n2_β
.Lx335_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx335_2:
 test rax, rax
 je .Lx335_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx335_7]
 lea rdx, [rip + .Lx335_8]
 jmp rax
.Lx335_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx335_2
.Lx335_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx335_2
.Lx335_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain329_n2_β
 mov r14d, eax
 lea rax, [rip + .Lx335_6]
 sub rsp, 8
 push rax
 jmp xchain329_n0_as
.Lx335_6:
 add rsp, 16
 jmp xchain329_n2_β
 xchain329_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain329_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain329_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 je xchain329_n2_af
 add r14d, 1
 jmp xchain329_n2_as
 xchain329_n4_β:
 sub r14d, 1
 jmp xchain329_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain329_n5_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx337_0
 mov r8d, 0
 lea rcx, [rip + .Lx337_4]
 lea rdx, [rip + .Lx337_5]
 jmp rax
.Lx337_4:
 jmp xchain329_n2_as
.Lx337_5:
 jmp xchain329_n2_af
.Lx337_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx337_2:
 test rax, rax
 je .Lx337_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx337_7]
 lea rdx, [rip + .Lx337_8]
 jmp rax
.Lx337_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx337_2
.Lx337_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx337_2
.Lx337_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain329_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx337_6]
 sub rsp, 8
 push rax
 jmp xchain329_n2_as
.Lx337_6:
 add rsp, 16
 jmp xchain329_n2_af
 xchain329_n5_β:
 jmp qword ptr [rsp]
proc_PAT$11_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$11_γ
proc_PAT$11_scanfail:
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
jmp proc_PAT$11_attempt
8:
jmp proc_PAT$11_ω
proc_PAT$11_res:
add rsp, 8
pop rbp
proc_PAT$11_β:
jmp qword ptr [rbp + 176]
proc_PAT$11_γ:
push rbp
lea rax, [rip + proc_PAT$11_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$11_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$12_α
proc_PAT$12_α:
#=======================================================================================================================
    .global proc_PAT$12_α
    .global proc_PAT$12_β
    .global proc_PAT$12_γ
    .global proc_PAT$12_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
mov qword ptr [rbp + 448], r8
mov dword ptr [rbp + 440], r14d
proc_PAT$12_attempt:
proc_PAT$12_α_body:
lea rax, [rip + xchain338_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain338_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain338_n1_α
xchain338_n0_as:
 jmp proc_PAT$12_γ
 xchain338_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain338_n1_β
 jmp xchain338_n2_β
xchain338_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain338_n2_α
 jmp proc_PAT$12_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain338_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain338_n3_α
xchain338_n1_as:
 jmp xchain338_n0_as
 xchain338_n1_β:
 jmp xchain338_n6_β
xchain338_n1_af:
 jmp xchain338_n0_af
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain338_n2_α:
 mov dword ptr [rbp + 336], r14d
 jmp xchain338_n7_α
xchain338_n2_as:
 jmp xchain338_n0_as
 xchain338_n2_β:
 jmp xchain338_n8_β
xchain338_n2_af:
 jmp xchain338_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx345_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx345_10
.Lx345_9:
 xor eax, eax
.Lx345_10:
 test rax, rax
 jz .Lx345_0
 mov r8d, 0
 lea rcx, [rip + .Lx345_4]
 lea rdx, [rip + .Lx345_5]
 jmp rax
.Lx345_4:
 jmp xchain338_n4_α
.Lx345_5:
 jmp xchain338_n1_af
.Lx345_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx345_2:
 test rax, rax
 je .Lx345_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx345_7]
 lea rdx, [rip + .Lx345_8]
 jmp rax
.Lx345_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx345_2
.Lx345_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx345_2
.Lx345_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx345_6]
 sub rsp, 8
 push rax
 jmp xchain338_n4_α
.Lx345_6:
 add rsp, 16
 jmp xchain338_n1_af
 xchain338_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain338_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain338_n3_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 46
 jne xchain338_n3_β
 add r14d, 1
 jmp xchain338_n5_α
 xchain338_n4_β:
 sub r14d, 1
 jmp xchain338_n3_β
# IR_MATCH_ALT_NARY
 xchain338_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain338_n9_α
xchain338_n5_as:
 jmp xchain338_n6_α
 xchain338_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain338_n9_β
 jmp xchain338_n10_β
xchain338_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain338_n10_α
 jmp xchain338_n4_β
# IR_MATCH_ALT_NARY
 xchain338_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain338_n11_α
xchain338_n6_as:
 jmp xchain338_n1_as
 xchain338_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain338_n11_β
 jmp xchain338_n12_β
xchain338_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain338_n12_α
 jmp xchain338_n5_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n7_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx352_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx352_10
.Lx352_9:
 xor eax, eax
.Lx352_10:
 test rax, rax
 jz .Lx352_0
 mov r8d, 0
 lea rcx, [rip + .Lx352_4]
 lea rdx, [rip + .Lx352_5]
 jmp rax
.Lx352_4:
 jmp xchain338_n8_α
.Lx352_5:
 jmp xchain338_n2_af
.Lx352_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx352_2:
 test rax, rax
 je .Lx352_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx352_7]
 lea rdx, [rip + .Lx352_8]
 jmp rax
.Lx352_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx352_2
.Lx352_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx352_2
.Lx352_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx352_6]
 sub rsp, 8
 push rax
 jmp xchain338_n8_α
.Lx352_6:
 add rsp, 16
 jmp xchain338_n2_af
 xchain338_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n8_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx353_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx353_10
.Lx353_9:
 xor eax, eax
.Lx353_10:
 test rax, rax
 jz .Lx353_0
 mov r8d, 0
 lea rcx, [rip + .Lx353_4]
 lea rdx, [rip + .Lx353_5]
 jmp rax
.Lx353_4:
 jmp xchain338_n2_as
.Lx353_5:
 jmp xchain338_n7_β
.Lx353_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx353_2:
 test rax, rax
 je .Lx353_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx353_7]
 lea rdx, [rip + .Lx353_8]
 jmp rax
.Lx353_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx353_2
.Lx353_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx353_2
.Lx353_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx353_6]
 sub rsp, 8
 push rax
 jmp xchain338_n2_as
.Lx353_6:
 add rsp, 16
 jmp xchain338_n7_β
 xchain338_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n9_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx354_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx354_10
.Lx354_9:
 xor eax, eax
.Lx354_10:
 test rax, rax
 jz .Lx354_0
 mov r8d, 0
 lea rcx, [rip + .Lx354_4]
 lea rdx, [rip + .Lx354_5]
 jmp rax
.Lx354_4:
 jmp xchain338_n5_as
.Lx354_5:
 jmp xchain338_n5_af
.Lx354_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx354_2:
 test rax, rax
 je .Lx354_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx354_7]
 lea rdx, [rip + .Lx354_8]
 jmp rax
.Lx354_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx354_2
.Lx354_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx354_2
.Lx354_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx354_6]
 sub rsp, 8
 push rax
 jmp xchain338_n5_as
.Lx354_6:
 add rsp, 16
 jmp xchain338_n5_af
 xchain338_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n10_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx355_0
 mov r8d, 0
 lea rcx, [rip + .Lx355_4]
 lea rdx, [rip + .Lx355_5]
 jmp rax
.Lx355_4:
 jmp xchain338_n5_as
.Lx355_5:
 jmp xchain338_n5_af
.Lx355_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx355_2:
 test rax, rax
 je .Lx355_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx355_7]
 lea rdx, [rip + .Lx355_8]
 jmp rax
.Lx355_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx355_2
.Lx355_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx355_2
.Lx355_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx355_6]
 sub rsp, 8
 push rax
 jmp xchain338_n5_as
.Lx355_6:
 add rsp, 16
 jmp xchain338_n5_af
 xchain338_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n11_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx356_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx356_10
.Lx356_9:
 xor eax, eax
.Lx356_10:
 test rax, rax
 jz .Lx356_0
 mov r8d, 0
 lea rcx, [rip + .Lx356_4]
 lea rdx, [rip + .Lx356_5]
 jmp rax
.Lx356_4:
 jmp xchain338_n6_as
.Lx356_5:
 jmp xchain338_n6_af
.Lx356_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx356_2:
 test rax, rax
 je .Lx356_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx356_7]
 lea rdx, [rip + .Lx356_8]
 jmp rax
.Lx356_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx356_2
.Lx356_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx356_2
.Lx356_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx356_6]
 sub rsp, 8
 push rax
 jmp xchain338_n6_as
.Lx356_6:
 add rsp, 16
 jmp xchain338_n6_af
 xchain338_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain338_n12_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx357_0
 mov r8d, 0
 lea rcx, [rip + .Lx357_4]
 lea rdx, [rip + .Lx357_5]
 jmp rax
.Lx357_4:
 jmp xchain338_n6_as
.Lx357_5:
 jmp xchain338_n6_af
.Lx357_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx357_2:
 test rax, rax
 je .Lx357_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx357_7]
 lea rdx, [rip + .Lx357_8]
 jmp rax
.Lx357_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx357_2
.Lx357_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx357_2
.Lx357_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain338_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx357_6]
 sub rsp, 8
 push rax
 jmp xchain338_n6_as
.Lx357_6:
 add rsp, 16
 jmp xchain338_n6_af
 xchain338_n12_β:
 jmp qword ptr [rsp]
proc_PAT$12_scanhit:
cmp qword ptr [rbp + 448], 1
jne 7f
mov ecx, dword ptr [rbp + 440]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$12_γ
proc_PAT$12_scanfail:
cmp qword ptr [rbp + 448], 1
jne 8f
mov eax, dword ptr [rbp + 440]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 440], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$12_attempt
8:
jmp proc_PAT$12_ω
proc_PAT$12_res:
add rsp, 8
pop rbp
proc_PAT$12_β:
jmp qword ptr [rbp + 416]
proc_PAT$12_γ:
push rbp
lea rax, [rip + proc_PAT$12_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$12_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$13_α
proc_PAT$13_α:
#=======================================================================================================================
    .global proc_PAT$13_α
    .global proc_PAT$13_β
    .global proc_PAT$13_γ
    .global proc_PAT$13_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$13_attempt:
proc_PAT$13_α_body:
lea rax, [rip + xchain358_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain358_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain358_n1_α
 xchain358_n0_β:
 add rsp, 16
 jmp proc_PAT$13_scanfail
# IR_MATCH_ANY
 xchain358_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx361_239
 add rsp, 16
 jmp proc_PAT$13_scanfail
.Lx361_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx361_240
 add rsp, 16
 jmp proc_PAT$13_scanfail
.Lx361_240:
 add r14d, 1
 jmp xchain358_n2_α
 xchain358_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$13_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain358_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S17]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$13_scanhit
 xchain358_n2_β:
 sub r12, 24
 jmp xchain358_n1_β
proc_PAT$13_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$13_γ
proc_PAT$13_scanfail:
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
jmp proc_PAT$13_attempt
8:
jmp proc_PAT$13_ω
proc_PAT$13_res:
add rsp, 8
pop rbp
proc_PAT$13_β:
jmp qword ptr [rbp + 80]
proc_PAT$13_γ:
push rbp
lea rax, [rip + proc_PAT$13_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$13_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$14_α
proc_PAT$14_α:
#=======================================================================================================================
    .global proc_PAT$14_α
    .global proc_PAT$14_β
    .global proc_PAT$14_γ
    .global proc_PAT$14_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$14_attempt:
proc_PAT$14_α_body:
lea rax, [rip + xchain364_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain364_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain364_n1_α
 xchain364_n0_β:
 add rsp, 16
 jmp proc_PAT$14_scanfail
# IR_MATCH_ANY
 xchain364_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx367_239
 add rsp, 16
 jmp proc_PAT$14_scanfail
.Lx367_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C3]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx367_240
 add rsp, 16
 jmp proc_PAT$14_scanfail
.Lx367_240:
 add r14d, 1
 jmp xchain364_n2_α
 xchain364_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$14_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain364_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S18]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$14_scanhit
 xchain364_n2_β:
 sub r12, 24
 jmp xchain364_n1_β
proc_PAT$14_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$14_γ
proc_PAT$14_scanfail:
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
jmp proc_PAT$14_attempt
8:
jmp proc_PAT$14_ω
proc_PAT$14_res:
add rsp, 8
pop rbp
proc_PAT$14_β:
jmp qword ptr [rbp + 80]
proc_PAT$14_γ:
push rbp
lea rax, [rip + proc_PAT$14_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$14_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$15_α
proc_PAT$15_α:
#=======================================================================================================================
    .global proc_PAT$15_α
    .global proc_PAT$15_β
    .global proc_PAT$15_γ
    .global proc_PAT$15_ω
  sub rsp, 224
  mov [rsp + 200], rcx
  mov [rsp + 208], rdx
  mov [rsp + 216], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 192
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 184], rsp
mov qword ptr [rbp + 192], r8
mov dword ptr [rbp + 184], r14d
proc_PAT$15_attempt:
proc_PAT$15_α_body:
lea rax, [rip + xchain370_n2_β]
mov qword ptr [rbp + 160], rax
# IR_MATCH_CAPTURE_SAVE push
 xchain370_n0_α:
 lea rdi, [rbp + 48]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain370_n1_α
 xchain370_n0_β:
 lea rdi, [rbp + 48]
 call rt_cap_pop@PLT
 jmp proc_PAT$15_scanfail
# IR_MATCH_ALT_NARY
 xchain370_n1_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain370_n3_α
xchain370_n1_as:
 jmp xchain370_n2_α
 xchain370_n1_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain370_n3_β
 jmp xchain370_n4_β
xchain370_n1_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain370_n4_α
 jmp xchain370_n0_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain370_n2_α:
 lea rdi, [rbp + 48]
 call rt_cap_top@PLT
 lea rcx, [rip + .S19]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$15_scanhit
 xchain370_n2_β:
 sub r12, 24
 jmp xchain370_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain370_n3_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 cmp eax, 3
 jne .Lx377_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx377_10
.Lx377_9:
 xor eax, eax
.Lx377_10:
 test rax, rax
 jz .Lx377_0
 mov r8d, 0
 lea rcx, [rip + .Lx377_4]
 lea rdx, [rip + .Lx377_5]
 jmp rax
.Lx377_4:
 jmp xchain370_n1_as
.Lx377_5:
 jmp xchain370_n1_af
.Lx377_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx377_2:
 test rax, rax
 je .Lx377_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx377_7]
 lea rdx, [rip + .Lx377_8]
 jmp rax
.Lx377_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx377_2
.Lx377_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx377_2
.Lx377_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain370_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx377_6]
 sub rsp, 8
 push rax
 jmp xchain370_n1_as
.Lx377_6:
 add rsp, 16
 jmp xchain370_n1_af
 xchain370_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain370_n4_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx378_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx378_10
.Lx378_9:
 xor eax, eax
.Lx378_10:
 test rax, rax
 jz .Lx378_0
 mov r8d, 0
 lea rcx, [rip + .Lx378_4]
 lea rdx, [rip + .Lx378_5]
 jmp rax
.Lx378_4:
 jmp xchain370_n1_as
.Lx378_5:
 jmp xchain370_n1_af
.Lx378_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx378_2:
 test rax, rax
 je .Lx378_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx378_7]
 lea rdx, [rip + .Lx378_8]
 jmp rax
.Lx378_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx378_2
.Lx378_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx378_2
.Lx378_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain370_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx378_6]
 sub rsp, 8
 push rax
 jmp xchain370_n1_as
.Lx378_6:
 add rsp, 16
 jmp xchain370_n1_af
 xchain370_n4_β:
 jmp qword ptr [rsp]
proc_PAT$15_scanhit:
cmp qword ptr [rbp + 192], 1
jne 7f
mov ecx, dword ptr [rbp + 184]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$15_γ
proc_PAT$15_scanfail:
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
jmp proc_PAT$15_attempt
8:
jmp proc_PAT$15_ω
proc_PAT$15_res:
add rsp, 8
pop rbp
proc_PAT$15_β:
jmp qword ptr [rbp + 160]
proc_PAT$15_γ:
push rbp
lea rax, [rip + proc_PAT$15_res]
push rax
mov rax, [rbp + 200]
mov rbp, [rbp + 216]
jmp rax
proc_PAT$15_ω:
mov rax, [rbp + 208]
lea rsp, [rbp + 224]
mov rbp, [rbp + 216]
jmp rax
  .globl proc_PAT$16_α
proc_PAT$16_α:
#=======================================================================================================================
    .global proc_PAT$16_α
    .global proc_PAT$16_β
    .global proc_PAT$16_γ
    .global proc_PAT$16_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$16_attempt:
proc_PAT$16_α_body:
lea rax, [rip + xchain379_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain379_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain379_n1_α
xchain379_n0_as:
 jmp proc_PAT$16_γ
 xchain379_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain379_n1_β
 jmp xchain379_n2_β
xchain379_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain379_n2_α
 jmp proc_PAT$16_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain379_n1_α:
 mov rax, qword ptr [1879052544]
 mov rdx, qword ptr [1879052552]
 cmp eax, 3
 jne .Lx382_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx382_10
.Lx382_9:
 xor eax, eax
.Lx382_10:
 test rax, rax
 jz .Lx382_0
 mov r8d, 0
 lea rcx, [rip + .Lx382_4]
 lea rdx, [rip + .Lx382_5]
 jmp rax
.Lx382_4:
 jmp xchain379_n0_as
.Lx382_5:
 jmp xchain379_n0_af
.Lx382_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx382_2:
 test rax, rax
 je .Lx382_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx382_7]
 lea rdx, [rip + .Lx382_8]
 jmp rax
.Lx382_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx382_2
.Lx382_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx382_2
.Lx382_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain379_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx382_6]
 sub rsp, 8
 push rax
 jmp xchain379_n0_as
.Lx382_6:
 add rsp, 16
 jmp xchain379_n0_af
 xchain379_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain379_n2_α:
 mov dword ptr [rbp + 96], r14d
 jmp xchain379_n3_α
xchain379_n2_as:
 jmp xchain379_n0_as
 xchain379_n2_β:
 jmp xchain379_n5_β
xchain379_n2_af:
 jmp xchain379_n0_af
# IR_MATCH_LIT
 xchain379_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain379_n2_af
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 40
 jne xchain379_n2_af
 add r14d, 1
 jmp xchain379_n4_α
 xchain379_n3_β:
 sub r14d, 1
 jmp xchain379_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain379_n4_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx387_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx387_10
.Lx387_9:
 xor eax, eax
.Lx387_10:
 test rax, rax
 jz .Lx387_0
 mov r8d, 0
 lea rcx, [rip + .Lx387_4]
 lea rdx, [rip + .Lx387_5]
 jmp rax
.Lx387_4:
 jmp xchain379_n5_α
.Lx387_5:
 jmp xchain379_n3_β
.Lx387_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx387_2:
 test rax, rax
 je .Lx387_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx387_7]
 lea rdx, [rip + .Lx387_8]
 jmp rax
.Lx387_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx387_2
.Lx387_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx387_2
.Lx387_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain379_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx387_6]
 sub rsp, 8
 push rax
 jmp xchain379_n5_α
.Lx387_6:
 add rsp, 16
 jmp xchain379_n3_β
 xchain379_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain379_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain379_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 41
 jne xchain379_n4_β
 add r14d, 1
 jmp xchain379_n2_as
 xchain379_n5_β:
 sub r14d, 1
 jmp xchain379_n4_β
proc_PAT$16_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$16_γ
proc_PAT$16_scanfail:
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
jmp proc_PAT$16_attempt
8:
jmp proc_PAT$16_ω
proc_PAT$16_res:
add rsp, 8
pop rbp
proc_PAT$16_β:
jmp qword ptr [rbp + 176]
proc_PAT$16_γ:
push rbp
lea rax, [rip + proc_PAT$16_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$16_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$17_α
proc_PAT$17_α:
#=======================================================================================================================
    .global proc_PAT$17_α
    .global proc_PAT$17_β
    .global proc_PAT$17_γ
    .global proc_PAT$17_ω
  sub rsp, 288
  mov [rsp + 264], rcx
  mov [rsp + 272], rdx
  mov [rsp + 280], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 256
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 248], rsp
mov qword ptr [rbp + 256], r8
mov dword ptr [rbp + 248], r14d
proc_PAT$17_attempt:
proc_PAT$17_α_body:
lea rax, [rip + xchain390_n0_β]
mov qword ptr [rbp + 224], rax
# IR_MATCH_ALT_NARY
 xchain390_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain390_n1_α
xchain390_n0_as:
 jmp proc_PAT$17_γ
 xchain390_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain390_n1_β
 jmp xchain390_n2_β
xchain390_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain390_n2_α
 jmp proc_PAT$17_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain390_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain390_n3_α
xchain390_n1_as:
 jmp xchain390_n0_as
 xchain390_n1_β:
 jmp xchain390_n5_β
xchain390_n1_af:
 jmp xchain390_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain390_n2_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 cmp eax, 3
 jne .Lx395_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx395_10
.Lx395_9:
 xor eax, eax
.Lx395_10:
 test rax, rax
 jz .Lx395_0
 mov r8d, 0
 lea rcx, [rip + .Lx395_4]
 lea rdx, [rip + .Lx395_5]
 jmp rax
.Lx395_4:
 jmp xchain390_n0_as
.Lx395_5:
 jmp xchain390_n0_af
.Lx395_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx395_2:
 test rax, rax
 je .Lx395_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx395_7]
 lea rdx, [rip + .Lx395_8]
 jmp rax
.Lx395_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx395_2
.Lx395_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx395_2
.Lx395_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain390_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx395_6]
 sub rsp, 8
 push rax
 jmp xchain390_n0_as
.Lx395_6:
 add rsp, 16
 jmp xchain390_n0_af
 xchain390_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain390_n3_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx396_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx396_10
.Lx396_9:
 xor eax, eax
.Lx396_10:
 test rax, rax
 jz .Lx396_0
 mov r8d, 0
 lea rcx, [rip + .Lx396_4]
 lea rdx, [rip + .Lx396_5]
 jmp rax
.Lx396_4:
 jmp xchain390_n4_α
.Lx396_5:
 jmp xchain390_n1_af
.Lx396_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx396_2:
 test rax, rax
 je .Lx396_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx396_7]
 lea rdx, [rip + .Lx396_8]
 jmp rax
.Lx396_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx396_2
.Lx396_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx396_2
.Lx396_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain390_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx396_6]
 sub rsp, 8
 push rax
 jmp xchain390_n4_α
.Lx396_6:
 add rsp, 16
 jmp xchain390_n1_af
 xchain390_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain390_n4_α:
 lea rdi, [rbp + 144]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain390_n6_α
 xchain390_n4_β:
 lea rdi, [rbp + 144]
 call rt_cap_pop@PLT
 jmp xchain390_n3_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain390_n5_α:
 lea rdi, [rbp + 144]
 call rt_cap_top@PLT
 lea rcx, [rip + .S20]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain390_n1_as
 xchain390_n5_β:
 sub r12, 24
 jmp xchain390_n6_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain390_n6_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx401_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx401_10
.Lx401_9:
 xor eax, eax
.Lx401_10:
 test rax, rax
 jz .Lx401_0
 mov r8d, 0
 lea rcx, [rip + .Lx401_4]
 lea rdx, [rip + .Lx401_5]
 jmp rax
.Lx401_4:
 jmp xchain390_n5_α
.Lx401_5:
 jmp xchain390_n4_β
.Lx401_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx401_2:
 test rax, rax
 je .Lx401_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx401_7]
 lea rdx, [rip + .Lx401_8]
 jmp rax
.Lx401_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx401_2
.Lx401_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx401_2
.Lx401_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain390_n4_β
 mov r14d, eax
 lea rax, [rip + .Lx401_6]
 sub rsp, 8
 push rax
 jmp xchain390_n5_α
.Lx401_6:
 add rsp, 16
 jmp xchain390_n4_β
 xchain390_n6_β:
 jmp qword ptr [rsp]
proc_PAT$17_scanhit:
cmp qword ptr [rbp + 256], 1
jne 7f
mov ecx, dword ptr [rbp + 248]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$17_γ
proc_PAT$17_scanfail:
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
jmp proc_PAT$17_attempt
8:
jmp proc_PAT$17_ω
proc_PAT$17_res:
add rsp, 8
pop rbp
proc_PAT$17_β:
jmp qword ptr [rbp + 224]
proc_PAT$17_γ:
push rbp
lea rax, [rip + proc_PAT$17_res]
push rax
mov rax, [rbp + 264]
mov rbp, [rbp + 280]
jmp rax
proc_PAT$17_ω:
mov rax, [rbp + 272]
lea rsp, [rbp + 288]
mov rbp, [rbp + 280]
jmp rax
  .globl proc_PAT$18_α
proc_PAT$18_α:
#=======================================================================================================================
    .global proc_PAT$18_α
    .global proc_PAT$18_β
    .global proc_PAT$18_γ
    .global proc_PAT$18_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$18_attempt:
proc_PAT$18_α_body:
lea rax, [rip + xchain402_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain402_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain402_n1_α
xchain402_n0_as:
 jmp proc_PAT$18_γ
 xchain402_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain402_n1_β
 jmp xchain402_n2_β
xchain402_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain402_n2_α
 jmp proc_PAT$18_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain402_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain402_n3_α
xchain402_n1_as:
 jmp xchain402_n0_as
 xchain402_n1_β:
 jmp xchain402_n6_β
xchain402_n1_af:
 jmp xchain402_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain402_n2_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx407_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx407_10
.Lx407_9:
 xor eax, eax
.Lx407_10:
 test rax, rax
 jz .Lx407_0
 mov r8d, 0
 lea rcx, [rip + .Lx407_4]
 lea rdx, [rip + .Lx407_5]
 jmp rax
.Lx407_4:
 jmp xchain402_n0_as
.Lx407_5:
 jmp xchain402_n0_af
.Lx407_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx407_2:
 test rax, rax
 je .Lx407_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx407_7]
 lea rdx, [rip + .Lx407_8]
 jmp rax
.Lx407_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx407_2
.Lx407_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx407_2
.Lx407_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain402_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx407_6]
 sub rsp, 8
 push rax
 jmp xchain402_n0_as
.Lx407_6:
 add rsp, 16
 jmp xchain402_n0_af
 xchain402_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain402_n3_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx408_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx408_10
.Lx408_9:
 xor eax, eax
.Lx408_10:
 test rax, rax
 jz .Lx408_0
 mov r8d, 0
 lea rcx, [rip + .Lx408_4]
 lea rdx, [rip + .Lx408_5]
 jmp rax
.Lx408_4:
 jmp xchain402_n4_α
.Lx408_5:
 jmp xchain402_n1_af
.Lx408_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx408_2:
 test rax, rax
 je .Lx408_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx408_7]
 lea rdx, [rip + .Lx408_8]
 jmp rax
.Lx408_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx408_2
.Lx408_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx408_2
.Lx408_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain402_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx408_6]
 sub rsp, 8
 push rax
 jmp xchain402_n4_α
.Lx408_6:
 add rsp, 16
 jmp xchain402_n1_af
 xchain402_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain402_n4_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 cmp eax, 3
 jne .Lx409_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx409_10
.Lx409_9:
 xor eax, eax
.Lx409_10:
 test rax, rax
 jz .Lx409_0
 mov r8d, 0
 lea rcx, [rip + .Lx409_4]
 lea rdx, [rip + .Lx409_5]
 jmp rax
.Lx409_4:
 jmp xchain402_n5_α
.Lx409_5:
 jmp xchain402_n3_β
.Lx409_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx409_2:
 test rax, rax
 je .Lx409_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx409_7]
 lea rdx, [rip + .Lx409_8]
 jmp rax
.Lx409_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx409_2
.Lx409_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx409_2
.Lx409_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain402_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx409_6]
 sub rsp, 8
 push rax
 jmp xchain402_n5_α
.Lx409_6:
 add rsp, 16
 jmp xchain402_n3_β
 xchain402_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain402_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain402_n7_α
 xchain402_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain402_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain402_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S21]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain402_n1_as
 xchain402_n6_β:
 sub r12, 24
 jmp xchain402_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain402_n7_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx414_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx414_10
.Lx414_9:
 xor eax, eax
.Lx414_10:
 test rax, rax
 jz .Lx414_0
 mov r8d, 0
 lea rcx, [rip + .Lx414_4]
 lea rdx, [rip + .Lx414_5]
 jmp rax
.Lx414_4:
 jmp xchain402_n6_α
.Lx414_5:
 jmp xchain402_n5_β
.Lx414_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx414_2:
 test rax, rax
 je .Lx414_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx414_7]
 lea rdx, [rip + .Lx414_8]
 jmp rax
.Lx414_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx414_2
.Lx414_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx414_2
.Lx414_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain402_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx414_6]
 sub rsp, 8
 push rax
 jmp xchain402_n6_α
.Lx414_6:
 add rsp, 16
 jmp xchain402_n5_β
 xchain402_n7_β:
 jmp qword ptr [rsp]
proc_PAT$18_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$18_γ
proc_PAT$18_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$18_attempt
8:
jmp proc_PAT$18_ω
proc_PAT$18_res:
add rsp, 8
pop rbp
proc_PAT$18_β:
jmp qword ptr [rbp + 256]
proc_PAT$18_γ:
push rbp
lea rax, [rip + proc_PAT$18_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$18_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$19_α
proc_PAT$19_α:
#=======================================================================================================================
    .global proc_PAT$19_α
    .global proc_PAT$19_β
    .global proc_PAT$19_γ
    .global proc_PAT$19_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$19_attempt:
proc_PAT$19_α_body:
lea rax, [rip + xchain415_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain415_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain415_n1_α
xchain415_n0_as:
 jmp proc_PAT$19_γ
 xchain415_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain415_n1_β
 jmp xchain415_n2_β
xchain415_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain415_n2_α
 jmp proc_PAT$19_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain415_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain415_n3_α
xchain415_n1_as:
 jmp xchain415_n0_as
 xchain415_n1_β:
 jmp xchain415_n6_β
xchain415_n1_af:
 jmp xchain415_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain415_n2_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx420_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx420_10
.Lx420_9:
 xor eax, eax
.Lx420_10:
 test rax, rax
 jz .Lx420_0
 mov r8d, 0
 lea rcx, [rip + .Lx420_4]
 lea rdx, [rip + .Lx420_5]
 jmp rax
.Lx420_4:
 jmp xchain415_n0_as
.Lx420_5:
 jmp xchain415_n0_af
.Lx420_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx420_2:
 test rax, rax
 je .Lx420_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx420_7]
 lea rdx, [rip + .Lx420_8]
 jmp rax
.Lx420_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx420_2
.Lx420_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx420_2
.Lx420_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain415_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx420_6]
 sub rsp, 8
 push rax
 jmp xchain415_n0_as
.Lx420_6:
 add rsp, 16
 jmp xchain415_n0_af
 xchain415_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain415_n3_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx421_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx421_10
.Lx421_9:
 xor eax, eax
.Lx421_10:
 test rax, rax
 jz .Lx421_0
 mov r8d, 0
 lea rcx, [rip + .Lx421_4]
 lea rdx, [rip + .Lx421_5]
 jmp rax
.Lx421_4:
 jmp xchain415_n4_α
.Lx421_5:
 jmp xchain415_n1_af
.Lx421_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx421_2:
 test rax, rax
 je .Lx421_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx421_7]
 lea rdx, [rip + .Lx421_8]
 jmp rax
.Lx421_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx421_2
.Lx421_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx421_2
.Lx421_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain415_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx421_6]
 sub rsp, 8
 push rax
 jmp xchain415_n4_α
.Lx421_6:
 add rsp, 16
 jmp xchain415_n1_af
 xchain415_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain415_n4_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx422_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx422_10
.Lx422_9:
 xor eax, eax
.Lx422_10:
 test rax, rax
 jz .Lx422_0
 mov r8d, 0
 lea rcx, [rip + .Lx422_4]
 lea rdx, [rip + .Lx422_5]
 jmp rax
.Lx422_4:
 jmp xchain415_n5_α
.Lx422_5:
 jmp xchain415_n3_β
.Lx422_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx422_2:
 test rax, rax
 je .Lx422_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx422_7]
 lea rdx, [rip + .Lx422_8]
 jmp rax
.Lx422_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx422_2
.Lx422_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx422_2
.Lx422_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain415_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx422_6]
 sub rsp, 8
 push rax
 jmp xchain415_n5_α
.Lx422_6:
 add rsp, 16
 jmp xchain415_n3_β
 xchain415_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain415_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain415_n7_α
 xchain415_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain415_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain415_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S22]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain415_n1_as
 xchain415_n6_β:
 sub r12, 24
 jmp xchain415_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain415_n7_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx427_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx427_10
.Lx427_9:
 xor eax, eax
.Lx427_10:
 test rax, rax
 jz .Lx427_0
 mov r8d, 0
 lea rcx, [rip + .Lx427_4]
 lea rdx, [rip + .Lx427_5]
 jmp rax
.Lx427_4:
 jmp xchain415_n6_α
.Lx427_5:
 jmp xchain415_n5_β
.Lx427_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx427_2:
 test rax, rax
 je .Lx427_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx427_7]
 lea rdx, [rip + .Lx427_8]
 jmp rax
.Lx427_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx427_2
.Lx427_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx427_2
.Lx427_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain415_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx427_6]
 sub rsp, 8
 push rax
 jmp xchain415_n6_α
.Lx427_6:
 add rsp, 16
 jmp xchain415_n5_β
 xchain415_n7_β:
 jmp qword ptr [rsp]
proc_PAT$19_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$19_γ
proc_PAT$19_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$19_attempt
8:
jmp proc_PAT$19_ω
proc_PAT$19_res:
add rsp, 8
pop rbp
proc_PAT$19_β:
jmp qword ptr [rbp + 256]
proc_PAT$19_γ:
push rbp
lea rax, [rip + proc_PAT$19_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$19_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$20_α
proc_PAT$20_α:
#=======================================================================================================================
    .global proc_PAT$20_α
    .global proc_PAT$20_β
    .global proc_PAT$20_γ
    .global proc_PAT$20_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
mov qword ptr [rbp + 80], r8
mov dword ptr [rbp + 72], r14d
proc_PAT$20_attempt:
proc_PAT$20_α_body:
lea rax, [rip + xchain428_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain428_n0_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx430_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx430_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx430_1
 add dword ptr [rsp + 0], 1
 jmp .Lx430_0
.Lx430_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx430_240
 add rsp, 16
 jmp proc_PAT$20_scanfail
.Lx430_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$20_scanhit
 xchain428_n0_β:
 xchain428_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$20_scanfail
proc_PAT$20_scanhit:
cmp qword ptr [rbp + 80], 1
jne 7f
mov ecx, dword ptr [rbp + 72]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$20_γ
proc_PAT$20_scanfail:
cmp qword ptr [rbp + 80], 1
jne 8f
mov eax, dword ptr [rbp + 72]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 72], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$20_attempt
8:
jmp proc_PAT$20_ω
proc_PAT$20_res:
add rsp, 8
pop rbp
proc_PAT$20_β:
jmp qword ptr [rbp + 48]
proc_PAT$20_γ:
push rbp
lea rax, [rip + proc_PAT$20_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$20_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$21_α
proc_PAT$21_α:
#=======================================================================================================================
    .global proc_PAT$21_α
    .global proc_PAT$21_β
    .global proc_PAT$21_γ
    .global proc_PAT$21_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$21_attempt:
proc_PAT$21_α_body:
lea rax, [rip + xchain431_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain431_n0_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain431_n1_α
xchain431_n0_as:
 jmp proc_PAT$21_γ
 xchain431_n0_β:
 jmp xchain431_n3_β
xchain431_n0_af:
 jmp proc_PAT$21_ω
# IR_MATCH_ANY
 xchain431_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain431_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C1]
 cmp byte ptr [rdi+rsi], 0
 je xchain431_n0_af
 add r14d, 1
 jmp xchain431_n2_α
 xchain431_n1_β:
 sub r14d, 1
 jmp xchain431_n0_af
# IR_MATCH_ALT_NARY
 xchain431_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain431_n4_α
xchain431_n2_as:
 jmp xchain431_n3_α
 xchain431_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain431_n4_β
 jmp xchain431_n5_β
xchain431_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain431_n5_α
 jmp xchain431_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain431_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx437_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx437_10
.Lx437_9:
 xor eax, eax
.Lx437_10:
 test rax, rax
 jz .Lx437_0
 mov r8d, 0
 lea rcx, [rip + .Lx437_4]
 lea rdx, [rip + .Lx437_5]
 jmp rax
.Lx437_4:
 jmp xchain431_n0_as
.Lx437_5:
 jmp xchain431_n2_β
.Lx437_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx437_2:
 test rax, rax
 je .Lx437_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx437_7]
 lea rdx, [rip + .Lx437_8]
 jmp rax
.Lx437_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx437_2
.Lx437_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx437_2
.Lx437_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain431_n2_β
 mov r14d, eax
 lea rax, [rip + .Lx437_6]
 sub rsp, 8
 push rax
 jmp xchain431_n0_as
.Lx437_6:
 add rsp, 16
 jmp xchain431_n2_β
 xchain431_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain431_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain431_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 je xchain431_n2_af
 add r14d, 1
 jmp xchain431_n2_as
 xchain431_n4_β:
 sub r14d, 1
 jmp xchain431_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain431_n5_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx439_0
 mov r8d, 0
 lea rcx, [rip + .Lx439_4]
 lea rdx, [rip + .Lx439_5]
 jmp rax
.Lx439_4:
 jmp xchain431_n2_as
.Lx439_5:
 jmp xchain431_n2_af
.Lx439_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx439_2:
 test rax, rax
 je .Lx439_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx439_7]
 lea rdx, [rip + .Lx439_8]
 jmp rax
.Lx439_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx439_2
.Lx439_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx439_2
.Lx439_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain431_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx439_6]
 sub rsp, 8
 push rax
 jmp xchain431_n2_as
.Lx439_6:
 add rsp, 16
 jmp xchain431_n2_af
 xchain431_n5_β:
 jmp qword ptr [rsp]
proc_PAT$21_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$21_γ
proc_PAT$21_scanfail:
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
jmp proc_PAT$21_attempt
8:
jmp proc_PAT$21_ω
proc_PAT$21_res:
add rsp, 8
pop rbp
proc_PAT$21_β:
jmp qword ptr [rbp + 176]
proc_PAT$21_γ:
push rbp
lea rax, [rip + proc_PAT$21_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$21_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$22_α
proc_PAT$22_α:
#=======================================================================================================================
    .global proc_PAT$22_α
    .global proc_PAT$22_β
    .global proc_PAT$22_γ
    .global proc_PAT$22_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
mov qword ptr [rbp + 448], r8
mov dword ptr [rbp + 440], r14d
proc_PAT$22_attempt:
proc_PAT$22_α_body:
lea rax, [rip + xchain440_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain440_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain440_n1_α
xchain440_n0_as:
 jmp proc_PAT$22_γ
 xchain440_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain440_n1_β
 jmp xchain440_n2_β
xchain440_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain440_n2_α
 jmp proc_PAT$22_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain440_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain440_n3_α
xchain440_n1_as:
 jmp xchain440_n0_as
 xchain440_n1_β:
 jmp xchain440_n6_β
xchain440_n1_af:
 jmp xchain440_n0_af
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain440_n2_α:
 mov dword ptr [rbp + 336], r14d
 jmp xchain440_n7_α
xchain440_n2_as:
 jmp xchain440_n0_as
 xchain440_n2_β:
 jmp xchain440_n8_β
xchain440_n2_af:
 jmp xchain440_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx447_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx447_10
.Lx447_9:
 xor eax, eax
.Lx447_10:
 test rax, rax
 jz .Lx447_0
 mov r8d, 0
 lea rcx, [rip + .Lx447_4]
 lea rdx, [rip + .Lx447_5]
 jmp rax
.Lx447_4:
 jmp xchain440_n4_α
.Lx447_5:
 jmp xchain440_n1_af
.Lx447_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx447_2:
 test rax, rax
 je .Lx447_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx447_7]
 lea rdx, [rip + .Lx447_8]
 jmp rax
.Lx447_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx447_2
.Lx447_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx447_2
.Lx447_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx447_6]
 sub rsp, 8
 push rax
 jmp xchain440_n4_α
.Lx447_6:
 add rsp, 16
 jmp xchain440_n1_af
 xchain440_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain440_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain440_n3_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 46
 jne xchain440_n3_β
 add r14d, 1
 jmp xchain440_n5_α
 xchain440_n4_β:
 sub r14d, 1
 jmp xchain440_n3_β
# IR_MATCH_ALT_NARY
 xchain440_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain440_n9_α
xchain440_n5_as:
 jmp xchain440_n6_α
 xchain440_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain440_n9_β
 jmp xchain440_n10_β
xchain440_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain440_n10_α
 jmp xchain440_n4_β
# IR_MATCH_ALT_NARY
 xchain440_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain440_n11_α
xchain440_n6_as:
 jmp xchain440_n1_as
 xchain440_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain440_n11_β
 jmp xchain440_n12_β
xchain440_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain440_n12_α
 jmp xchain440_n5_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n7_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx454_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx454_10
.Lx454_9:
 xor eax, eax
.Lx454_10:
 test rax, rax
 jz .Lx454_0
 mov r8d, 0
 lea rcx, [rip + .Lx454_4]
 lea rdx, [rip + .Lx454_5]
 jmp rax
.Lx454_4:
 jmp xchain440_n8_α
.Lx454_5:
 jmp xchain440_n2_af
.Lx454_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx454_2:
 test rax, rax
 je .Lx454_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx454_7]
 lea rdx, [rip + .Lx454_8]
 jmp rax
.Lx454_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx454_2
.Lx454_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx454_2
.Lx454_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx454_6]
 sub rsp, 8
 push rax
 jmp xchain440_n8_α
.Lx454_6:
 add rsp, 16
 jmp xchain440_n2_af
 xchain440_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n8_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx455_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx455_10
.Lx455_9:
 xor eax, eax
.Lx455_10:
 test rax, rax
 jz .Lx455_0
 mov r8d, 0
 lea rcx, [rip + .Lx455_4]
 lea rdx, [rip + .Lx455_5]
 jmp rax
.Lx455_4:
 jmp xchain440_n2_as
.Lx455_5:
 jmp xchain440_n7_β
.Lx455_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx455_2:
 test rax, rax
 je .Lx455_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx455_7]
 lea rdx, [rip + .Lx455_8]
 jmp rax
.Lx455_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx455_2
.Lx455_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx455_2
.Lx455_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx455_6]
 sub rsp, 8
 push rax
 jmp xchain440_n2_as
.Lx455_6:
 add rsp, 16
 jmp xchain440_n7_β
 xchain440_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n9_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx456_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx456_10
.Lx456_9:
 xor eax, eax
.Lx456_10:
 test rax, rax
 jz .Lx456_0
 mov r8d, 0
 lea rcx, [rip + .Lx456_4]
 lea rdx, [rip + .Lx456_5]
 jmp rax
.Lx456_4:
 jmp xchain440_n5_as
.Lx456_5:
 jmp xchain440_n5_af
.Lx456_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx456_2:
 test rax, rax
 je .Lx456_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx456_7]
 lea rdx, [rip + .Lx456_8]
 jmp rax
.Lx456_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx456_2
.Lx456_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx456_2
.Lx456_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx456_6]
 sub rsp, 8
 push rax
 jmp xchain440_n5_as
.Lx456_6:
 add rsp, 16
 jmp xchain440_n5_af
 xchain440_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n10_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx457_0
 mov r8d, 0
 lea rcx, [rip + .Lx457_4]
 lea rdx, [rip + .Lx457_5]
 jmp rax
.Lx457_4:
 jmp xchain440_n5_as
.Lx457_5:
 jmp xchain440_n5_af
.Lx457_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx457_2:
 test rax, rax
 je .Lx457_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx457_7]
 lea rdx, [rip + .Lx457_8]
 jmp rax
.Lx457_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx457_2
.Lx457_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx457_2
.Lx457_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx457_6]
 sub rsp, 8
 push rax
 jmp xchain440_n5_as
.Lx457_6:
 add rsp, 16
 jmp xchain440_n5_af
 xchain440_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n11_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx458_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx458_10
.Lx458_9:
 xor eax, eax
.Lx458_10:
 test rax, rax
 jz .Lx458_0
 mov r8d, 0
 lea rcx, [rip + .Lx458_4]
 lea rdx, [rip + .Lx458_5]
 jmp rax
.Lx458_4:
 jmp xchain440_n6_as
.Lx458_5:
 jmp xchain440_n6_af
.Lx458_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx458_2:
 test rax, rax
 je .Lx458_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx458_7]
 lea rdx, [rip + .Lx458_8]
 jmp rax
.Lx458_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx458_2
.Lx458_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx458_2
.Lx458_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx458_6]
 sub rsp, 8
 push rax
 jmp xchain440_n6_as
.Lx458_6:
 add rsp, 16
 jmp xchain440_n6_af
 xchain440_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain440_n12_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx459_0
 mov r8d, 0
 lea rcx, [rip + .Lx459_4]
 lea rdx, [rip + .Lx459_5]
 jmp rax
.Lx459_4:
 jmp xchain440_n6_as
.Lx459_5:
 jmp xchain440_n6_af
.Lx459_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx459_2:
 test rax, rax
 je .Lx459_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx459_7]
 lea rdx, [rip + .Lx459_8]
 jmp rax
.Lx459_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx459_2
.Lx459_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx459_2
.Lx459_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain440_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx459_6]
 sub rsp, 8
 push rax
 jmp xchain440_n6_as
.Lx459_6:
 add rsp, 16
 jmp xchain440_n6_af
 xchain440_n12_β:
 jmp qword ptr [rsp]
proc_PAT$22_scanhit:
cmp qword ptr [rbp + 448], 1
jne 7f
mov ecx, dword ptr [rbp + 440]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$22_γ
proc_PAT$22_scanfail:
cmp qword ptr [rbp + 448], 1
jne 8f
mov eax, dword ptr [rbp + 440]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 440], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$22_attempt
8:
jmp proc_PAT$22_ω
proc_PAT$22_res:
add rsp, 8
pop rbp
proc_PAT$22_β:
jmp qword ptr [rbp + 416]
proc_PAT$22_γ:
push rbp
lea rax, [rip + proc_PAT$22_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$22_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$23_α
proc_PAT$23_α:
#=======================================================================================================================
    .global proc_PAT$23_α
    .global proc_PAT$23_β
    .global proc_PAT$23_γ
    .global proc_PAT$23_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$23_attempt:
proc_PAT$23_α_body:
lea rax, [rip + xchain460_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain460_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain460_n1_α
 xchain460_n0_β:
 add rsp, 16
 jmp proc_PAT$23_scanfail
# IR_MATCH_ANY
 xchain460_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx463_239
 add rsp, 16
 jmp proc_PAT$23_scanfail
.Lx463_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx463_240
 add rsp, 16
 jmp proc_PAT$23_scanfail
.Lx463_240:
 add r14d, 1
 jmp xchain460_n2_α
 xchain460_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$23_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain460_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S23]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$23_scanhit
 xchain460_n2_β:
 sub r12, 24
 jmp xchain460_n1_β
proc_PAT$23_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$23_γ
proc_PAT$23_scanfail:
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
jmp proc_PAT$23_attempt
8:
jmp proc_PAT$23_ω
proc_PAT$23_res:
add rsp, 8
pop rbp
proc_PAT$23_β:
jmp qword ptr [rbp + 80]
proc_PAT$23_γ:
push rbp
lea rax, [rip + proc_PAT$23_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$23_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$24_α
proc_PAT$24_α:
#=======================================================================================================================
    .global proc_PAT$24_α
    .global proc_PAT$24_β
    .global proc_PAT$24_γ
    .global proc_PAT$24_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$24_attempt:
proc_PAT$24_α_body:
lea rax, [rip + xchain466_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain466_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain466_n1_α
 xchain466_n0_β:
 add rsp, 16
 jmp proc_PAT$24_scanfail
# IR_MATCH_ANY
 xchain466_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx469_239
 add rsp, 16
 jmp proc_PAT$24_scanfail
.Lx469_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C3]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx469_240
 add rsp, 16
 jmp proc_PAT$24_scanfail
.Lx469_240:
 add r14d, 1
 jmp xchain466_n2_α
 xchain466_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$24_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain466_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S24]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$24_scanhit
 xchain466_n2_β:
 sub r12, 24
 jmp xchain466_n1_β
proc_PAT$24_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$24_γ
proc_PAT$24_scanfail:
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
jmp proc_PAT$24_attempt
8:
jmp proc_PAT$24_ω
proc_PAT$24_res:
add rsp, 8
pop rbp
proc_PAT$24_β:
jmp qword ptr [rbp + 80]
proc_PAT$24_γ:
push rbp
lea rax, [rip + proc_PAT$24_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$24_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$25_α
proc_PAT$25_α:
#=======================================================================================================================
    .global proc_PAT$25_α
    .global proc_PAT$25_β
    .global proc_PAT$25_γ
    .global proc_PAT$25_ω
  sub rsp, 224
  mov [rsp + 200], rcx
  mov [rsp + 208], rdx
  mov [rsp + 216], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 192
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 184], rsp
mov qword ptr [rbp + 192], r8
mov dword ptr [rbp + 184], r14d
proc_PAT$25_attempt:
proc_PAT$25_α_body:
lea rax, [rip + xchain472_n2_β]
mov qword ptr [rbp + 160], rax
# IR_MATCH_CAPTURE_SAVE push
 xchain472_n0_α:
 lea rdi, [rbp + 48]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain472_n1_α
 xchain472_n0_β:
 lea rdi, [rbp + 48]
 call rt_cap_pop@PLT
 jmp proc_PAT$25_scanfail
# IR_MATCH_ALT_NARY
 xchain472_n1_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain472_n3_α
xchain472_n1_as:
 jmp xchain472_n2_α
 xchain472_n1_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain472_n3_β
 jmp xchain472_n4_β
xchain472_n1_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain472_n4_α
 jmp xchain472_n0_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain472_n2_α:
 lea rdi, [rbp + 48]
 call rt_cap_top@PLT
 lea rcx, [rip + .S25]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$25_scanhit
 xchain472_n2_β:
 sub r12, 24
 jmp xchain472_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain472_n3_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 cmp eax, 3
 jne .Lx479_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx479_10
.Lx479_9:
 xor eax, eax
.Lx479_10:
 test rax, rax
 jz .Lx479_0
 mov r8d, 0
 lea rcx, [rip + .Lx479_4]
 lea rdx, [rip + .Lx479_5]
 jmp rax
.Lx479_4:
 jmp xchain472_n1_as
.Lx479_5:
 jmp xchain472_n1_af
.Lx479_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx479_2:
 test rax, rax
 je .Lx479_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx479_7]
 lea rdx, [rip + .Lx479_8]
 jmp rax
.Lx479_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx479_2
.Lx479_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx479_2
.Lx479_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain472_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx479_6]
 sub rsp, 8
 push rax
 jmp xchain472_n1_as
.Lx479_6:
 add rsp, 16
 jmp xchain472_n1_af
 xchain472_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain472_n4_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx480_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx480_10
.Lx480_9:
 xor eax, eax
.Lx480_10:
 test rax, rax
 jz .Lx480_0
 mov r8d, 0
 lea rcx, [rip + .Lx480_4]
 lea rdx, [rip + .Lx480_5]
 jmp rax
.Lx480_4:
 jmp xchain472_n1_as
.Lx480_5:
 jmp xchain472_n1_af
.Lx480_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx480_2:
 test rax, rax
 je .Lx480_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx480_7]
 lea rdx, [rip + .Lx480_8]
 jmp rax
.Lx480_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx480_2
.Lx480_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx480_2
.Lx480_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain472_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx480_6]
 sub rsp, 8
 push rax
 jmp xchain472_n1_as
.Lx480_6:
 add rsp, 16
 jmp xchain472_n1_af
 xchain472_n4_β:
 jmp qword ptr [rsp]
proc_PAT$25_scanhit:
cmp qword ptr [rbp + 192], 1
jne 7f
mov ecx, dword ptr [rbp + 184]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$25_γ
proc_PAT$25_scanfail:
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
jmp proc_PAT$25_attempt
8:
jmp proc_PAT$25_ω
proc_PAT$25_res:
add rsp, 8
pop rbp
proc_PAT$25_β:
jmp qword ptr [rbp + 160]
proc_PAT$25_γ:
push rbp
lea rax, [rip + proc_PAT$25_res]
push rax
mov rax, [rbp + 200]
mov rbp, [rbp + 216]
jmp rax
proc_PAT$25_ω:
mov rax, [rbp + 208]
lea rsp, [rbp + 224]
mov rbp, [rbp + 216]
jmp rax
  .globl proc_PAT$26_α
proc_PAT$26_α:
#=======================================================================================================================
    .global proc_PAT$26_α
    .global proc_PAT$26_β
    .global proc_PAT$26_γ
    .global proc_PAT$26_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$26_attempt:
proc_PAT$26_α_body:
lea rax, [rip + xchain481_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain481_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain481_n1_α
xchain481_n0_as:
 jmp proc_PAT$26_γ
 xchain481_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain481_n1_β
 jmp xchain481_n2_β
xchain481_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain481_n2_α
 jmp proc_PAT$26_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain481_n1_α:
 mov rax, qword ptr [1879052544]
 mov rdx, qword ptr [1879052552]
 cmp eax, 3
 jne .Lx484_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx484_10
.Lx484_9:
 xor eax, eax
.Lx484_10:
 test rax, rax
 jz .Lx484_0
 mov r8d, 0
 lea rcx, [rip + .Lx484_4]
 lea rdx, [rip + .Lx484_5]
 jmp rax
.Lx484_4:
 jmp xchain481_n0_as
.Lx484_5:
 jmp xchain481_n0_af
.Lx484_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx484_2:
 test rax, rax
 je .Lx484_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx484_7]
 lea rdx, [rip + .Lx484_8]
 jmp rax
.Lx484_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx484_2
.Lx484_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx484_2
.Lx484_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain481_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx484_6]
 sub rsp, 8
 push rax
 jmp xchain481_n0_as
.Lx484_6:
 add rsp, 16
 jmp xchain481_n0_af
 xchain481_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain481_n2_α:
 mov dword ptr [rbp + 96], r14d
 jmp xchain481_n3_α
xchain481_n2_as:
 jmp xchain481_n0_as
 xchain481_n2_β:
 jmp xchain481_n5_β
xchain481_n2_af:
 jmp xchain481_n0_af
# IR_MATCH_LIT
 xchain481_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain481_n2_af
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 40
 jne xchain481_n2_af
 add r14d, 1
 jmp xchain481_n4_α
 xchain481_n3_β:
 sub r14d, 1
 jmp xchain481_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain481_n4_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx489_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx489_10
.Lx489_9:
 xor eax, eax
.Lx489_10:
 test rax, rax
 jz .Lx489_0
 mov r8d, 0
 lea rcx, [rip + .Lx489_4]
 lea rdx, [rip + .Lx489_5]
 jmp rax
.Lx489_4:
 jmp xchain481_n5_α
.Lx489_5:
 jmp xchain481_n3_β
.Lx489_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx489_2:
 test rax, rax
 je .Lx489_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx489_7]
 lea rdx, [rip + .Lx489_8]
 jmp rax
.Lx489_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx489_2
.Lx489_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx489_2
.Lx489_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain481_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx489_6]
 sub rsp, 8
 push rax
 jmp xchain481_n5_α
.Lx489_6:
 add rsp, 16
 jmp xchain481_n3_β
 xchain481_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain481_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain481_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 41
 jne xchain481_n4_β
 add r14d, 1
 jmp xchain481_n2_as
 xchain481_n5_β:
 sub r14d, 1
 jmp xchain481_n4_β
proc_PAT$26_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$26_γ
proc_PAT$26_scanfail:
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
jmp proc_PAT$26_attempt
8:
jmp proc_PAT$26_ω
proc_PAT$26_res:
add rsp, 8
pop rbp
proc_PAT$26_β:
jmp qword ptr [rbp + 176]
proc_PAT$26_γ:
push rbp
lea rax, [rip + proc_PAT$26_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$26_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$27_α
proc_PAT$27_α:
#=======================================================================================================================
    .global proc_PAT$27_α
    .global proc_PAT$27_β
    .global proc_PAT$27_γ
    .global proc_PAT$27_ω
  sub rsp, 288
  mov [rsp + 264], rcx
  mov [rsp + 272], rdx
  mov [rsp + 280], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 256
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 248], rsp
mov qword ptr [rbp + 256], r8
mov dword ptr [rbp + 248], r14d
proc_PAT$27_attempt:
proc_PAT$27_α_body:
lea rax, [rip + xchain492_n0_β]
mov qword ptr [rbp + 224], rax
# IR_MATCH_ALT_NARY
 xchain492_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain492_n1_α
xchain492_n0_as:
 jmp proc_PAT$27_γ
 xchain492_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain492_n1_β
 jmp xchain492_n2_β
xchain492_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain492_n2_α
 jmp proc_PAT$27_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain492_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain492_n3_α
xchain492_n1_as:
 jmp xchain492_n0_as
 xchain492_n1_β:
 jmp xchain492_n5_β
xchain492_n1_af:
 jmp xchain492_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain492_n2_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 cmp eax, 3
 jne .Lx497_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx497_10
.Lx497_9:
 xor eax, eax
.Lx497_10:
 test rax, rax
 jz .Lx497_0
 mov r8d, 0
 lea rcx, [rip + .Lx497_4]
 lea rdx, [rip + .Lx497_5]
 jmp rax
.Lx497_4:
 jmp xchain492_n0_as
.Lx497_5:
 jmp xchain492_n0_af
.Lx497_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx497_2:
 test rax, rax
 je .Lx497_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx497_7]
 lea rdx, [rip + .Lx497_8]
 jmp rax
.Lx497_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx497_2
.Lx497_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx497_2
.Lx497_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain492_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx497_6]
 sub rsp, 8
 push rax
 jmp xchain492_n0_as
.Lx497_6:
 add rsp, 16
 jmp xchain492_n0_af
 xchain492_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain492_n3_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx498_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx498_10
.Lx498_9:
 xor eax, eax
.Lx498_10:
 test rax, rax
 jz .Lx498_0
 mov r8d, 0
 lea rcx, [rip + .Lx498_4]
 lea rdx, [rip + .Lx498_5]
 jmp rax
.Lx498_4:
 jmp xchain492_n4_α
.Lx498_5:
 jmp xchain492_n1_af
.Lx498_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx498_2:
 test rax, rax
 je .Lx498_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx498_7]
 lea rdx, [rip + .Lx498_8]
 jmp rax
.Lx498_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx498_2
.Lx498_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx498_2
.Lx498_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain492_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx498_6]
 sub rsp, 8
 push rax
 jmp xchain492_n4_α
.Lx498_6:
 add rsp, 16
 jmp xchain492_n1_af
 xchain492_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain492_n4_α:
 lea rdi, [rbp + 144]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain492_n6_α
 xchain492_n4_β:
 lea rdi, [rbp + 144]
 call rt_cap_pop@PLT
 jmp xchain492_n3_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain492_n5_α:
 lea rdi, [rbp + 144]
 call rt_cap_top@PLT
 lea rcx, [rip + .S26]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain492_n1_as
 xchain492_n5_β:
 sub r12, 24
 jmp xchain492_n6_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain492_n6_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx503_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx503_10
.Lx503_9:
 xor eax, eax
.Lx503_10:
 test rax, rax
 jz .Lx503_0
 mov r8d, 0
 lea rcx, [rip + .Lx503_4]
 lea rdx, [rip + .Lx503_5]
 jmp rax
.Lx503_4:
 jmp xchain492_n5_α
.Lx503_5:
 jmp xchain492_n4_β
.Lx503_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx503_2:
 test rax, rax
 je .Lx503_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx503_7]
 lea rdx, [rip + .Lx503_8]
 jmp rax
.Lx503_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx503_2
.Lx503_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx503_2
.Lx503_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain492_n4_β
 mov r14d, eax
 lea rax, [rip + .Lx503_6]
 sub rsp, 8
 push rax
 jmp xchain492_n5_α
.Lx503_6:
 add rsp, 16
 jmp xchain492_n4_β
 xchain492_n6_β:
 jmp qword ptr [rsp]
proc_PAT$27_scanhit:
cmp qword ptr [rbp + 256], 1
jne 7f
mov ecx, dword ptr [rbp + 248]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$27_γ
proc_PAT$27_scanfail:
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
jmp proc_PAT$27_attempt
8:
jmp proc_PAT$27_ω
proc_PAT$27_res:
add rsp, 8
pop rbp
proc_PAT$27_β:
jmp qword ptr [rbp + 224]
proc_PAT$27_γ:
push rbp
lea rax, [rip + proc_PAT$27_res]
push rax
mov rax, [rbp + 264]
mov rbp, [rbp + 280]
jmp rax
proc_PAT$27_ω:
mov rax, [rbp + 272]
lea rsp, [rbp + 288]
mov rbp, [rbp + 280]
jmp rax
  .globl proc_PAT$28_α
proc_PAT$28_α:
#=======================================================================================================================
    .global proc_PAT$28_α
    .global proc_PAT$28_β
    .global proc_PAT$28_γ
    .global proc_PAT$28_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$28_attempt:
proc_PAT$28_α_body:
lea rax, [rip + xchain504_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain504_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain504_n1_α
xchain504_n0_as:
 jmp proc_PAT$28_γ
 xchain504_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain504_n1_β
 jmp xchain504_n2_β
xchain504_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain504_n2_α
 jmp proc_PAT$28_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain504_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain504_n3_α
xchain504_n1_as:
 jmp xchain504_n0_as
 xchain504_n1_β:
 jmp xchain504_n6_β
xchain504_n1_af:
 jmp xchain504_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain504_n2_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx509_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx509_10
.Lx509_9:
 xor eax, eax
.Lx509_10:
 test rax, rax
 jz .Lx509_0
 mov r8d, 0
 lea rcx, [rip + .Lx509_4]
 lea rdx, [rip + .Lx509_5]
 jmp rax
.Lx509_4:
 jmp xchain504_n0_as
.Lx509_5:
 jmp xchain504_n0_af
.Lx509_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx509_2:
 test rax, rax
 je .Lx509_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx509_7]
 lea rdx, [rip + .Lx509_8]
 jmp rax
.Lx509_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx509_2
.Lx509_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx509_2
.Lx509_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain504_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx509_6]
 sub rsp, 8
 push rax
 jmp xchain504_n0_as
.Lx509_6:
 add rsp, 16
 jmp xchain504_n0_af
 xchain504_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain504_n3_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx510_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx510_10
.Lx510_9:
 xor eax, eax
.Lx510_10:
 test rax, rax
 jz .Lx510_0
 mov r8d, 0
 lea rcx, [rip + .Lx510_4]
 lea rdx, [rip + .Lx510_5]
 jmp rax
.Lx510_4:
 jmp xchain504_n4_α
.Lx510_5:
 jmp xchain504_n1_af
.Lx510_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx510_2:
 test rax, rax
 je .Lx510_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx510_7]
 lea rdx, [rip + .Lx510_8]
 jmp rax
.Lx510_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx510_2
.Lx510_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx510_2
.Lx510_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain504_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx510_6]
 sub rsp, 8
 push rax
 jmp xchain504_n4_α
.Lx510_6:
 add rsp, 16
 jmp xchain504_n1_af
 xchain504_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain504_n4_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 cmp eax, 3
 jne .Lx511_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx511_10
.Lx511_9:
 xor eax, eax
.Lx511_10:
 test rax, rax
 jz .Lx511_0
 mov r8d, 0
 lea rcx, [rip + .Lx511_4]
 lea rdx, [rip + .Lx511_5]
 jmp rax
.Lx511_4:
 jmp xchain504_n5_α
.Lx511_5:
 jmp xchain504_n3_β
.Lx511_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx511_2:
 test rax, rax
 je .Lx511_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx511_7]
 lea rdx, [rip + .Lx511_8]
 jmp rax
.Lx511_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx511_2
.Lx511_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx511_2
.Lx511_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain504_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx511_6]
 sub rsp, 8
 push rax
 jmp xchain504_n5_α
.Lx511_6:
 add rsp, 16
 jmp xchain504_n3_β
 xchain504_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain504_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain504_n7_α
 xchain504_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain504_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain504_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S27]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain504_n1_as
 xchain504_n6_β:
 sub r12, 24
 jmp xchain504_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain504_n7_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx516_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx516_10
.Lx516_9:
 xor eax, eax
.Lx516_10:
 test rax, rax
 jz .Lx516_0
 mov r8d, 0
 lea rcx, [rip + .Lx516_4]
 lea rdx, [rip + .Lx516_5]
 jmp rax
.Lx516_4:
 jmp xchain504_n6_α
.Lx516_5:
 jmp xchain504_n5_β
.Lx516_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx516_2:
 test rax, rax
 je .Lx516_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx516_7]
 lea rdx, [rip + .Lx516_8]
 jmp rax
.Lx516_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx516_2
.Lx516_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx516_2
.Lx516_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain504_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx516_6]
 sub rsp, 8
 push rax
 jmp xchain504_n6_α
.Lx516_6:
 add rsp, 16
 jmp xchain504_n5_β
 xchain504_n7_β:
 jmp qword ptr [rsp]
proc_PAT$28_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$28_γ
proc_PAT$28_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$28_attempt
8:
jmp proc_PAT$28_ω
proc_PAT$28_res:
add rsp, 8
pop rbp
proc_PAT$28_β:
jmp qword ptr [rbp + 256]
proc_PAT$28_γ:
push rbp
lea rax, [rip + proc_PAT$28_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$28_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$29_α
proc_PAT$29_α:
#=======================================================================================================================
    .global proc_PAT$29_α
    .global proc_PAT$29_β
    .global proc_PAT$29_γ
    .global proc_PAT$29_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$29_attempt:
proc_PAT$29_α_body:
lea rax, [rip + xchain517_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain517_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain517_n1_α
xchain517_n0_as:
 jmp proc_PAT$29_γ
 xchain517_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain517_n1_β
 jmp xchain517_n2_β
xchain517_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain517_n2_α
 jmp proc_PAT$29_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain517_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain517_n3_α
xchain517_n1_as:
 jmp xchain517_n0_as
 xchain517_n1_β:
 jmp xchain517_n6_β
xchain517_n1_af:
 jmp xchain517_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain517_n2_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx522_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx522_10
.Lx522_9:
 xor eax, eax
.Lx522_10:
 test rax, rax
 jz .Lx522_0
 mov r8d, 0
 lea rcx, [rip + .Lx522_4]
 lea rdx, [rip + .Lx522_5]
 jmp rax
.Lx522_4:
 jmp xchain517_n0_as
.Lx522_5:
 jmp xchain517_n0_af
.Lx522_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx522_2:
 test rax, rax
 je .Lx522_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx522_7]
 lea rdx, [rip + .Lx522_8]
 jmp rax
.Lx522_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx522_2
.Lx522_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx522_2
.Lx522_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain517_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx522_6]
 sub rsp, 8
 push rax
 jmp xchain517_n0_as
.Lx522_6:
 add rsp, 16
 jmp xchain517_n0_af
 xchain517_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain517_n3_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx523_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx523_10
.Lx523_9:
 xor eax, eax
.Lx523_10:
 test rax, rax
 jz .Lx523_0
 mov r8d, 0
 lea rcx, [rip + .Lx523_4]
 lea rdx, [rip + .Lx523_5]
 jmp rax
.Lx523_4:
 jmp xchain517_n4_α
.Lx523_5:
 jmp xchain517_n1_af
.Lx523_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx523_2:
 test rax, rax
 je .Lx523_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx523_7]
 lea rdx, [rip + .Lx523_8]
 jmp rax
.Lx523_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx523_2
.Lx523_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx523_2
.Lx523_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain517_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx523_6]
 sub rsp, 8
 push rax
 jmp xchain517_n4_α
.Lx523_6:
 add rsp, 16
 jmp xchain517_n1_af
 xchain517_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain517_n4_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx524_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx524_10
.Lx524_9:
 xor eax, eax
.Lx524_10:
 test rax, rax
 jz .Lx524_0
 mov r8d, 0
 lea rcx, [rip + .Lx524_4]
 lea rdx, [rip + .Lx524_5]
 jmp rax
.Lx524_4:
 jmp xchain517_n5_α
.Lx524_5:
 jmp xchain517_n3_β
.Lx524_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx524_2:
 test rax, rax
 je .Lx524_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx524_7]
 lea rdx, [rip + .Lx524_8]
 jmp rax
.Lx524_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx524_2
.Lx524_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx524_2
.Lx524_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain517_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx524_6]
 sub rsp, 8
 push rax
 jmp xchain517_n5_α
.Lx524_6:
 add rsp, 16
 jmp xchain517_n3_β
 xchain517_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain517_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain517_n7_α
 xchain517_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain517_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain517_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S28]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain517_n1_as
 xchain517_n6_β:
 sub r12, 24
 jmp xchain517_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain517_n7_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx529_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx529_10
.Lx529_9:
 xor eax, eax
.Lx529_10:
 test rax, rax
 jz .Lx529_0
 mov r8d, 0
 lea rcx, [rip + .Lx529_4]
 lea rdx, [rip + .Lx529_5]
 jmp rax
.Lx529_4:
 jmp xchain517_n6_α
.Lx529_5:
 jmp xchain517_n5_β
.Lx529_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx529_2:
 test rax, rax
 je .Lx529_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx529_7]
 lea rdx, [rip + .Lx529_8]
 jmp rax
.Lx529_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx529_2
.Lx529_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx529_2
.Lx529_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain517_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx529_6]
 sub rsp, 8
 push rax
 jmp xchain517_n6_α
.Lx529_6:
 add rsp, 16
 jmp xchain517_n5_β
 xchain517_n7_β:
 jmp qword ptr [rsp]
proc_PAT$29_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$29_γ
proc_PAT$29_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$29_attempt
8:
jmp proc_PAT$29_ω
proc_PAT$29_res:
add rsp, 8
pop rbp
proc_PAT$29_β:
jmp qword ptr [rbp + 256]
proc_PAT$29_γ:
push rbp
lea rax, [rip + proc_PAT$29_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$29_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$30_α
proc_PAT$30_α:
#=======================================================================================================================
    .global proc_PAT$30_α
    .global proc_PAT$30_β
    .global proc_PAT$30_γ
    .global proc_PAT$30_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
mov qword ptr [rbp + 80], r8
mov dword ptr [rbp + 72], r14d
proc_PAT$30_attempt:
proc_PAT$30_α_body:
lea rax, [rip + xchain530_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain530_n0_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx532_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx532_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx532_1
 add dword ptr [rsp + 0], 1
 jmp .Lx532_0
.Lx532_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx532_240
 add rsp, 16
 jmp proc_PAT$30_scanfail
.Lx532_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$30_scanhit
 xchain530_n0_β:
 xchain530_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$30_scanfail
proc_PAT$30_scanhit:
cmp qword ptr [rbp + 80], 1
jne 7f
mov ecx, dword ptr [rbp + 72]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$30_γ
proc_PAT$30_scanfail:
cmp qword ptr [rbp + 80], 1
jne 8f
mov eax, dword ptr [rbp + 72]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 72], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$30_attempt
8:
jmp proc_PAT$30_ω
proc_PAT$30_res:
add rsp, 8
pop rbp
proc_PAT$30_β:
jmp qword ptr [rbp + 48]
proc_PAT$30_γ:
push rbp
lea rax, [rip + proc_PAT$30_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$30_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$31_α
proc_PAT$31_α:
#=======================================================================================================================
    .global proc_PAT$31_α
    .global proc_PAT$31_β
    .global proc_PAT$31_γ
    .global proc_PAT$31_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$31_attempt:
proc_PAT$31_α_body:
lea rax, [rip + xchain533_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain533_n0_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain533_n1_α
xchain533_n0_as:
 jmp proc_PAT$31_γ
 xchain533_n0_β:
 jmp xchain533_n3_β
xchain533_n0_af:
 jmp proc_PAT$31_ω
# IR_MATCH_ANY
 xchain533_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain533_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C1]
 cmp byte ptr [rdi+rsi], 0
 je xchain533_n0_af
 add r14d, 1
 jmp xchain533_n2_α
 xchain533_n1_β:
 sub r14d, 1
 jmp xchain533_n0_af
# IR_MATCH_ALT_NARY
 xchain533_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain533_n4_α
xchain533_n2_as:
 jmp xchain533_n3_α
 xchain533_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain533_n4_β
 jmp xchain533_n5_β
xchain533_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain533_n5_α
 jmp xchain533_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain533_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx539_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx539_10
.Lx539_9:
 xor eax, eax
.Lx539_10:
 test rax, rax
 jz .Lx539_0
 mov r8d, 0
 lea rcx, [rip + .Lx539_4]
 lea rdx, [rip + .Lx539_5]
 jmp rax
.Lx539_4:
 jmp xchain533_n0_as
.Lx539_5:
 jmp xchain533_n2_β
.Lx539_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx539_2:
 test rax, rax
 je .Lx539_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx539_7]
 lea rdx, [rip + .Lx539_8]
 jmp rax
.Lx539_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx539_2
.Lx539_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx539_2
.Lx539_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain533_n2_β
 mov r14d, eax
 lea rax, [rip + .Lx539_6]
 sub rsp, 8
 push rax
 jmp xchain533_n0_as
.Lx539_6:
 add rsp, 16
 jmp xchain533_n2_β
 xchain533_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain533_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain533_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 je xchain533_n2_af
 add r14d, 1
 jmp xchain533_n2_as
 xchain533_n4_β:
 sub r14d, 1
 jmp xchain533_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain533_n5_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx541_0
 mov r8d, 0
 lea rcx, [rip + .Lx541_4]
 lea rdx, [rip + .Lx541_5]
 jmp rax
.Lx541_4:
 jmp xchain533_n2_as
.Lx541_5:
 jmp xchain533_n2_af
.Lx541_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx541_2:
 test rax, rax
 je .Lx541_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx541_7]
 lea rdx, [rip + .Lx541_8]
 jmp rax
.Lx541_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx541_2
.Lx541_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx541_2
.Lx541_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain533_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx541_6]
 sub rsp, 8
 push rax
 jmp xchain533_n2_as
.Lx541_6:
 add rsp, 16
 jmp xchain533_n2_af
 xchain533_n5_β:
 jmp qword ptr [rsp]
proc_PAT$31_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$31_γ
proc_PAT$31_scanfail:
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
jmp proc_PAT$31_attempt
8:
jmp proc_PAT$31_ω
proc_PAT$31_res:
add rsp, 8
pop rbp
proc_PAT$31_β:
jmp qword ptr [rbp + 176]
proc_PAT$31_γ:
push rbp
lea rax, [rip + proc_PAT$31_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$31_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$32_α
proc_PAT$32_α:
#=======================================================================================================================
    .global proc_PAT$32_α
    .global proc_PAT$32_β
    .global proc_PAT$32_γ
    .global proc_PAT$32_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
mov qword ptr [rbp + 448], r8
mov dword ptr [rbp + 440], r14d
proc_PAT$32_attempt:
proc_PAT$32_α_body:
lea rax, [rip + xchain542_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain542_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain542_n1_α
xchain542_n0_as:
 jmp proc_PAT$32_γ
 xchain542_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain542_n1_β
 jmp xchain542_n2_β
xchain542_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain542_n2_α
 jmp proc_PAT$32_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain542_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain542_n3_α
xchain542_n1_as:
 jmp xchain542_n0_as
 xchain542_n1_β:
 jmp xchain542_n6_β
xchain542_n1_af:
 jmp xchain542_n0_af
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain542_n2_α:
 mov dword ptr [rbp + 336], r14d
 jmp xchain542_n7_α
xchain542_n2_as:
 jmp xchain542_n0_as
 xchain542_n2_β:
 jmp xchain542_n8_β
xchain542_n2_af:
 jmp xchain542_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx549_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx549_10
.Lx549_9:
 xor eax, eax
.Lx549_10:
 test rax, rax
 jz .Lx549_0
 mov r8d, 0
 lea rcx, [rip + .Lx549_4]
 lea rdx, [rip + .Lx549_5]
 jmp rax
.Lx549_4:
 jmp xchain542_n4_α
.Lx549_5:
 jmp xchain542_n1_af
.Lx549_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx549_2:
 test rax, rax
 je .Lx549_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx549_7]
 lea rdx, [rip + .Lx549_8]
 jmp rax
.Lx549_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx549_2
.Lx549_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx549_2
.Lx549_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx549_6]
 sub rsp, 8
 push rax
 jmp xchain542_n4_α
.Lx549_6:
 add rsp, 16
 jmp xchain542_n1_af
 xchain542_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain542_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain542_n3_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 46
 jne xchain542_n3_β
 add r14d, 1
 jmp xchain542_n5_α
 xchain542_n4_β:
 sub r14d, 1
 jmp xchain542_n3_β
# IR_MATCH_ALT_NARY
 xchain542_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain542_n9_α
xchain542_n5_as:
 jmp xchain542_n6_α
 xchain542_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain542_n9_β
 jmp xchain542_n10_β
xchain542_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain542_n10_α
 jmp xchain542_n4_β
# IR_MATCH_ALT_NARY
 xchain542_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain542_n11_α
xchain542_n6_as:
 jmp xchain542_n1_as
 xchain542_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain542_n11_β
 jmp xchain542_n12_β
xchain542_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain542_n12_α
 jmp xchain542_n5_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n7_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx556_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx556_10
.Lx556_9:
 xor eax, eax
.Lx556_10:
 test rax, rax
 jz .Lx556_0
 mov r8d, 0
 lea rcx, [rip + .Lx556_4]
 lea rdx, [rip + .Lx556_5]
 jmp rax
.Lx556_4:
 jmp xchain542_n8_α
.Lx556_5:
 jmp xchain542_n2_af
.Lx556_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx556_2:
 test rax, rax
 je .Lx556_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx556_7]
 lea rdx, [rip + .Lx556_8]
 jmp rax
.Lx556_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx556_2
.Lx556_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx556_2
.Lx556_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx556_6]
 sub rsp, 8
 push rax
 jmp xchain542_n8_α
.Lx556_6:
 add rsp, 16
 jmp xchain542_n2_af
 xchain542_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n8_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx557_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx557_10
.Lx557_9:
 xor eax, eax
.Lx557_10:
 test rax, rax
 jz .Lx557_0
 mov r8d, 0
 lea rcx, [rip + .Lx557_4]
 lea rdx, [rip + .Lx557_5]
 jmp rax
.Lx557_4:
 jmp xchain542_n2_as
.Lx557_5:
 jmp xchain542_n7_β
.Lx557_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx557_2:
 test rax, rax
 je .Lx557_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx557_7]
 lea rdx, [rip + .Lx557_8]
 jmp rax
.Lx557_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx557_2
.Lx557_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx557_2
.Lx557_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx557_6]
 sub rsp, 8
 push rax
 jmp xchain542_n2_as
.Lx557_6:
 add rsp, 16
 jmp xchain542_n7_β
 xchain542_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n9_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx558_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx558_10
.Lx558_9:
 xor eax, eax
.Lx558_10:
 test rax, rax
 jz .Lx558_0
 mov r8d, 0
 lea rcx, [rip + .Lx558_4]
 lea rdx, [rip + .Lx558_5]
 jmp rax
.Lx558_4:
 jmp xchain542_n5_as
.Lx558_5:
 jmp xchain542_n5_af
.Lx558_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx558_2:
 test rax, rax
 je .Lx558_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx558_7]
 lea rdx, [rip + .Lx558_8]
 jmp rax
.Lx558_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx558_2
.Lx558_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx558_2
.Lx558_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx558_6]
 sub rsp, 8
 push rax
 jmp xchain542_n5_as
.Lx558_6:
 add rsp, 16
 jmp xchain542_n5_af
 xchain542_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n10_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx559_0
 mov r8d, 0
 lea rcx, [rip + .Lx559_4]
 lea rdx, [rip + .Lx559_5]
 jmp rax
.Lx559_4:
 jmp xchain542_n5_as
.Lx559_5:
 jmp xchain542_n5_af
.Lx559_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx559_2:
 test rax, rax
 je .Lx559_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx559_7]
 lea rdx, [rip + .Lx559_8]
 jmp rax
.Lx559_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx559_2
.Lx559_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx559_2
.Lx559_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx559_6]
 sub rsp, 8
 push rax
 jmp xchain542_n5_as
.Lx559_6:
 add rsp, 16
 jmp xchain542_n5_af
 xchain542_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n11_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx560_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx560_10
.Lx560_9:
 xor eax, eax
.Lx560_10:
 test rax, rax
 jz .Lx560_0
 mov r8d, 0
 lea rcx, [rip + .Lx560_4]
 lea rdx, [rip + .Lx560_5]
 jmp rax
.Lx560_4:
 jmp xchain542_n6_as
.Lx560_5:
 jmp xchain542_n6_af
.Lx560_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx560_2:
 test rax, rax
 je .Lx560_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx560_7]
 lea rdx, [rip + .Lx560_8]
 jmp rax
.Lx560_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx560_2
.Lx560_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx560_2
.Lx560_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx560_6]
 sub rsp, 8
 push rax
 jmp xchain542_n6_as
.Lx560_6:
 add rsp, 16
 jmp xchain542_n6_af
 xchain542_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain542_n12_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx561_0
 mov r8d, 0
 lea rcx, [rip + .Lx561_4]
 lea rdx, [rip + .Lx561_5]
 jmp rax
.Lx561_4:
 jmp xchain542_n6_as
.Lx561_5:
 jmp xchain542_n6_af
.Lx561_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx561_2:
 test rax, rax
 je .Lx561_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx561_7]
 lea rdx, [rip + .Lx561_8]
 jmp rax
.Lx561_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx561_2
.Lx561_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx561_2
.Lx561_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain542_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx561_6]
 sub rsp, 8
 push rax
 jmp xchain542_n6_as
.Lx561_6:
 add rsp, 16
 jmp xchain542_n6_af
 xchain542_n12_β:
 jmp qword ptr [rsp]
proc_PAT$32_scanhit:
cmp qword ptr [rbp + 448], 1
jne 7f
mov ecx, dword ptr [rbp + 440]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$32_γ
proc_PAT$32_scanfail:
cmp qword ptr [rbp + 448], 1
jne 8f
mov eax, dword ptr [rbp + 440]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 440], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$32_attempt
8:
jmp proc_PAT$32_ω
proc_PAT$32_res:
add rsp, 8
pop rbp
proc_PAT$32_β:
jmp qword ptr [rbp + 416]
proc_PAT$32_γ:
push rbp
lea rax, [rip + proc_PAT$32_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$32_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$33_α
proc_PAT$33_α:
#=======================================================================================================================
    .global proc_PAT$33_α
    .global proc_PAT$33_β
    .global proc_PAT$33_γ
    .global proc_PAT$33_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$33_attempt:
proc_PAT$33_α_body:
lea rax, [rip + xchain562_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain562_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain562_n1_α
 xchain562_n0_β:
 add rsp, 16
 jmp proc_PAT$33_scanfail
# IR_MATCH_ANY
 xchain562_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx565_239
 add rsp, 16
 jmp proc_PAT$33_scanfail
.Lx565_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx565_240
 add rsp, 16
 jmp proc_PAT$33_scanfail
.Lx565_240:
 add r14d, 1
 jmp xchain562_n2_α
 xchain562_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$33_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain562_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S29]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$33_scanhit
 xchain562_n2_β:
 sub r12, 24
 jmp xchain562_n1_β
proc_PAT$33_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$33_γ
proc_PAT$33_scanfail:
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
jmp proc_PAT$33_attempt
8:
jmp proc_PAT$33_ω
proc_PAT$33_res:
add rsp, 8
pop rbp
proc_PAT$33_β:
jmp qword ptr [rbp + 80]
proc_PAT$33_γ:
push rbp
lea rax, [rip + proc_PAT$33_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$33_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$34_α
proc_PAT$34_α:
#=======================================================================================================================
    .global proc_PAT$34_α
    .global proc_PAT$34_β
    .global proc_PAT$34_γ
    .global proc_PAT$34_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$34_attempt:
proc_PAT$34_α_body:
lea rax, [rip + xchain568_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain568_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain568_n1_α
 xchain568_n0_β:
 add rsp, 16
 jmp proc_PAT$34_scanfail
# IR_MATCH_ANY
 xchain568_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx571_239
 add rsp, 16
 jmp proc_PAT$34_scanfail
.Lx571_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C3]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx571_240
 add rsp, 16
 jmp proc_PAT$34_scanfail
.Lx571_240:
 add r14d, 1
 jmp xchain568_n2_α
 xchain568_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$34_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain568_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S30]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$34_scanhit
 xchain568_n2_β:
 sub r12, 24
 jmp xchain568_n1_β
proc_PAT$34_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$34_γ
proc_PAT$34_scanfail:
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
jmp proc_PAT$34_attempt
8:
jmp proc_PAT$34_ω
proc_PAT$34_res:
add rsp, 8
pop rbp
proc_PAT$34_β:
jmp qword ptr [rbp + 80]
proc_PAT$34_γ:
push rbp
lea rax, [rip + proc_PAT$34_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$34_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$35_α
proc_PAT$35_α:
#=======================================================================================================================
    .global proc_PAT$35_α
    .global proc_PAT$35_β
    .global proc_PAT$35_γ
    .global proc_PAT$35_ω
  sub rsp, 224
  mov [rsp + 200], rcx
  mov [rsp + 208], rdx
  mov [rsp + 216], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 192
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 184], rsp
mov qword ptr [rbp + 192], r8
mov dword ptr [rbp + 184], r14d
proc_PAT$35_attempt:
proc_PAT$35_α_body:
lea rax, [rip + xchain574_n2_β]
mov qword ptr [rbp + 160], rax
# IR_MATCH_CAPTURE_SAVE push
 xchain574_n0_α:
 lea rdi, [rbp + 48]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain574_n1_α
 xchain574_n0_β:
 lea rdi, [rbp + 48]
 call rt_cap_pop@PLT
 jmp proc_PAT$35_scanfail
# IR_MATCH_ALT_NARY
 xchain574_n1_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain574_n3_α
xchain574_n1_as:
 jmp xchain574_n2_α
 xchain574_n1_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain574_n3_β
 jmp xchain574_n4_β
xchain574_n1_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain574_n4_α
 jmp xchain574_n0_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain574_n2_α:
 lea rdi, [rbp + 48]
 call rt_cap_top@PLT
 lea rcx, [rip + .S31]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$35_scanhit
 xchain574_n2_β:
 sub r12, 24
 jmp xchain574_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain574_n3_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 cmp eax, 3
 jne .Lx581_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx581_10
.Lx581_9:
 xor eax, eax
.Lx581_10:
 test rax, rax
 jz .Lx581_0
 mov r8d, 0
 lea rcx, [rip + .Lx581_4]
 lea rdx, [rip + .Lx581_5]
 jmp rax
.Lx581_4:
 jmp xchain574_n1_as
.Lx581_5:
 jmp xchain574_n1_af
.Lx581_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx581_2:
 test rax, rax
 je .Lx581_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx581_7]
 lea rdx, [rip + .Lx581_8]
 jmp rax
.Lx581_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx581_2
.Lx581_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx581_2
.Lx581_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain574_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx581_6]
 sub rsp, 8
 push rax
 jmp xchain574_n1_as
.Lx581_6:
 add rsp, 16
 jmp xchain574_n1_af
 xchain574_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain574_n4_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx582_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx582_10
.Lx582_9:
 xor eax, eax
.Lx582_10:
 test rax, rax
 jz .Lx582_0
 mov r8d, 0
 lea rcx, [rip + .Lx582_4]
 lea rdx, [rip + .Lx582_5]
 jmp rax
.Lx582_4:
 jmp xchain574_n1_as
.Lx582_5:
 jmp xchain574_n1_af
.Lx582_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx582_2:
 test rax, rax
 je .Lx582_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx582_7]
 lea rdx, [rip + .Lx582_8]
 jmp rax
.Lx582_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx582_2
.Lx582_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx582_2
.Lx582_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain574_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx582_6]
 sub rsp, 8
 push rax
 jmp xchain574_n1_as
.Lx582_6:
 add rsp, 16
 jmp xchain574_n1_af
 xchain574_n4_β:
 jmp qword ptr [rsp]
proc_PAT$35_scanhit:
cmp qword ptr [rbp + 192], 1
jne 7f
mov ecx, dword ptr [rbp + 184]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$35_γ
proc_PAT$35_scanfail:
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
jmp proc_PAT$35_attempt
8:
jmp proc_PAT$35_ω
proc_PAT$35_res:
add rsp, 8
pop rbp
proc_PAT$35_β:
jmp qword ptr [rbp + 160]
proc_PAT$35_γ:
push rbp
lea rax, [rip + proc_PAT$35_res]
push rax
mov rax, [rbp + 200]
mov rbp, [rbp + 216]
jmp rax
proc_PAT$35_ω:
mov rax, [rbp + 208]
lea rsp, [rbp + 224]
mov rbp, [rbp + 216]
jmp rax
  .globl proc_PAT$36_α
proc_PAT$36_α:
#=======================================================================================================================
    .global proc_PAT$36_α
    .global proc_PAT$36_β
    .global proc_PAT$36_γ
    .global proc_PAT$36_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$36_attempt:
proc_PAT$36_α_body:
lea rax, [rip + xchain583_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain583_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain583_n1_α
xchain583_n0_as:
 jmp proc_PAT$36_γ
 xchain583_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain583_n1_β
 jmp xchain583_n2_β
xchain583_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain583_n2_α
 jmp proc_PAT$36_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain583_n1_α:
 mov rax, qword ptr [1879052544]
 mov rdx, qword ptr [1879052552]
 cmp eax, 3
 jne .Lx586_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx586_10
.Lx586_9:
 xor eax, eax
.Lx586_10:
 test rax, rax
 jz .Lx586_0
 mov r8d, 0
 lea rcx, [rip + .Lx586_4]
 lea rdx, [rip + .Lx586_5]
 jmp rax
.Lx586_4:
 jmp xchain583_n0_as
.Lx586_5:
 jmp xchain583_n0_af
.Lx586_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx586_2:
 test rax, rax
 je .Lx586_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx586_7]
 lea rdx, [rip + .Lx586_8]
 jmp rax
.Lx586_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx586_2
.Lx586_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx586_2
.Lx586_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain583_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx586_6]
 sub rsp, 8
 push rax
 jmp xchain583_n0_as
.Lx586_6:
 add rsp, 16
 jmp xchain583_n0_af
 xchain583_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain583_n2_α:
 mov dword ptr [rbp + 96], r14d
 jmp xchain583_n3_α
xchain583_n2_as:
 jmp xchain583_n0_as
 xchain583_n2_β:
 jmp xchain583_n5_β
xchain583_n2_af:
 jmp xchain583_n0_af
# IR_MATCH_LIT
 xchain583_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain583_n2_af
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 40
 jne xchain583_n2_af
 add r14d, 1
 jmp xchain583_n4_α
 xchain583_n3_β:
 sub r14d, 1
 jmp xchain583_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain583_n4_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx591_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx591_10
.Lx591_9:
 xor eax, eax
.Lx591_10:
 test rax, rax
 jz .Lx591_0
 mov r8d, 0
 lea rcx, [rip + .Lx591_4]
 lea rdx, [rip + .Lx591_5]
 jmp rax
.Lx591_4:
 jmp xchain583_n5_α
.Lx591_5:
 jmp xchain583_n3_β
.Lx591_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx591_2:
 test rax, rax
 je .Lx591_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx591_7]
 lea rdx, [rip + .Lx591_8]
 jmp rax
.Lx591_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx591_2
.Lx591_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx591_2
.Lx591_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain583_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx591_6]
 sub rsp, 8
 push rax
 jmp xchain583_n5_α
.Lx591_6:
 add rsp, 16
 jmp xchain583_n3_β
 xchain583_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain583_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain583_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 41
 jne xchain583_n4_β
 add r14d, 1
 jmp xchain583_n2_as
 xchain583_n5_β:
 sub r14d, 1
 jmp xchain583_n4_β
proc_PAT$36_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$36_γ
proc_PAT$36_scanfail:
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
jmp proc_PAT$36_attempt
8:
jmp proc_PAT$36_ω
proc_PAT$36_res:
add rsp, 8
pop rbp
proc_PAT$36_β:
jmp qword ptr [rbp + 176]
proc_PAT$36_γ:
push rbp
lea rax, [rip + proc_PAT$36_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$36_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$37_α
proc_PAT$37_α:
#=======================================================================================================================
    .global proc_PAT$37_α
    .global proc_PAT$37_β
    .global proc_PAT$37_γ
    .global proc_PAT$37_ω
  sub rsp, 288
  mov [rsp + 264], rcx
  mov [rsp + 272], rdx
  mov [rsp + 280], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 256
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 248], rsp
mov qword ptr [rbp + 256], r8
mov dword ptr [rbp + 248], r14d
proc_PAT$37_attempt:
proc_PAT$37_α_body:
lea rax, [rip + xchain594_n0_β]
mov qword ptr [rbp + 224], rax
# IR_MATCH_ALT_NARY
 xchain594_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain594_n1_α
xchain594_n0_as:
 jmp proc_PAT$37_γ
 xchain594_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain594_n1_β
 jmp xchain594_n2_β
xchain594_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain594_n2_α
 jmp proc_PAT$37_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain594_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain594_n3_α
xchain594_n1_as:
 jmp xchain594_n0_as
 xchain594_n1_β:
 jmp xchain594_n5_β
xchain594_n1_af:
 jmp xchain594_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain594_n2_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 cmp eax, 3
 jne .Lx599_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx599_10
.Lx599_9:
 xor eax, eax
.Lx599_10:
 test rax, rax
 jz .Lx599_0
 mov r8d, 0
 lea rcx, [rip + .Lx599_4]
 lea rdx, [rip + .Lx599_5]
 jmp rax
.Lx599_4:
 jmp xchain594_n0_as
.Lx599_5:
 jmp xchain594_n0_af
.Lx599_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx599_2:
 test rax, rax
 je .Lx599_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx599_7]
 lea rdx, [rip + .Lx599_8]
 jmp rax
.Lx599_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx599_2
.Lx599_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx599_2
.Lx599_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain594_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx599_6]
 sub rsp, 8
 push rax
 jmp xchain594_n0_as
.Lx599_6:
 add rsp, 16
 jmp xchain594_n0_af
 xchain594_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain594_n3_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx600_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx600_10
.Lx600_9:
 xor eax, eax
.Lx600_10:
 test rax, rax
 jz .Lx600_0
 mov r8d, 0
 lea rcx, [rip + .Lx600_4]
 lea rdx, [rip + .Lx600_5]
 jmp rax
.Lx600_4:
 jmp xchain594_n4_α
.Lx600_5:
 jmp xchain594_n1_af
.Lx600_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx600_2:
 test rax, rax
 je .Lx600_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx600_7]
 lea rdx, [rip + .Lx600_8]
 jmp rax
.Lx600_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx600_2
.Lx600_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx600_2
.Lx600_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain594_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx600_6]
 sub rsp, 8
 push rax
 jmp xchain594_n4_α
.Lx600_6:
 add rsp, 16
 jmp xchain594_n1_af
 xchain594_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain594_n4_α:
 lea rdi, [rbp + 144]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain594_n6_α
 xchain594_n4_β:
 lea rdi, [rbp + 144]
 call rt_cap_pop@PLT
 jmp xchain594_n3_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain594_n5_α:
 lea rdi, [rbp + 144]
 call rt_cap_top@PLT
 lea rcx, [rip + .S32]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain594_n1_as
 xchain594_n5_β:
 sub r12, 24
 jmp xchain594_n6_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain594_n6_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx605_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx605_10
.Lx605_9:
 xor eax, eax
.Lx605_10:
 test rax, rax
 jz .Lx605_0
 mov r8d, 0
 lea rcx, [rip + .Lx605_4]
 lea rdx, [rip + .Lx605_5]
 jmp rax
.Lx605_4:
 jmp xchain594_n5_α
.Lx605_5:
 jmp xchain594_n4_β
.Lx605_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx605_2:
 test rax, rax
 je .Lx605_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx605_7]
 lea rdx, [rip + .Lx605_8]
 jmp rax
.Lx605_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx605_2
.Lx605_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx605_2
.Lx605_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain594_n4_β
 mov r14d, eax
 lea rax, [rip + .Lx605_6]
 sub rsp, 8
 push rax
 jmp xchain594_n5_α
.Lx605_6:
 add rsp, 16
 jmp xchain594_n4_β
 xchain594_n6_β:
 jmp qword ptr [rsp]
proc_PAT$37_scanhit:
cmp qword ptr [rbp + 256], 1
jne 7f
mov ecx, dword ptr [rbp + 248]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$37_γ
proc_PAT$37_scanfail:
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
jmp proc_PAT$37_attempt
8:
jmp proc_PAT$37_ω
proc_PAT$37_res:
add rsp, 8
pop rbp
proc_PAT$37_β:
jmp qword ptr [rbp + 224]
proc_PAT$37_γ:
push rbp
lea rax, [rip + proc_PAT$37_res]
push rax
mov rax, [rbp + 264]
mov rbp, [rbp + 280]
jmp rax
proc_PAT$37_ω:
mov rax, [rbp + 272]
lea rsp, [rbp + 288]
mov rbp, [rbp + 280]
jmp rax
  .globl proc_PAT$38_α
proc_PAT$38_α:
#=======================================================================================================================
    .global proc_PAT$38_α
    .global proc_PAT$38_β
    .global proc_PAT$38_γ
    .global proc_PAT$38_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$38_attempt:
proc_PAT$38_α_body:
lea rax, [rip + xchain606_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain606_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain606_n1_α
xchain606_n0_as:
 jmp proc_PAT$38_γ
 xchain606_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain606_n1_β
 jmp xchain606_n2_β
xchain606_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain606_n2_α
 jmp proc_PAT$38_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain606_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain606_n3_α
xchain606_n1_as:
 jmp xchain606_n0_as
 xchain606_n1_β:
 jmp xchain606_n6_β
xchain606_n1_af:
 jmp xchain606_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain606_n2_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx611_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx611_10
.Lx611_9:
 xor eax, eax
.Lx611_10:
 test rax, rax
 jz .Lx611_0
 mov r8d, 0
 lea rcx, [rip + .Lx611_4]
 lea rdx, [rip + .Lx611_5]
 jmp rax
.Lx611_4:
 jmp xchain606_n0_as
.Lx611_5:
 jmp xchain606_n0_af
.Lx611_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx611_2:
 test rax, rax
 je .Lx611_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx611_7]
 lea rdx, [rip + .Lx611_8]
 jmp rax
.Lx611_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx611_2
.Lx611_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx611_2
.Lx611_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain606_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx611_6]
 sub rsp, 8
 push rax
 jmp xchain606_n0_as
.Lx611_6:
 add rsp, 16
 jmp xchain606_n0_af
 xchain606_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain606_n3_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx612_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx612_10
.Lx612_9:
 xor eax, eax
.Lx612_10:
 test rax, rax
 jz .Lx612_0
 mov r8d, 0
 lea rcx, [rip + .Lx612_4]
 lea rdx, [rip + .Lx612_5]
 jmp rax
.Lx612_4:
 jmp xchain606_n4_α
.Lx612_5:
 jmp xchain606_n1_af
.Lx612_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx612_2:
 test rax, rax
 je .Lx612_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx612_7]
 lea rdx, [rip + .Lx612_8]
 jmp rax
.Lx612_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx612_2
.Lx612_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx612_2
.Lx612_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain606_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx612_6]
 sub rsp, 8
 push rax
 jmp xchain606_n4_α
.Lx612_6:
 add rsp, 16
 jmp xchain606_n1_af
 xchain606_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain606_n4_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 cmp eax, 3
 jne .Lx613_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx613_10
.Lx613_9:
 xor eax, eax
.Lx613_10:
 test rax, rax
 jz .Lx613_0
 mov r8d, 0
 lea rcx, [rip + .Lx613_4]
 lea rdx, [rip + .Lx613_5]
 jmp rax
.Lx613_4:
 jmp xchain606_n5_α
.Lx613_5:
 jmp xchain606_n3_β
.Lx613_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx613_2:
 test rax, rax
 je .Lx613_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx613_7]
 lea rdx, [rip + .Lx613_8]
 jmp rax
.Lx613_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx613_2
.Lx613_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx613_2
.Lx613_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain606_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx613_6]
 sub rsp, 8
 push rax
 jmp xchain606_n5_α
.Lx613_6:
 add rsp, 16
 jmp xchain606_n3_β
 xchain606_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain606_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain606_n7_α
 xchain606_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain606_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain606_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S33]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain606_n1_as
 xchain606_n6_β:
 sub r12, 24
 jmp xchain606_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain606_n7_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx618_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx618_10
.Lx618_9:
 xor eax, eax
.Lx618_10:
 test rax, rax
 jz .Lx618_0
 mov r8d, 0
 lea rcx, [rip + .Lx618_4]
 lea rdx, [rip + .Lx618_5]
 jmp rax
.Lx618_4:
 jmp xchain606_n6_α
.Lx618_5:
 jmp xchain606_n5_β
.Lx618_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx618_2:
 test rax, rax
 je .Lx618_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx618_7]
 lea rdx, [rip + .Lx618_8]
 jmp rax
.Lx618_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx618_2
.Lx618_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx618_2
.Lx618_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain606_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx618_6]
 sub rsp, 8
 push rax
 jmp xchain606_n6_α
.Lx618_6:
 add rsp, 16
 jmp xchain606_n5_β
 xchain606_n7_β:
 jmp qword ptr [rsp]
proc_PAT$38_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$38_γ
proc_PAT$38_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$38_attempt
8:
jmp proc_PAT$38_ω
proc_PAT$38_res:
add rsp, 8
pop rbp
proc_PAT$38_β:
jmp qword ptr [rbp + 256]
proc_PAT$38_γ:
push rbp
lea rax, [rip + proc_PAT$38_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$38_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$39_α
proc_PAT$39_α:
#=======================================================================================================================
    .global proc_PAT$39_α
    .global proc_PAT$39_β
    .global proc_PAT$39_γ
    .global proc_PAT$39_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$39_attempt:
proc_PAT$39_α_body:
lea rax, [rip + xchain619_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain619_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain619_n1_α
xchain619_n0_as:
 jmp proc_PAT$39_γ
 xchain619_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain619_n1_β
 jmp xchain619_n2_β
xchain619_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain619_n2_α
 jmp proc_PAT$39_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain619_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain619_n3_α
xchain619_n1_as:
 jmp xchain619_n0_as
 xchain619_n1_β:
 jmp xchain619_n6_β
xchain619_n1_af:
 jmp xchain619_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain619_n2_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx624_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx624_10
.Lx624_9:
 xor eax, eax
.Lx624_10:
 test rax, rax
 jz .Lx624_0
 mov r8d, 0
 lea rcx, [rip + .Lx624_4]
 lea rdx, [rip + .Lx624_5]
 jmp rax
.Lx624_4:
 jmp xchain619_n0_as
.Lx624_5:
 jmp xchain619_n0_af
.Lx624_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx624_2:
 test rax, rax
 je .Lx624_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx624_7]
 lea rdx, [rip + .Lx624_8]
 jmp rax
.Lx624_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx624_2
.Lx624_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx624_2
.Lx624_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain619_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx624_6]
 sub rsp, 8
 push rax
 jmp xchain619_n0_as
.Lx624_6:
 add rsp, 16
 jmp xchain619_n0_af
 xchain619_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain619_n3_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx625_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx625_10
.Lx625_9:
 xor eax, eax
.Lx625_10:
 test rax, rax
 jz .Lx625_0
 mov r8d, 0
 lea rcx, [rip + .Lx625_4]
 lea rdx, [rip + .Lx625_5]
 jmp rax
.Lx625_4:
 jmp xchain619_n4_α
.Lx625_5:
 jmp xchain619_n1_af
.Lx625_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx625_2:
 test rax, rax
 je .Lx625_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx625_7]
 lea rdx, [rip + .Lx625_8]
 jmp rax
.Lx625_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx625_2
.Lx625_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx625_2
.Lx625_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain619_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx625_6]
 sub rsp, 8
 push rax
 jmp xchain619_n4_α
.Lx625_6:
 add rsp, 16
 jmp xchain619_n1_af
 xchain619_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain619_n4_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx626_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx626_10
.Lx626_9:
 xor eax, eax
.Lx626_10:
 test rax, rax
 jz .Lx626_0
 mov r8d, 0
 lea rcx, [rip + .Lx626_4]
 lea rdx, [rip + .Lx626_5]
 jmp rax
.Lx626_4:
 jmp xchain619_n5_α
.Lx626_5:
 jmp xchain619_n3_β
.Lx626_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx626_2:
 test rax, rax
 je .Lx626_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx626_7]
 lea rdx, [rip + .Lx626_8]
 jmp rax
.Lx626_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx626_2
.Lx626_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx626_2
.Lx626_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain619_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx626_6]
 sub rsp, 8
 push rax
 jmp xchain619_n5_α
.Lx626_6:
 add rsp, 16
 jmp xchain619_n3_β
 xchain619_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain619_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain619_n7_α
 xchain619_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain619_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain619_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S34]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain619_n1_as
 xchain619_n6_β:
 sub r12, 24
 jmp xchain619_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain619_n7_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx631_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx631_10
.Lx631_9:
 xor eax, eax
.Lx631_10:
 test rax, rax
 jz .Lx631_0
 mov r8d, 0
 lea rcx, [rip + .Lx631_4]
 lea rdx, [rip + .Lx631_5]
 jmp rax
.Lx631_4:
 jmp xchain619_n6_α
.Lx631_5:
 jmp xchain619_n5_β
.Lx631_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx631_2:
 test rax, rax
 je .Lx631_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx631_7]
 lea rdx, [rip + .Lx631_8]
 jmp rax
.Lx631_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx631_2
.Lx631_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx631_2
.Lx631_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain619_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx631_6]
 sub rsp, 8
 push rax
 jmp xchain619_n6_α
.Lx631_6:
 add rsp, 16
 jmp xchain619_n5_β
 xchain619_n7_β:
 jmp qword ptr [rsp]
proc_PAT$39_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$39_γ
proc_PAT$39_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$39_attempt
8:
jmp proc_PAT$39_ω
proc_PAT$39_res:
add rsp, 8
pop rbp
proc_PAT$39_β:
jmp qword ptr [rbp + 256]
proc_PAT$39_γ:
push rbp
lea rax, [rip + proc_PAT$39_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$39_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$40_α
proc_PAT$40_α:
#=======================================================================================================================
    .global proc_PAT$40_α
    .global proc_PAT$40_β
    .global proc_PAT$40_γ
    .global proc_PAT$40_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 80
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 72], rsp
mov qword ptr [rbp + 80], r8
mov dword ptr [rbp + 72], r14d
proc_PAT$40_attempt:
proc_PAT$40_α_body:
lea rax, [rip + xchain632_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_SPAN
 xchain632_n0_α:
 sub rsp, 16
 lea rdi, [rip + .C0]
 mov dword ptr [rsp + 0], 0
.Lx634_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx634_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 cmp byte ptr [rdi+rsi], 0
 je .Lx634_1
 add dword ptr [rsp + 0], 1
 jmp .Lx634_0
.Lx634_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx634_240
 add rsp, 16
 jmp proc_PAT$40_scanfail
.Lx634_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp proc_PAT$40_scanhit
 xchain632_n0_β:
 xchain632_n0_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp proc_PAT$40_scanfail
proc_PAT$40_scanhit:
cmp qword ptr [rbp + 80], 1
jne 7f
mov ecx, dword ptr [rbp + 72]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$40_γ
proc_PAT$40_scanfail:
cmp qword ptr [rbp + 80], 1
jne 8f
mov eax, dword ptr [rbp + 72]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 72], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$40_attempt
8:
jmp proc_PAT$40_ω
proc_PAT$40_res:
add rsp, 8
pop rbp
proc_PAT$40_β:
jmp qword ptr [rbp + 48]
proc_PAT$40_γ:
push rbp
lea rax, [rip + proc_PAT$40_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$40_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
  .globl proc_PAT$41_α
proc_PAT$41_α:
#=======================================================================================================================
    .global proc_PAT$41_α
    .global proc_PAT$41_β
    .global proc_PAT$41_γ
    .global proc_PAT$41_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$41_attempt:
proc_PAT$41_α_body:
lea rax, [rip + xchain635_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain635_n0_α:
 mov dword ptr [rbp + 32], r14d
 jmp xchain635_n1_α
xchain635_n0_as:
 jmp proc_PAT$41_γ
 xchain635_n0_β:
 jmp xchain635_n3_β
xchain635_n0_af:
 jmp proc_PAT$41_ω
# IR_MATCH_ANY
 xchain635_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain635_n0_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C1]
 cmp byte ptr [rdi+rsi], 0
 je xchain635_n0_af
 add r14d, 1
 jmp xchain635_n2_α
 xchain635_n1_β:
 sub r14d, 1
 jmp xchain635_n0_af
# IR_MATCH_ALT_NARY
 xchain635_n2_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain635_n4_α
xchain635_n2_as:
 jmp xchain635_n3_α
 xchain635_n2_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain635_n4_β
 jmp xchain635_n5_β
xchain635_n2_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain635_n5_α
 jmp xchain635_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain635_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx641_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx641_10
.Lx641_9:
 xor eax, eax
.Lx641_10:
 test rax, rax
 jz .Lx641_0
 mov r8d, 0
 lea rcx, [rip + .Lx641_4]
 lea rdx, [rip + .Lx641_5]
 jmp rax
.Lx641_4:
 jmp xchain635_n0_as
.Lx641_5:
 jmp xchain635_n2_β
.Lx641_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx641_2:
 test rax, rax
 je .Lx641_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx641_7]
 lea rdx, [rip + .Lx641_8]
 jmp rax
.Lx641_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx641_2
.Lx641_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx641_2
.Lx641_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain635_n2_β
 mov r14d, eax
 lea rax, [rip + .Lx641_6]
 sub rsp, 8
 push rax
 jmp xchain635_n0_as
.Lx641_6:
 add rsp, 16
 jmp xchain635_n2_β
 xchain635_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_ANY
 xchain635_n4_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain635_n2_af
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 je xchain635_n2_af
 add r14d, 1
 jmp xchain635_n2_as
 xchain635_n4_β:
 sub r14d, 1
 jmp xchain635_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain635_n5_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx643_0
 mov r8d, 0
 lea rcx, [rip + .Lx643_4]
 lea rdx, [rip + .Lx643_5]
 jmp rax
.Lx643_4:
 jmp xchain635_n2_as
.Lx643_5:
 jmp xchain635_n2_af
.Lx643_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx643_2:
 test rax, rax
 je .Lx643_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx643_7]
 lea rdx, [rip + .Lx643_8]
 jmp rax
.Lx643_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx643_2
.Lx643_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx643_2
.Lx643_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain635_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx643_6]
 sub rsp, 8
 push rax
 jmp xchain635_n2_as
.Lx643_6:
 add rsp, 16
 jmp xchain635_n2_af
 xchain635_n5_β:
 jmp qword ptr [rsp]
proc_PAT$41_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$41_γ
proc_PAT$41_scanfail:
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
jmp proc_PAT$41_attempt
8:
jmp proc_PAT$41_ω
proc_PAT$41_res:
add rsp, 8
pop rbp
proc_PAT$41_β:
jmp qword ptr [rbp + 176]
proc_PAT$41_γ:
push rbp
lea rax, [rip + proc_PAT$41_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$41_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$42_α
proc_PAT$42_α:
#=======================================================================================================================
    .global proc_PAT$42_α
    .global proc_PAT$42_β
    .global proc_PAT$42_γ
    .global proc_PAT$42_ω
  sub rsp, 480
  mov [rsp + 456], rcx
  mov [rsp + 464], rdx
  mov [rsp + 472], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 448
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 440], rsp
mov qword ptr [rbp + 448], r8
mov dword ptr [rbp + 440], r14d
proc_PAT$42_attempt:
proc_PAT$42_α_body:
lea rax, [rip + xchain644_n0_β]
mov qword ptr [rbp + 416], rax
# IR_MATCH_ALT_NARY
 xchain644_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain644_n1_α
xchain644_n0_as:
 jmp proc_PAT$42_γ
 xchain644_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain644_n1_β
 jmp xchain644_n2_β
xchain644_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain644_n2_α
 jmp proc_PAT$42_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain644_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain644_n3_α
xchain644_n1_as:
 jmp xchain644_n0_as
 xchain644_n1_β:
 jmp xchain644_n6_β
xchain644_n1_af:
 jmp xchain644_n0_af
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain644_n2_α:
 mov dword ptr [rbp + 336], r14d
 jmp xchain644_n7_α
xchain644_n2_as:
 jmp xchain644_n0_as
 xchain644_n2_β:
 jmp xchain644_n8_β
xchain644_n2_af:
 jmp xchain644_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n3_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx651_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx651_10
.Lx651_9:
 xor eax, eax
.Lx651_10:
 test rax, rax
 jz .Lx651_0
 mov r8d, 0
 lea rcx, [rip + .Lx651_4]
 lea rdx, [rip + .Lx651_5]
 jmp rax
.Lx651_4:
 jmp xchain644_n4_α
.Lx651_5:
 jmp xchain644_n1_af
.Lx651_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx651_2:
 test rax, rax
 je .Lx651_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx651_7]
 lea rdx, [rip + .Lx651_8]
 jmp rax
.Lx651_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx651_2
.Lx651_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx651_2
.Lx651_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx651_6]
 sub rsp, 8
 push rax
 jmp xchain644_n4_α
.Lx651_6:
 add rsp, 16
 jmp xchain644_n1_af
 xchain644_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain644_n4_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain644_n3_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 46
 jne xchain644_n3_β
 add r14d, 1
 jmp xchain644_n5_α
 xchain644_n4_β:
 sub r14d, 1
 jmp xchain644_n3_β
# IR_MATCH_ALT_NARY
 xchain644_n5_α:
 mov dword ptr [rbp + 144], r14d
 mov dword ptr [rbp + 148], 0
 jmp xchain644_n9_α
xchain644_n5_as:
 jmp xchain644_n6_α
 xchain644_n5_β:
 mov eax, dword ptr [rbp + 148]
 cmp eax, 0
 je xchain644_n9_β
 jmp xchain644_n10_β
xchain644_n5_af:
 add dword ptr [rbp + 148], 1
 mov r14d, dword ptr [rbp + 144]
 mov eax, dword ptr [rbp + 148]
 cmp eax, 1
 je xchain644_n10_α
 jmp xchain644_n4_β
# IR_MATCH_ALT_NARY
 xchain644_n6_α:
 mov dword ptr [rbp + 240], r14d
 mov dword ptr [rbp + 244], 0
 jmp xchain644_n11_α
xchain644_n6_as:
 jmp xchain644_n1_as
 xchain644_n6_β:
 mov eax, dword ptr [rbp + 244]
 cmp eax, 0
 je xchain644_n11_β
 jmp xchain644_n12_β
xchain644_n6_af:
 add dword ptr [rbp + 244], 1
 mov r14d, dword ptr [rbp + 240]
 mov eax, dword ptr [rbp + 244]
 cmp eax, 1
 je xchain644_n12_α
 jmp xchain644_n5_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n7_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx658_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx658_10
.Lx658_9:
 xor eax, eax
.Lx658_10:
 test rax, rax
 jz .Lx658_0
 mov r8d, 0
 lea rcx, [rip + .Lx658_4]
 lea rdx, [rip + .Lx658_5]
 jmp rax
.Lx658_4:
 jmp xchain644_n8_α
.Lx658_5:
 jmp xchain644_n2_af
.Lx658_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx658_2:
 test rax, rax
 je .Lx658_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx658_7]
 lea rdx, [rip + .Lx658_8]
 jmp rax
.Lx658_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx658_2
.Lx658_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx658_2
.Lx658_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n2_af
 mov r14d, eax
 lea rax, [rip + .Lx658_6]
 sub rsp, 8
 push rax
 jmp xchain644_n8_α
.Lx658_6:
 add rsp, 16
 jmp xchain644_n2_af
 xchain644_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n8_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx659_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx659_10
.Lx659_9:
 xor eax, eax
.Lx659_10:
 test rax, rax
 jz .Lx659_0
 mov r8d, 0
 lea rcx, [rip + .Lx659_4]
 lea rdx, [rip + .Lx659_5]
 jmp rax
.Lx659_4:
 jmp xchain644_n2_as
.Lx659_5:
 jmp xchain644_n7_β
.Lx659_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx659_2:
 test rax, rax
 je .Lx659_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx659_7]
 lea rdx, [rip + .Lx659_8]
 jmp rax
.Lx659_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx659_2
.Lx659_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx659_2
.Lx659_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n7_β
 mov r14d, eax
 lea rax, [rip + .Lx659_6]
 sub rsp, 8
 push rax
 jmp xchain644_n2_as
.Lx659_6:
 add rsp, 16
 jmp xchain644_n7_β
 xchain644_n8_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n9_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx660_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx660_10
.Lx660_9:
 xor eax, eax
.Lx660_10:
 test rax, rax
 jz .Lx660_0
 mov r8d, 0
 lea rcx, [rip + .Lx660_4]
 lea rdx, [rip + .Lx660_5]
 jmp rax
.Lx660_4:
 jmp xchain644_n5_as
.Lx660_5:
 jmp xchain644_n5_af
.Lx660_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx660_2:
 test rax, rax
 je .Lx660_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx660_7]
 lea rdx, [rip + .Lx660_8]
 jmp rax
.Lx660_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx660_2
.Lx660_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx660_2
.Lx660_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx660_6]
 sub rsp, 8
 push rax
 jmp xchain644_n5_as
.Lx660_6:
 add rsp, 16
 jmp xchain644_n5_af
 xchain644_n9_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n10_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx661_0
 mov r8d, 0
 lea rcx, [rip + .Lx661_4]
 lea rdx, [rip + .Lx661_5]
 jmp rax
.Lx661_4:
 jmp xchain644_n5_as
.Lx661_5:
 jmp xchain644_n5_af
.Lx661_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx661_2:
 test rax, rax
 je .Lx661_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx661_7]
 lea rdx, [rip + .Lx661_8]
 jmp rax
.Lx661_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx661_2
.Lx661_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx661_2
.Lx661_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n5_af
 mov r14d, eax
 lea rax, [rip + .Lx661_6]
 sub rsp, 8
 push rax
 jmp xchain644_n5_as
.Lx661_6:
 add rsp, 16
 jmp xchain644_n5_af
 xchain644_n10_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n11_α:
 mov rax, qword ptr [1879052480]
 mov rdx, qword ptr [1879052488]
 cmp eax, 3
 jne .Lx662_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx662_10
.Lx662_9:
 xor eax, eax
.Lx662_10:
 test rax, rax
 jz .Lx662_0
 mov r8d, 0
 lea rcx, [rip + .Lx662_4]
 lea rdx, [rip + .Lx662_5]
 jmp rax
.Lx662_4:
 jmp xchain644_n6_as
.Lx662_5:
 jmp xchain644_n6_af
.Lx662_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S2]
 xor esi, esi
 call rt_defer_open@PLT
.Lx662_2:
 test rax, rax
 je .Lx662_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx662_7]
 lea rdx, [rip + .Lx662_8]
 jmp rax
.Lx662_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx662_2
.Lx662_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx662_2
.Lx662_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx662_6]
 sub rsp, 8
 push rax
 jmp xchain644_n6_as
.Lx662_6:
 add rsp, 16
 jmp xchain644_n6_af
 xchain644_n11_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain644_n12_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx663_0
 mov r8d, 0
 lea rcx, [rip + .Lx663_4]
 lea rdx, [rip + .Lx663_5]
 jmp rax
.Lx663_4:
 jmp xchain644_n6_as
.Lx663_5:
 jmp xchain644_n6_af
.Lx663_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx663_2:
 test rax, rax
 je .Lx663_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx663_7]
 lea rdx, [rip + .Lx663_8]
 jmp rax
.Lx663_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx663_2
.Lx663_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx663_2
.Lx663_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain644_n6_af
 mov r14d, eax
 lea rax, [rip + .Lx663_6]
 sub rsp, 8
 push rax
 jmp xchain644_n6_as
.Lx663_6:
 add rsp, 16
 jmp xchain644_n6_af
 xchain644_n12_β:
 jmp qword ptr [rsp]
proc_PAT$42_scanhit:
cmp qword ptr [rbp + 448], 1
jne 7f
mov ecx, dword ptr [rbp + 440]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$42_γ
proc_PAT$42_scanfail:
cmp qword ptr [rbp + 448], 1
jne 8f
mov eax, dword ptr [rbp + 440]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 440], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$42_attempt
8:
jmp proc_PAT$42_ω
proc_PAT$42_res:
add rsp, 8
pop rbp
proc_PAT$42_β:
jmp qword ptr [rbp + 416]
proc_PAT$42_γ:
push rbp
lea rax, [rip + proc_PAT$42_res]
push rax
mov rax, [rbp + 456]
mov rbp, [rbp + 472]
jmp rax
proc_PAT$42_ω:
mov rax, [rbp + 464]
lea rsp, [rbp + 480]
mov rbp, [rbp + 472]
jmp rax
  .globl proc_PAT$43_α
proc_PAT$43_α:
#=======================================================================================================================
    .global proc_PAT$43_α
    .global proc_PAT$43_β
    .global proc_PAT$43_γ
    .global proc_PAT$43_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$43_attempt:
proc_PAT$43_α_body:
lea rax, [rip + xchain664_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain664_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain664_n1_α
 xchain664_n0_β:
 add rsp, 16
 jmp proc_PAT$43_scanfail
# IR_MATCH_ANY
 xchain664_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx667_239
 add rsp, 16
 jmp proc_PAT$43_scanfail
.Lx667_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C2]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx667_240
 add rsp, 16
 jmp proc_PAT$43_scanfail
.Lx667_240:
 add r14d, 1
 jmp xchain664_n2_α
 xchain664_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$43_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain664_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S35]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$43_scanhit
 xchain664_n2_β:
 sub r12, 24
 jmp xchain664_n1_β
proc_PAT$43_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$43_γ
proc_PAT$43_scanfail:
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
jmp proc_PAT$43_attempt
8:
jmp proc_PAT$43_ω
proc_PAT$43_res:
add rsp, 8
pop rbp
proc_PAT$43_β:
jmp qword ptr [rbp + 80]
proc_PAT$43_γ:
push rbp
lea rax, [rip + proc_PAT$43_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$43_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$44_α
proc_PAT$44_α:
#=======================================================================================================================
    .global proc_PAT$44_α
    .global proc_PAT$44_β
    .global proc_PAT$44_γ
    .global proc_PAT$44_ω
  sub rsp, 144
  mov [rsp + 120], rcx
  mov [rsp + 128], rdx
  mov [rsp + 136], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 112
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 104], rsp
mov qword ptr [rbp + 112], r8
mov dword ptr [rbp + 104], r14d
proc_PAT$44_attempt:
proc_PAT$44_α_body:
lea rax, [rip + xchain670_n2_β]
mov qword ptr [rbp + 80], rax
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain670_n0_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain670_n1_α
 xchain670_n0_β:
 add rsp, 16
 jmp proc_PAT$44_scanfail
# IR_MATCH_ANY
 xchain670_n1_α:
 mov eax, r14d
 cmp eax, r15d
 jl .Lx673_239
 add rsp, 16
 jmp proc_PAT$44_scanfail
.Lx673_239:
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .C3]
 cmp byte ptr [rdi+rsi], 0
 jne .Lx673_240
 add rsp, 16
 jmp proc_PAT$44_scanfail
.Lx673_240:
 add r14d, 1
 jmp xchain670_n2_α
 xchain670_n1_β:
 sub r14d, 1
 add rsp, 16
 jmp proc_PAT$44_scanfail
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain670_n2_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S36]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$44_scanhit
 xchain670_n2_β:
 sub r12, 24
 jmp xchain670_n1_β
proc_PAT$44_scanhit:
cmp qword ptr [rbp + 112], 1
jne 7f
mov ecx, dword ptr [rbp + 104]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$44_γ
proc_PAT$44_scanfail:
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
jmp proc_PAT$44_attempt
8:
jmp proc_PAT$44_ω
proc_PAT$44_res:
add rsp, 8
pop rbp
proc_PAT$44_β:
jmp qword ptr [rbp + 80]
proc_PAT$44_γ:
push rbp
lea rax, [rip + proc_PAT$44_res]
push rax
mov rax, [rbp + 120]
mov rbp, [rbp + 136]
jmp rax
proc_PAT$44_ω:
mov rax, [rbp + 128]
lea rsp, [rbp + 144]
mov rbp, [rbp + 136]
jmp rax
  .globl proc_PAT$45_α
proc_PAT$45_α:
#=======================================================================================================================
    .global proc_PAT$45_α
    .global proc_PAT$45_β
    .global proc_PAT$45_γ
    .global proc_PAT$45_ω
  sub rsp, 224
  mov [rsp + 200], rcx
  mov [rsp + 208], rdx
  mov [rsp + 216], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 192
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 184], rsp
mov qword ptr [rbp + 192], r8
mov dword ptr [rbp + 184], r14d
proc_PAT$45_attempt:
proc_PAT$45_α_body:
lea rax, [rip + xchain676_n2_β]
mov qword ptr [rbp + 160], rax
# IR_MATCH_CAPTURE_SAVE push
 xchain676_n0_α:
 lea rdi, [rbp + 48]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain676_n1_α
 xchain676_n0_β:
 lea rdi, [rbp + 48]
 call rt_cap_pop@PLT
 jmp proc_PAT$45_scanfail
# IR_MATCH_ALT_NARY
 xchain676_n1_α:
 mov dword ptr [rbp + 80], r14d
 mov dword ptr [rbp + 84], 0
 jmp xchain676_n3_α
xchain676_n1_as:
 jmp xchain676_n2_α
 xchain676_n1_β:
 mov eax, dword ptr [rbp + 84]
 cmp eax, 0
 je xchain676_n3_β
 jmp xchain676_n4_β
xchain676_n1_af:
 add dword ptr [rbp + 84], 1
 mov r14d, dword ptr [rbp + 80]
 mov eax, dword ptr [rbp + 84]
 cmp eax, 1
 je xchain676_n4_α
 jmp xchain676_n0_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain676_n2_α:
 lea rdi, [rbp + 48]
 call rt_cap_top@PLT
 lea rcx, [rip + .S37]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$45_scanhit
 xchain676_n2_β:
 sub r12, 24
 jmp xchain676_n1_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain676_n3_α:
 mov rax, qword ptr [1879052496]
 mov rdx, qword ptr [1879052504]
 cmp eax, 3
 jne .Lx683_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx683_10
.Lx683_9:
 xor eax, eax
.Lx683_10:
 test rax, rax
 jz .Lx683_0
 mov r8d, 0
 lea rcx, [rip + .Lx683_4]
 lea rdx, [rip + .Lx683_5]
 jmp rax
.Lx683_4:
 jmp xchain676_n1_as
.Lx683_5:
 jmp xchain676_n1_af
.Lx683_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S6]
 xor esi, esi
 call rt_defer_open@PLT
.Lx683_2:
 test rax, rax
 je .Lx683_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx683_7]
 lea rdx, [rip + .Lx683_8]
 jmp rax
.Lx683_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx683_2
.Lx683_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx683_2
.Lx683_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain676_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx683_6]
 sub rsp, 8
 push rax
 jmp xchain676_n1_as
.Lx683_6:
 add rsp, 16
 jmp xchain676_n1_af
 xchain676_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain676_n4_α:
 mov rax, qword ptr [1879052464]
 mov rdx, qword ptr [1879052472]
 cmp eax, 3
 jne .Lx684_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx684_10
.Lx684_9:
 xor eax, eax
.Lx684_10:
 test rax, rax
 jz .Lx684_0
 mov r8d, 0
 lea rcx, [rip + .Lx684_4]
 lea rdx, [rip + .Lx684_5]
 jmp rax
.Lx684_4:
 jmp xchain676_n1_as
.Lx684_5:
 jmp xchain676_n1_af
.Lx684_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S0]
 xor esi, esi
 call rt_defer_open@PLT
.Lx684_2:
 test rax, rax
 je .Lx684_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx684_7]
 lea rdx, [rip + .Lx684_8]
 jmp rax
.Lx684_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx684_2
.Lx684_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx684_2
.Lx684_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain676_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx684_6]
 sub rsp, 8
 push rax
 jmp xchain676_n1_as
.Lx684_6:
 add rsp, 16
 jmp xchain676_n1_af
 xchain676_n4_β:
 jmp qword ptr [rsp]
proc_PAT$45_scanhit:
cmp qword ptr [rbp + 192], 1
jne 7f
mov ecx, dword ptr [rbp + 184]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$45_γ
proc_PAT$45_scanfail:
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
jmp proc_PAT$45_attempt
8:
jmp proc_PAT$45_ω
proc_PAT$45_res:
add rsp, 8
pop rbp
proc_PAT$45_β:
jmp qword ptr [rbp + 160]
proc_PAT$45_γ:
push rbp
lea rax, [rip + proc_PAT$45_res]
push rax
mov rax, [rbp + 200]
mov rbp, [rbp + 216]
jmp rax
proc_PAT$45_ω:
mov rax, [rbp + 208]
lea rsp, [rbp + 224]
mov rbp, [rbp + 216]
jmp rax
  .globl proc_PAT$46_α
proc_PAT$46_α:
#=======================================================================================================================
    .global proc_PAT$46_α
    .global proc_PAT$46_β
    .global proc_PAT$46_γ
    .global proc_PAT$46_ω
  sub rsp, 240
  mov [rsp + 216], rcx
  mov [rsp + 224], rdx
  mov [rsp + 232], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 208
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 200], rsp
mov qword ptr [rbp + 208], r8
mov dword ptr [rbp + 200], r14d
proc_PAT$46_attempt:
proc_PAT$46_α_body:
lea rax, [rip + xchain685_n0_β]
mov qword ptr [rbp + 176], rax
# IR_MATCH_ALT_NARY
 xchain685_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain685_n1_α
xchain685_n0_as:
 jmp proc_PAT$46_γ
 xchain685_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain685_n1_β
 jmp xchain685_n2_β
xchain685_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain685_n2_α
 jmp proc_PAT$46_ω
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain685_n1_α:
 mov rax, qword ptr [1879052544]
 mov rdx, qword ptr [1879052552]
 cmp eax, 3
 jne .Lx688_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx688_10
.Lx688_9:
 xor eax, eax
.Lx688_10:
 test rax, rax
 jz .Lx688_0
 mov r8d, 0
 lea rcx, [rip + .Lx688_4]
 lea rdx, [rip + .Lx688_5]
 jmp rax
.Lx688_4:
 jmp xchain685_n0_as
.Lx688_5:
 jmp xchain685_n0_af
.Lx688_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S7]
 xor esi, esi
 call rt_defer_open@PLT
.Lx688_2:
 test rax, rax
 je .Lx688_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx688_7]
 lea rdx, [rip + .Lx688_8]
 jmp rax
.Lx688_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx688_2
.Lx688_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx688_2
.Lx688_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain685_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx688_6]
 sub rsp, 8
 push rax
 jmp xchain685_n0_as
.Lx688_6:
 add rsp, 16
 jmp xchain685_n0_af
 xchain685_n1_β:
 jmp qword ptr [rsp]
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain685_n2_α:
 mov dword ptr [rbp + 96], r14d
 jmp xchain685_n3_α
xchain685_n2_as:
 jmp xchain685_n0_as
 xchain685_n2_β:
 jmp xchain685_n5_β
xchain685_n2_af:
 jmp xchain685_n0_af
# IR_MATCH_LIT
 xchain685_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain685_n2_af
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 40
 jne xchain685_n2_af
 add r14d, 1
 jmp xchain685_n4_α
 xchain685_n3_β:
 sub r14d, 1
 jmp xchain685_n2_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain685_n4_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx693_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx693_10
.Lx693_9:
 xor eax, eax
.Lx693_10:
 test rax, rax
 jz .Lx693_0
 mov r8d, 0
 lea rcx, [rip + .Lx693_4]
 lea rdx, [rip + .Lx693_5]
 jmp rax
.Lx693_4:
 jmp xchain685_n5_α
.Lx693_5:
 jmp xchain685_n3_β
.Lx693_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx693_2:
 test rax, rax
 je .Lx693_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx693_7]
 lea rdx, [rip + .Lx693_8]
 jmp rax
.Lx693_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx693_2
.Lx693_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx693_2
.Lx693_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain685_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx693_6]
 sub rsp, 8
 push rax
 jmp xchain685_n5_α
.Lx693_6:
 add rsp, 16
 jmp xchain685_n3_β
 xchain685_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_LIT
 xchain685_n5_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain685_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 41
 jne xchain685_n4_β
 add r14d, 1
 jmp xchain685_n2_as
 xchain685_n5_β:
 sub r14d, 1
 jmp xchain685_n4_β
proc_PAT$46_scanhit:
cmp qword ptr [rbp + 208], 1
jne 7f
mov ecx, dword ptr [rbp + 200]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$46_γ
proc_PAT$46_scanfail:
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
jmp proc_PAT$46_attempt
8:
jmp proc_PAT$46_ω
proc_PAT$46_res:
add rsp, 8
pop rbp
proc_PAT$46_β:
jmp qword ptr [rbp + 176]
proc_PAT$46_γ:
push rbp
lea rax, [rip + proc_PAT$46_res]
push rax
mov rax, [rbp + 216]
mov rbp, [rbp + 232]
jmp rax
proc_PAT$46_ω:
mov rax, [rbp + 224]
lea rsp, [rbp + 240]
mov rbp, [rbp + 232]
jmp rax
  .globl proc_PAT$47_α
proc_PAT$47_α:
#=======================================================================================================================
    .global proc_PAT$47_α
    .global proc_PAT$47_β
    .global proc_PAT$47_γ
    .global proc_PAT$47_ω
  sub rsp, 288
  mov [rsp + 264], rcx
  mov [rsp + 272], rdx
  mov [rsp + 280], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 256
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 248], rsp
mov qword ptr [rbp + 256], r8
mov dword ptr [rbp + 248], r14d
proc_PAT$47_attempt:
proc_PAT$47_α_body:
lea rax, [rip + xchain696_n0_β]
mov qword ptr [rbp + 224], rax
# IR_MATCH_ALT_NARY
 xchain696_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain696_n1_α
xchain696_n0_as:
 jmp proc_PAT$47_γ
 xchain696_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain696_n1_β
 jmp xchain696_n2_β
xchain696_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain696_n2_α
 jmp proc_PAT$47_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain696_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain696_n3_α
xchain696_n1_as:
 jmp xchain696_n0_as
 xchain696_n1_β:
 jmp xchain696_n5_β
xchain696_n1_af:
 jmp xchain696_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain696_n2_α:
 mov rax, qword ptr [1879052560]
 mov rdx, qword ptr [1879052568]
 cmp eax, 3
 jne .Lx701_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx701_10
.Lx701_9:
 xor eax, eax
.Lx701_10:
 test rax, rax
 jz .Lx701_0
 mov r8d, 0
 lea rcx, [rip + .Lx701_4]
 lea rdx, [rip + .Lx701_5]
 jmp rax
.Lx701_4:
 jmp xchain696_n0_as
.Lx701_5:
 jmp xchain696_n0_af
.Lx701_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S9]
 xor esi, esi
 call rt_defer_open@PLT
.Lx701_2:
 test rax, rax
 je .Lx701_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx701_7]
 lea rdx, [rip + .Lx701_8]
 jmp rax
.Lx701_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx701_2
.Lx701_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx701_2
.Lx701_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain696_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx701_6]
 sub rsp, 8
 push rax
 jmp xchain696_n0_as
.Lx701_6:
 add rsp, 16
 jmp xchain696_n0_af
 xchain696_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain696_n3_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx702_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx702_10
.Lx702_9:
 xor eax, eax
.Lx702_10:
 test rax, rax
 jz .Lx702_0
 mov r8d, 0
 lea rcx, [rip + .Lx702_4]
 lea rdx, [rip + .Lx702_5]
 jmp rax
.Lx702_4:
 jmp xchain696_n4_α
.Lx702_5:
 jmp xchain696_n1_af
.Lx702_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx702_2:
 test rax, rax
 je .Lx702_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx702_7]
 lea rdx, [rip + .Lx702_8]
 jmp rax
.Lx702_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx702_2
.Lx702_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx702_2
.Lx702_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain696_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx702_6]
 sub rsp, 8
 push rax
 jmp xchain696_n4_α
.Lx702_6:
 add rsp, 16
 jmp xchain696_n1_af
 xchain696_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain696_n4_α:
 lea rdi, [rbp + 144]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain696_n6_α
 xchain696_n4_β:
 lea rdi, [rbp + 144]
 call rt_cap_pop@PLT
 jmp xchain696_n3_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain696_n5_α:
 lea rdi, [rbp + 144]
 call rt_cap_top@PLT
 lea rcx, [rip + .S38]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain696_n1_as
 xchain696_n5_β:
 sub r12, 24
 jmp xchain696_n6_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain696_n6_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx707_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx707_10
.Lx707_9:
 xor eax, eax
.Lx707_10:
 test rax, rax
 jz .Lx707_0
 mov r8d, 0
 lea rcx, [rip + .Lx707_4]
 lea rdx, [rip + .Lx707_5]
 jmp rax
.Lx707_4:
 jmp xchain696_n5_α
.Lx707_5:
 jmp xchain696_n4_β
.Lx707_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx707_2:
 test rax, rax
 je .Lx707_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx707_7]
 lea rdx, [rip + .Lx707_8]
 jmp rax
.Lx707_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx707_2
.Lx707_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx707_2
.Lx707_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain696_n4_β
 mov r14d, eax
 lea rax, [rip + .Lx707_6]
 sub rsp, 8
 push rax
 jmp xchain696_n5_α
.Lx707_6:
 add rsp, 16
 jmp xchain696_n4_β
 xchain696_n6_β:
 jmp qword ptr [rsp]
proc_PAT$47_scanhit:
cmp qword ptr [rbp + 256], 1
jne 7f
mov ecx, dword ptr [rbp + 248]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$47_γ
proc_PAT$47_scanfail:
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
jmp proc_PAT$47_attempt
8:
jmp proc_PAT$47_ω
proc_PAT$47_res:
add rsp, 8
pop rbp
proc_PAT$47_β:
jmp qword ptr [rbp + 224]
proc_PAT$47_γ:
push rbp
lea rax, [rip + proc_PAT$47_res]
push rax
mov rax, [rbp + 264]
mov rbp, [rbp + 280]
jmp rax
proc_PAT$47_ω:
mov rax, [rbp + 272]
lea rsp, [rbp + 288]
mov rbp, [rbp + 280]
jmp rax
  .globl proc_PAT$48_α
proc_PAT$48_α:
#=======================================================================================================================
    .global proc_PAT$48_α
    .global proc_PAT$48_β
    .global proc_PAT$48_γ
    .global proc_PAT$48_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$48_attempt:
proc_PAT$48_α_body:
lea rax, [rip + xchain708_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain708_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain708_n1_α
xchain708_n0_as:
 jmp proc_PAT$48_γ
 xchain708_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain708_n1_β
 jmp xchain708_n2_β
xchain708_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain708_n2_α
 jmp proc_PAT$48_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain708_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain708_n3_α
xchain708_n1_as:
 jmp xchain708_n0_as
 xchain708_n1_β:
 jmp xchain708_n6_β
xchain708_n1_af:
 jmp xchain708_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain708_n2_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx713_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx713_10
.Lx713_9:
 xor eax, eax
.Lx713_10:
 test rax, rax
 jz .Lx713_0
 mov r8d, 0
 lea rcx, [rip + .Lx713_4]
 lea rdx, [rip + .Lx713_5]
 jmp rax
.Lx713_4:
 jmp xchain708_n0_as
.Lx713_5:
 jmp xchain708_n0_af
.Lx713_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx713_2:
 test rax, rax
 je .Lx713_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx713_7]
 lea rdx, [rip + .Lx713_8]
 jmp rax
.Lx713_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx713_2
.Lx713_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx713_2
.Lx713_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain708_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx713_6]
 sub rsp, 8
 push rax
 jmp xchain708_n0_as
.Lx713_6:
 add rsp, 16
 jmp xchain708_n0_af
 xchain708_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain708_n3_α:
 mov rax, qword ptr [1879052576]
 mov rdx, qword ptr [1879052584]
 cmp eax, 3
 jne .Lx714_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx714_10
.Lx714_9:
 xor eax, eax
.Lx714_10:
 test rax, rax
 jz .Lx714_0
 mov r8d, 0
 lea rcx, [rip + .Lx714_4]
 lea rdx, [rip + .Lx714_5]
 jmp rax
.Lx714_4:
 jmp xchain708_n4_α
.Lx714_5:
 jmp xchain708_n1_af
.Lx714_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S12]
 xor esi, esi
 call rt_defer_open@PLT
.Lx714_2:
 test rax, rax
 je .Lx714_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx714_7]
 lea rdx, [rip + .Lx714_8]
 jmp rax
.Lx714_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx714_2
.Lx714_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx714_2
.Lx714_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain708_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx714_6]
 sub rsp, 8
 push rax
 jmp xchain708_n4_α
.Lx714_6:
 add rsp, 16
 jmp xchain708_n1_af
 xchain708_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain708_n4_α:
 mov rax, qword ptr [1879052528]
 mov rdx, qword ptr [1879052536]
 cmp eax, 3
 jne .Lx715_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx715_10
.Lx715_9:
 xor eax, eax
.Lx715_10:
 test rax, rax
 jz .Lx715_0
 mov r8d, 0
 lea rcx, [rip + .Lx715_4]
 lea rdx, [rip + .Lx715_5]
 jmp rax
.Lx715_4:
 jmp xchain708_n5_α
.Lx715_5:
 jmp xchain708_n3_β
.Lx715_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S13]
 xor esi, esi
 call rt_defer_open@PLT
.Lx715_2:
 test rax, rax
 je .Lx715_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx715_7]
 lea rdx, [rip + .Lx715_8]
 jmp rax
.Lx715_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx715_2
.Lx715_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx715_2
.Lx715_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain708_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx715_6]
 sub rsp, 8
 push rax
 jmp xchain708_n5_α
.Lx715_6:
 add rsp, 16
 jmp xchain708_n3_β
 xchain708_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain708_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain708_n7_α
 xchain708_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain708_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain708_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S39]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain708_n1_as
 xchain708_n6_β:
 sub r12, 24
 jmp xchain708_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain708_n7_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx720_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx720_10
.Lx720_9:
 xor eax, eax
.Lx720_10:
 test rax, rax
 jz .Lx720_0
 mov r8d, 0
 lea rcx, [rip + .Lx720_4]
 lea rdx, [rip + .Lx720_5]
 jmp rax
.Lx720_4:
 jmp xchain708_n6_α
.Lx720_5:
 jmp xchain708_n5_β
.Lx720_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx720_2:
 test rax, rax
 je .Lx720_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx720_7]
 lea rdx, [rip + .Lx720_8]
 jmp rax
.Lx720_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx720_2
.Lx720_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx720_2
.Lx720_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain708_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx720_6]
 sub rsp, 8
 push rax
 jmp xchain708_n6_α
.Lx720_6:
 add rsp, 16
 jmp xchain708_n5_β
 xchain708_n7_β:
 jmp qword ptr [rsp]
proc_PAT$48_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$48_γ
proc_PAT$48_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$48_attempt
8:
jmp proc_PAT$48_ω
proc_PAT$48_res:
add rsp, 8
pop rbp
proc_PAT$48_β:
jmp qword ptr [rbp + 256]
proc_PAT$48_γ:
push rbp
lea rax, [rip + proc_PAT$48_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$48_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
jmp rax
  .globl proc_PAT$49_α
proc_PAT$49_α:
#=======================================================================================================================
    .global proc_PAT$49_α
    .global proc_PAT$49_β
    .global proc_PAT$49_γ
    .global proc_PAT$49_ω
  sub rsp, 320
  mov [rsp + 296], rcx
  mov [rsp + 304], rdx
  mov [rsp + 312], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 288
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 280], rsp
mov qword ptr [rbp + 288], r8
mov dword ptr [rbp + 280], r14d
proc_PAT$49_attempt:
proc_PAT$49_α_body:
lea rax, [rip + xchain721_n0_β]
mov qword ptr [rbp + 256], rax
# IR_MATCH_ALT_NARY
 xchain721_n0_α:
 mov dword ptr [rbp + 32], r14d
 mov dword ptr [rbp + 36], 0
 jmp xchain721_n1_α
xchain721_n0_as:
 jmp proc_PAT$49_γ
 xchain721_n0_β:
 mov eax, dword ptr [rbp + 36]
 cmp eax, 0
 je xchain721_n1_β
 jmp xchain721_n2_β
xchain721_n0_af:
 add dword ptr [rbp + 36], 1
 mov r14d, dword ptr [rbp + 32]
 mov eax, dword ptr [rbp + 36]
 cmp eax, 1
 je xchain721_n2_α
 jmp proc_PAT$49_ω
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain721_n1_α:
 mov dword ptr [rbp + 64], r14d
 jmp xchain721_n3_α
xchain721_n1_as:
 jmp xchain721_n0_as
 xchain721_n1_β:
 jmp xchain721_n6_β
xchain721_n1_af:
 jmp xchain721_n0_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain721_n2_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx726_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx726_10
.Lx726_9:
 xor eax, eax
.Lx726_10:
 test rax, rax
 jz .Lx726_0
 mov r8d, 0
 lea rcx, [rip + .Lx726_4]
 lea rdx, [rip + .Lx726_5]
 jmp rax
.Lx726_4:
 jmp xchain721_n0_as
.Lx726_5:
 jmp xchain721_n0_af
.Lx726_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx726_2:
 test rax, rax
 je .Lx726_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx726_7]
 lea rdx, [rip + .Lx726_8]
 jmp rax
.Lx726_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx726_2
.Lx726_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx726_2
.Lx726_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain721_n0_af
 mov r14d, eax
 lea rax, [rip + .Lx726_6]
 sub rsp, 8
 push rax
 jmp xchain721_n0_as
.Lx726_6:
 add rsp, 16
 jmp xchain721_n0_af
 xchain721_n2_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain721_n3_α:
 mov rax, qword ptr [1879052592]
 mov rdx, qword ptr [1879052600]
 cmp eax, 3
 jne .Lx727_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx727_10
.Lx727_9:
 xor eax, eax
.Lx727_10:
 test rax, rax
 jz .Lx727_0
 mov r8d, 0
 lea rcx, [rip + .Lx727_4]
 lea rdx, [rip + .Lx727_5]
 jmp rax
.Lx727_4:
 jmp xchain721_n4_α
.Lx727_5:
 jmp xchain721_n1_af
.Lx727_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S15]
 xor esi, esi
 call rt_defer_open@PLT
.Lx727_2:
 test rax, rax
 je .Lx727_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx727_7]
 lea rdx, [rip + .Lx727_8]
 jmp rax
.Lx727_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx727_2
.Lx727_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx727_2
.Lx727_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain721_n1_af
 mov r14d, eax
 lea rax, [rip + .Lx727_6]
 sub rsp, 8
 push rax
 jmp xchain721_n4_α
.Lx727_6:
 add rsp, 16
 jmp xchain721_n1_af
 xchain721_n3_β:
 jmp qword ptr [rsp]
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain721_n4_α:
 mov rax, qword ptr [1879052512]
 mov rdx, qword ptr [1879052520]
 cmp eax, 3
 jne .Lx728_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx728_10
.Lx728_9:
 xor eax, eax
.Lx728_10:
 test rax, rax
 jz .Lx728_0
 mov r8d, 0
 lea rcx, [rip + .Lx728_4]
 lea rdx, [rip + .Lx728_5]
 jmp rax
.Lx728_4:
 jmp xchain721_n5_α
.Lx728_5:
 jmp xchain721_n3_β
.Lx728_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S10]
 xor esi, esi
 call rt_defer_open@PLT
.Lx728_2:
 test rax, rax
 je .Lx728_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx728_7]
 lea rdx, [rip + .Lx728_8]
 jmp rax
.Lx728_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx728_2
.Lx728_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx728_2
.Lx728_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain721_n3_β
 mov r14d, eax
 lea rax, [rip + .Lx728_6]
 sub rsp, 8
 push rax
 jmp xchain721_n5_α
.Lx728_6:
 add rsp, 16
 jmp xchain721_n3_β
 xchain721_n4_β:
 jmp qword ptr [rsp]
# IR_MATCH_CAPTURE_SAVE push
 xchain721_n5_α:
 lea rdi, [rbp + 176]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain721_n7_α
 xchain721_n5_β:
 lea rdi, [rbp + 176]
 call rt_cap_pop@PLT
 jmp xchain721_n4_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain721_n6_α:
 lea rdi, [rbp + 176]
 call rt_cap_top@PLT
 lea rcx, [rip + .S40]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain721_n1_as
 xchain721_n6_β:
 sub r12, 24
 jmp xchain721_n7_β
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain721_n7_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx733_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx733_10
.Lx733_9:
 xor eax, eax
.Lx733_10:
 test rax, rax
 jz .Lx733_0
 mov r8d, 0
 lea rcx, [rip + .Lx733_4]
 lea rdx, [rip + .Lx733_5]
 jmp rax
.Lx733_4:
 jmp xchain721_n6_α
.Lx733_5:
 jmp xchain721_n5_β
.Lx733_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx733_2:
 test rax, rax
 je .Lx733_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx733_7]
 lea rdx, [rip + .Lx733_8]
 jmp rax
.Lx733_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx733_2
.Lx733_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx733_2
.Lx733_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain721_n5_β
 mov r14d, eax
 lea rax, [rip + .Lx733_6]
 sub rsp, 8
 push rax
 jmp xchain721_n6_α
.Lx733_6:
 add rsp, 16
 jmp xchain721_n5_β
 xchain721_n7_β:
 jmp qword ptr [rsp]
proc_PAT$49_scanhit:
cmp qword ptr [rbp + 288], 1
jne 7f
mov ecx, dword ptr [rbp + 280]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$49_γ
proc_PAT$49_scanfail:
cmp qword ptr [rbp + 288], 1
jne 8f
mov eax, dword ptr [rbp + 280]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 280], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$49_attempt
8:
jmp proc_PAT$49_ω
proc_PAT$49_res:
add rsp, 8
pop rbp
proc_PAT$49_β:
jmp qword ptr [rbp + 256]
proc_PAT$49_γ:
push rbp
lea rax, [rip + proc_PAT$49_res]
push rax
mov rax, [rbp + 296]
mov rbp, [rbp + 312]
jmp rax
proc_PAT$49_ω:
mov rax, [rbp + 304]
lea rsp, [rbp + 320]
mov rbp, [rbp + 312]
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
  mov esi, 3920
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
  mov esi, 3920
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
  mov esi, 3920
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
  mov esi, 3920
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname3]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname4: .string "EXPR$0"
  .align 8
  .Lstartup_pnames4:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + .Lstartup_pnames4]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname4]
  lea rsi, [rip + proc_EXPR$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname4]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname5: .string "EXPR$1"
  .align 8
  .Lstartup_pnames5:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + .Lstartup_pnames5]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname5]
  lea rsi, [rip + proc_EXPR$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname5]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname6: .string "EXPR$2"
  .align 8
  .Lstartup_pnames6:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + .Lstartup_pnames6]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname6]
  lea rsi, [rip + proc_EXPR$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname6]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname7: .string "EXPR$3"
  .align 8
  .Lstartup_pnames7:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + .Lstartup_pnames7]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname7]
  lea rsi, [rip + proc_EXPR$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname7]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname8: .string "EXPR$4"
  .align 8
  .Lstartup_pnames8:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + .Lstartup_pnames8]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname8]
  lea rsi, [rip + proc_EXPR$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname8]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname9: .string "EXPR$5"
  .align 8
  .Lstartup_pnames9:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + .Lstartup_pnames9]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname9]
  lea rsi, [rip + proc_EXPR$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname9]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname10: .string "EXPR$6"
  .align 8
  .Lstartup_pnames10:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + .Lstartup_pnames10]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname10]
  lea rsi, [rip + proc_EXPR$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname10]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname11: .string "EXPR$7"
  .align 8
  .Lstartup_pnames11:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + .Lstartup_pnames11]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname11]
  lea rsi, [rip + proc_EXPR$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname11]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname12: .string "EXPR$8"
  .align 8
  .Lstartup_pnames12:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + .Lstartup_pnames12]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname12]
  lea rsi, [rip + proc_EXPR$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname12]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname13: .string "EXPR$9"
  .align 8
  .Lstartup_pnames13:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + .Lstartup_pnames13]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname13]
  lea rsi, [rip + proc_EXPR$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname13]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname14: .string "EXPR$10"
  .align 8
  .Lstartup_pnames14:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname14]
  lea rsi, [rip + .Lstartup_pnames14]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname14]
  lea rsi, [rip + proc_EXPR$10_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname14]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname15: .string "EXPR$11"
  .align 8
  .Lstartup_pnames15:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname15]
  lea rsi, [rip + .Lstartup_pnames15]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname15]
  lea rsi, [rip + proc_EXPR$11_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname15]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname16: .string "EXPR$12"
  .align 8
  .Lstartup_pnames16:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname16]
  lea rsi, [rip + .Lstartup_pnames16]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname16]
  lea rsi, [rip + proc_EXPR$12_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname16]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname17: .string "EXPR$13"
  .align 8
  .Lstartup_pnames17:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname17]
  lea rsi, [rip + .Lstartup_pnames17]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname17]
  lea rsi, [rip + proc_EXPR$13_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname17]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname18: .string "EXPR$14"
  .align 8
  .Lstartup_pnames18:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname18]
  lea rsi, [rip + .Lstartup_pnames18]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname18]
  lea rsi, [rip + proc_EXPR$14_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname18]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname19: .string "EXPR$15"
  .align 8
  .Lstartup_pnames19:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname19]
  lea rsi, [rip + .Lstartup_pnames19]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname19]
  lea rsi, [rip + proc_EXPR$15_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname19]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname20: .string "EXPR$16"
  .align 8
  .Lstartup_pnames20:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname20]
  lea rsi, [rip + .Lstartup_pnames20]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname20]
  lea rsi, [rip + proc_EXPR$16_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname20]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname21: .string "EXPR$17"
  .align 8
  .Lstartup_pnames21:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname21]
  lea rsi, [rip + .Lstartup_pnames21]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname21]
  lea rsi, [rip + proc_EXPR$17_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname21]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname22: .string "EXPR$18"
  .align 8
  .Lstartup_pnames22:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname22]
  lea rsi, [rip + .Lstartup_pnames22]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname22]
  lea rsi, [rip + proc_EXPR$18_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname22]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname23: .string "EXPR$19"
  .align 8
  .Lstartup_pnames23:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname23]
  lea rsi, [rip + .Lstartup_pnames23]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname23]
  lea rsi, [rip + proc_EXPR$19_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname23]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname24: .string "EXPR$20"
  .align 8
  .Lstartup_pnames24:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname24]
  lea rsi, [rip + .Lstartup_pnames24]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname24]
  lea rsi, [rip + proc_EXPR$20_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname24]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname25: .string "EXPR$21"
  .align 8
  .Lstartup_pnames25:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname25]
  lea rsi, [rip + .Lstartup_pnames25]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname25]
  lea rsi, [rip + proc_EXPR$21_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname25]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname26: .string "EXPR$22"
  .align 8
  .Lstartup_pnames26:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname26]
  lea rsi, [rip + .Lstartup_pnames26]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname26]
  lea rsi, [rip + proc_EXPR$22_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname26]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname27: .string "EXPR$23"
  .align 8
  .Lstartup_pnames27:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname27]
  lea rsi, [rip + .Lstartup_pnames27]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname27]
  lea rsi, [rip + proc_EXPR$23_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname27]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname28: .string "EXPR$24"
  .align 8
  .Lstartup_pnames28:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname28]
  lea rsi, [rip + .Lstartup_pnames28]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname28]
  lea rsi, [rip + proc_EXPR$24_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname28]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname29: .string "EXPR$25"
  .align 8
  .Lstartup_pnames29:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname29]
  lea rsi, [rip + .Lstartup_pnames29]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname29]
  lea rsi, [rip + proc_EXPR$25_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname29]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname30: .string "EXPR$26"
  .align 8
  .Lstartup_pnames30:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname30]
  lea rsi, [rip + .Lstartup_pnames30]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname30]
  lea rsi, [rip + proc_EXPR$26_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname30]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname31: .string "EXPR$27"
  .align 8
  .Lstartup_pnames31:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname31]
  lea rsi, [rip + .Lstartup_pnames31]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname31]
  lea rsi, [rip + proc_EXPR$27_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname31]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname32: .string "EXPR$28"
  .align 8
  .Lstartup_pnames32:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname32]
  lea rsi, [rip + .Lstartup_pnames32]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname32]
  lea rsi, [rip + proc_EXPR$28_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname32]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname33: .string "EXPR$29"
  .align 8
  .Lstartup_pnames33:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname33]
  lea rsi, [rip + .Lstartup_pnames33]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname33]
  lea rsi, [rip + proc_EXPR$29_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname33]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname34: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname34]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname34]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname35: .string "PAT$1"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname35]
  lea rsi, [rip + proc_PAT$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname35]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname36: .string "PAT$2"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname36]
  lea rsi, [rip + proc_PAT$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname36]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname37: .string "PAT$3"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname37]
  lea rsi, [rip + proc_PAT$3_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname37]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname38: .string "PAT$4"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname38]
  lea rsi, [rip + proc_PAT$4_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname38]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname39: .string "PAT$5"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname39]
  lea rsi, [rip + proc_PAT$5_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 192
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname39]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname40: .string "PAT$6"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname40]
  lea rsi, [rip + proc_PAT$6_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname40]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname41: .string "PAT$7"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname41]
  lea rsi, [rip + proc_PAT$7_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 256
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname41]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname42: .string "PAT$8"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname42]
  lea rsi, [rip + proc_PAT$8_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname42]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname43: .string "PAT$9"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname43]
  lea rsi, [rip + proc_PAT$9_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname43]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname44: .string "PAT$10"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname44]
  lea rsi, [rip + proc_PAT$10_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname44]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname45: .string "PAT$11"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname45]
  lea rsi, [rip + proc_PAT$11_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname45]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname46: .string "PAT$12"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname46]
  lea rsi, [rip + proc_PAT$12_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname46]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname47: .string "PAT$13"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname47]
  lea rsi, [rip + proc_PAT$13_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname47]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname48: .string "PAT$14"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname48]
  lea rsi, [rip + proc_PAT$14_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname48]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname49: .string "PAT$15"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname49]
  lea rsi, [rip + proc_PAT$15_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 192
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname49]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname50: .string "PAT$16"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname50]
  lea rsi, [rip + proc_PAT$16_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname50]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname51: .string "PAT$17"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname51]
  lea rsi, [rip + proc_PAT$17_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 256
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname51]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname52: .string "PAT$18"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname52]
  lea rsi, [rip + proc_PAT$18_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname52]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname53: .string "PAT$19"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname53]
  lea rsi, [rip + proc_PAT$19_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname53]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname54: .string "PAT$20"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname54]
  lea rsi, [rip + proc_PAT$20_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname54]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname55: .string "PAT$21"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname55]
  lea rsi, [rip + proc_PAT$21_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname55]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname56: .string "PAT$22"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname56]
  lea rsi, [rip + proc_PAT$22_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname56]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname57: .string "PAT$23"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname57]
  lea rsi, [rip + proc_PAT$23_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname57]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname58: .string "PAT$24"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname58]
  lea rsi, [rip + proc_PAT$24_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname58]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname59: .string "PAT$25"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname59]
  lea rsi, [rip + proc_PAT$25_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 192
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname59]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname60: .string "PAT$26"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname60]
  lea rsi, [rip + proc_PAT$26_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname60]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname61: .string "PAT$27"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname61]
  lea rsi, [rip + proc_PAT$27_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 256
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname61]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname62: .string "PAT$28"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname62]
  lea rsi, [rip + proc_PAT$28_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname62]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname63: .string "PAT$29"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname63]
  lea rsi, [rip + proc_PAT$29_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname63]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname64: .string "PAT$30"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname64]
  lea rsi, [rip + proc_PAT$30_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname64]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname65: .string "PAT$31"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname65]
  lea rsi, [rip + proc_PAT$31_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname65]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname66: .string "PAT$32"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname66]
  lea rsi, [rip + proc_PAT$32_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname66]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname67: .string "PAT$33"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname67]
  lea rsi, [rip + proc_PAT$33_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname67]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname68: .string "PAT$34"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname68]
  lea rsi, [rip + proc_PAT$34_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname68]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname69: .string "PAT$35"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname69]
  lea rsi, [rip + proc_PAT$35_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 192
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname69]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname70: .string "PAT$36"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname70]
  lea rsi, [rip + proc_PAT$36_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname70]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname71: .string "PAT$37"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname71]
  lea rsi, [rip + proc_PAT$37_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 256
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname71]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname72: .string "PAT$38"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname72]
  lea rsi, [rip + proc_PAT$38_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname72]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname73: .string "PAT$39"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname73]
  lea rsi, [rip + proc_PAT$39_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname73]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname74: .string "PAT$40"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname74]
  lea rsi, [rip + proc_PAT$40_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 80
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname74]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname75: .string "PAT$41"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname75]
  lea rsi, [rip + proc_PAT$41_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname75]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname76: .string "PAT$42"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname76]
  lea rsi, [rip + proc_PAT$42_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 448
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname76]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname77: .string "PAT$43"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname77]
  lea rsi, [rip + proc_PAT$43_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname77]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname78: .string "PAT$44"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname78]
  lea rsi, [rip + proc_PAT$44_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 112
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname78]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname79: .string "PAT$45"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname79]
  lea rsi, [rip + proc_PAT$45_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 192
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname79]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname80: .string "PAT$46"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname80]
  lea rsi, [rip + proc_PAT$46_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 208
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname80]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname81: .string "PAT$47"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname81]
  lea rsi, [rip + proc_PAT$47_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 256
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname81]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname82: .string "PAT$48"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname82]
  lea rsi, [rip + proc_PAT$48_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname82]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  .section .rodata
  .Lstartup_pname83: .string "PAT$49"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname83]
  lea rsi, [rip + proc_PAT$49_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname83]
  mov esi, 288
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname83]
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
  .Lgvan22: .string "EXPR$0"
  .Lgvan23: .string "EXPR$1"
  .Lgvan24: .string "EXPR$2"
  .Lgvan25: .string "EXPR$3"
  .Lgvan26: .string "EXPR$4"
  .Lgvan27: .string "EXPR$5"
  .Lgvan28: .string "EXPR$6"
  .Lgvan29: .string "EXPR$7"
  .Lgvan30: .string "EXPR$8"
  .Lgvan31: .string "EXPR$9"
  .Lgvan32: .string "EXPR$10"
  .Lgvan33: .string "EXPR$11"
  .Lgvan34: .string "EXPR$12"
  .Lgvan35: .string "EXPR$13"
  .Lgvan36: .string "EXPR$14"
  .Lgvan37: .string "EXPR$15"
  .Lgvan38: .string "EXPR$16"
  .Lgvan39: .string "EXPR$17"
  .Lgvan40: .string "EXPR$18"
  .Lgvan41: .string "EXPR$19"
  .Lgvan42: .string "EXPR$20"
  .Lgvan43: .string "EXPR$21"
  .Lgvan44: .string "EXPR$22"
  .Lgvan45: .string "EXPR$23"
  .Lgvan46: .string "EXPR$24"
  .Lgvan47: .string "EXPR$25"
  .Lgvan48: .string "EXPR$26"
  .Lgvan49: .string "EXPR$27"
  .Lgvan50: .string "EXPR$28"
  .Lgvan51: .string "EXPR$29"
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
  .quad .Lgvan22
  .quad .Lgvan23
  .quad .Lgvan24
  .quad .Lgvan25
  .quad .Lgvan26
  .quad .Lgvan27
  .quad .Lgvan28
  .quad .Lgvan29
  .quad .Lgvan30
  .quad .Lgvan31
  .quad .Lgvan32
  .quad .Lgvan33
  .quad .Lgvan34
  .quad .Lgvan35
  .quad .Lgvan36
  .quad .Lgvan37
  .quad .Lgvan38
  .quad .Lgvan39
  .quad .Lgvan40
  .quad .Lgvan41
  .quad .Lgvan42
  .quad .Lgvan43
  .quad .Lgvan44
  .quad .Lgvan45
  .quad .Lgvan46
  .quad .Lgvan47
  .quad .Lgvan48
  .quad .Lgvan49
  .quad .Lgvan50
  .quad .Lgvan51
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 52
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 52
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
  mov qword ptr [rsp + 3912], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
 xchain734_n0_α:
# BOX IR_CALL TABLE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn736: .string "TABLE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn736]
 lea rsi, [rbp + 128]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain734_n2_α
 jmp xchain734_n1_α
 xchain734_n0_β:
 jmp xchain734_n2_α
# IR_ASSIGN gva
 xchain734_n1_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052432], rax
 mov qword ptr [1879052440], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain734_n2_α
# IR_LIT_STRING
 xchain734_n2_α:
 mov qword ptr [rbp + 2480], 1
 mov rax, qword ptr [rip + .Lx738_0]
 mov qword ptr [rbp + 2488], rax
 jmp xchain734_n3_α
.Lx738_0:
 .quad .Lx738_0_s
.Lx738_0_s:
 .string "PAT$0"
 xchain734_n3_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2480] -> [zr+2448]
 mov rax, qword ptr [rbp + 2480]
 mov qword ptr [rbp + 2448], rax
 mov rax, qword ptr [rbp + 2488]
 mov qword ptr [rbp + 2456], rax
  .section .rodata
  .Lrkfn740: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn740]
 lea rsi, [rbp + 2448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2432], rax
 mov qword ptr [rbp + 2440], rdx
 cmp eax, 99
 je xchain734_n5_α
 jmp xchain734_n4_α
 xchain734_n3_β:
 jmp xchain734_n5_α
# IR_ASSIGN gva
 xchain734_n4_α:
 mov rax, qword ptr [rbp + 2432]
 mov rdx, qword ptr [rbp + 2440]
 mov qword ptr [1879052464], rax
 mov qword ptr [1879052472], rdx
 mov qword ptr [rbp + 2416], rax
 mov qword ptr [rbp + 2424], rdx
 jmp xchain734_n5_α
# IR_LIT_STRING
 xchain734_n5_α:
 mov qword ptr [rbp + 2576], 1
 mov rax, qword ptr [rip + .Lx742_0]
 mov qword ptr [rbp + 2584], rax
 jmp xchain734_n6_α
.Lx742_0:
 .quad .Lx742_0_s
.Lx742_0_s:
 .string "PAT$1"
 xchain734_n6_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2576] -> [zr+2544]
 mov rax, qword ptr [rbp + 2576]
 mov qword ptr [rbp + 2544], rax
 mov rax, qword ptr [rbp + 2584]
 mov qword ptr [rbp + 2552], rax
  .section .rodata
  .Lrkfn744: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn744]
 lea rsi, [rbp + 2544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2528], rax
 mov qword ptr [rbp + 2536], rdx
 cmp eax, 99
 je xchain734_n8_α
 jmp xchain734_n7_α
 xchain734_n6_β:
 jmp xchain734_n8_α
# IR_ASSIGN gva
 xchain734_n7_α:
 mov rax, qword ptr [rbp + 2528]
 mov rdx, qword ptr [rbp + 2536]
 mov qword ptr [1879052480], rax
 mov qword ptr [1879052488], rdx
 mov qword ptr [rbp + 2512], rax
 mov qword ptr [rbp + 2520], rdx
 jmp xchain734_n8_α
# IR_LIT_STRING
 xchain734_n8_α:
 mov qword ptr [rbp + 2672], 1
 mov rax, qword ptr [rip + .Lx746_0]
 mov qword ptr [rbp + 2680], rax
 jmp xchain734_n9_α
.Lx746_0:
 .quad .Lx746_0_s
.Lx746_0_s:
 .string "PAT$2"
 xchain734_n9_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2672] -> [zr+2640]
 mov rax, qword ptr [rbp + 2672]
 mov qword ptr [rbp + 2640], rax
 mov rax, qword ptr [rbp + 2680]
 mov qword ptr [rbp + 2648], rax
  .section .rodata
  .Lrkfn748: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn748]
 lea rsi, [rbp + 2640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2624], rax
 mov qword ptr [rbp + 2632], rdx
 cmp eax, 99
 je xchain734_n11_α
 jmp xchain734_n10_α
 xchain734_n9_β:
 jmp xchain734_n11_α
# IR_ASSIGN gva
 xchain734_n10_α:
 mov rax, qword ptr [rbp + 2624]
 mov rdx, qword ptr [rbp + 2632]
 mov qword ptr [1879052496], rax
 mov qword ptr [1879052504], rdx
 mov qword ptr [rbp + 2608], rax
 mov qword ptr [rbp + 2616], rdx
 jmp xchain734_n11_α
# IR_LIT_STRING
 xchain734_n11_α:
 mov qword ptr [rbp + 2768], 1
 mov rax, qword ptr [rip + .Lx750_0]
 mov qword ptr [rbp + 2776], rax
 jmp xchain734_n12_α
.Lx750_0:
 .quad .Lx750_0_s
.Lx750_0_s:
 .string "PAT$3"
 xchain734_n12_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2768] -> [zr+2736]
 mov rax, qword ptr [rbp + 2768]
 mov qword ptr [rbp + 2736], rax
 mov rax, qword ptr [rbp + 2776]
 mov qword ptr [rbp + 2744], rax
  .section .rodata
  .Lrkfn752: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn752]
 lea rsi, [rbp + 2736]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2720], rax
 mov qword ptr [rbp + 2728], rdx
 cmp eax, 99
 je xchain734_n14_α
 jmp xchain734_n13_α
 xchain734_n12_β:
 jmp xchain734_n14_α
# IR_ASSIGN gva
 xchain734_n13_α:
 mov rax, qword ptr [rbp + 2720]
 mov rdx, qword ptr [rbp + 2728]
 mov qword ptr [1879052512], rax
 mov qword ptr [1879052520], rdx
 mov qword ptr [rbp + 2704], rax
 mov qword ptr [rbp + 2712], rdx
 jmp xchain734_n14_α
# IR_LIT_STRING
 xchain734_n14_α:
 mov qword ptr [rbp + 2864], 1
 mov rax, qword ptr [rip + .Lx754_0]
 mov qword ptr [rbp + 2872], rax
 jmp xchain734_n15_α
.Lx754_0:
 .quad .Lx754_0_s
.Lx754_0_s:
 .string "PAT$4"
 xchain734_n15_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2864] -> [zr+2832]
 mov rax, qword ptr [rbp + 2864]
 mov qword ptr [rbp + 2832], rax
 mov rax, qword ptr [rbp + 2872]
 mov qword ptr [rbp + 2840], rax
  .section .rodata
  .Lrkfn756: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn756]
 lea rsi, [rbp + 2832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2816], rax
 mov qword ptr [rbp + 2824], rdx
 cmp eax, 99
 je xchain734_n17_α
 jmp xchain734_n16_α
 xchain734_n15_β:
 jmp xchain734_n17_α
# IR_ASSIGN gva
 xchain734_n16_α:
 mov rax, qword ptr [rbp + 2816]
 mov rdx, qword ptr [rbp + 2824]
 mov qword ptr [1879052528], rax
 mov qword ptr [1879052536], rdx
 mov qword ptr [rbp + 2800], rax
 mov qword ptr [rbp + 2808], rdx
 jmp xchain734_n17_α
# IR_LIT_STRING
 xchain734_n17_α:
 mov qword ptr [rbp + 2960], 1
 mov rax, qword ptr [rip + .Lx758_0]
 mov qword ptr [rbp + 2968], rax
 jmp xchain734_n18_α
.Lx758_0:
 .quad .Lx758_0_s
.Lx758_0_s:
 .string "PAT$5"
 xchain734_n18_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2960] -> [zr+2928]
 mov rax, qword ptr [rbp + 2960]
 mov qword ptr [rbp + 2928], rax
 mov rax, qword ptr [rbp + 2968]
 mov qword ptr [rbp + 2936], rax
  .section .rodata
  .Lrkfn760: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn760]
 lea rsi, [rbp + 2928]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2912], rax
 mov qword ptr [rbp + 2920], rdx
 cmp eax, 99
 je xchain734_n20_α
 jmp xchain734_n19_α
 xchain734_n18_β:
 jmp xchain734_n20_α
# IR_ASSIGN gva
 xchain734_n19_α:
 mov rax, qword ptr [rbp + 2912]
 mov rdx, qword ptr [rbp + 2920]
 mov qword ptr [1879052544], rax
 mov qword ptr [1879052552], rdx
 mov qword ptr [rbp + 2896], rax
 mov qword ptr [rbp + 2904], rdx
 jmp xchain734_n20_α
# IR_LIT_STRING
 xchain734_n20_α:
 mov qword ptr [rbp + 3056], 1
 mov rax, qword ptr [rip + .Lx762_0]
 mov qword ptr [rbp + 3064], rax
 jmp xchain734_n21_α
.Lx762_0:
 .quad .Lx762_0_s
.Lx762_0_s:
 .string "PAT$6"
 xchain734_n21_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3056] -> [zr+3024]
 mov rax, qword ptr [rbp + 3056]
 mov qword ptr [rbp + 3024], rax
 mov rax, qword ptr [rbp + 3064]
 mov qword ptr [rbp + 3032], rax
  .section .rodata
  .Lrkfn764: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn764]
 lea rsi, [rbp + 3024]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3008], rax
 mov qword ptr [rbp + 3016], rdx
 cmp eax, 99
 je xchain734_n23_α
 jmp xchain734_n22_α
 xchain734_n21_β:
 jmp xchain734_n23_α
# IR_ASSIGN gva
 xchain734_n22_α:
 mov rax, qword ptr [rbp + 3008]
 mov rdx, qword ptr [rbp + 3016]
 mov qword ptr [1879052560], rax
 mov qword ptr [1879052568], rdx
 mov qword ptr [rbp + 2992], rax
 mov qword ptr [rbp + 3000], rdx
 jmp xchain734_n23_α
# IR_LIT_STRING
 xchain734_n23_α:
 mov qword ptr [rbp + 3152], 1
 mov rax, qword ptr [rip + .Lx766_0]
 mov qword ptr [rbp + 3160], rax
 jmp xchain734_n24_α
.Lx766_0:
 .quad .Lx766_0_s
.Lx766_0_s:
 .string "PAT$7"
 xchain734_n24_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3152] -> [zr+3120]
 mov rax, qword ptr [rbp + 3152]
 mov qword ptr [rbp + 3120], rax
 mov rax, qword ptr [rbp + 3160]
 mov qword ptr [rbp + 3128], rax
  .section .rodata
  .Lrkfn768: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn768]
 lea rsi, [rbp + 3120]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3104], rax
 mov qword ptr [rbp + 3112], rdx
 cmp eax, 99
 je xchain734_n26_α
 jmp xchain734_n25_α
 xchain734_n24_β:
 jmp xchain734_n26_α
# IR_ASSIGN gva
 xchain734_n25_α:
 mov rax, qword ptr [rbp + 3104]
 mov rdx, qword ptr [rbp + 3112]
 mov qword ptr [1879052576], rax
 mov qword ptr [1879052584], rdx
 mov qword ptr [rbp + 3088], rax
 mov qword ptr [rbp + 3096], rdx
 jmp xchain734_n26_α
# IR_LIT_STRING
 xchain734_n26_α:
 mov qword ptr [rbp + 3248], 1
 mov rax, qword ptr [rip + .Lx770_0]
 mov qword ptr [rbp + 3256], rax
 jmp xchain734_n27_α
.Lx770_0:
 .quad .Lx770_0_s
.Lx770_0_s:
 .string "PAT$8"
 xchain734_n27_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3248] -> [zr+3216]
 mov rax, qword ptr [rbp + 3248]
 mov qword ptr [rbp + 3216], rax
 mov rax, qword ptr [rbp + 3256]
 mov qword ptr [rbp + 3224], rax
  .section .rodata
  .Lrkfn772: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn772]
 lea rsi, [rbp + 3216]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3200], rax
 mov qword ptr [rbp + 3208], rdx
 cmp eax, 99
 je xchain734_n29_α
 jmp xchain734_n28_α
 xchain734_n27_β:
 jmp xchain734_n29_α
# IR_ASSIGN gva
 xchain734_n28_α:
 mov rax, qword ptr [rbp + 3200]
 mov rdx, qword ptr [rbp + 3208]
 mov qword ptr [1879052592], rax
 mov qword ptr [1879052600], rdx
 mov qword ptr [rbp + 3184], rax
 mov qword ptr [rbp + 3192], rdx
 jmp xchain734_n29_α
# IR_LIT_STRING
 xchain734_n29_α:
 mov qword ptr [rbp + 3344], 1
 mov rax, qword ptr [rip + .Lx774_0]
 mov qword ptr [rbp + 3352], rax
 jmp xchain734_n30_α
.Lx774_0:
 .quad .Lx774_0_s
.Lx774_0_s:
 .string "PAT$9"
 xchain734_n30_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3344] -> [zr+3312]
 mov rax, qword ptr [rbp + 3344]
 mov qword ptr [rbp + 3312], rax
 mov rax, qword ptr [rbp + 3352]
 mov qword ptr [rbp + 3320], rax
  .section .rodata
  .Lrkfn776: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn776]
 lea rsi, [rbp + 3312]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3296], rax
 mov qword ptr [rbp + 3304], rdx
 cmp eax, 99
 je xchain734_n32_α
 jmp xchain734_n31_α
 xchain734_n30_β:
 jmp xchain734_n32_α
# IR_ASSIGN gva
 xchain734_n31_α:
 mov rax, qword ptr [rbp + 3296]
 mov rdx, qword ptr [rbp + 3304]
 mov qword ptr [1879052608], rax
 mov qword ptr [1879052616], rdx
 mov qword ptr [rbp + 3280], rax
 mov qword ptr [rbp + 3288], rdx
 jmp xchain734_n32_α
# IR_LIT_STRING
 xchain734_n32_α:
 mov qword ptr [rbp + 3440], 1
 mov rax, qword ptr [rip + .Lx778_0]
 mov qword ptr [rbp + 3448], rax
 jmp xchain734_n33_α
.Lx778_0:
 .quad .Lx778_0_s
.Lx778_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain734_n33_α:
 mov qword ptr [rbp + 3472], 6
 mov rax, qword ptr [rip + .Lx779_0]
 mov qword ptr [rbp + 3480], rax
 jmp xchain734_n34_α
.Lx779_0:
 .quad 1
 xchain734_n34_α:
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
  .Lrkfn781: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn781]
 lea rsi, [rbp + 3392]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 3376], rax
 mov qword ptr [rbp + 3384], rdx
 cmp eax, 99
 je xchain734_n35_α
 jmp xchain734_n35_α
 xchain734_n34_β:
 jmp xchain734_n35_α
# IR_VAR
 xchain734_n35_α:
 mov rdi, qword ptr [rip + .Lx782_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 3520], rax
 mov qword ptr [rbp + 3528], rdx
 jmp xchain734_n36_α
.Lx782_0:
 .quad .Lx782_0_s
.Lx782_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain734_n36_α:
 mov rax, qword ptr [rbp + 3520]
 mov rdx, qword ptr [rbp + 3528]
 mov qword ptr [1879052624], rax
 mov qword ptr [1879052632], rdx
 mov qword ptr [rbp + 3504], rax
 mov qword ptr [rbp + 3512], rdx
 jmp xchain734_n37_α
# IR_VAR
 xchain734_n37_α:
 mov rax, qword ptr [1879052624]
 mov rdx, qword ptr [1879052632]
 mov qword ptr [rbp + 3776], rax
 mov qword ptr [rbp + 3784], rdx
 jmp xchain734_n38_α
# IR_MATCH_HEAD
 xchain734_n38_α:
 mov qword ptr [rbp + 3608], rbp
 mov rdi, qword ptr [rbp + 3776]
 mov rsi, qword ptr [rbp + 3784]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 3600], r12
 mov qword ptr [rbp + 3584], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 3576], rax
 mov dword ptr [rbp + 3568], 0
.Lx786_0:
 mov r14d, dword ptr [rbp + 3568]
 jmp xchain734_n40_α
 xchain734_n38_β:
 add dword ptr [rbp + 3568], 1
 mov eax, dword ptr [rbp + 3568]
 cmp eax, r15d
 jg .Lx786_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx786_1
 jmp .Lx786_0
.Lx786_1:
 mov rax, qword ptr [rbp + 3576]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 3584]
 mov r12, qword ptr [rbp + 3600]
 mov rbp, qword ptr [rbp + 3608]
 jmp xchain734_n39_α
# IR_LIT_STRING
 xchain734_n39_α:
 mov qword ptr [rbp + 3872], 1
 mov rax, qword ptr [rip + .Lx787_0]
 mov qword ptr [rbp + 3880], rax
 jmp xchain734_n41_α
.Lx787_0:
 .quad .Lx787_0_s
.Lx787_0_s:
 .string "Bad input, try again"
# IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)
 xchain734_n40_α:
 mov dword ptr [rbp + 3632], r14d
 jmp xchain734_n43_α
xchain734_n40_as:
 jmp xchain734_n42_α
 xchain734_n40_β:
 jmp xchain734_n47_β
xchain734_n40_af:
 jmp xchain734_n38_β
# IR_ASSIGN global
 xchain734_n41_α:
 mov rsi, qword ptr [rbp + 3872]
 mov rdx, qword ptr [rbp + 3880]
 mov rdi, qword ptr [rip + .Lx790_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 3856], rax
 mov qword ptr [rbp + 3864], rdx
 jmp xchain734_n35_α
.Lx790_0:
 .quad .Lx790_0_s
.Lx790_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain734_n42_α:
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
.Lx792_1:
 test rax, rax
 je .Lx792_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx792_3]
 lea rdx, [rip + .Lx792_4]
 jmp rax
.Lx792_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx792_1
.Lx792_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx792_1
.Lx792_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 3600]
 mov rbp, qword ptr [rbp + 3608]
 jmp xchain734_n48_α
# IR_LIT_INTEGER
 xchain734_n43_α:
 mov qword ptr [rbp + 3664], 6
 mov rax, qword ptr [rip + .Lx793_0]
 mov qword ptr [rbp + 3672], rax
 jmp xchain734_n44_α
 xchain734_n43_β:
 jmp xchain734_n40_af
.Lx793_0:
 .quad 0
# IR_MATCH_POS
 xchain734_n44_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain734_n40_af
 jmp xchain734_n45_α
 xchain734_n44_β:
 jmp xchain734_n40_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain734_n45_α:
 mov rax, qword ptr [1879052608]
 mov rdx, qword ptr [1879052616]
 cmp eax, 3
 jne .Lx795_9
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx795_10
.Lx795_9:
 xor eax, eax
.Lx795_10:
 test rax, rax
 jz .Lx795_0
 mov r8d, 0
 lea rcx, [rip + .Lx795_4]
 lea rdx, [rip + .Lx795_5]
 jmp rax
.Lx795_4:
 jmp xchain734_n46_α
.Lx795_5:
 jmp xchain734_n40_af
.Lx795_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S8]
 xor esi, esi
 call rt_defer_open@PLT
.Lx795_2:
 test rax, rax
 je .Lx795_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx795_7]
 lea rdx, [rip + .Lx795_8]
 jmp rax
.Lx795_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx795_2
.Lx795_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx795_2
.Lx795_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain734_n40_af
 mov r14d, eax
 lea rax, [rip + .Lx795_6]
 sub rsp, 8
 push rax
 jmp xchain734_n46_α
.Lx795_6:
 add rsp, 16
 jmp xchain734_n40_af
 xchain734_n45_β:
 jmp qword ptr [rsp]
# IR_LIT_INTEGER
 xchain734_n46_α:
 mov qword ptr [rbp + 3744], 6
 mov rax, qword ptr [rip + .Lx796_0]
 mov qword ptr [rbp + 3752], rax
 jmp xchain734_n47_α
 xchain734_n46_β:
 jmp xchain734_n45_β
.Lx796_0:
 .quad 0
# IR_MATCH_RPOS
 xchain734_n47_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain734_n45_β
 jmp xchain734_n40_as
 xchain734_n47_β:
 jmp xchain734_n45_β
 xchain734_n48_α:
 sub rsp, 16
 mov rax, qword ptr [1879052320]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052328]
 mov qword ptr [rsp + 8], rax
 mov rdi, qword ptr [rip + .Lx799_0]
 mov esi, 0
 mov edx, 0
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx799_5
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx799_6]
 lea rdx, [rip + .Lx799_7]
 jmp rax
.Lx799_6:
 mov rdi, qword ptr [1879052320]
 mov rsi, qword ptr [1879052328]
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx799_2
.Lx799_7:
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052320], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052328], rax
 add rsp, 16
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx799_2
.Lx799_5:
 add rsp, 16
 mov rdi, qword ptr [rip + .Lx799_0]
 mov esi, 0
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx799_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx799_3]
 lea rdx, [rip + .Lx799_4]
 jmp rax
.Lx799_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx799_2
.Lx799_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx799_2
.Lx799_1:
 call rt_faildescr@PLT
.Lx799_2:
 mov qword ptr [rbp + 3824], rax
 mov qword ptr [rbp + 3832], rdx
 cmp eax, 99
 je xchain734_n35_α
 jmp xchain734_n49_α
 xchain734_n48_β:
 jmp xchain734_n35_α
.Lx799_0:
 .quad .Lx799_0_s
.Lx799_0_s:
 .string "Pop"
# IR_ASSIGN global
 xchain734_n49_α:
 mov rsi, qword ptr [rbp + 3824]
 mov rdx, qword ptr [rbp + 3832]
 mov rdi, qword ptr [rip + .Lx800_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 3808], rax
 mov qword ptr [rbp + 3816], rdx
 jmp xchain734_n35_α
.Lx800_0:
 .quad .Lx800_0_s
.Lx800_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 3912]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 3912]
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
.S3: .string "*EXPR$30"
.S4: .string "*EXPR$31"
.S5: .string "*EXPR$32"
.S6: .string "real"
.S7: .string "constant"
.S8: .string "expr"
.S9: .string "primary"
.S10: .string "addop"
.S11: .string "*EXPR$33"
.S12: .string "factor"
.S13: .string "mulop"
.S14: .string "*EXPR$34"
.S15: .string "term"
.S16: .string "*EXPR$35"
.S17: .string "*EXPR$36"
.S18: .string "*EXPR$37"
.S19: .string "*EXPR$38"
.S20: .string "*EXPR$39"
.S21: .string "*EXPR$40"
.S22: .string "*EXPR$41"
.S23: .string "*EXPR$42"
.S24: .string "*EXPR$43"
.S25: .string "*EXPR$44"
.S26: .string "*EXPR$45"
.S27: .string "*EXPR$46"
.S28: .string "*EXPR$47"
.S29: .string "*EXPR$48"
.S30: .string "*EXPR$49"
.S31: .string "*EXPR$50"
.S32: .string "*EXPR$51"
.S33: .string "*EXPR$52"
.S34: .string "*EXPR$53"
.S35: .string "*EXPR$54"
.S36: .string "*EXPR$55"
.S37: .string "*EXPR$56"
.S38: .string "*EXPR$57"
.S39: .string "*EXPR$58"
.S40: .string "*EXPR$59"
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
.C2:
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0
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
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.C3:
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1
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
 .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.text
