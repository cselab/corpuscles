#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/generation.h>
#include <co/y.h>

#define SIZE (9999)

static const char *me = "generation/file";
static void
usg()
{
    fprintf(stderr, "%s -f file < OFF > OFF\n", me);
    exit(2);
}

static int tri_xyz(int t, He * he, const real * x, const real * y,
                   const real * z, real * u, real * v, real * w);

int
main(int argc, char **argv)
{
    char line[SIZE];
    const char *File;
    FILE *file;
    Generation *generation;
    He *he;
    int i;
    int Color;
    int *level;
    int nt;
    real *color;
    real u;
    real v;
    real w;
    real *x;
    real *y;
    real *z;

    USED(argc);
    File = NULL;
    Color = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'f':
            argv++;
            if ((File = *argv) == NULL) {
                fprintf(stderr, "%s: -f needs an argument\n", me);
                exit(2);
            }
            break;
        case 'c':
            Color = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }

    if (File == NULL) {
        fprintf(stderr, "%s: file (-f) is not given\n", me);
        exit(1);
    }

    y_inif(stdin, &he, &x, &y, &z);
    if ((file = fopen(File, "r")) == NULL) {
        fprintf(stderr, "%s: cannot read '%s'\n", me, File);
        exit(2);
    }
    nt = he_nt(he);
    MALLOC(nt, &level);
    for (i = 0; i < nt; i++) {
        if (fgets(line, SIZE, file) == NULL) {
            fprintf(stderr, "%s: fail to read '%s'\n", me, File);
            exit(2);
        }
        if (sscanf(line, "%d", &level[i]) != 1) {
            fprintf(stderr, "%s: fail to parse '%s' in '%s'\n", me, line,
                    File);
            exit(2);
        }
    }
    fclose(file);
    generation_ini(he, &generation);
    for (i = 0; i < nt; i++) {
        tri_xyz(i, he, x, y, z, &u, &v, &w);
        if (level[i])
            generation_refine(generation, i, he, &x, &y, &z);
    }
    if (Color) {
        nt = he_nt(he);
        MALLOC(nt, &color);
	generation_color(generation, he, color);
        if (boff_tri_fwrite(he, x, y, z, color, stdout) != CO_OK)
            ER("boff_tri_fwrite failed");
        FREE(color);
    } else {
        if (off_he_xyz_fwrite(he, x, y, z, stdout) != CO_OK)
            ER("off_he_xyz_fwrite");
    }

    FREE(level);
    generation_fin(generation);
    y_fin(he, x, y, z);
}

static int
tri_xyz(int t, He * he, const real * x, const real * y, const real * z,
        real * u, real * v, real * w)
{
    enum { X, Y, Z };
    int i;
    int j;
    int k;
    real a[3];
    real b[3];
    real c[3];
    real d[3];

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    tri_center(a, b, c, d);
    *u = d[X];
    *v = d[Y];
    *w = d[Z];
    return CO_OK;
}
