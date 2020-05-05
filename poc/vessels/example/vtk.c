#include <assert.h>
#include <stdlib.h>
#include <tiffio.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>

static const char *me = "vtk";
static const real spacing[] = { 1.6, 1.6, 1.0 };
static const real origin[] = {0, 0, 0};
static const real delta[][3] = {
    {-1, -1, -1},
    {-1, -1,  1},
    {-1,  1, -1},
    {-1,  1,  1},
    {1, -1, -1},
    {1, -1,  1},
    {1,  1, -1},
    {1,  1,  1},
};
static const int n_per_cell = sizeof delta / sizeof *delta;

static void
usg(void)
{
    fprintf(stderr, "%s [-v] [-s] file\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    enum { X, Y, Z };
    char *hostcomputer;
    double *ver;
    int dircount;
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int size[3];
    int Sparse;
    int Verbose;
    long *cells;
    long cnt;
    long *types;
    real *data;
    real origin[3];
    real x;
    real y;
    real z;
    tdata_t buf;
    TIFF *tif;
    tsize_t scanline;
    uint16 bitspersample;
    uint16 compression;
    uint16 photometric;
    uint16 samplesperpixel;
    uint32 length;
    uint32 width;
    unsigned char *inptr;

    USED(argc);
    Verbose = Sparse = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'v':
            Verbose = 1;
            break;
        case 's':
            Sparse = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    if (argv[0] == NULL) {
        fprintf(stderr, "%s: missing an argument\n", me);
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
        fprintf(stderr, "%s:%d: TIFFGetField failed\n", __FILE__,
                __LINE__);
        exit(2);
    };
    if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length) != 1) {
        fprintf(stderr, "%s:%d: TIFFGetField failed\n", __FILE__,
                __LINE__);
        exit(2);
    };
    if (TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample) != 1) {
        fprintf(stderr, "%s:%d: TIFFGetField failed\n", __FILE__,
                __LINE__);
        exit(2);
    };
    if (TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel) != 1) {
        fprintf(stderr, "%s:%d: TIFFGetField failed\n", __FILE__,
                __LINE__);
        exit(2);
    };
    if (TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression) != 1) {
        fprintf(stderr, "%s:%d: TIFFGetField failed\n", __FILE__,
                __LINE__);
        exit(2);
    }
    if (TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) != 1) {
        fprintf(stderr, "%s:%d: TIFFGetField failed\n", __FILE__,
                __LINE__);
        exit(2);
    }
    if (compression != 1) {
        fprintf(stderr, "%s: compression = %d is not supported\n", me,
                compression);
        exit(2);
    }
    if (bitspersample != 8 * sizeof *inptr) {
        fprintf(stderr, "%s: bitspersample = %d\n", me, bitspersample);
        exit(2);
    }
    if (samplesperpixel != 1) {
        fprintf(stderr, "%s: bitspersample = %d\n", me, samplesperpixel);
        exit(2);
    }
    if (photometric != 1) {
        fprintf(stderr, "%s: photometric = %d\n", me, photometric);
        exit(2);
    }
    size[X] = width;
    size[Y] = length;
    size[Z] = dircount;
    n = size[X] * size[Y] * size[Z];
    MALLOC(n, &data);
    scanline = TIFFScanlineSize(tif);
    if ((buf = _TIFFmalloc(scanline * sizeof *buf)) == NULL) {
        fprintf(stderr, "%s: _TIFFmalloc failed\n", me);
        exit(2);
    }
    l = 0;
    for (k = 0; k < dircount; k++) {
        if (TIFFSetDirectory(tif, k) != 1) {
            fprintf(stderr, "%s: TIFFSetDirectory(k=%d) failed\n", me, k);
            exit(2);
        }
        for (j = 0; j < size[Y]; j++) {
            if (TIFFReadScanline(tif, buf, j, 0) != 1) {
                fprintf(stderr, "%s: TIFFReadScanline failed\n", me);
                exit(2);
            }
            inptr = buf;
            for (i = 0; i < size[X]; i++)
                data[l++] = *inptr++;
        }
    }
    assert(n == l);
    _TIFFfree(buf);
    if (Verbose)
        fprintf(stderr, "size: %d %d %d\n", size[X], size[Y], size[Z]);

    if (Sparse) {
        cnt = 0;
        for (i = 0; i < n; i++) {
            if (data[i] > 0)
                cnt++;
        }
        if ((ver = malloc(3 * n_per_cell * cnt * sizeof *ver)) == NULL) {
            fprintf(stderr, "%s:%d: malloc failed\n", __FILE__, __LINE__);
            exit(2);
        }
        if ((cells = malloc(8 * n_per_cell * sizeof *cells)) == NULL) {
            fprintf(stderr, "%s:%d: malloc failed\n", __FILE__, __LINE__);
            exit(2);
        }
        if ((types = malloc(cnt * sizeof *types)) == NULL) {
            fprintf(stderr, "%s:%d: malloc failed\n", __FILE__, __LINE__);
            exit(2);
        }

        m = 0;
        for (k = 0; k < size[Z]; k++)
            for (j = 0; j < size[Y]; j++)
                for (i = 0; i < size[X]; i++)
                    if (data[i] > 0) {
                        x = origin[X] + spacing[X] * (i + 0.5);
                        y = origin[Y] + spacing[Y] * (j + 0.5);
                        z = origin[Z] + spacing[Z] * (k + 0.5);
                        for (l = 0; l < n_per_cell; l++) {
                            ver[m++] = x + delta[l][X] * spacing[X];
                            ver[m++] = y + delta[l][Y] * spacing[Y];
                            ver[m++] = y + delta[l][Z] * spacing[Z];
                        }
                    }
        fprintf(stderr, "%d/%d\n", cnt, n);
        fprintf(stderr, "%d\n", n_per_cell);

    } else
        if (vtk_grid_write(stdout, size, origin, spacing, data) != CO_OK) {
            fprintf(stderr, "%s: vtk_grid_write failed\n", me);
            exit(2);
        }
    FREE(data);
    TIFFClose(tif);
}
