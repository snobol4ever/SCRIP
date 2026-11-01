/*  time.pl -- SWI-Prolog 9.0.4's call_with_time_limit/2,3 over SCRIP's goal interrupt (the cao, 2026-11-01; ARCH-PROLOG-DATABASE section 19).
    SCRIP's own text, AGPL like the rest of SCRIP, in the generator's LOCAL mode: parsed only when a program names an export.

    THE MODEL.  The thread's trail header holds three words: DUE, DEADLINE (CLOCK_MONOTONIC microseconds, 0 = no alarm) and a per-thread
    timer whose SIGALRM handler stores DUE = 1 and nothing else.  '$alarm_push'(T, Prev) answers the old deadline and arms the nearer of
    the two; '$alarm_pop'(Prev) restores it (re-arming, raising DUE at once when it has already passed, or disarming).  Nesting lives in
    this frame's Prev.  The consumers take DUE: the blocking leaves (sleep/1, the character and byte reads, accept, connect, the process
    wait) at EINTR raise the bare atom time_limit_exceeded, as swipl's time.pl does (9.0.4, 135-151); a CPU-bound goal is interrupted only
    by the alpha probe of step 2.  alarm/3,4, remove_alarm/1 and current_alarm/4 are NOT here.  */
:- module(time, [ call_with_time_limit/2, call_with_time_limit/3 ]).

call_with_time_limit(T, G) :- T > 0, !, setup_call_cleanup('$alarm_push'(T, P), once(G), '$alarm_pop'(P)).
call_with_time_limit(_, _) :- throw(time_limit_exceeded).

call_with_time_limit(T, G, _) :- call_with_time_limit(T, G).
