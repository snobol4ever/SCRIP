#!/usr/bin/env python3
"""util_dyn_caps_gen.py <dir> -- writes the three cap-exceeding witnesses of ARCH-DYNAMIC-STORAGE.md into <dir>:
defines200.sno (200 DEFINEs; SNO_DEF_MAX is 128), preds600.pl (600 predicates; MAX_PREDS is 512), args70.icn (a 70-argument
call; CALL_ARGS_MAX is 64). Each is a legal program of its industry-standard language whose answer the oracle prints."""
import os
import sys


def main(argv):
    d = argv[1]
    os.makedirs(d, exist_ok=True)
    n = 200
    L = ['        DEFINE("f%d()")' % i for i in range(1, n + 1)]
    L.append("                                                 :(go)")
    L += ["f%d      f%d = %d                                :(RETURN)" % (i, i, i) for i in range(1, n + 1)]
    L.append("go      total = 0")
    L += ["        total = total + f%d()" % i for i in range(1, n + 1)]
    L += ["        OUTPUT = total", "END"]
    open(os.path.join(d, "defines200.sno"), "w", newline="\n").write("\n".join(L) + "\n")
    n = 600
    L = [":- initialization(main)."] + ["p%d(%d)." % (i, i) for i in range(1, n + 1)]
    L.append("main :- " + ", ".join("p%d(X%d), write(X%d), nl" % (i, i, i) for i in range(1, n + 1)) + ", halt.")
    open(os.path.join(d, "preds600.pl"), "w", newline="\n").write("\n".join(L) + "\n")
    n = 70
    args = ", ".join("a%d" % i for i in range(1, n + 1))
    body = "s := 0; " + " ".join("s +:= a%d;" % i for i in range(1, n + 1)) + " return s;"
    src = "procedure f(%s)\n  %s\nend\nprocedure main()\n  write(f(%s));\nend\n" % (args, body, ", ".join(str(i) for i in range(1, n + 1)))
    open(os.path.join(d, "args70.icn"), "w", newline="\n").write(src)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
