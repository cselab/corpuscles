#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>
#include "nifti1.h"

enum { NII_HEADER_SIZE = 348 };

static char me[] = "nifti2";
static void
usg(void)
{
    fprintf(stderr, "%s < in > out \n", me);
    exit(1);
}

int
main(int argc, char **argv)
{
  (void)argc;
    const char *path;
    FILE *file;
    int version;
    int i;
    int nx;
    int ny;
    int nz;
    long offset;
    nifti_1_header h;
    real *field;

    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(2);
	}
    if ((path = argv[0]) == NULL) {
	fprintf(stderr, "%s: missing file path\n", me);
	exit(2);
    }
    file = fopen(argv[0], "r");
    if (file == NULL) {
	fprintf(stderr, "%s: fail to open\n", path);
	exit(2);
    }

    if (fread(&h, NII_HEADER_SIZE, 1, file) != 1) {
	fprintf(stderr, "%s: fail to read header\n", me);
	exit(1);
    }

    version = NIFTI_VERSION(h);
    if (version == 0) {
	fprintf(stderr, "%s: not nifti file\n", me);
	exit(1);
    }
    if (version != 1) {
      fprintf(stderr, "%s: does not work for version=%d\n", me, version);
      exit(2);
    }

    if (h.sizeof_hdr != NII_HEADER_SIZE) {
      fprintf(stderr, "%s: wrong sizeof_hdr=%d\n", me, h.sizeof_hdr);
      exit(2);
    }

    if (h.dim[0] != 3) {
      fprintf(stderr, "%s: wrong number of dimensions '%d'\n", me, h.dim[0]);
      exit(2);
    }

    offset = h.vox_offset;
    fprintf(stderr, "XYZ dimensions: %d %d %d\n", h.dim[1],
	    h.dim[2], h.dim[3]);
    fprintf(stderr, "Datatype code and bits/pixel: %d %d\n", h.datatype,
	    h.bitpix);
    fprintf(stderr, "Scaling slope and intercept: %.6f %.6f\n",
	    h.scl_slope, h.scl_inter);
    fprintf(stderr, "pixdim: %g %g %g %g\n", h.pixdim[0], h.pixdim[1], h.pixdim[2], h.pixdim[3]);
    fprintf(stderr, "xyzt_units: %d\n", h.xyzt_units);
    fprintf(stderr, "datatype: %d\n", h.datatype);

    if (fseek(file, offset, SEEK_SET) != 0) {
	fprintf(stderr, "%s: fseek failed for offset = %ld\n", me, offset);
	exit(2);
    }

    nx = h.dim[1];
    ny = h.dim[2];
    nz = h.dim[3];
    if ((field = malloc(sizeof(field) * nx * ny * nz)) == NULL) {
	fprintf(stderr, "%s:%d malloc failed\n", __FILE__, __LINE__);
	exit(2);
    }

    unsigned char *data_uchar;
    double *data_double;

    switch (h.datatype) {
    case NIFTI_TYPE_UINT8:
        if (h.bitpix != 8*sizeof(*data_uchar)) {
	  fprintf(stderr, "%s: wrong bitpix = %d != %ld\n", me, h.bitpix, 8*sizeof(*data_uchar));
	  exit(2);
        }
	if ((data_uchar =
	     malloc(sizeof(*data_uchar) * nx * ny * nz)) == NULL) {
	    fprintf(stderr, "%s:%d malloc failed\n", __FILE__, __LINE__);
	    exit(2);
	}
	if (fread(data_uchar, sizeof(*data_uchar), nx * ny * nz, file) !=
	    (size_t)nx * ny * nz) {
	    fprintf(stderr, "%s: fread failed\n", me);
	    exit(2);
	}
	for (i = 0; i < nx * ny * nz; i++)
	    field[i] = data_uchar[i];
	free(data_uchar);
	break;
    case NIFTI_TYPE_FLOAT64:
        if (h.bitpix != 8*sizeof(*data_double)) {
	  fprintf(stderr, "%s: wrong bitpix = %d != %ld\n", me, h.bitpix, 8*sizeof(*data_double));
	  exit(2);
        }
	if ((data_double =
	     malloc(sizeof(*data_double) * nx * ny * nz)) == NULL) {
	    fprintf(stderr, "%s:%d malloc failed\n", __FILE__, __LINE__);
	    exit(2);
	}
	if (fread(data_double, sizeof(*data_double), nx * ny * nz, file) !=
	    (size_t)nx * ny * nz) {
	    fprintf(stderr, "%s: fread failed\n", me);
	    exit(2);
	}
	for (i = 0; i < nx * ny * nz; i++)
	    field[i] = data_double[i];
	free(data_double);
	break;
    default:
	fprintf(stderr, "%s: unsupported datatype\n", me);
	exit(2);
    }
    const int size[3] = { nx, ny, nz };
    const real origin[3] = { 0, 0, 0 };
    const real spacing[3] = { 1, 1, 1 };
    if (vtk_grid_write(stdout, size, origin, spacing, field) != CO_OK) {
	fprintf(stderr, "%s: vtk_grid_write failed\n", me);
	exit(2);
    }
    free(field);
}
