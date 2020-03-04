#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <co/macro.h>

const char *me = "images";

/* magic tag from ImageJ */
enum { TAG = 270 };

static void
usg(void)
{
    fprintf(stderr, "%s file\n", me);
    exit(2);
}

struct Meta {
    char *key[999];
    char *val[999];
};
static int meta_ini(TIFF * tif, struct Meta *q);
static int meta_fin(struct Meta *q);
static int meta_get(struct Meta *q, const char *, char **);

int
main(int argc, char **argv)
{
    struct Meta meta;
    int images;
    TIFF *tif;
    char *val;

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
    meta_ini(tif, &meta);
    meta_get(&meta, "images", &val);
    if (val == NULL) {
        fprintf(stderr, "%s: no 'images' in meta\n", me);
        exit(2);
    }
    images = atoi(val);
    printf("%d\n", images);
    meta_fin(&meta);
    TIFFClose(tif);
}

static int
meta_fin(struct Meta *q)
{
    int i;
    for (i = 0; q->key[i] != NULL; i++) {
        free(q->key[i]);
        free(q->val[i]);
    }
    return 0;
}

static int
meta_get(struct Meta *q, const char *key, char **val)
{
    int i;

    for (i = 0; q->key[i] != NULL; i++) {
        if (!strcmp(q->key[i], key)) {
            *val = q->val[i];
            return 0;
        }
    }
    *val = NULL;
    return 0;
}

static int
meta_ini(TIFF * tif, struct Meta *q)
{
    char *str;
    char *token;
    char *x;
    const char *s = "\n";
    int i;
    void *data;

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
    q->val[i] = q->key[i] = NULL;
    while (token != NULL) {
        x = strchr(token, '=');
        *x = '\0';
        q->val[i] = strdup(x + 1);
        q->key[i] = strdup(token);
        q->val[i + 1] = q->key[i + 1] = NULL;
        i++;
        token = strtok(NULL, s);
    }
    return 0;
}
