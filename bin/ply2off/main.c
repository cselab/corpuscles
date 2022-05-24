#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/ply.h>

static const char *me = "co.ply2off";
static void
usg(void)
{
    fprintf(stderr, "%s < PLY > OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    real *x;
    real *y;
    real *z;
    He *he;

    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }

    err_set_ignore();
    if (ply_he_ini(stdin, &he, &x, &y, &z) != CO_OK) {
      fprintf(stderr, "%s: fail to read ply file\n", me);
      exit(1);
    }

    if (off_he_xyz_fwrite(he, x, y, z, stdout) != CO_OK) {
      fprintf(stderr, "%s: fail to write OFF file\n", me);
      exit(1);
    }
    
    ply_he_fin(he, x, y, z);
}
