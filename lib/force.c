#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "real.h"

#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/container.h"
#include "co/macro.h"
#include "co/util.h"

#include "co/f/area.h"
#include "co/f/garea.h"
#include "co/f/volume.h"
#include "co/f/juelicher_xin.h"
#include "co/f/edg_sq.h"
#include "co/f/harmonic.h"
#include "co/f/harmonic_ref.h"
#include "co/f/area_voronoi.h"
#include "co/f/garea_voronoi.h"
#include "co/f/volume_normal.h"
#include "co/f/area_sq.h"
#include "co/f/gompper.h"
#include "co/f/strain.h"
#define SIZE (4048)

#include "co/force.h"

#define T Force

static int force_area_argv(char***, He*, T**);
static int force_garea_argv(char***, He*, T**);
static int force_volume_argv(char***, He*, T**);
static int force_juelicher_xin_argv(char***, He*, T**);
static int force_edg_sq_argv(char***, He*, T**);
static int force_harmonic_argv(char***, He*, T**);
static int force_harmonic_ref_argv(char***, He*, T**);
static int force_area_voronoi_argv(char***, He*, T**);
static int force_garea_voronoi_argv(char***, He*, T**);
static int force_volume_normal_argv(char***, He*, T**);
static int force_area_sq_argv(char***, He*, T**);
static int force_gompper_argv(char***, He*, T**);
static int force_strain_argv(char***, He*, T**);

struct T {
    struct Vtable *vtable;
    const char *name;
};

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeArgv)(char***, He*, T**);

static const char *Name[] = {
    "area",
    "garea",
    "volume",
    "juelicher_xin",
    "edg_sq",
    "harmonic",
    "harmonic_ref",
    "area_voronoi",
    "garea_voronoi",
    "volume_normal",
    "area_sq",
    "gompper",
    "strain",
};

static const TypeArgv Argv[] = {
    force_area_argv,
    force_garea_argv,
    force_volume_argv,
    force_juelicher_xin_argv,
    force_edg_sq_argv,
    force_harmonic_argv,
    force_harmonic_ref_argv,
    force_area_voronoi_argv,
    force_garea_voronoi_argv,
    force_volume_normal_argv,
    force_area_sq_argv,
    force_gompper_argv,
    force_strain_argv,
};

int force_argv(const char *name, char ***parg, He *he, T **pq)
{
    int status;
    T *q;
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i])) {
            status = Argv[i](parg, he, &q);
            q->name = Name[i];
            *pq = q;
            return status;
        }
    MSG("unknown force: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(CO_INDEX, "");
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
int force_area_argv(char ***p, He *he, /**/ T **pq)
{
    Area *q;
    MALLOC(1, &q);
    q->force.vtable = &area_vtable;
    *pq = &q->force;
    return he_f_area_argv(p, he, &q->local);
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
int force_garea_argv(char ***p, He *he, /**/ T **pq)
{
    Garea *q;
    MALLOC(1, &q);
    q->force.vtable = &garea_vtable;
    *pq = &q->force;
    return he_f_garea_argv(p, he, &q->local);
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
int force_volume_argv(char ***p, He *he, /**/ T **pq)
{
    Volume *q;
    MALLOC(1, &q);
    q->force.vtable = &volume_vtable;
    *pq = &q->force;
    return he_f_volume_argv(p, he, &q->local);
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
int force_juelicher_xin_argv(char ***p, He *he, /**/ T **pq)
{
    Juelicher_xin *q;
    MALLOC(1, &q);
    q->force.vtable = &juelicher_xin_vtable;
    *pq = &q->force;
    return he_f_juelicher_xin_argv(p, he, &q->local);
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
int force_edg_sq_argv(char ***p, He *he, /**/ T **pq)
{
    Edg_sq *q;
    MALLOC(1, &q);
    q->force.vtable = &edg_sq_vtable;
    *pq = &q->force;
    return he_f_edg_sq_argv(p, he, &q->local);
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
int force_harmonic_argv(char ***p, He *he, /**/ T **pq)
{
    Harmonic *q;
    MALLOC(1, &q);
    q->force.vtable = &harmonic_vtable;
    *pq = &q->force;
    return he_f_harmonic_argv(p, he, &q->local);
}
typedef struct Harmonic_ref Harmonic_ref;
struct Harmonic_ref {
    T force;
    HeFHarmonicRef *local;
};
static int harmonic_ref_fin(T *q)
{
    int status;
    Harmonic_ref *b = CONTAINER_OF(q, Harmonic_ref, force);
    status = he_f_harmonic_ref_fin(b->local);
    FREE(q);
    return status;
}
static int harmonic_ref_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Harmonic_ref *b = CONTAINER_OF(q, Harmonic_ref, force);
    return he_f_harmonic_ref_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real harmonic_ref_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Harmonic_ref *b = CONTAINER_OF(q, Harmonic_ref, force);
    return he_f_harmonic_ref_energy(b->local, he, x, y, z);
}
static Vtable harmonic_ref_vtable = {
    harmonic_ref_fin,
    harmonic_ref_force,
    harmonic_ref_energy,
};
int force_harmonic_ref_argv(char ***p, He *he, /**/ T **pq)
{
    Harmonic_ref *q;
    MALLOC(1, &q);
    q->force.vtable = &harmonic_ref_vtable;
    *pq = &q->force;
    return he_f_harmonic_ref_argv(p, he, &q->local);
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
int force_area_voronoi_argv(char ***p, He *he, /**/ T **pq)
{
    Area_voronoi *q;
    MALLOC(1, &q);
    q->force.vtable = &area_voronoi_vtable;
    *pq = &q->force;
    return he_f_area_voronoi_argv(p, he, &q->local);
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
int force_garea_voronoi_argv(char ***p, He *he, /**/ T **pq)
{
    Garea_voronoi *q;
    MALLOC(1, &q);
    q->force.vtable = &garea_voronoi_vtable;
    *pq = &q->force;
    return he_f_garea_voronoi_argv(p, he, &q->local);
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
int force_volume_normal_argv(char ***p, He *he, /**/ T **pq)
{
    Volume_normal *q;
    MALLOC(1, &q);
    q->force.vtable = &volume_normal_vtable;
    *pq = &q->force;
    return he_f_volume_normal_argv(p, he, &q->local);
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
int force_area_sq_argv(char ***p, He *he, /**/ T **pq)
{
    Area_sq *q;
    MALLOC(1, &q);
    q->force.vtable = &area_sq_vtable;
    *pq = &q->force;
    return he_f_area_sq_argv(p, he, &q->local);
}
typedef struct Gompper Gompper;
struct Gompper {
    T force;
    HeFGompper *local;
};
static int gompper_fin(T *q)
{
    int status;
    Gompper *b = CONTAINER_OF(q, Gompper, force);
    status = he_f_gompper_fin(b->local);
    FREE(q);
    return status;
}
static int gompper_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Gompper *b = CONTAINER_OF(q, Gompper, force);
    return he_f_gompper_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real gompper_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Gompper *b = CONTAINER_OF(q, Gompper, force);
    return he_f_gompper_energy(b->local, he, x, y, z);
}
static Vtable gompper_vtable = {
    gompper_fin,
    gompper_force,
    gompper_energy,
};
int force_gompper_argv(char ***p, He *he, /**/ T **pq)
{
    Gompper *q;
    MALLOC(1, &q);
    q->force.vtable = &gompper_vtable;
    *pq = &q->force;
    return he_f_gompper_argv(p, he, &q->local);
}
typedef struct Strain Strain;
struct Strain {
    T force;
    HeFStrain *local;
};
static int strain_fin(T *q)
{
    int status;
    Strain *b = CONTAINER_OF(q, Strain, force);
    status = he_f_strain_fin(b->local);
    FREE(q);
    return status;
}
static int strain_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz)
{
    Strain *b = CONTAINER_OF(q, Strain, force);
    return he_f_strain_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real strain_energy(T *q, He *he, const real *x, const real *y, const real *z)
{
    Strain *b = CONTAINER_OF(q, Strain, force);
    return he_f_strain_energy(b->local, he, x, y, z);
}
static Vtable strain_vtable = {
    strain_fin,
    strain_force,
    strain_energy,
};
int force_strain_argv(char ***p, He *he, /**/ T **pq)
{
    Strain *q;
    MALLOC(1, &q);
    q->force.vtable = &strain_vtable;
    *pq = &q->force;
    return he_f_strain_argv(p, he, &q->local);
}
