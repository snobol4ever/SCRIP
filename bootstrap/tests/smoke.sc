Shift('foo', 'bar');
sno = Pop();
OUTPUT = v(sno);
OUTPUT = (IDENT(digits, '0123456789') 'global-OK', 'global-FAIL');
smoke_node_ = tree('A', '');
Append(smoke_node_, tree('Name', 'b'));
OUTPUT = (IDENT(TLump(smoke_node_, 80), '(A b)') 'tdump-OK', 'tdump-FAIL');
smoke_cap_ = '';
assign('smoke_cap_', 'hel');
OUTPUT = (IDENT(smoke_cap_, 'hel') 'assign-OK', 'assign-FAIL');
OUTPUT = (match('foo bar baz', 'bar') 'match-OK', 'match-FAIL');
OUTPUT = (notmatch('foo bar baz', 'qux') 'notmatch-OK', 'notmatch-FAIL');
OUTPUT = (IDENT(lwr('AbC'), 'abc') 'lwr-OK', 'lwr-FAIL');
OUTPUT = (IDENT(upr('AbC'), 'ABC') 'upr-OK', 'upr-FAIL');
OUTPUT = (IDENT(cap('aBc'), 'Abc') 'cap-OK', 'cap-FAIL');
smoke_icp_ = icase('End');
OUTPUT = (IDENT('eNd' ? smoke_icp_, 'eNd') 'icase-OK', 'icase-FAIL');
OUTPUT = (IDENT(Qize(''),       "''")            'qize-empty-OK', 'qize-empty-FAIL');
OUTPUT = (IDENT(Qize('hello'),  "'hello'")       'qize-plain-OK', 'qize-plain-FAIL');
OUTPUT = (IDENT(SQize('hello'), "'hello'")       'sqize-OK',      'sqize-FAIL');
OUTPUT = (IDENT(DQize('hello'), '"hello"')       'dqize-OK',      'dqize-FAIL');
OUTPUT = (IDENT(SqlSQize("o'clock"), "o''clock") 'sqlsqize-OK',   'sqlsqize-FAIL');
smoke_qn_ = tree('R', '');
Append(smoke_qn_, tree('string', "o'clock"));
OUTPUT = (IDENT(TLump(smoke_qn_, 80), "(R 'o''clock')") 'tdump-quote-OK', 'tdump-quote-FAIL');
smoke_infra7a_str_ = 'X';
smoke_infra7a_cap_ = 'unset';
smoke_infra7a_r_ = (smoke_infra7a_str_ ? (POS(0) LEN(1) . *assign(.smoke_infra7a_cap_, 'fired')));
OUTPUT = (IDENT(smoke_infra7a_cap_, 'fired') 'infra7a-inline-assign-OK', 'infra7a-inline-assign-FAIL');
OUTPUT = (IDENT(Qize('a' tab 'b'), "'a' tab 'b'") 'infra7a-qize-tab-OK', 'infra7a-qize-tab-FAIL');
T8Trace(0, 'unused', 0);
T8Trace(99, '? prefix', 42);
T8Trace(1, 'plain', 0);
OUTPUT = 'trace-silent-OK';
t8Max = 0;
smoke_omega_p_ = TZ(0, 'probe', 'hi') @smoke_omega_cur;
smoke_omega_dummy_ = ('hi' ? smoke_omega_p_);
OUTPUT = (EQ(smoke_omega_cur, 2) GT(t8Max, 0) 'omega-silent-OK', 'omega-silent-FAIL');
smoke_o10_a_ = shift('foo', 'Word');
('foo' ? smoke_o10_a_);
smoke_o10_t1_ = Pop();
smoke_o10_b_ = APPLY('~', 'foo', 'Word');
('foo' ? smoke_o10_b_);
smoke_o10_t2_ = Pop();
Shift('Leaf', 'a');
smoke_o10_c_ = reduce("'P'", 1);
('' ? smoke_o10_c_);
smoke_o10_t3_ = Pop();
Shift('Leaf', 'b');
smoke_o10_d_ = APPLY('&', "'P'", 1);
('' ? smoke_o10_d_);
smoke_o10_t4_ = Pop();
OUTPUT = (IDENT(t(smoke_o10_t1_), 'Word') IDENT(v(smoke_o10_t1_), 'foo')
          IDENT(t(smoke_o10_t2_), 'Word') IDENT(v(smoke_o10_t2_), 'foo')
          IDENT(t(smoke_o10_t3_), 'P') EQ(n(smoke_o10_t3_), 1)
          IDENT(t(smoke_o10_t4_), 'P') EQ(n(smoke_o10_t4_), 1)
          'opsyn-OK', 'opsyn-FAIL');
fw1_foo_ = tree('FOO_KIND', 'bar');
fw1_ic_  = tree('IC_VAR', 'x');
fw1_eq_  = tree('TT_QLIT', 'hi');
OUTPUT = (IDENT(TLump(fw1_foo_, 256), '(FOO_KIND bar)')
          IDENT(TLump(fw1_ic_,  256), '(IC_VAR x)')
          IDENT(TLump(fw1_eq_,  256), '(TT_QLIT "hi")')
          'fw1-generic-leaf-OK', 'fw1-generic-leaf-FAIL');
fw2_c1_ = Tree('TT_VAR', 'a');
fw2_c2_ = Tree('TT_VAR', 'b');
fw2_c3_ = Tree('TT_VAR', 'c');
fw2_x3_ = Tree(':args', '', 3, fw2_c1_, fw2_c2_, fw2_c3_);
OUTPUT = (IDENT(TLump(fw2_x3_, 256), ':args ((TT_VAR a) (TT_VAR b) (TT_VAR c))')
          'fw2-multichild-role-OK', 'fw2-multichild-role-FAIL');
