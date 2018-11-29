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

#include "he/f/kantor.h"
#include "he/f/juelicher_xin.h"
#include "he/f/gompper.h"
#include "he/f/gompper_kroll.h"
#include "he/f/meyer.h"
#include "he/f/gompper_xin.h"
#include "he/f/meyer_xin.h"

#include "he/bending.h"

#define T Bending

struct T {struct Vtable *vtable; };

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeIni)(BendingParam, He*, T**);
static const char *Name[] = {
    "kantor",
    "gompper",
    "gompper_kroll",
    "meyer",
    "gompper_xin",
    "meyer_xin",
    "juelicher_xin"
};
static const TypeIni Ini[]  = {
    bending_kantor_ini,
    bending_gompper_ini,
    bending_gompper_kroll_ini,
    bending_meyer_ini,
    bending_gompper_xin_ini,
    bending_meyer_xin_ini,
    bending_juelicher_xin_ini
};

int bending_ini(const char *name, BendingParam param, He *he, T **pq) {
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i]))
            return Ini[i](param, he, pq);

    MSG("unknown bending: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(HE_INDEX, "");
}

const char *bending_list() {
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
    int (*energy_ver)(T*, real**);
    int (*area_ver)(T*, real**);
    int (*curva_mean_ver)(T*, real**);
    int (*norm_ver)(T*, real**, real**, real**);
    int (*laplace_ver)(T*, real**, real**, real**);
};

int bending_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
    return q->vtable->force(q, he, x, y, z, fx, fy, fz);
}
real bending_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    return q->vtable->energy(q, he, x, y, z);
}
int bending_energy_ver(T *q, /**/ real **e) { return q->vtable->energy_ver(q, e); }
int bending_area_ver(T *q, /**/ real **e) { return q->vtable->area_ver(q, e); }
int bending_curva_mean_ver(T *q, /**/ real **e) { return q->vtable->curva_mean_ver(q, e); }
int bending_norm_ver(T *q, /**/ real **e, real **f, real **g) { return q->vtable->norm_ver(q, e, f, g); }
int bending_laplace_ver(T *q, /**/ real **e, real **f, real **g) { return q->vtable->laplace_ver(q, e, f, g); }
int bending_fin(T *q) { return q->vtable->fin(q); }


/* begin kantor */
typedef struct Kantor Kantor;
struct Kantor {T bending; HeFKantor *local; };
static int kantor_fin(T *q) {
    int status;
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    status = he_f_kantor_fin(b->local);
    FREE(q);
    return status;
}
static int kantor_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    return he_f_kantor_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real kantor_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    return he_f_kantor_energy(b->local, he, x, y, z);
}
static int kantor_energy_ver(T *q, /**/ real **e) {
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    return he_f_kantor_energy_ver(b->local, /**/ e);
}
static int kantor_area_ver(T *q, /**/ real **e) {
  Kantor *b = CONTAINER_OF(q, Kantor, bending);
  return he_f_kantor_area_ver(b->local, /**/ e);
}
static int kantor_curva_mean_ver(__UNUSED T *q, /**/ __UNUSED real **e) {
  return HE_OK;
}
static Vtable kantor_vtable = { kantor_fin, kantor_force, kantor_energy, kantor_energy_ver, kantor_area_ver, kantor_curva_mean_ver};
int bending_kantor_ini(BendingParam param, He *he, /**/ T **pq) {
  real Kb, C0;
    Kantor *q;
    Kb = param.Kb;
    C0 = param.C0;
    MALLOC(1, &q);
    q->bending.vtable = &kantor_vtable;
    *pq = &q->bending;
    return he_f_kantor_ini(Kb, C0, he, &q->local);
}
/* end kantor */


/* begin gompper */
typedef struct Gompper Gompper;
struct Gompper {T bending; HeFGompper *local; };
static int gompper_fin(T *q) {
    int status;
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    status = he_f_gompper_fin(b->local);
    FREE(q);
    return status;
}
static int gompper_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real gompper_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_energy(b->local, he, x, y, z);
}
static int gompper_energy_ver(T *q, /**/ real **e) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_energy_ver(b->local, /**/ e);
}
static int gompper_area_ver(T *q, /**/ real **e) {
  Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_area_ver(b->local, /**/ e);
    return HE_OK;
}
static int gompper_curva_mean_ver(T *q, /**/ real **e) {
  Gompper *b = CONTAINER_OF(q, Gompper, bending);
  return he_f_gompper_curva_mean_ver(b->local, /**/ e);
  return HE_OK;
}
static int gompper_norm_ver(T *q, /**/ real **e, real **f, real **g) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_norm_ver(b->local, /**/ e, f, g);
}
static int gompper_laplace_ver(T *q, /**/ real **e, real **f, real **g) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_laplace_ver(b->local, /**/ e, f, g);
}
static Vtable gompper_vtable = { gompper_fin, gompper_force, gompper_energy, gompper_energy_ver, gompper_area_ver,
				 gompper_curva_mean_ver, gompper_norm_ver, gompper_laplace_ver};
int bending_gompper_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Gompper *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &gompper_vtable;
    *pq = &q->bending;
    return he_f_gompper_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end gompper */

/* begin gompper_kroll */
typedef struct Gompper_Kroll Gompper_Kroll;
struct Gompper_Kroll {T bending; HeFGompper_Kroll *local; };
static int gompper_kroll_fin(T *q) {
    int status;
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    status = he_f_gompper_kroll_fin(b->local);
    FREE(q);
    return status;
}
static int gompper_kroll_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real gompper_kroll_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_energy(b->local, he, x, y, z);
}
static int gompper_kroll_energy_ver(T *q, /**/ real **e) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_energy_ver(b->local, /**/ e);
}
static int gompper_kroll_area_ver(T *q, /**/ real **e) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_area_ver(b->local, /**/ e);
    return HE_OK;
}
static int gompper_kroll_curva_mean_ver(T *q, /**/ real **e) {
  Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
  return he_f_gompper_kroll_curva_mean_ver(b->local, /**/ e);
  return HE_OK;
}

static Vtable gompper_kroll_vtable = { gompper_kroll_fin, gompper_kroll_force, gompper_kroll_energy, gompper_kroll_energy_ver, gompper_kroll_area_ver, gompper_kroll_curva_mean_ver};
int bending_gompper_kroll_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Gompper_Kroll *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &gompper_kroll_vtable;
    *pq = &q->bending;
    return he_f_gompper_kroll_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end gompper_kroll */

/* begin juelicher_xin */
typedef struct JuelicherXin JuelicherXin;
struct JuelicherXin {T bending; HeFJuelicherXin *local; };
static int juelicher_xin_fin(T *q) {
    int status;
    JuelicherXin *b = CONTAINER_OF(q, JuelicherXin, bending);
    status = he_f_juelicher_xin_fin(b->local);
    FREE(q);
    return status;
}
static int juelicher_xin_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    JuelicherXin *b = CONTAINER_OF(q, JuelicherXin, bending);
    return he_f_juelicher_xin_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real juelicher_xin_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    JuelicherXin *b = CONTAINER_OF(q, JuelicherXin, bending);
    return he_f_juelicher_xin_energy(b->local, he, x, y, z);
}
static int juelicher_xin_energy_ver(T *q, /**/ real **e) {
    JuelicherXin *b = CONTAINER_OF(q, JuelicherXin, bending);
    return he_f_juelicher_xin_energy_ver(b->local, /**/ e);
}
static int juelicher_xin_area_ver(T *q, /**/ real **e) {
  JuelicherXin *b = CONTAINER_OF(q, JuelicherXin, bending);
  return he_f_juelicher_xin_area_ver(b->local, /**/ e);
}
static int juelicher_xin_curva_mean_ver(T *q, /**/ real **e) {
    JuelicherXin *b = CONTAINER_OF(q, JuelicherXin, bending);
    return he_f_juelicher_xin_curva_mean_ver(b->local, /**/ e);
}
static Vtable juelicher_xin_vtable = { juelicher_xin_fin, juelicher_xin_force, juelicher_xin_energy, juelicher_xin_energy_ver, juelicher_xin_area_ver, juelicher_xin_curva_mean_ver};
int bending_juelicher_xin_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    JuelicherXin *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &juelicher_xin_vtable;
    *pq = &q->bending;
    return he_f_juelicher_xin_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end juelicher_xin */


/* begin meyer */
typedef struct Meyer Meyer;
struct Meyer {T bending; HeFMeyer *local; };
static int meyer_fin(T *q) {
    int status;
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    status = he_f_meyer_fin(b->local);
    FREE(q);
    return status;
}
static int meyer_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real meyer_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_energy(b->local, he, x, y, z);
}
static int meyer_energy_ver(T *q, /**/ real **e) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_energy_ver(b->local, /**/ e);
}
static int meyer_area_ver(T *q, /**/ real **e) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_area_ver(b->local, /**/ e);
}
static int meyer_curva_mean_ver(T *q, /**/ real **e) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_curva_mean_ver(b->local, /**/ e);
}
static int meyer_norm_ver(T *q, /**/ real **e, real **f, real **g) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_norm_ver(b->local, /**/ e, f, g);
}
static int meyer_laplace_ver(T *q, /**/ real **e, real **f, real **g) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_laplace_ver(b->local, /**/ e, f, g);
}
static Vtable meyer_vtable = { meyer_fin, meyer_force, meyer_energy, meyer_energy_ver,
			       meyer_area_ver, meyer_curva_mean_ver, meyer_norm_ver, meyer_laplace_ver};
int bending_meyer_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Meyer *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &meyer_vtable;
    *pq = &q->bending;
    return he_f_meyer_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end meyer */

/* begin gompper_xin */
typedef struct GompperXin GompperXin;
struct GompperXin {T bending; HeFGompperXin *local; };
static int gompper_xin_fin(T *q) {
    int status;
    GompperXin *b = CONTAINER_OF(q, GompperXin, bending);
    status = he_f_gompper_xin_fin(b->local);
    FREE(q);
    return status;
}
static int gompper_xin_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    GompperXin *b = CONTAINER_OF(q, GompperXin, bending);
    return he_f_gompper_xin_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real gompper_xin_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    GompperXin *b = CONTAINER_OF(q, GompperXin, bending);
    return he_f_gompper_xin_energy(b->local, he, x, y, z);
}
static int gompper_xin_energy_ver(T *q, /**/ real **e) {
    GompperXin *b = CONTAINER_OF(q, GompperXin, bending);
    return he_f_gompper_xin_energy_ver(b->local, /**/ e);
}
static int gompper_xin_area_ver(T *q, /**/ real **e) {
    GompperXin *b = CONTAINER_OF(q, GompperXin, bending);
    return he_f_gompper_xin_area_ver(b->local, /**/ e);
}
static Vtable gompper_xin_vtable = { gompper_xin_fin, gompper_xin_force, gompper_xin_energy, gompper_xin_energy_ver, gompper_xin_area_ver };
int bending_gompper_xin_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    GompperXin *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &gompper_xin_vtable;
    *pq = &q->bending;
    return he_f_gompper_xin_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end gompper_xin */

/* begin meyer_xin */
typedef struct MeyerXin MeyerXin;
struct MeyerXin {T bending; HeFMeyerXin *local; };
static int meyer_xin_fin(T *q) {
    int status;
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    status = he_f_meyer_xin_fin(b->local);
    FREE(q);
    return status;
}
static int meyer_xin_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real meyer_xin_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_energy(b->local, he, x, y, z);
}
static int meyer_xin_energy_ver(T *q, /**/ real **e) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_energy_ver(b->local, /**/ e);
}
static int meyer_xin_area_ver(T *q, /**/ real **e) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_area_ver(b->local, /**/ e);
}
static int meyer_xin_curva_mean_ver(T *q, /**/ real **e) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_curva_mean_ver(b->local, /**/ e);
}
static int meyer_xin_norm_ver(T *q, /**/ real **e, real **f, real **g) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_norm_ver(b->local, /**/ e, f, g);
}
static int meyer_xin_laplace_ver(T *q, /**/ real **e, real **f, real **g) {
    MeyerXin *b = CONTAINER_OF(q, MeyerXin, bending);
    return he_f_meyer_xin_laplace_ver(b->local, /**/ e, f, g);
}
static Vtable meyer_xin_vtable = { meyer_xin_fin, meyer_xin_force, meyer_xin_energy, meyer_xin_energy_ver,
				   meyer_xin_area_ver, meyer_xin_curva_mean_ver, meyer_xin_norm_ver, meyer_xin_laplace_ver};
int bending_meyer_xin_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    MeyerXin *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &meyer_xin_vtable;
    *pq = &q->bending;
    return he_f_meyer_xin_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end meyer_xin */
