/* ==================================================================================================================== */
function lwr(s) {
    lwr = REPLACE(s, &UCASE, &LCASE);
    return;
}
/* ==================================================================================================================== */
function upr(s) {
    upr = REPLACE(s, &LCASE, &UCASE);
    return;
}
/* ==================================================================================================================== */
function cap(s) {
    if (~(cap = REPLACE(SUBSTR(s, 1, 1), &LCASE, &UCASE) REPLACE(SUBSTR(s, 2), &UCASE, &LCASE)))
        error();
    return;
}
/* ==================================================================================================================== */
function icase(str, letter, character) {
    while (~IDENT(str)) {
        if (str ? (POS(0) ANY(&UCASE &LCASE) . letter) = )
            icase = icase (upr(letter) | lwr(letter));
        else {
            str ? (POS(0) LEN(1) . character) = ;
            icase = icase character;
        }
    }
    return;
}
