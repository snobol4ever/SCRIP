#include "ir/ct_corpus.h"
#include "ir/ct_arena.h"
#include <sys/stat.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *ct_corpus_root(int k, const char *env) {
    struct stat st;
    if (k == 0) return env && *env ? ct_strdup(env) : (char *)0;
    if (k == 1 && stat("/home/corpus", &st) == 0 && S_ISDIR(st.st_mode)) return ct_strdup("/home/corpus");
    return (char *)0;
}
