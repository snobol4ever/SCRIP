stack         = TABLE();
sp            = 0;
stack_cap     = 0;
last_ok       = 1;
pc            = 0;
si_cap_tmp    = '';
si_args       = TABLE();
si_call_stack = TABLE();
si_csp        = 0;
si_glocals    = TABLE();

function sm_state_init() {
    stack = TABLE();   sp = 0;   stack_cap = 0;   last_ok = 1;   pc = 0;
    si_call_stack = TABLE();   si_csp = 0;
    return;
}

function sm_push(d) {
    stack[sp] = d;   sp = sp + 1;
    return;
}

function sm_pop(d) {
    sp = sp - 1;   d = stack[sp];   sm_pop = d;
    return;
}

function si_push_frame(ret_pc_v, retval_name_v, fr, k) {
    fr = TABLE();
    fr['ret_pc']       = ret_pc_v;
    fr['retval_name']  = retval_name_v;
    fr['caller_sp']    = sp;
    fr['caller_stack'] = TABLE();
    k = 0;   while (LT(k, sp)) { fr['caller_stack'][k] = stack[k];   k = k + 1; }
    si_call_stack[si_csp] = fr;   si_csp = si_csp + 1;
    sp = 0;
    return;
}

function si_pop_frame(is_fret, is_nret, fr, retval, k) {
    si_csp = si_csp - 1;   fr = si_call_stack[si_csp];
    if (DIFFER(fr['retval_name'], '')) { retval = $(fr['retval_name']); }
    else                               { retval = (GT(sp, 0) stack[sp - 1], ''); }
    sp = fr['caller_sp'];
    k = 0;   while (LT(k, sp)) { stack[k] = fr['caller_stack'][k];   k = k + 1; }
    pc = fr['ret_pc'];
    if (IDENT(is_fret, 1)) { sm_push('');     last_ok = 0; }
    else                   { sm_push(retval); last_ok = 1; }
    return;
}

function sm_interp_step(ins, opc, nm, a, b, repl_v, subj_v, pat_v, sname_v, has_repl_v) {
    ins = g_instr_tbl[pc];   pc = pc + 1;   opc = op(ins);
    switch (opc) {

        case 'SM_LABEL':           return;
        case 'SM_HALT':            pc = g_count; return;

        case 'SM_STNO':            sp = 0; return;
        case 'SM_PUSH_LIT_S':      sm_push(a0(ins));          last_ok = 1; return;
        case 'SM_PUSH_LIT_I':      sm_push(+a0(ins));         last_ok = 1; return;
        case 'SM_PUSH_LIT_F':      sm_push(a0(ins) + 0.0);    last_ok = 1; return;
        case 'SM_PUSH_NULL':       sm_push('');                last_ok = 1; return;
        case 'SM_PUSH_VAR':        nm = a0(ins); sm_push($nm); last_ok = 1; return;
        case 'SM_STORE_VAR':       nm = a0(ins); $nm = sm_pop(); return;
        case 'SM_VOID_POP':        sm_pop(); return;

        case 'SM_JUMP':            pc = +a0(ins); return;
        case 'SM_JUMP_S':          if (IDENT(last_ok, 1)) pc = +a0(ins); return;
        case 'SM_JUMP_F':          if (IDENT(last_ok, 0)) pc = +a0(ins); return;

        case 'SM_JUMP_INDIR':      nm = sm_pop(); pc = g_labtab[nm]; return;

        case 'SM_ADD':             b = sm_pop(); a = sm_pop(); sm_push((+a) + (+b));   last_ok = 1; return;
        case 'SM_SUB':             b = sm_pop(); a = sm_pop(); sm_push((+a) - (+b));   last_ok = 1; return;
        case 'SM_MUL':             b = sm_pop(); a = sm_pop(); sm_push((+a) * (+b));   last_ok = 1; return;
        case 'SM_DIV':             b = sm_pop(); a = sm_pop(); sm_push((+a) / (+b));   last_ok = 1; return;
        case 'SM_MOD':             b = sm_pop(); a = sm_pop(); sm_push(REMDR(+a, +b)); last_ok = 1; return;
        case 'SM_NEG':             a = sm_pop(); sm_push(0 - (+a));                    last_ok = 1; return;
        case 'SM_EXP':             b = sm_pop(); a = sm_pop(); sm_push((+a) ^ (+b));   last_ok = 1; return;
        case 'SM_CONCAT':          b = sm_pop(); a = sm_pop(); sm_push(a b);           last_ok = 1; return;
        case 'SM_COERCE_NUM':      a = sm_pop(); sm_push(+a);                          last_ok = 1; return;

        case 'SM_PAT_LIT':         sm_push(a0(ins));           last_ok = 1; return;
        case 'SM_PAT_DEREF':       a = sm_pop(); sm_push(a);   last_ok = 1; return;
        case 'SM_PAT_REFNAME':     nm = a0(ins); sm_push($nm); last_ok = 1; return;
        case 'SM_PUSH_EXPR':       sm_push('');                last_ok = 1; return;
        case 'SM_EXEC_STMT':       goto exec_stmt;

        case 'SM_PAT_ABORT':       sm_push(ABORT);             last_ok = 1; return;
        case 'SM_PAT_ARB':         sm_push(ARB);               last_ok = 1; return;
        case 'SM_PAT_BAL':         sm_push(BAL);               last_ok = 1; return;
        case 'SM_PAT_FAIL':        sm_push(FAIL);              last_ok = 1; return;
        case 'SM_PAT_REM':         sm_push(REM);               last_ok = 1; return;
        case 'SM_PAT_SUCCEED':     sm_push(SUCCEED);           last_ok = 1; return;
        case 'SM_PAT_FENCE0':      sm_push(FENCE);             last_ok = 1; return;
        case 'SM_PAT_FENCE1':      a = sm_pop(); sm_push(FENCE(a));  last_ok = 1; return;
        case 'SM_PAT_CAPTURE':     goto pat_capture;

        case 'SM_PAT_LEN':         a = sm_pop(); sm_push(LEN(+a));    last_ok = 1; return;
        case 'SM_PAT_POS':         a = sm_pop(); sm_push(POS(+a));    last_ok = 1; return;
        case 'SM_PAT_RPOS':        a = sm_pop(); sm_push(RPOS(+a));   last_ok = 1; return;
        case 'SM_PAT_TAB':         a = sm_pop(); sm_push(TAB(+a));    last_ok = 1; return;
        case 'SM_PAT_RTAB':        a = sm_pop(); sm_push(RTAB(+a));   last_ok = 1; return;
        case 'SM_PAT_ANY':         a = sm_pop(); sm_push(ANY(a));      last_ok = 1; return;
        case 'SM_PAT_NOTANY':      a = sm_pop(); sm_push(NOTANY(a));  last_ok = 1; return;
        case 'SM_PAT_SPAN':        a = sm_pop(); sm_push(SPAN(a));    last_ok = 1; return;
        case 'SM_PAT_BREAK':       a = sm_pop(); sm_push(BREAK(a));   last_ok = 1; return;
        case 'SM_PAT_ARBNO':       a = sm_pop(); sm_push(ARBNO(a));   last_ok = 1; return;
        case 'SM_PAT_EPS':         sm_push('');                        last_ok = 1; return;

        case 'SM_PAT_CAT':         b = sm_pop(); a = sm_pop(); sm_push(a b);   last_ok = 1; return;
        case 'SM_PAT_ALT':         b = sm_pop(); a = sm_pop(); sm_push(a | b); last_ok = 1; return;

        case 'SM_ACOMP':           goto acomp;
        case 'SM_LCOMP':           goto lcomp;

        case 'SM_CALL_FN':         goto call_fn;

        case 'SM_PUSH_EXPRESSION': goto push_expr_desc;
        case 'SM_CALL_EXPRESSION': goto call_expr;
        case 'SM_RETURN':          goto si_return;
        case 'SM_FRETURN':         goto si_freturn;
        case 'SM_NRETURN':         goto si_nreturn;

        case 'SM_RETURN_S':        if (IDENT(last_ok, 1)) goto si_return;  return;
        case 'SM_RETURN_F':        if (IDENT(last_ok, 0)) goto si_return;  return;
        case 'SM_FRETURN_S':       if (IDENT(last_ok, 1)) goto si_freturn; return;
        case 'SM_FRETURN_F':       if (IDENT(last_ok, 0)) goto si_freturn; return;
        case 'SM_NRETURN_S':       if (IDENT(last_ok, 1)) goto si_nreturn; return;
        case 'SM_NRETURN_F':       if (IDENT(last_ok, 0)) goto si_nreturn; return;

        case 'SM_PUSH_NULL_NOFLIP': sm_push('');  return;
        case 'SM_DEFINE':           return;
        case 'SM_DEFINE_ENTRY':     return;

        case 'SM_ICMP_GT':          b = sm_pop(); a = sm_pop(); last_ok = (GT(+a, +b) 1, 0); return;
        case 'SM_ICMP_LT':          b = sm_pop(); a = sm_pop(); last_ok = (LT(+a, +b) 1, 0); return;

        case 'SM_INCR':             a = sm_pop(); sm_push((+a) + (+a0(ins))); return;
        case 'SM_DECR':             a = sm_pop(); sm_push((+a) - (+a0(ins))); return;

        case 'SM_PAT_CAPTURE_FN':
        case 'SM_PAT_CAPTURE_FN_ARGS':
        case 'SM_PAT_USERCALL':
        case 'SM_PAT_USERCALL_ARGS':
            TERMINAL = 'sm_interp: ' opc ' (pat callback, Ph3 stub) at pc=' (pc - 1);
            sm_push(''); last_ok = 0; return;



        case 'SM_LOAD_FRAME':       sm_push(''); last_ok = 0; return;
        case 'SM_STORE_FRAME':      a = sm_pop(); sm_push(a); last_ok = 0; return;

        case 'SM_LOAD_GLOCAL':      nm = a0(ins); sm_push(si_glocals[nm]); last_ok = 1; return;
        case 'SM_STORE_GLOCAL':     nm = a0(ins); a = sm_pop(); si_glocals[nm] = a; sm_push(a); last_ok = 1; return;



        case 'SM_SUSPEND':          sm_push(''); last_ok = 0; return;
        case 'SM_SUSPEND_VALUE':    return;
        case 'SM_RESUME':           return;
        case 'SM_GEN_TICK':         sm_push(''); last_ok = 0; return;

        case 'SM_BB_PUMP':          a = sm_pop(); sm_push(0); last_ok = 1; return;
        case 'SM_BB_ONCE':          a = sm_pop(); last_ok = 0; return;
        case 'SM_BB_ONCE_PROC':     last_ok = 0; return;
        case 'SM_BB_PUMP_PROC':     goto bb_pump_proc;
        case 'SM_BB_PUMP_CASE':     goto bb_pump_case;
        case 'SM_BB_PUMP_SM':       a = sm_pop(); sm_push(0); last_ok = 1; return;
        case 'SM_BB_PUMP_EVERY':    sm_push(''); return;

        default:
            TERMINAL = 'sm_interp: unimpl ' opc ' at pc=' (pc - 1);   pc = g_count;   return;
    }

exec_stmt:



    repl_v = sm_pop();   subj_v = sm_pop();   pat_v = sm_pop();
    sname_v = a0(ins);   has_repl_v = +a1(ins);   last_ok = 0;
    if (DIFFER(sname_v, '')) {
        subj_v = $sname_v;
        if (EQ(has_repl_v, 1)) { if (subj_v ? pat_v = repl_v) last_ok = 1; }
        else                   { if (subj_v ? pat_v)           last_ok = 1; }
        $sname_v = subj_v;
    } else {
        if (EQ(has_repl_v, 1)) { if (subj_v ? pat_v = repl_v) last_ok = 1; }
        else                   { if (subj_v ? pat_v)           last_ok = 1; }
    }
    return;

pat_capture:

    nm = a0(ins);   si_cap_tmp = sm_pop();   b = +a1(ins);
    if      (EQ(b, 1)) sm_push(EVAL('si_cap_tmp $ ' nm));
    else if (EQ(b, 2)) sm_push(EVAL('si_cap_tmp @ ' nm));
    else               sm_push(EVAL('si_cap_tmp . ' nm));
    last_ok = 1;   return;

acomp:

    b = sm_pop();   a = sm_pop();   nm = a0(ins);   last_ok = 0;
    switch (nm) {
        case 'TT_EQ': if (EQ(+a, +b)) last_ok = 1; goto acomp_done;
        case 'TT_NE': if (NE(+a, +b)) last_ok = 1; goto acomp_done;
        case 'TT_LT': if (LT(+a, +b)) last_ok = 1; goto acomp_done;
        case 'TT_LE': if (LE(+a, +b)) last_ok = 1; goto acomp_done;
        case 'TT_GT': if (GT(+a, +b)) last_ok = 1; goto acomp_done;
        case 'TT_GE': if (GE(+a, +b)) last_ok = 1; goto acomp_done;
        default:      if (EQ(+a, +b)) last_ok = 1; goto acomp_done;
    }
acomp_done:
    if (IDENT(last_ok, 1)) sm_push(b); else sm_push('');   return;

lcomp:

    b = sm_pop();   a = sm_pop();   nm = a0(ins);   last_ok = 0;
    switch (nm) {
        case 'TT_LEQ': if (LEQ(a, b)) last_ok = 1; goto lcomp_done;
        case 'TT_LNE': if (LNE(a, b)) last_ok = 1; goto lcomp_done;
        case 'TT_LLT': if (LLT(a, b)) last_ok = 1; goto lcomp_done;
        case 'TT_LLE': if (LLE(a, b)) last_ok = 1; goto lcomp_done;
        case 'TT_LGT': if (LGT(a, b)) last_ok = 1; goto lcomp_done;
        case 'TT_LGE': if (LGE(a, b)) last_ok = 1; goto lcomp_done;
        default:       if (LEQ(a, b)) last_ok = 1; goto lcomp_done;
    }
lcomp_done:
    if (IDENT(last_ok, 1)) sm_push(b); else sm_push('');   return;

call_fn:

    nm = a0(ins);   b = +a1(ins);   a = b;
    while (GT(a, 0)) { si_args[a] = sm_pop();   a = a - 1; }
    switch (nm) {
        case 'INDIR_GET':    case 'NAME_PUSH':    case 'ASGN_INDIR':   case 'NRETURN_ASGN':
        case 'IDX_SET':      case 'IDX':          case 'ITEM_SET':      case 'FIELD_SET':
        case 'FIELD_GET':    case 'ASGN':         case 'SWAP':          case 'AUGOP':
        case 'MAKELIST':     case 'RECORD_MAKE':  case 'ICN_SCAN_PUSH': case 'ICN_SCAN_POP':
        case 'ICN_CASE_EQ':  case 'ICN_NULL':     case 'ICN_RANDOM':    case 'NONNULL':
        case 'IDENTICAL':    case 'FAIL':
            TERMINAL = 'sm_interp: SM_CALL_FN ' nm ' (pseudo/Ph3) at pc=' (pc - 1);
            sm_push('');   last_ok = 0;   return;
        default: goto call_fn_apply;
    }
call_fn_apply:

    switch (b) {
        case 0: a = ''; last_ok = 0; if (a = APPLY(nm))                                                             last_ok = 1; sm_push(a); return;
        case 1: a = ''; last_ok = 0; if (a = APPLY(nm, si_args[1]))                                                 last_ok = 1; sm_push(a); return;
        case 2: a = ''; last_ok = 0; if (a = APPLY(nm, si_args[1], si_args[2]))                                     last_ok = 1; sm_push(a); return;
        case 3: a = ''; last_ok = 0; if (a = APPLY(nm, si_args[1], si_args[2], si_args[3]))                         last_ok = 1; sm_push(a); return;
        case 4: a = ''; last_ok = 0; if (a = APPLY(nm, si_args[1], si_args[2], si_args[3], si_args[4]))             last_ok = 1; sm_push(a); return;
        case 5: a = ''; last_ok = 0; if (a = APPLY(nm, si_args[1], si_args[2], si_args[3], si_args[4], si_args[5])) last_ok = 1; sm_push(a); return;
        default:
            TERMINAL = 'sm_interp: SM_CALL_FN ' nm ' nargs=' b ' (>5 unsupported) at pc=' (pc - 1);
            sm_push('');   last_ok = 0;   return;
    }

push_expr_desc:

    a = TABLE();   a['type'] = 'EXPR';   a['entry_pc'] = +a0(ins);
    sm_push(a);   last_ok = 1;   return;

call_expr:


    b = +a0(ins);   si_push_frame(pc, '');   pc = b;   return;

si_return:

    if (EQ(si_csp, 0)) { pc = g_count; return; }
    si_pop_frame(0, 0);   return;

si_freturn:

    if (EQ(si_csp, 0)) { pc = g_count; return; }
    si_pop_frame(1, 0);   return;

si_nreturn:

    if (EQ(si_csp, 0)) { pc = g_count; return; }
    si_pop_frame(0, 1);   return;

bb_pump_proc:

    b = +a1(ins);   a = b;
    while (GT(a, 0)) { si_args[a] = sm_pop();   a = a - 1; }
    sm_push(''); last_ok = 0; return;

bb_pump_case:


    a = +a0(ins);   b = +a1(ins);
    nm = b + a + a + a + 1;
    while (GT(nm, 0)) { sm_pop(); nm = nm - 1; }
    sm_push(''); last_ok = 0; return;
}

function sm_interp_run() {
    sm_state_init();
    while (LT(pc, g_count)) sm_interp_step();
    return;
}
