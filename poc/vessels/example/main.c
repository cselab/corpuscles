#include <stdlib.h>
#include <tiffio.h>
#include <co/macro.h>

const char *me = "main";

static void
usg(void)
{
    fprintf(stderr, "%s file\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    int x;
    int j;
    int length;
    int row;
    int scanline;
    int width;
    tdata_t buf;
    TIFF *tif;
    unsigned char *inptr;

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


    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length);
    scanline = TIFFScanlineSize(tif);
    buf = _TIFFmalloc(scanline);
    for (row = 0; row < length; row++) {
        TIFFReadScanline(tif, buf, row, 0);
        inptr = buf;
        for (j = 0; j < width; j++) {
            x = *inptr++;
            printf("%d\n", x);
        }
    }
    _TIFFfree(buf);

    //printf("%d %ld\n", TIFFNumberOfStrips(tif), TIFFStripSize(tif));

    TIFFClose(tif);
}
