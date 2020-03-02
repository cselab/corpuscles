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
    int dircount;
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
    printf("dircount: %d\n", dircount);
    //TIFFPrintDirectory(tif, stdout, TIFFPRINT_NONE);

    int count;
    uint32 tag;
    int i;
    void *data;
    const TIFFField *field;

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
