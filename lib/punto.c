#include <stdio.h>
#include <string.h>
#include "he/err.h"

#define FMT "%e"
#define SIZE 4096

void punto_write(int n, double *queue[], /**/ const char *path) {
    FILE *f;
    int i;
    double **q;
    if ((f = fopen(path, "w")) == NULL)
        ERR("fail to open '%s'", path);
    for (i = 0; i < n; i++) {
        q = queue;
        for (;;) {
            fprintf(f, FMT, (*q)[i]); q++;
            if (*q != NULL) fputc(' ', f);
            else {
                fputc('\n', f);
                break;
            }
        }
    }
    if (fclose(f) != 0)
        ERR("fail to close '%s'", path);
}

enum {OK, FAIL};
static int read(char *s, int i, double *q[]) {
    const char delim[] = " \t";
    char *tok;
    for ( ; ; s = NULL) {
        if (*q == NULL) break;
        if ( (tok = strtok(s, delim)) == NULL)
            return FAIL;
        if (sscanf(tok, "%lf", &(*q)[i]) != 1)
            return FAIL;
        q++;
    }
    return OK;
}
static int blank(const char *s) {
    char c;
    while ( (c = *s++) != '\0')
        if (c != ' ' && c != '\t' && c != '\n') return 0;
    return 1;
}

void punto_read(const char *path, /**/ int *pn, double *queue[]) {
    FILE *f;
    int n;
    char s[SIZE];
    
    if ((f = fopen(path, "r")) == NULL)
        ERR("fail to open '%s'", path);

    n = 0;
    while (fgets(s, SIZE, f) != NULL  && !blank(s)) {
        if (read(s, n++, queue) != OK)
            ERR("wrong line '%s' in file '%s'", s, path);
    }
    
    if (fclose(f) != 0)
        ERR("fail to close '%s'", path);

    *pn = n;
}
