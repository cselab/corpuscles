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
    int length;
    int dircount;
    int scanline;
    int width;
    tdata_t buf;
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
    scanline = TIFFScanlineSize(tif);
    buf = _TIFFmalloc(scanline);
    if ((buf = _TIFFmalloc(scanline)) == NULL) {
        fprintf(stderr, "%s: _TIFFmalloc failed failed\n", me);
        exit(2);
    }


    _TIFFfree(buf);

    //printf("%d %ld\n", TIFFNumberOfStrips(tif), TIFFStripSize(tif));

    TIFFClose(tif);
}
