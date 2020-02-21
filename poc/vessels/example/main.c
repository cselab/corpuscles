#include <stdlib.h>
#include <tiffio.h>

const char *me = "main";

static void
usg(void)
{
  fprintf(stderr, "%s file\n", me);
  exit(2);
}
#define	COLOR_DEPTH	(8)
#define	B_DEPTH		(5)
#define	COLOR_SHIFT	(COLOR_DEPTH-B_DEPTH)

enum {ERROR  = 0, OK = 1};

int
main(int argc, char **argv)
{
    TIFF *tif;
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

    /*int dircount = 0;
    do {
	dircount++;
	} while (TIFFReadDirectory(tif)); */
    //printf("%d directories\n", dircount);
    
    
    int width;
    int length;
    int j;
    int row;
    int col;
    int scanline;
    unsigned char *inptr;
    tdata_t buf;
    int v;
    int red, green, blue;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length);
    scanline = TIFFScanlineSize(tif);
    buf = _TIFFmalloc(TIFFScanlineSize(tif));
    for (row = 0; row < length; row++) {
	TIFFReadScanline(tif, buf, row, 0);
	inptr = buf;
	for (j = 0; j < width; j++) {
	    red = (*inptr++) & 0xff >> COLOR_SHIFT;
	    green = (*inptr++) & 0xff >> COLOR_SHIFT;
	    blue = (*inptr++) & 0xff >> COLOR_SHIFT;
	    printf("%d\n", blue);
	}
    }
    _TIFFfree(buf);
    
    //printf("%d %ld\n", TIFFNumberOfStrips(tif), TIFFStripSize(tif));

    TIFFClose(tif);
}
