#include <assert.h>
#include <stdlib.h>
#include <tiffio.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>

const char *me = "roi";
static const real spacing[] = { 1.6, 1.6, 1.0 };

static void
usg(void)
{
    fprintf(stderr, "%s -r int[6] file > vtk\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    enum { X, Y, Z };
    int dircount;
    int hi[3];
    int i;
    int j;
    int k;
    int l;
    int length;
    int lo[3];
    int n;
    int Rflag;
    int roi[3];
    int size[3];
    int width;
    real *data;
    real origin[3];
    tdata_t buf;
    TIFF *tif;
    tsize_t scanline;
    unsigned short *inptr;

    USED(argc);
    Rflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'r':
            if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL ||
                argv[4] == NULL || argv[5] == NULL || argv[6] == NULL) {
                fprintf(stderr, "%s: -r needs six arguments\n", me);
                exit(2);
            }
            lo[X] = atoi(*++argv);
            lo[Y] = atoi(*++argv);
            lo[Z] = atoi(*++argv);
            hi[X] = atoi(*++argv);
            hi[Y] = atoi(*++argv);
            hi[Z] = atoi(*++argv);
            Rflag = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    if (argv[0] == NULL) {
        fprintf(stderr, "%s: missing an argument\n", me);
        exit(2);
    }

    if (Rflag == 0) {
        fprintf(stderr, "%s: -r is not set\n", me);
        exit(2);
    }
    if (lo[X] >= hi[X]) {
        fprintf(stderr, "%s: lo[X]=%d >= hi[X]=%d\n", me, lo[X], hi[X]);
        exit(2);
    }
    if (lo[Y] >= hi[Y]) {
        fprintf(stderr, "%s: lo[Y]=%d >= hi[Y]=%d\n", me, lo[Y], hi[Y]);
        exit(2);
    }
    if (lo[Z] >= hi[Z]) {
        fprintf(stderr, "%s: lo[Z]=%d >= hi[Z]=%d\n", me, lo[Z], hi[Z]);
        exit(2);
    }

    if ((tif = TIFFOpen(argv[0], "r")) == NULL) {
        fprintf(stderr, "%s: fail to open %s\n", me, argv[0]);
        exit(2);
    }

    dircount = 0;
    do
        dircount++;
    while (TIFFReadDirectory(tif));

    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) != 1) {
        fprintf(stderr, "%s: TIFFGetField failed\n", me);
        exit(2);
    };
    if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length) != 1) {
        fprintf(stderr, "%s: TIFFGetField failed\n", me);
        exit(2);
    };
    size[X] = width;
    size[Y] = length;
    size[Z] = dircount;

    roi[X] = hi[X] - lo[X];
    roi[Y] = hi[Y] - lo[Y];
    roi[Z] = hi[Z] - lo[Z];

    origin[X] = lo[X] * spacing[X];
    origin[Y] = lo[Y] * spacing[Y];
    origin[Z] = lo[Z] * spacing[Z];

    n = roi[X] * roi[Y] * roi[Z];
    MALLOC(n, &data);
    scanline = TIFFScanlineSize(tif);
    if ((buf = _TIFFmalloc(scanline)) == NULL) {
        fprintf(stderr, "%s: _TIFFmalloc failed failed\n", me);
        exit(2);
    }
    l = 0;
    for (k = 0; k < size[Z]; k++) {
        if (lo[Z] > k || k >= hi[Z])
            continue;
        if (TIFFSetDirectory(tif, k) != 1) {
            fprintf(stderr, "%s: TIFFSetDirectory(k=%d) failed\n", me, k);
            exit(2);
        }
        for (j = 0; j < size[Y]; j++) {
            if (lo[Y] > j || j >= hi[Y])
                continue;
            if (TIFFReadScanline(tif, buf, j, 0) != 1) {
                fprintf(stderr, "%s: TIFFReadScanline failed\n", me);
                exit(2);
            }
            inptr = buf;
            for (i = 0; i < size[X]; i++) {
                if (lo[X] > i || i >= hi[X])
                    continue;
                data[l++] = inptr[i];
            }
        }
    }
    assert(l == n);
    _TIFFfree(buf);
    if (vtk_grid_write(stdout, roi, origin, spacing, data) != CO_OK) {
        fprintf(stderr, "%s: vtk_grid_write failed\n", me);
        exit(2);
    }
    FREE(data);
    TIFFClose(tif);
}
