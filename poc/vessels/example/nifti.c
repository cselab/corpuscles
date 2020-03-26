#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>
#include "nifti1.h"

enum {NII_HEADER_SIZE = 352};

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
  int ret;
  nifti_1_header hdr;
  long offset;
  const char *path;
  FILE *file;
  real *field;
  int nx;
  int ny;
  int nz;
  int i;

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

    ret = fread(&hdr, NII_HEADER_SIZE, 1, file);
    if (ret != 1) {
      fprintf(stderr, "%s: fail to read header\n", me);
      exit(1);
    }

    offset = hdr.vox_offset;
    fprintf(stderr, "XYZT dimensions: %d %d %d %d\n", hdr.dim[1], hdr.dim[2], hdr.dim[3], hdr.dim[4]);
    fprintf(stderr, "Datatype code and bits/pixel: %d %d\n", hdr.datatype, hdr.bitpix);
    fprintf(stderr, "Scaling slope and intercept: %.6f %.6f\n", hdr.scl_slope, hdr.scl_inter);
    fprintf(stderr, "Offset to data in datafile: %ld\n", offset);
    fprintf(stderr, "datatype: %d\n", hdr.datatype);

    if (fseek(file, offset, SEEK_SET) != 0) {
	fprintf(stderr, "%s: fseek failed for offset = %u\n", me, offset);
	exit(2);
    }

    nx = hdr.dim[1];
    ny = hdr.dim[2];
    nz = hdr.dim[3];
    if ((field = malloc(sizeof(field) * nx * ny * nz)) == NULL) {
      fprintf(stderr, "%s:%d malloc failed\n", __FILE__, __LINE__);
      exit(2);
    }

    unsigned char *data_uchar;
    double *data_double;
    switch (hdr.datatype) {
    case NIFTI_TYPE_UINT8:
      if ((data_uchar = malloc(sizeof(*data_uchar) * nx * ny * nz)) == NULL) {
	fprintf(stderr, "%s:%d malloc failed\n", __FILE__, __LINE__);
	exit(2);
      }
      if (fread(data_uchar, sizeof(*data_uchar), nx * ny * nz, file) != nx * ny * nz) {
	fprintf(stderr, "%s: fread failed\n", me);
	exit(2);
      }
      for (i = 0; i < nx * ny * nz; i++)
	field[i] = data_uchar[i];
      free(data_uchar);
      break;
    case NIFTI_TYPE_FLOAT64:
      if ((data_double = malloc(sizeof(*data_double) * nx * ny * nz)) == NULL) {
	fprintf(stderr, "%s:%d malloc failed\n", __FILE__, __LINE__);
	exit(2);
      }
      if (fread(data_double, sizeof(*data_double), nx * ny * nz, file) != nx * ny * nz) {
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
    const int size[3] = {nx, ny, nz};
    const real origin[3] = {0, 0, 0};
    const real spacing[3] = {1, 1, 1};
    if (vtk_grid_write(stdout, size, origin, spacing, field) != CO_OK) {
      fprintf(stderr, "%s: vtk_grid_write failed\n", me);
      exit(2);
    }
    free(field);
}
