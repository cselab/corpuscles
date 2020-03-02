#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <co/macro.h>

const char *me = "imagej";

/* magic tag from ImageJ */
enum { TAG = 270 };

static void
usg(void)
{
    fprintf(stderr, "%s file\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    char *key[999] = { NULL };
    char *str;
    char *token;
    char *val[999] = { NULL };
    char *x;
    const char *s = "\n";
    const TIFFField *field;
    int count;
    int i;
    TIFF *tif;
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
        fprintf(stderr, "%s: fail to topen %s\n", me, argv[0]);
        exit(2);
    }
    if (TIFFGetField(tif, TAG, &data) != 1) {
        fprintf(stderr, "%s: TIFFGetField failed\n", me);
        exit(2);
    }
    if (TIFFFieldWithTag(tif, TAG) == NULL) {
        fprintf(stderr, "%s: TIFFFieldWithTag failed\n", me);
        exit(2);
    }
    str = (char *) data;
    token = strtok(str, s);
    i = 0;
    while (token != NULL) {
        x = strchr(token, '=');
        *x = '\0';
        val[i] = strdup(x + 1);
        key[i] = strdup(token);
        i++;
        token = strtok(NULL, s);
    }
    for (i = 0; key[i] != NULL; i++)
        printf("%s: %s\n", key[i], val[i]);
    for (i = 0; key[i] != NULL; i++) {
        free(key[i]);
        free(val[i]);
    }

    TIFFClose(tif);
}
