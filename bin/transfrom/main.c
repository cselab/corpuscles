#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/argv.h>
#include <co/bbox.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/transform.h>
#include <co/util.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static const char *me = "co.transform";
static const real FV = 40;      /* default field of view  */
enum {X, Y, Z};

static int t_gamma(real, int, real *, real *, real *);
static real radian(real);

static void
usg(void)
{
    fprintf(stderr,
	    "%s [-c|-b] [-l] [-g gamma] [-t x y z] [-r ox oy oz] [-s sx sy sz] [-f field of view] < IN.off > OUT.off\n",
	    me);
    fprintf(stderr, "-c move to the center of the mass\n");
    fprintf(stderr, "-b move to the center of the bounding box\n");
    fprintf(stderr, "-g float stretch x += gamma*z\n");
    fprintf(stderr, "-s sx sy sz scale\n");
    fprintf(stderr, "-t tx ty tz translate\n");
    fprintf(stderr, "-r rx ry rz rotate (degree)\n");
    fprintf(stderr, "-l log\n");
    exit(2);
}

int
main(__UNUSED int argc, char **argv)
{
    enum { NONE, COM, COM_Z, BOX };
    char *arg;
    char name[999];
    FILE *file;
    He *he;
    int Center;
    int i;
    int Log;
    int n;
    int Gamma;
    real com[3];
    real f;
    real rx;
    real ry;
    real rz;
    real sx;
    real sy;
    real sz;
    real tx;
    real ty;
    real tz;
    real *x;
    real *y;
    real *z;
    real gamma;

    err_set_ignore();
    argv++;
    tx = ty = tz = rx = ry = rz = 0;
    sx = sy = sz = 1;
    f = FV;
    i = -1;
    n = -1;
    Center = NONE;
    Log = 0;
    Gamma = 0;
    while (*argv != NULL) {
	arg = argv++[0];
	if (arg[0] != '-')
	    break;
	switch (arg[1]) {
	case 'h':
	    usg();
	    break;
	case 'g':
	    if (argv_real(&argv, &gamma) != CO_OK)
		ER("wrong -g option");
	    Gamma = 1;
	    break;
	case 'l':
	    Log = 1;
	    break;
	case 'c':
	    switch (arg[2]) {
	    case '\0': 
		Center = COM;
		break;
	    case 'z':
		Center = COM_Z;
		break;
	    default:
		ER("%s: unknown option: '%s'", me, arg);
	    }
	    break;
	case 'b':
	    Center = BOX;
	    break;
	case 't':
	    if (argv_real(&argv, &tx) != CO_OK)
		ER("wrong -t option");
	    if (argv_real(&argv, &ty) != CO_OK)
		ER("wrong -t option");
	    if (argv_real(&argv, &tz) != CO_OK)
		ER("wrong -t option");
	    break;
	case 's':
	    if (argv_real(&argv, &sx) != CO_OK)
		ER("wrong -s option");
	    if (argv_real(&argv, &sy) != CO_OK)
		ER("wrong -s option");
	    if (argv_real(&argv, &sz) != CO_OK)
		ER("wrong -s option");
	    break;
	case 'r':
	    if (argv_real(&argv, &rx) != CO_OK)
		ER("wrong -r option");
	    if (argv_real(&argv, &ry) != CO_OK)
		ER("wrong -r option");
	    if (argv_real(&argv, &rz) != CO_OK)
		ER("wrong -r option");
	    break;
	case 'f':
	    if (argv_real(&argv, &f) != CO_OK)
		ER("wrong -f option");
	    break;
	case 'i':
	    if (argv_int(&argv, &i) != CO_OK)
		ER("wrong -i option");
	    break;
	case 'n':
	    if (argv_int(&argv, &n) != CO_OK)
		ER("wrong -n option");
	    break;
	default:
	    ER("%s: unknown option: '%s'", me, arg);
	}
    }
    if (y_inif(stdin, &he, &x, &y, &z) != CO_OK)
	ER("fail to open input file");
    n = he_nv(he);

    Bbox *box;

    switch (Center) {
    case COM:
	transform_centroid(he, x, y, z, com);
	if (Log)
	    vec_fprintf(com, stderr, FMT);
	vec_neg(com);
	transform_tran(com, n, x, y, z);
	break;
    case COM_Z:
	transform_centroid(he, x, y, z, com);
	if (Log)
	    vec_fprintf(com, stderr, FMT);
	vec_neg(com);
	com[Z] = 0;
	transform_tran(com, n, x, y, z);
	break;	
    case BOX:
        bbox_ini(&box);
        bbox_update(box, n, x, y, z);
        bbox_center(box, com);
        bbox_fin(box);
        if (Log)
            vec_fprintf(com, stderr, FMT);
        vec_neg(com);
        transform_tran(com, n, x, y, z);
        break;
    }
    transform_rotx(radian(-rx), n, x, y, z);
    transform_roty(radian(-ry), n, x, y, z);
    transform_rotz(radian(-rz), n, x, y, z);
    transform_scalx(sx, n, x, y, z);
    transform_scaly(sy, n, x, y, z);
    transform_scalz(sz, n, x, y, z);
    transform_tranx(tx, n, x, y, z);
    transform_trany(ty, n, x, y, z);
    transform_tranz(tz, n, x, y, z);
    if (Gamma)
        t_gamma(gamma, n, x, y, z);

    if (i == -1) {
        if (off_he_xyz_fwrite(he, x, y, z, stdout) != CO_OK)
            ER("fail to write");
    } else {
        sprintf(name, "%05d.t.off", i);
        file = fopen(name, "w");
        if (file == NULL)
            ER("fail to open '%s'", name);
        if (off_he_xyz_fwrite(he, x, y, z, file) != CO_OK)
            ER("fail to write '%s'", name);
        fprintf(stderr, "%s: %s\n", me, name);
        fclose(file);
    }

    y_fin(he, x, y, z);
    return 0;
}

static int
t_gamma(real gamma, int n, real * x, real * y, real * z)
{
    int i;

    for (i = 0; i < n; i++)
        x[i] += gamma * z[i];
    return CO_OK;
}

static real
radian(real x)
{
    return 0.0174532925199433 * x;
}
