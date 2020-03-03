#include <stdlib.h>
#include <tiffio.h>
#include <co/macro.h>

const char *me = "info";

static void
usg(void)
{
    fprintf(stderr, "%s file\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    const TIFFField *field;
    int count;
    int dircount;
    int i;
    TIFF *tif;
    uint32 tag;
    void *data;

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
    printf("dircount: %d\n", dircount);
    count = TIFFGetTagListCount(tif);
    printf("tag count: %d\n", count);

    for (i = 0; i < count; i++) {
        tag = TIFFGetTagListEntry(tif, i);
        if (TIFFGetField(tif, tag, &data) != 1) {
            fprintf(stderr, "%s: TIFFGetField failed (tag = %d)\n", me,
                    tag);
            exit(2);
        }
        field = TIFFFieldWithTag(tif, tag);
        if (field == NULL) {
            fprintf(stderr, "%s: TIFFFieldWithTag failed (tag = %d)\n", me,
                    tag);
            exit(2);
        }
        printf("  tag: %d\n", tag);
        printf("  content:\n");
        printf("%s", (char *) data);
        printf("\n");
    }
    TIFFClose(tif);
}
