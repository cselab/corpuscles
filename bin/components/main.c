#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/multi.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/ply.h>
#include <co/y.h>

static const char *me = "co.components";
static void
usg(void)
{
    fprintf(stderr, "color connected components\n"
	    "%s [-s] < OFF > OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    He *he;
    int c;
    int *cnt;
    int *cumsum;
    int i;
    int j;
    int k;
    int *label;
    int *map;
    int *rmap;
    int nc;
    int nt;
    int nv;
    int s;
    int Split;
    int t;
    int *tri;
    int v;
    int *vcnt;
    int *vcumsum;
    int *vcumsum0;
    int *vlabel;
    int *vmap;
    real *color;
    real *x;
    real *xyz;
    real *y;
    real *z;
    int *ii;
    int *jj;
    int *kk;
    FILE *file;
    char path[FILENAME_MAX];

    Split = 0;
    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	case 's':
	    Split = 1;
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(1);
	}
    err_set_ignore();
    if (y_inif(stdin, &he, &x, &y, &z) != CO_OK) {
	fprintf(stderr, "%s: fail to read OFF file\n", me);
	exit(1);
    }
    nt = he_nt(he);
    MALLOC(nt, &label);
    if (he_multi_label(he, &nc, label) != CO_OK) {
	fprintf(stderr, "%s: he_multi_label failed\n", me);
	exit(1);
    }

    if (Split) {
      nv = he_nv(he);
      CALLOC(nc, &cumsum);
      CALLOC(nc, &vcumsum);
      CALLOC(nc, &vcumsum0);
      CALLOC(nc, &cnt);
      CALLOC(nc, &vcnt);
      MALLOC(nt, &map);
      MALLOC(nv, &vlabel);
      MALLOC(nv, &vmap);
      MALLOC(nv, &rmap);

      for (t = 0; t < nt; t++) {
	cnt[label[t]]++;
	he_tri_ijk(he, t, &i, &j, &k);
	vlabel[i] = label[t];
	vlabel[j] = label[t];
	vlabel[k] = label[t];
      }
      for (i = 0; i < nv; i++)
	vcnt[vlabel[i]]++;
      for (i = s = 0; i < nc; i++) {
	cumsum[i] = s;
	s += cnt[i];
      }
      for (i = s = 0; i < nc; i++) {
	vcumsum0[i] = vcumsum[i] = s;
	s += vcnt[i];
      }
      for (t = 0; t < nt; t++)
	map[cumsum[label[t]]++] = t;
      for (i = 0; i < nv; i++) {
	vmap[vcumsum[vlabel[i]]] = i;
	rmap[i] = vcumsum[vlabel[i]] - vcumsum0[vlabel[i]];
	vcumsum[vlabel[i]]++;
      }

      v = 0;
      t = 0;
      he_T(he, &ii, &jj, &kk);
      for (c = 0; c < nc; c++) {
	MALLOC(3 * cnt[c], &tri);
	MALLOC(3 * vcnt[c], &xyz);

	for (j = 0; j < cnt[c]; t++, j++) {
	  tri[3 * j] = rmap[ii[map[t]]];
	  tri[3 * j + 1] = rmap[jj[map[t]]];
	  tri[3 * j + 2] = rmap[kk[map[t]]];
	}

	for (i = 0; i < vcnt[c]; v++, i++) {
	  xyz[3 * i] = x[vmap[v]];
	  xyz[3 * i + 1] = y[vmap[v]];
	  xyz[3 * i + 2] = z[vmap[v]];
	}
	sprintf(path, "%09d.off", c);
	file = fopen(path, "w");
	off_xyz_tri_fwrite(vcnt[c], xyz, cnt[c], tri, file);
	fclose(file);
	FREE(xyz);
	FREE(tri);
      }

      FREE(cnt);
      FREE(cumsum);
      FREE(map);
      FREE(vcnt);
      FREE(vcumsum);
      FREE(vcumsum0);
      FREE(vlabel);
      FREE(vmap);
      FREE(rmap);
    } else {
      MALLOC(nt, &color);
      for (i = 0; i < nt; i++)
	color[i] = label[i];
      if (boff_tri_fwrite(he, x, y, z, color, stdout) != CO_OK) {
	fprintf(stderr, "%s: fail to write OFF file\n", me);
	exit(1);
      }
      fprintf(stderr, "%d\n", nc);
      FREE(color);
    }
    FREE(label);
    if (y_fin(he, x, y, z) != CO_OK)
      fprintf(stderr, "%s: y_fin failed\n", me);
}
