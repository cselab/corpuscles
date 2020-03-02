#include <stdlib.h>
#include <tiffio.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>

const char *me = "dir";

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

    int status;
    dircount = 0;
    do
	dircount++;
    while (TIFFReadDirectory(tif));
    printf("%d\n", dircount);

    status = TIFFSetDirectory(tif, 0);
    printf("status: %d\n", status);
    printf("offset: %ld\n", TIFFCurrentDirOffset(tif));

    status = TIFFSetDirectory(tif, 1);
    printf("status: %d\n", status);
    printf("offset: %ld\n", TIFFCurrentDirOffset(tif));

    status = TIFFSetDirectory(tif, 2);
    printf("status: %d\n", status);
    printf("offset: %ld\n", TIFFCurrentDirOffset(tif));

    status = TIFFSetDirectory(tif, 3);
    printf("status: %d\n", status);
    printf("offset: %ld\n", TIFFCurrentDirOffset(tif));    

    TIFFClose(tif);
}
