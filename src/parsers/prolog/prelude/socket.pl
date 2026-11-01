/*  socket.pl -- SWI-Prolog 9.0.4's library(socket) as a Prolog layer over SCRIP's socket leaves (the cao, 2026-11-01; ARCH-PROLOG-DATABASE
    section 18).  SCRIP's own text, AGPL like the rest of SCRIP, in the generator's LOCAL mode: parsed only when a program names an export.

    THE SHAPE.  A socket is '$sock'(Idx) over a slot of the one stream table whose stream is the socket's own descriptor; it never unifies
    with '$socket'/1 (swipl's handle is a blob, so S = '$socket'(_) fails there too).  tcp_open_socket/2 makes the existing pair
    '$stream_pair'(In, Out) over two duplicates of the descriptor.  Every leaf ('$sk_*') makes one system call and answers its value, ok,
    or '$oserr'(Code, Message) with the errno's lower-case name; this file raises swipl's terms: socket_error(Code, Message) with the
    context UNBOUND (9.0.4 leaves it so), type_error(socket, X) for a non-socket, existence_error(file, Path) for a unix connect to a
    missing path, domain_error(socket_option, O) for an option swipl refuses.  The blocking leaves (accept, connect) take the goal
    interrupt of call_with_time_limit/2 at EINTR (section 19.4 (a)).

    NOT HERE: udp_send/4, udp_receive/4, tcp_select/3, tcp_fcntl/3, negotiate_socks_connection/2, and ip_name/2 (absent in 9.0.4).  */
:- module(socket, [ tcp_socket/1, udp_socket/1, unix_domain_socket/1, tcp_close_socket/1, tcp_open_socket/2, tcp_open_socket/3, tcp_connect/2,
                    tcp_connect/3, tcp_connect/4, tcp_bind/2, tcp_listen/2, tcp_accept/3, tcp_setopt/2, tcp_getopt/2, tcp_host_to_address/2,
                    gethostname/1 ]).

'$sk_raise'('$oserr'(C, M)) :- throw(error(socket_error(C, M), _)).
'$sk_ok'(R) :- ( R == ok -> true ; '$sk_raise'(R) ).
'$sk_val'(R, V) :- ( R = '$oserr'(_, _) -> '$sk_raise'(R) ; V = R ).
'$sk_check'(S) :- ( var(S) -> throw(error(instantiation_error, _)) ; S = '$sock'(_) -> true ; throw(error(type_error(socket, S), _)) ).

'$sk_host'(H, _) :- var(H), !, throw(error(instantiation_error, _)).
'$sk_host'(ip(A, B, C, D), HA) :- !, atomic_list_concat([A, B, C, D], '.', HA).
'$sk_host'(H, H) :- atom(H), !.
'$sk_host'(H, HA) :- string(H), !, atom_string(HA, H).
'$sk_host'(H, _) :- throw(error(type_error(socket_address, H), _)).

'$sk_port'(P, 0) :- var(P), !.
'$sk_port'(P, P) :- integer(P), !.
'$sk_port'(P, _) :- throw(error(type_error(integer, P), _)).

'$sk_addr'(A, inet('', 0), bind) :- var(A), !.
'$sk_addr'(H:P, inet(HA, PN), _) :- !, '$sk_host'(H, HA), '$sk_port'(P, PN).
'$sk_addr'(P, inet('', P), bind) :- integer(P), !.
'$sk_addr'(P, inet(localhost, P), connect) :- integer(P), !.
'$sk_addr'(P, unix(P), _) :- atom(P), !.
'$sk_addr'(P, unix(PA), _) :- string(P), !, atom_string(PA, P).
'$sk_addr'(A, _, _) :- throw(error(type_error(socket_address, A), _)).

tcp_socket(S) :- '$sk_socket'(inet, R), '$sk_val'(R, S).
udp_socket(S) :- '$sk_socket'(udp, R), '$sk_val'(R, S).
unix_domain_socket(S) :- '$sk_socket'(unix, R), '$sk_val'(R, S).

tcp_close_socket(S) :- '$sk_check'(S), '$sk_close'(S, R), '$sk_ok'(R).

tcp_open_socket(S, P) :- '$sk_check'(S), '$sk_open'(S, R), '$sk_val'(R, P).
tcp_open_socket(S, I, O) :- tcp_open_socket(S, P), stream_pair(P, I, O).

tcp_bind(S, A) :-
    '$sk_check'(S), '$sk_addr'(A, X, bind), '$sk_bind'(S, X, R), '$sk_val'(R, V),
    ( var(A) -> A = V ; A = _:P, var(P), integer(V) -> P = V ; true ).

tcp_listen(S, N) :- '$sk_check'(S), '$sk_listen'(S, N, R), '$sk_ok'(R).

tcp_accept(S, Slave, Peer) :- '$sk_check'(S), '$sk_accept'(S, R), '$sk_val'(R, acc(Slave0, Peer0)), Slave = Slave0, Peer = Peer0.

tcp_connect(S, A) :-
    '$sk_check'(S), '$sk_addr'(A, X, connect), '$sk_connect'(S, X, R),
    ( R = '$oserr'(enoent, _), X = unix(P) -> throw(error(existence_error(file, P), _)) ; '$sk_ok'(R) ).

tcp_connect(A, P, _) :-
    var(P), !,
    ( atom(A) -> unix_domain_socket(S) ; string(A) -> unix_domain_socket(S) ; tcp_socket(S) ),
    catch(tcp_connect(S, A), E, ( tcp_close_socket(S), throw(E) )),
    tcp_open_socket(S, P), tcp_close_socket(S).
tcp_connect(S, A, P) :- tcp_connect(S, A), tcp_open_socket(S, P).
tcp_connect(S, A, I, O) :- tcp_connect(S, A), tcp_open_socket(S, I, O).

tcp_setopt(S, O) :-
    '$sk_check'(S),
    (   var(O) -> throw(error(instantiation_error, _))
    ;   '$sk_opt'(O, N, V) -> ( N == ignore -> true ; '$sk_setopt'(S, N, V, R), '$sk_ok'(R) )
    ;   throw(error(domain_error(socket_option, O), _))
    ).
'$sk_opt'(reuseaddr, reuseaddr, 1).
'$sk_opt'(nodelay, nodelay, 1).
'$sk_opt'(nodelay(B), nodelay, V) :- ( B == true -> V = 1 ; B == false -> V = 0 ).
'$sk_opt'(broadcast, broadcast, 1).
'$sk_opt'(keepalive, keepalive, 1).
'$sk_opt'(sndbuf(N), sndbuf, N) :- integer(N).
'$sk_opt'(rcvbuf(N), rcvbuf, N) :- integer(N).
'$sk_opt'(dispatch(B), ignore, 0) :- ( B == true ; B == false ), !.

tcp_getopt(S, O) :-
    '$sk_check'(S),
    ( O = file_no(F) -> '$sk_fileno'(S, F0), F = F0 ; throw(error(domain_error(socket_option, O), _)) ).

gethostname(H) :- '$sk_hostname'(H0), H = H0.

tcp_host_to_address(H, A) :-
    (   nonvar(H) -> '$sk_host'(H, HA), '$sk_resolve'(HA, R), '$sk_val'(R, A0), A = A0
    ;   nonvar(A) -> '$sk_resolve'(A, R), '$sk_val'(R, H0), H = H0
    ;   throw(error(instantiation_error, _))
    ).
