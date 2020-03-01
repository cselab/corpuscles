#include <assert.h>
#include <stdlib.h>
#include <tiffio.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>

const char *me = "vtk";

static void
usg(void)
{
    fprintf(stderr, "%s file\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    enum { X, Y, Z };
    int dircount;
    int i;
    int j;
    int k;
    int l;
    int length;
    int n;
    int size[3];
    int lo[3];
    int hi[3];
    int roi[3];
    int width;
    real *data;
    unsigned short *inptr;
    int x;
    real origin[3];
    real spacing[3];
    tdata_t buf;
    tsize_t scanline;
    TIFF *tif;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
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
        fprintf(stderr, "%s: fail to topen %s\n", me, argv[0]);
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
    origin[X] = origin[Y] = origin[Z] = 0;
    spacing[X] = spacing[Y] = 1.6;
    spacing[Z] = 1.0;

    lo[X] = lo[Y] = 974;
    lo[Z] = 191;
    
    hi[X] = hi[Y] = 1074;
    hi[Z] = 291;
    
    roi[X] = hi[X] - lo[X];
    roi[Y] = hi[Y] - lo[Y];
    roi[Z] = hi[Z] - lo[Z];

    n = roi[X] * roi[Y] * roi[Z];
    MALLOC(n, &data);
    MSG("roi: %d %d %d", roi[X], roi[Y], roi[Z]);

    scanline = TIFFScanlineSize(tif);
    if ((buf = _TIFFmalloc(scanline)) == NULL) {
        fprintf(stderr, "%s: _TIFFmalloc failed failed\n", me);
        exit(2);
    }
    MSG("scanline: %d", scanline);
    l = 0;
    for (k = 0; k < size[Z]; k++) {
	if (lo[Z] > k || k >= hi[Z]) continue;
        if (TIFFSetDirectory(tif, k) != 1) {
            fprintf(stderr, "%s: TIFFSetDirectory(k=%d) failed\n", me, k);
            exit(2);
        }
        for (j = 0; j < size[Y]; j++) {
	    if (lo[Y] > j || j >= hi[Y]) continue;
            if (TIFFReadScanline(tif, buf, j, 0) != 1) {
                fprintf(stderr, "%s: TIFFReadScanline failed\n", me);
                exit(2);
            }
            inptr = buf;
            for (i = 0; i < size[X]; i++) {
		if (lo[X] > i || i >= hi[X]) continue;		
                data[l++] = inptr[i];
            }
        }
    }
    assert(l == n);
    _TIFFfree(buf);
    if (vtk_grid(stdout, roi, origin, spacing, data) != CO_OK) {
        fprintf(stderr, "%s: vtk_grid failed\n", me);
        exit(2);
    }
    FREE(data);
    TIFFClose(tif);
}
