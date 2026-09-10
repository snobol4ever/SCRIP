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
