#include <stdio.h>
#include <dirent.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *rt_dir_snapshot(const char *path) {
    DIR *dp = opendir(path);
    if (!dp) return NULL;
    FILE *fp = tmpfile();
    if (!fp) { closedir(dp); return NULL; }
    struct dirent *de;
    while ((de = readdir(dp)) != NULL) fprintf(fp, "%s\n", de->d_name);
    closedir(dp);
    rewind(fp);
    return fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_dir_open(const char *path) { return (void *)opendir(path); }
const char *rt_dir_next(void *dh) { struct dirent *de = readdir((DIR *)dh); return de ? de->d_name : (const char *)0; }
void rt_dir_close(void *dh) { closedir((DIR *)dh); }
