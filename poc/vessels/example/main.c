#include <stdlib.h>
#include <tiffio.h>

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
    TIFF *q;
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
    
    if ((q = TIFFOpen(argv[0], "r")) == NULL) {
	fprintf(stderr, "%s: fail to topen %s\n", me, argv[0]);
	exit(2);
    }
    TIFFClose(q);
}
