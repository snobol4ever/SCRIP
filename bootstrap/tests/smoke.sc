Shift('foo', 'bar');
sno = Pop();
OUTPUT = v(sno);
OUTPUT = (IDENT(digits, '0123456789') 'global-OK', 'global-FAIL');
_smoke_node = tree('A', '');
Append(_smoke_node, tree('Name', 'b'));
OUTPUT = (IDENT(TLump(_smoke_node, 80), '(A b)') 'tdump-OK', 'tdump-FAIL');
_smoke_cap = '';
assign('_smoke_cap', 'hel');
OUTPUT = (IDENT(_smoke_cap, 'hel') 'assign-OK', 'assign-FAIL');
OUTPUT = (match('foo bar baz', 'bar') 'match-OK', 'match-FAIL');
OUTPUT = (notmatch('foo bar baz', 'qux') 'notmatch-OK', 'notmatch-FAIL');
OUTPUT = (IDENT(lwr('AbC'), 'abc') 'lwr-OK', 'lwr-FAIL');
OUTPUT = (IDENT(upr('AbC'), 'ABC') 'upr-OK', 'upr-FAIL');
OUTPUT = (IDENT(cap('aBc'), 'Abc') 'cap-OK', 'cap-FAIL');
_smoke_icp = icase('End');
OUTPUT = (IDENT('eNd' ? _smoke_icp, 'eNd') 'icase-OK', 'icase-FAIL');
OUTPUT = (IDENT(Qize(''),       "''")            'qize-empty-OK', 'qize-empty-FAIL');
OUTPUT = (IDENT(Qize('hello'),  "'hello'")       'qize-plain-OK', 'qize-plain-FAIL');
OUTPUT = (IDENT(SQize('hello'), "'hello'")       'sqize-OK',      'sqize-FAIL');
OUTPUT = (IDENT(DQize('hello'), '"hello"')       'dqize-OK',      'dqize-FAIL');
OUTPUT = (IDENT(SqlSQize("o'clock"), "o''clock") 'sqlsqize-OK',   'sqlsqize-FAIL');
_smoke_qn = tree('R', '');
Append(_smoke_qn, tree('string', "o'clock"));
OUTPUT = (IDENT(TLump(_smoke_qn, 80), "(R 'o''clock')") 'tdump-quote-OK', 'tdump-quote-FAIL');
_smoke_infra7a_str = 'X';
_smoke_infra7a_cap = 'unset';
_smoke_infra7a_r = (_smoke_infra7a_str ? (POS(0) LEN(1) . *assign(._smoke_infra7a_cap, 'fired')));
OUTPUT = (IDENT(_smoke_infra7a_cap, 'fired') 'infra7a-inline-assign-OK', 'infra7a-inline-assign-FAIL');
OUTPUT = (IDENT(Qize('a' tab 'b'), "'a' tab 'b'") 'infra7a-qize-tab-OK', 'infra7a-qize-tab-FAIL');
T8Trace(0, 'unused', 0);
T8Trace(99, '? prefix', 42);
T8Trace(1, 'plain', 0);
OUTPUT = 'trace-silent-OK';
t8Max = 0;
_smoke_omega_p = TZ(0, 'probe', 'hi') @smoke_omega_cur;
_smoke_omega_dummy = ('hi' ? _smoke_omega_p);
OUTPUT = (EQ(smoke_omega_cur, 2) GT(t8Max, 0) 'omega-silent-OK', 'omega-silent-FAIL');
_smoke_o10_a = shift('foo', 'Word');
('foo' ? _smoke_o10_a);
_smoke_o10_t1 = Pop();
_smoke_o10_b = APPLY('~', 'foo', 'Word');
('foo' ? _smoke_o10_b);
_smoke_o10_t2 = Pop();
Shift('Leaf', 'a');
_smoke_o10_c = reduce("'P'", 1);
('' ? _smoke_o10_c);
_smoke_o10_t3 = Pop();
Shift('Leaf', 'b');
_smoke_o10_d = APPLY('&', "'P'", 1);
('' ? _smoke_o10_d);
_smoke_o10_t4 = Pop();
OUTPUT = (IDENT(t(_smoke_o10_t1), 'Word') IDENT(v(_smoke_o10_t1), 'foo')
          IDENT(t(_smoke_o10_t2), 'Word') IDENT(v(_smoke_o10_t2), 'foo')
          IDENT(t(_smoke_o10_t3), 'P') EQ(n(_smoke_o10_t3), 1)
          IDENT(t(_smoke_o10_t4), 'P') EQ(n(_smoke_o10_t4), 1)
          'opsyn-OK', 'opsyn-FAIL');
_fw1_foo = tree('FOO_KIND', 'bar');
_fw1_ic  = tree('IC_VAR', 'x');
_fw1_eq  = tree('TT_QLIT', 'hi');
OUTPUT = (IDENT(TLump(_fw1_foo, 256), '(FOO_KIND bar)')
          IDENT(TLump(_fw1_ic,  256), '(IC_VAR x)')
          IDENT(TLump(_fw1_eq,  256), '(TT_QLIT "hi")')
          'fw1-generic-leaf-OK', 'fw1-generic-leaf-FAIL');
_fw2_c1 = Tree('TT_VAR', 'a');
_fw2_c2 = Tree('TT_VAR', 'b');
_fw2_c3 = Tree('TT_VAR', 'c');
_fw2_x3 = Tree(':args', '', 3, _fw2_c1, _fw2_c2, _fw2_c3);
OUTPUT = (IDENT(TLump(_fw2_x3, 256), ':args ((TT_VAR a) (TT_VAR b) (TT_VAR c))')
          'fw2-multichild-role-OK', 'fw2-multichild-role-FAIL');
