#include <stdlib.h>
#include <stdio.h>
#include <co/err.h>
#include <co/macro.h>

const char *me = "byte2";

static int16_t to_int16l(int, int);
static int16_t to_int16b(int, int);
static int32_t to_int32l(int, int, int, int);
static int32_t to_int32b(int, int, int, int);
static int type2name(int, const char **);
enum {
    BYTE = 1,
    ASCII = 2,
    SHORT = 3,
    LONG = 4,
    RATIONAL = 5,
    SBYTE = 6,
    UNDEFINED = 7,
    SSHORT = 8,
    SLONG = 9,
    SRATIONAL = 10,
    FLOAT = 11,
    DOUBLE = 12,
};

static int Type[] = {
    BYTE,
    ASCII,
    SHORT,
    LONG,
    RATIONAL,
    SBYTE,
    UNDEFINED,
    SSHORT,
    SLONG,
    SRATIONAL,
    FLOAT,
    DOUBLE,
};

static const char *Name[] = {
    "BYTE",
    "ASCII",
    "SHORT",
    "LONG",
    "RATIONAL",
    "SBYTE",
    "UNDEFINED",
    "SSHORT",
    "SLONG",
    "SRATIONAL",
    "FLOAT",
    "DOUBLE",
};

enum {
    NewSubfileType = 254,
    ImageWidth = 256,
    ImageLength = 257,
    BitsPerSample = 258,
    PhotometricInterpretation = 262,
};

static int Tag[] = {
    NewSubfileType,
    ImageWidth,
    ImageLength,
    BitsPerSample,
    PhotometricInterpretation,
};

static const char *TagName[] = {
    "NewSubfileType",
    "ImageWidth",
    "ImageLength",
    "BitsPerSample",
    "PhotometricInterpretation",
};

#define NXT(c)						\
  do {							\
    if ((*c = fgetc(f)) == EOF) {			\
      fprintf(stderr, "%s: fail to read a byte\n", me); \
      exit(2);						\
    }							\
  } while(0)

static void
usg(void)
{
    fprintf(stderr, "%s file\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    FILE *f;
    int16_t count;
    int16_t magic;

    int16_t(*to_int16) (int, int);
    int32_t offset;

    int32_t(*to_int32) (int, int, int, int);
    int i;
    int w;
    int x;
    int y;
    int z;
    int16_t tag;
    int16_t type;
    int32_t entry_count;
    int32_t entry_offset;
    const char *name;

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
    if ((f = fopen(argv[0], "r")) == NULL) {
        fprintf(stderr, "%s: fail to open %s\n", me, argv[0]);
        exit(2);
    }
    NXT(&x);
    NXT(&y);
    if (x == 0x4D && y == 0x4D) {
        fprintf(stderr, "%s: big\n", me);
        to_int16 = to_int16b;
        to_int32 = to_int32b;
    } else if (x == 0x49 && y == 0x49) {
        fprintf(stderr, "%s: little\n", me);
        to_int16 = to_int16l;
        to_int32 = to_int32l;
    } else {
        fprintf(stderr, "%s: unexpected bytes\n", me);
        fprintf(stderr, "%s: %X %X\n", me, x, y);
        exit(2);
    }
    NXT(&x);
    NXT(&y);
    magic = to_int16(x, y);
    if (magic != 42) {
        fprintf(stderr, "%s: no magic => not a tif file\n", me);
        exit(2);
    }
    NXT(&x);
    NXT(&y);
    NXT(&z);
    NXT(&w);
    offset = to_int32(x, y, z, w);
    if (fseek(f, offset, SEEK_SET) != 0) {
        fprintf(stderr, "%s: fseek failed for offset = %d\n", me, offset);
        exit(2);
    }
    NXT(&x);
    NXT(&y);
    count = to_int16(x, y);
    for (i = 0; i < count; i++) {
        NXT(&x);
        NXT(&y);
        tag = to_int16(x, y);
        printf("tag: %d\n", tag);
        NXT(&x);
        NXT(&y);
        type = to_int16(x, y);
        if (type2name(type, &name) != 0) {
            fprintf(stderr, "%s: unknown type: %d\n", me, type);
            exit(2);
        }
        //printf("type(%d): %s\n", type, name);
        NXT(&x);
        NXT(&y);
        NXT(&z);
        NXT(&w);
        entry_count = to_int32(x, y, z, w);
        NXT(&x);
        NXT(&y);
        NXT(&z);
        NXT(&w);
        //entry_offset = to_int32(x, y, z, w);
	entry_offset = to_int16(x, y);

        if (tag == BitsPerSample) {
            printf("offset: %d\n", entry_offset);
            printf("count: %d\n", entry_count);
            printf("type: %s\n", name);
        }
    }
    fclose(f);
}

static int16_t
to_int16l(int x, int y)
{
    return (x << 0) | (y << 8);
}

static int16_t
to_int16b(int x, int y)
{
    return (y << 0) | (x << 8);
}

static int32_t
to_int32l(int x, int y, int z, int w)
{
    return (x << 0) | (y << 8) | (z << 16) | (w << 24);
}

static int32_t
to_int32b(int x, int y, int z, int w)
{
    return (w << 0) | (z << 8) | (y << 16) | (x << 24);
}

static int
type2name(int type, const char **p)
{
    int i;
    int n;

    n = sizeof(Type) / sizeof(Type[0]);
    for (i = 0; i < n; i++)
        if (type == Type[i]) {
            *p = Name[i];
            return 0;
        }
    return 1;
}
