enum { PLK_NONE = 0, PLK_AX, PLK_CMP, PLK_IS, PLK_TYPE, PLK_ATOP, PLK_ZGUARD, PLK_ANUM, PLK_MKC, PLK_DBDECLS, PLK_UNIFY };
enum { PLR_ANY = 0, PLR_TEXT, PLR_NUM, PLR_INT0, PLR_UNB, PLR_UNB_OR_INT0, PLR_UNB_OR_TEXT, PLR_COMP, PLR_NONVAR, PLR_TEXT_OR_NUM, PLR_INTCODE };
int pl_leaf_kind(const char * fn, int narg, const char ** op);
int pl_leaf_inline_known(const char * fn, int narg);
const int * pl_anum_rules(const char * name, int nargs);
