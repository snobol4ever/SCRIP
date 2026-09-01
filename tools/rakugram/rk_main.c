/* rk_main.c -- `rk_parse FILE`: prints MATCH | NOMATCH pos=N | REFUSED rule=NAME pos=N ; exit 0/1/2. */
#include <stdio.h>
#include <stdlib.h>
int rk_parse(const char *src, int len, int *pos_out, const char **refused_out);
int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "usage: rk_parse FILE.raku\n"); return 2; }
    FILE *f = fopen(argv[1], "rb"); if (!f) { perror(argv[1]); return 2; }
    fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
    char *src = malloc((size_t)n + 1); if (fread(src, 1, (size_t)n, f) != (size_t)n) { perror("read"); return 2; }
    src[n] = 0; fclose(f);
    int pos; const char *rule;
    int v = rk_parse(src, (int)n, &pos, &rule);
    if (v == 0) printf("MATCH %s\n", argv[1]);
    else if (v == 1) printf("NOMATCH %s pos=%d\n", argv[1], pos);
    else printf("REFUSED %s rule=%s pos=%d\n", argv[1], rule, pos);
    return v;
}
