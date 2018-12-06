#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/container.h"
#include "he/macro.h"
#include "he/util.h"

#include "he/f/area.h"
#include "he/f/garea.h"
#include "he/f/volume.h"
#include "he/f/juelicher_xin.h"
#include "he/f/edg_sq.h"
#include "he/f/harmonic.h"
#include "he/f/area_voronoi.h"
#include "he/f/garea_voronoi.h"
#include "he/f/volume_normal.h"
#include "he/f/area_sq.h"
#define SIZE (4048)

#include "he/force.h"

#define T Force

static int force_area_ini(void *param[], He*, /**/ T**);
static int force_garea_ini(void *param[], He*, /**/ T**);
static int force_volume_ini(void *param[], He*, /**/ T**);
static int force_juelicher_xin_ini(void *param[], He*, /**/ T**);
static int force_edg_sq_ini(void *param[], He*, /**/ T**);
static int force_harmonic_ini(void *param[], He*, /**/ T**);
static int force_area_voronoi_ini(void *param[], He*, /**/ T**);
static int force_garea_voronoi_ini(void *param[], He*, /**/ T**);
static int force_volume_normal_ini(void *param[], He*, /**/ T**);
static int force_area_sq_ini(void *param[], He*, /**/ T**);

struct T {
    struct Vtable *vtable;
    const char *name;
};

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeIni)(void**, He*, T**);

static const char *Name[] = {
    "area",
    "garea",
    "volume",
    "juelicher_xin",
    "edg_sq",
    "harmonic",
    "area_voronoi",
    "garea_voronoi",
    "volume_normal",
    "area_sq",
};

static int Narg[] = {
    2,
    2,
    2,
    4,
    1,
    2,
    2,
    2,
    2,
    1,
};

static const TypeIni Ini[] = {
    force_area_ini,
    force_garea_ini,
    force_volume_ini,
    force_juelicher_xin_ini,
    force_edg_sq_ini,
    force_harmonic_ini,
    force_area_voronoi_ini,
    force_garea_voronoi_ini,
    force_volume_normal_ini,
    force_area_sq_ini,
};

int force_ini(const char *name, void **param, He *he, T **pq)
{
    int status;
    T *q;
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i])) {
            status = Ini[i](param, he, &q);
            q->name = Name[i];
            *pq = q;
            return status;
        }
    MSG("unknown force: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(HE_INDEX, "");
}

int force_narg(const char *name)
{
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i]))
            return Narg[i];
    MSG("unknown force: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(HE_INDEX, "");
}

static int scl(const char **pargv[], real *p) {
    const char **argv;
    argv = *pargv;
    if (*argv == NULL)
        ERR(HE_IO, "not enough args");
    if (sscanf(*argv, HE_REAL_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;

    *pargv = argv;
    return HE_OK;
}

static int str(const char **pargv[], char *p) {
    const char **argv;
    argv = *pargv;

    if (*argv == NULL)
        ERR(HE_IO, "not enough args");

    strncpy(p, *argv, SIZE - 1);
    argv++;
    *pargv = argv;
    return HE_OK;
}

int force_argv(const char **pargv[], He *he, /**/ T **pq) {
    real   param[999];
    void *vparam[999];
    char name[SIZE];
    int narg, i;

    str(pargv, name);
    narg = force_narg(name);
    for (i = 0; i < narg; i++) {
        scl(pargv, &param[i]);
        vparam[i] = &param[i];
    }
    return force_ini(name, vparam, he,  pq);
}

const char *force_list()
{
    const int n = sizeof(Name)/sizeof(Name[0]);
    const char *sep = "/";
    int i;
    List[0] = '\0';
    for (i = 0; i < n; i++) {
        if (i > 0) strncat(List, sep, SIZE - 1);
        strncat(List, Name[i], SIZE - 1);
    }
    return List;
}

typedef struct Vtable Vtable;
struct Vtable {
    int (*fin)(T*);
    int (*force)(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
    real (*energy)(T*, He*, const real *x, const real *y, const real *z);
};

int force_fin(T *q)
{
    return q->vtable->fin(q);
}

int force_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz)
{
    return q->vtable->force(q, he, x, y, z, fx, fy, fz);
}

real force_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    return q->vtable->energy(q, he, x, y, z);
}

const char *force_name(T *q)
{
    return q->name;
}

typedef struct Area Area;
struct Area {
    T force;
    HeFArea *local;
};
static int area_fin(T *q)
{
    int status;
    Area *b = CONTAINER_OF(q, Area, force);
    status = he_f_area_fin(b->local);
    FREE(q);
    return status;
}
static int area_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Area *b = CONTAINER_OF(q, Area, force);
    return he_f_area_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real area_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Area *b = CONTAINER_OF(q, Area, force);
    return he_f_area_energy(b->local, he, x, y, z);
}
static Vtable area_vtable = {
    area_fin,
    area_force,
    area_energy,
};
int force_area_ini(void *param[], He *he, /**/ T **pq)
{
    Area *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &area_vtable;
    *pq = &q->force;
    return he_f_area_ini(g1, g2,  he, &q->local);
}
typedef struct Garea Garea;
struct Garea {
    T force;
    HeFGarea *local;
};
static int garea_fin(T *q)
{
    int status;
    Garea *b = CONTAINER_OF(q, Garea, force);
    status = he_f_garea_fin(b->local);
    FREE(q);
    return status;
}
static int garea_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Garea *b = CONTAINER_OF(q, Garea, force);
    return he_f_garea_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real garea_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Garea *b = CONTAINER_OF(q, Garea, force);
    return he_f_garea_energy(b->local, he, x, y, z);
}
static Vtable garea_vtable = {
    garea_fin,
    garea_force,
    garea_energy,
};
int force_garea_ini(void *param[], He *he, /**/ T **pq)
{
    Garea *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &garea_vtable;
    *pq = &q->force;
    return he_f_garea_ini(g1, g2,  he, &q->local);
}
typedef struct Volume Volume;
struct Volume {
    T force;
    HeFVolume *local;
};
static int volume_fin(T *q)
{
    int status;
    Volume *b = CONTAINER_OF(q, Volume, force);
    status = he_f_volume_fin(b->local);
    FREE(q);
    return status;
}
static int volume_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Volume *b = CONTAINER_OF(q, Volume, force);
    return he_f_volume_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real volume_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Volume *b = CONTAINER_OF(q, Volume, force);
    return he_f_volume_energy(b->local, he, x, y, z);
}
static Vtable volume_vtable = {
    volume_fin,
    volume_force,
    volume_energy,
};
int force_volume_ini(void *param[], He *he, /**/ T **pq)
{
    Volume *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &volume_vtable;
    *pq = &q->force;
    return he_f_volume_ini(g1, g2,  he, &q->local);
}
typedef struct Juelicher_xin Juelicher_xin;
struct Juelicher_xin {
    T force;
    HeFJuelicherXin *local;
};
static int juelicher_xin_fin(T *q)
{
    int status;
    Juelicher_xin *b = CONTAINER_OF(q, Juelicher_xin, force);
    status = he_f_juelicher_xin_fin(b->local);
    FREE(q);
    return status;
}
static int juelicher_xin_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Juelicher_xin *b = CONTAINER_OF(q, Juelicher_xin, force);
    return he_f_juelicher_xin_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real juelicher_xin_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Juelicher_xin *b = CONTAINER_OF(q, Juelicher_xin, force);
    return he_f_juelicher_xin_energy(b->local, he, x, y, z);
}
static Vtable juelicher_xin_vtable = {
    juelicher_xin_fin,
    juelicher_xin_force,
    juelicher_xin_energy,
};
int force_juelicher_xin_ini(void *param[], He *he, /**/ T **pq)
{
    Juelicher_xin *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    real g3 = *(real*)*param++;
    real g4 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &juelicher_xin_vtable;
    *pq = &q->force;
    return he_f_juelicher_xin_ini(g1, g2, g3, g4,  he, &q->local);
}
typedef struct Edg_sq Edg_sq;
struct Edg_sq {
    T force;
    HeFEdgSq *local;
};
static int edg_sq_fin(T *q)
{
    int status;
    Edg_sq *b = CONTAINER_OF(q, Edg_sq, force);
    status = he_f_edg_sq_fin(b->local);
    FREE(q);
    return status;
}
static int edg_sq_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Edg_sq *b = CONTAINER_OF(q, Edg_sq, force);
    return he_f_edg_sq_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real edg_sq_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Edg_sq *b = CONTAINER_OF(q, Edg_sq, force);
    return he_f_edg_sq_energy(b->local, he, x, y, z);
}
static Vtable edg_sq_vtable = {
    edg_sq_fin,
    edg_sq_force,
    edg_sq_energy,
};
int force_edg_sq_ini(void *param[], He *he, /**/ T **pq)
{
    Edg_sq *q;
    real g1 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &edg_sq_vtable;
    *pq = &q->force;
    return he_f_edg_sq_ini(g1,  he, &q->local);
}
typedef struct Harmonic Harmonic;
struct Harmonic {
    T force;
    HeFHarmonic *local;
};
static int harmonic_fin(T *q)
{
    int status;
    Harmonic *b = CONTAINER_OF(q, Harmonic, force);
    status = he_f_harmonic_fin(b->local);
    FREE(q);
    return status;
}
static int harmonic_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Harmonic *b = CONTAINER_OF(q, Harmonic, force);
    return he_f_harmonic_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real harmonic_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Harmonic *b = CONTAINER_OF(q, Harmonic, force);
    return he_f_harmonic_energy(b->local, he, x, y, z);
}
static Vtable harmonic_vtable = {
    harmonic_fin,
    harmonic_force,
    harmonic_energy,
};
int force_harmonic_ini(void *param[], He *he, /**/ T **pq)
{
    Harmonic *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &harmonic_vtable;
    *pq = &q->force;
    return he_f_harmonic_ini(g1, g2,  he, &q->local);
}
typedef struct Area_voronoi Area_voronoi;
struct Area_voronoi {
    T force;
    HeFAreaVoronoi *local;
};
static int area_voronoi_fin(T *q)
{
    int status;
    Area_voronoi *b = CONTAINER_OF(q, Area_voronoi, force);
    status = he_f_area_voronoi_fin(b->local);
    FREE(q);
    return status;
}
static int area_voronoi_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Area_voronoi *b = CONTAINER_OF(q, Area_voronoi, force);
    return he_f_area_voronoi_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real area_voronoi_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Area_voronoi *b = CONTAINER_OF(q, Area_voronoi, force);
    return he_f_area_voronoi_energy(b->local, he, x, y, z);
}
static Vtable area_voronoi_vtable = {
    area_voronoi_fin,
    area_voronoi_force,
    area_voronoi_energy,
};
int force_area_voronoi_ini(void *param[], He *he, /**/ T **pq)
{
    Area_voronoi *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &area_voronoi_vtable;
    *pq = &q->force;
    return he_f_area_voronoi_ini(g1, g2,  he, &q->local);
}
typedef struct Garea_voronoi Garea_voronoi;
struct Garea_voronoi {
    T force;
    HeFGareaVoronoi *local;
};
static int garea_voronoi_fin(T *q)
{
    int status;
    Garea_voronoi *b = CONTAINER_OF(q, Garea_voronoi, force);
    status = he_f_garea_voronoi_fin(b->local);
    FREE(q);
    return status;
}
static int garea_voronoi_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Garea_voronoi *b = CONTAINER_OF(q, Garea_voronoi, force);
    return he_f_garea_voronoi_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real garea_voronoi_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Garea_voronoi *b = CONTAINER_OF(q, Garea_voronoi, force);
    return he_f_garea_voronoi_energy(b->local, he, x, y, z);
}
static Vtable garea_voronoi_vtable = {
    garea_voronoi_fin,
    garea_voronoi_force,
    garea_voronoi_energy,
};
int force_garea_voronoi_ini(void *param[], He *he, /**/ T **pq)
{
    Garea_voronoi *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &garea_voronoi_vtable;
    *pq = &q->force;
    return he_f_garea_voronoi_ini(g1, g2,  he, &q->local);
}
typedef struct Volume_normal Volume_normal;
struct Volume_normal {
    T force;
    HeFVolumeNormal *local;
};
static int volume_normal_fin(T *q)
{
    int status;
    Volume_normal *b = CONTAINER_OF(q, Volume_normal, force);
    status = he_f_volume_normal_fin(b->local);
    FREE(q);
    return status;
}
static int volume_normal_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Volume_normal *b = CONTAINER_OF(q, Volume_normal, force);
    return he_f_volume_normal_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real volume_normal_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Volume_normal *b = CONTAINER_OF(q, Volume_normal, force);
    return he_f_volume_normal_energy(b->local, he, x, y, z);
}
static Vtable volume_normal_vtable = {
    volume_normal_fin,
    volume_normal_force,
    volume_normal_energy,
};
int force_volume_normal_ini(void *param[], He *he, /**/ T **pq)
{
    Volume_normal *q;
    real g1 = *(real*)*param++;
    real g2 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &volume_normal_vtable;
    *pq = &q->force;
    return he_f_volume_normal_ini(g1, g2,  he, &q->local);
}
typedef struct Area_sq Area_sq;
struct Area_sq {
    T force;
    HeFAreaSq *local;
};
static int area_sq_fin(T *q)
{
    int status;
    Area_sq *b = CONTAINER_OF(q, Area_sq, force);
    status = he_f_area_sq_fin(b->local);
    FREE(q);
    return status;
}
static int area_sq_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Area_sq *b = CONTAINER_OF(q, Area_sq, force);
    return he_f_area_sq_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real area_sq_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Area_sq *b = CONTAINER_OF(q, Area_sq, force);
    return he_f_area_sq_energy(b->local, he, x, y, z);
}
static Vtable area_sq_vtable = {
    area_sq_fin,
    area_sq_force,
    area_sq_energy,
};
int force_area_sq_ini(void *param[], He *he, /**/ T **pq)
{
    Area_sq *q;
    real g1 = *(real*)*param++;
    MALLOC(1, &q);
    q->force.vtable = &area_sq_vtable;
    *pq = &q->force;
    return he_f_area_sq_ini(g1,  he, &q->local);
}
