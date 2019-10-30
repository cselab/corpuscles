#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"

#include "co/err.h"
#include "co/he.h"
#include "co/util.h"
#include "inc/def.h"

#include "co/obj.h"

#define OUT CO_REAL_OUT

int
obj_fwrite(He * he, const real * x, const real * y, const real * z,
           /**/ FILE * f)
{
  int nv, nt, v, t, i, j, k;

  nv = he_nv(he);
  nt = he_nt(he);

  if (fputs("# File type: ASCII OBJ\n", f) == EOF)
    ERR(CO_IO, "file to write");

  for (v = 0; v < nv; v++)
    fprintf(f, "v " OUT " " OUT " " OUT "\n", x[v], y[v], z[v]);

  for (t = 0; t < nt; t++) {
    he_tri_ijk(he, t, &i, &j, &k);
    fprintf(f, "f %d %d %d\n", i + 1, j + 1, k + 1);
  }
  return CO_OK;
}
