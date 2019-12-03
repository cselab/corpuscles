#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/colormap.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/povray.h"

#define FMT CO_REAL_OUT
#define TAB "  "

int
povray_mesh2(He * he, const real * x, const real * y, const real * z,
	     FILE * f)
{
    int nv, nt;
    int i;
    int a, b, c, status;
    real *nx, *ny, *nz;

    nv = he_nv(he);
    nt = he_nt(he);
    MALLOC3(nv, &nx, &ny, &nz);
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
      ERR(CO_NUM, "normal_mwa failed");
    if (fprintf(f, "vertex_vectors {\n") < 0)
	ERR(CO_IO, "fail to write");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, ",\n");
	fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", x[i], y[i], z[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "normal_vectors {\n");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, ",\n");
	fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", nx[i], ny[i], nz[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "face_indices {\n");
    fprintf(f, TAB "%d", nt);
    for (i = 0; i < nt; i++) {
	fprintf(f, ",\n");
	he_tri_ijk(he, i, &a, &b, &c);
	fprintf(f, TAB "<%d %d %d>", a, b, c);
    }
    fprintf(f, "\n}\n");
    FREE3(nx, ny, nz);
    return CO_OK;
}

int
povray_tri_mesh2(He * he, const real * x, const real * y, const real * z,
		 const real *color,
		 FILE * f)
{
    int nv, nt;
    int i;
    int a, b, c, status;
    real *nx, *ny, *nz, lo, hi;
    float red, blue, green;

    nv = he_nv(he);
    nt = he_nt(he);
    lo = array_min(nt, color);
    hi = array_max(nt, color);

    MALLOC3(nv, &nx, &ny, &nz);
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
      ERR(CO_NUM, "normal_mwa failed");
    if (fprintf(f, "vertex_vectors {\n") < 0)
	ERR(CO_IO, "fail to write");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, ",\n");
	fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", x[i], y[i], z[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "normal_vectors {\n");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, ",\n");
	fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", nx[i], ny[i], nz[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "texture_list {\n");
    fprintf(f, TAB "%d,", nt);
    for (i = 0; i < nt; i++) {
	fprintf(f, "\n");
	colormap(color[i], lo, hi, /**/ &red, &green, &blue);
	fprintf(f, TAB "texture{pigment{rgb <%g,%g,%g>}}", red, green, blue);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "face_indices {\n");
    fprintf(f, TAB "%d", nt);
    for (i = 0; i < nt; i++) {
	fprintf(f, ",\n");
	he_tri_ijk(he, i, &a, &b, &c);
	fprintf(f, TAB "<%d %d %d>, %d", a, b, c, i);
    }
    fprintf(f, "\n}\n");
    FREE3(nx, ny, nz);
    return CO_OK;
}

int
povray_lh_ver_mesh2(He * he, const real * x, const real * y, const real * z, real lo, real hi, const real *color, FILE * f)
{
    int nv, nt;
    int i;
    int a, b, c, status;
    real *nx, *ny, *nz;
    float red, blue, green;

    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC3(nv, &nx, &ny, &nz);
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
      ERR(CO_NUM, "normal_mwa failed");
    if (fprintf(f, "vertex_vectors {\n") < 0)
	ERR(CO_IO, "fail to write");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, ",\n");
	fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", x[i], y[i], z[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "normal_vectors {\n");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, ",\n");
	fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", nx[i], ny[i], nz[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "texture_list {\n");
    fprintf(f, TAB "%d,", nv);
    for (i = 0; i < nv; i++) {
	fprintf(f, "\n");
	colormap(color[i], lo, hi, /**/ &red, &green, &blue);
	fprintf(f, TAB "CoTexture(%.6g, %.6g, %.6g)",
		red, green, blue);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "face_indices {\n");
    fprintf(f, TAB "%d", nt);
    for (i = 0; i < nt; i++) {
	fprintf(f, ",\n");
	he_tri_ijk(he, i, &a, &b, &c);
	fprintf(f, TAB "<%d %d %d>, %d, %d, %d", a, b, c, a, b, c);
    }
    fprintf(f, "\n}\n");
    FREE3(nx, ny, nz);
    return CO_OK;
}

int
povray_ver_mesh2(He * he, const real * x, const real * y, const real * z, const real *color, FILE * f) {
    int nv;
    real lo, hi;
    nv = he_nv(he);
    lo = array_min(nv, color);
    hi = array_max(nv, color);
    return povray_lh_ver_mesh2(he, x, y, z, lo, hi, color, f);
}
