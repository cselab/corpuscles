#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <real.h>
#include <co/argv.h>
#include <co/array.h>
#include <co/bbox.h>
#include <co/bi/cortez_zero.h>
#include <co/bi.h>
#include <co/err.h>
#include <co/f/garea.h>
#include <co/force.h>
#include <co/subst.h>
#include <co/he.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/util.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/surface.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static int fargv(char ***, He *);
static int force(He *, const real *, const real *, const real *, real *,
                 real *, real *);
static int fin(void);

static struct {
    Surface *surface;
    BiCortezZero *cortez;
} Grid;
static int grid_ini(BI * bi);
static int grid_write(He *, const real *, const real *, const real *,
                      const real *, const real *, const real *);
static int grid_fin(void);

static const char *me = "rbc/double";

#define FMT_IN CO_REAL_IN

static Force *Fo[20] = {
    NULL
};

static He *he;
static BI *bi;
static real eta, lambda, gamdot;
static real *fx, *fy, *fz;
static int nv, nt;

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

    //err_set_ignore();
    argv++;
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    fargv(&argv, he);
    CALLOC3(nv, &fx, &fy, &fz);
    grid_ini(bi);
    force(he, x, y, z, fx, fy, fz);
    grid_write(he, x, y, z, fx, fy, fz);
    grid_fin();

    FREE3(fx, fy, fz);
    bi_fin(bi);
    fin();
    y_fin(he, x, y, z);
}

static int
fargv(char ***p, He * he)
{

    char *name, **v;
    int i;

    i = 0;
    v = *p;

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
    argv_real(&v, &eta);
    argv_real(&v, &lambda);
    argv_real(&v, &gamdot);
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
    real r[3], vs[3], vd[3];
    real *distance, *vx, *vy, *vz;
    int nx, ny, nz, nv, n;
    real lx, ly, lz, hx, hy, hz, dx, dy, dz;
    real *blo, *bhi;
    FILE *f;
    Bbox *box;
    real margin[] = { 0.2, 0.2, 0.2 };
    nx = ny = nz = 40;

    nv = he_nv(he);
    bbox_ini(&box);
    bbox_update(box, nv, x, y, z);
    bbox_lo(box, &blo);
    bbox_hi(box, &bhi);
    vec_sub(margin, blo);
    vec_add(margin, bhi);
    lx = blo[X];
    ly = blo[Y];
    lz = blo[Z];
    hx = bhi[X];
    hy = bhi[Y];
    hz = bhi[Z];
    bbox_fin(box);
    dx = nx == 0 ? 0 : (hx - lx) / nx;
    dy = ny == 0 ? 0 : (hy - ly) / ny;
    dz = nz == 0 ? 0 : (hz - lz) / nz;
    n = (nx + 1) * (ny + 1) * (nz + 1);
    MALLOC3(n, &vx, &vy, &vz);
    MALLOC(n, &distance);

    Subst *subst;
    real alpha, beta, coef;
    real *ux, *uy, *uz;
    real *wx, *wy, *wz;
    MALLOC3(nv, &ux, &uy, &uz);
    CALLOC3(nv, &wx, &wy, &wz);
    alpha = 2 * (1 - lambda) / (1 + lambda);
    beta = -2 / (eta * (1 + lambda));
    real tol = 0.001;
    int iter_max = 100;
    coef = 2 / (1 + lambda);
    subst_ini(nv, alpha, tol, iter_max, &subst);
    for (i = 0; i < nv; i++)
        wx[i] += coef * gamdot * z[i];
    bi_update(bi, he, x, y, z);
    bi_single(bi, he, beta, x, y, z, fx, fy, fz, wx, wy, wz);
    subst_apply(subst, he, bi, x, y, z, wx, wy, wz, ux, uy, uz);
    if (subst_niter(subst)  > 1)
        MSG("Subst.iiter: %d", subst_niter(subst));    
    surface_update(Grid.surface, he, x, y, z);
    l = 0;
    for (k = 0; k <= nz; k++)
        for (j = 0; j <= ny; j++)
            for (i = 0; i <= nx; i++) {
                r[X] = lx + dx * i;
                r[Y] = ly + dy * j;
                r[Z] = lz + dz * k;
                bi_cortez_zero_single_velocity(Grid.cortez, he, x, y, z,
                                               fx, fy, fz, r, vs);
                bi_cortez_zero_double_velocity(Grid.cortez, he, x, y, z,
                                               ux, uy, uz, r, vd);
                vx[l] = -vs[X]/(2*eta) + (1 - lambda)/2*vd[X] + gamdot*r[Z];
                vy[l] = -vs[Y]/(2*eta) + (1 - lambda)/2*vd[Y];
                vz[l] = -vx[Z]/(2*eta) + (1 - lambda)/2*vd[Z];
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

    subst_fin(subst);
    FREE3(vx, vy, vz);
    FREE3(ux, uy, uz);
    FREE3(wx, wy, wz);
    FREE(distance);
    return CO_OK;
}
