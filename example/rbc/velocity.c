#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <real.h>
#include <co/array.h>
#include <co/bi/cortez_zero.h>
#include <co/bi.h>
#include <co/err.h>
#include <co/f/garea.h>
#include <co/f/juelicher_xin.h>
#include <co/force.h>
#include <co/f/volume.h>
#include <co/he.h>
#include <co/len.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/util.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/surface.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static int num(char **, /**/ int *);
static int scl(char **, /**/ real *);
static int fargv(char ***, He *);
static int force(He *, const real *, const real *, const real *, real *,
                 real *, real *);
static int fin(void);
static real reduced_volume(real, real);

static struct {
    Surface *surface;
    BiCortezZero *cortez;
} Grid;
static int grid_ini(BI * bi);
static int grid_write(He *, const real *, const real *, const real *,
                      const real *, const real *, const real *);
static int grid_fin(void);

static const char *me = "rbc";
static const real pi = 3.141592653589793115997964;
static const real tol = 0.01;
static const int iter_max = 100;

#define FMT_IN CO_REAL_IN
#define FMT_OUT CO_REAL_OUT

static Force *Fo[20] = {
    NULL
};

static HeFVolume *fvolume;
static He *he;
static BI *bi;
static real R, D;
static real rho, eta, lambda, gamdot, dt;
static int start, end, freq_out, freq_stat;
static real *fx, *fy, *fz;
static real *ux, *uy, *uz;
static real *wx, *wy, *wz;
static real *Vx, *Vy, *Vz;

static int nv, nt;

char file_out[999];
char file_stat[99] = "stat.dat";
char file_msg[99] = "msg.out";
FILE *fm;

static void
usg(void)
{
    fprintf(stderr, "%s volume V Kv garea A Kga area a Ka \n", me);
    fprintf(stderr, "juelicher_xin Kb C0 Kad DA0D\n");
    fprintf(stderr, "strain ref_file lim mua mub a3 a4 b1 b2\n");
    fprintf(stderr, "cortez R D rho eta lamda gamdot dt\n");
    fprintf(stderr, "start end freq_out freq_stat\n");
    fprintf(stderr, "< OFF input file\n");
}

int
main(__UNUSED int argc, char **argv)
{
    real *x, *y, *z;
    real t, time;
    int s, i;
    real eng, et, ega, ev, eb, ebl, ebn, es;
    char name[99];
    real A0, V0, v0;
    real A, V, v;
    real a, e, reg;
    real M, m;

    //err_set_ignore();
    argv++;
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    fargv(&argv, he);


    if ((fm = fopen(file_stat, "w")) == NULL) {
        ER("Failed to open '%s'", file_stat);
    }
    fputs("#dt s t A/A0 V/V0 v et ega ev eb ebl ebn es\n", fm);
    fclose(fm);

    V0 = he_f_volume_V0(fvolume);

    i = 0;
    A0 = -1;
    while (Fo[i]) {

        strcpy(name, force_name(Fo[i]));

        if (strcmp(name, "garea") == 0) {
            A0 = he_f_garea_A0(force_pointer(Fo[i]));
        }
        //else if ( strcmp(name, "volume") == 0 ) {
        //V0 = he_f_volume_V0(force_pointer(Fo[i]));}
        i++;
    }

    v0 = reduced_volume(A0, V0);

    M = rho * A0 * D * 2;
    m = M / nv;
    a = A0 / nt;
    e = 2 * sqrt(a) / sqrt(sqrt(3.0));
    reg = 0.1 * e;

    if ((fm = fopen(file_msg, "w")) == NULL) {
        ER("Failed to open '%s'", file_msg);
    }

    fprintf(fm, "A0 V0 v0 = %g %g %g\n", A0, V0, v0);
    fprintf(fm, "R D rho eta lambda gamdot = %g %g %g %g %g %g\n", R, D,
            rho, eta, lambda, gamdot);
    fprintf(fm, "Nv Nt = %i %i\n", nv, nt);
    fprintf(fm, "M m a e reg dt = %g %g %g %g %g %g\n", M, m, a, e, reg,
            dt);
    fclose(fm);

    CALLOC3(nv, &ux, &uy, &uz);
    CALLOC3(nv, &wx, &wy, &wz);
    CALLOC3(nv, &fx, &fy, &fz);

    t = time = start * dt;
    s = start;

    grid_ini(bi);
    force(he, x, y, z, fx, fy, fz);
    grid_write(he, x, y, z, fx, fy, fz);
    grid_fin();

    FREE3(Vx, Vy, Vz);
    FREE3(ux, uy, uz);
    FREE3(wx, wy, wz);
    FREE3(fx, fy, fz);
    bi_fin(bi);
    fin();
    y_fin(he, x, y, z);
    he_f_volume_fin(fvolume);

}

static int
num(char **v, /**/ int *p)
{
    if (*v == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*v, "%d", p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int
scl(char **v, /**/ real * p)
{
    if (*v == NULL)
        ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int
fargv(char ***p, He * he)
{

    char *name, **v;
    int i;

    i = 0;
    v = *p;

    if (strcmp(v[0], "volume") != 0) {
        ER("not a volume %s", v[0]);
    }
    v++;
    he_f_volume_argv(&v, he, &fvolume);
    MALLOC3(nv, &Vx, &Vy, &Vz);

    while (1) {
        if (v[0] == NULL)
            break;
        name = v[0];
        if (!force_good(name))
            break;
        MSG("%s", name);
        v++;
        force_argv(name, &v, he, &Fo[i]);
        i++;
    }

    name = v[0];
    if (name == NULL)
        ER("expecting BI");
    if (!bi_good(name)) {
        MSG("not a bi name: '%s'", name);
        ER("possible names are: %s", bi_list());
    }
    v++;
    bi_argv(name, &v, he, &bi);

    scl(v, &R);
    v++;
    scl(v, &D);
    v++;
    scl(v, &rho);
    v++;
    scl(v, &eta);
    v++;
    scl(v, &lambda);
    v++;
    scl(v, &gamdot);
    v++;
    scl(v, &dt);
    v++;
    num(v, &start);
    v++;
    num(v, &end);
    v++;
    num(v, &freq_out);
    v++;
    num(v, &freq_stat);
    v++;

    *p = v;
    return CO_OK;
}

static int
force(He * he, const real * x, const real * y, const real * z, real * fx,
      real * fy, real * fz)
{
    int i;

    i = 0;
    while (Fo[i]) {
        force_force(Fo[i], he, x, y, z, fx, fy, fz);
        i++;
    }
    return CO_OK;
}


static int
fin(void)
{
    int i;

    i = 0;
    while (Fo[i]) {
        force_fin(Fo[i]);
        i++;
    }
    return CO_OK;
}

static real
reduced_volume(real area, real volume)
{
    return (6 * sqrt(pi) * volume) / pow(area, 3.0 / 2);
}

static int
grid_ini(BI * bi)
{
    int status;
    real lo[2] = { -3, -3 }, hi[2] = { 3, 3 }, size = 0.25;
    if (!util_eq(bi_name(bi), "cortez_zero"))
        ER("grid is defined only for cortez_zero");
    Grid.cortez = bi_pointer(bi);
    status = surface_ini(lo, hi, size, &Grid.surface);
    if (status != CO_OK)
        ER("surface_ini faield");
    return CO_OK;
}

static int
grid_fin(void)
{
    surface_fin(Grid.surface);
    return CO_OK;
}

static int
grid_write(He * he, const real * x, const real * y, const real * z,
           const real * fx, const real * fy, const real * fz)
{
    enum { X, Y, Z };
    int i, j, k, l;
    real r[3], v[3];
    real *distance, *vx, *vy, *vz;
    int nx, ny, nz, n;
    real lx, ly, lz, hx, hy, hz, dx, dy, dz;
    FILE *f;

    nx = ny = nz = 40;
    lx = ly = lz = -2;
    hx = hy = hz = 2;
    dx = nx == 0 ? 0 : (hx - lx) / nx;
    dy = ny == 0 ? 0 : (hy - ly) / ny;
    dz = nz == 0 ? 0 : (hz - lz) / nz;
    n = (nx + 1) * (ny + 1) * (nz + 1);

    MALLOC3(n, &vx, &vy, &vz);
    MALLOC(n, &distance);

    surface_update(Grid.surface, he, x, y, z);
    l = 0;
    for (k = 0; k <= nz; k++)
        for (j = 0; j <= ny; j++)
            for (i = 0; i <= nx; i++) {
                r[X] = lx + dx * i;
                r[Y] = ly + dy * j;
                r[Z] = lz + dz * k;
                bi_cortez_zero_single_velocity(Grid.cortez, he, x, y, z,
                                               fx, fy, fz, r, v);
                vx[l] = -1 / eta * v[X] + gamdot * r[Z];
                vy[l] = -1 / eta * v[Y];
                vz[l] = -1 / eta * v[Z];
                surface_distance(Grid.surface, r[X], r[Y], r[Z],
                                 &distance[l]);
                l++;
            }

    f = stdout;
    fprintf(f, "# vtk DataFile Version 2.0\n"
            "generated by %s\n"
            "ASCII\n"
            "DATASET STRUCTURED_POINTS\n"
            "DIMENSIONS %d %d %d\n"
            "ORIGIN %.16g %.16g %.16g\n"
            "SPACING %.16g %.16g %.16g\n",
            me, nx + 1, ny + 1, nz + 1, lx, ly, lz, dx, dy, dz);
    fprintf(f, "POINT_DATA %d\n", n);
    fputs("VECTORS v double\n", f);
    for (i = 0; i < n; i++)
        fprintf(f, FMT " " FMT " " FMT "\n", vx[i], vy[i], vz[i]);
    fputs("SCALARS distance double\n", f);
    fputs("LOOKUP_TABLE DEFAULT\n", f);
    for (i = 0; i < n; i++)
        fprintf(f, FMT "\n", distance[i]);

    FREE3(vx, vy, vz);
    FREE(distance);
    return CO_OK;
}
