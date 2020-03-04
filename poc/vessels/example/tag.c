#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "def.h"

const char *me = "tag";

static uint16_t to_int16l(int, int);
static uint16_t to_int16b(int, int);
static uint32_t to_int32l(int, int, int, int);
static uint32_t to_int32b(int, int, int, int);
static int type2name(int, const char **);
static int tag2name(int, const char **);
static int tag2type(int type, int *);
static long file_end(FILE *);

#define	USED(x)		if(x);else{}
#define FREAD(n, p)							\
    do									\
	if (fread(p, sizeof(*(p)), (n), (f)) != (n)) {			\
	    fprintf(stderr, "%s: fread failed, n = %d\n", me, n);	\
	    exit(2);							\
	}								\
    while (0)

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
    char string[9999];
    const char *name;
    int i;
    int image_length;
    int image_width;
    int j;
    int k;
    int rows_per_strip;
    int strips_in_image;
    int tag_type;
    int value;
    int w;
    int x;
    int y;
    int z;
    uint16_t count;
    uint16_t grey;
    uint16_t magic;
    uint16_t tag;

    uint16_t(*to_int16) (int, int);
    uint16_t type;
    uint32_t entry_count;
    uint32_t entry_offset;
    uint32_t next_offset;
    uint32_t offset;
    uint32_t position;
    uint32_t strip_byte_counts;
    uint32_t strip_offsets;

    uint32_t(*to_int32) (int, int, int, int);
    long end_offset;
    unsigned char *image;

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
    end_offset = file_end(f);
    NXT(&x);
    NXT(&y);
    if (x == 0x4D && y == 0x4D) {
        to_int16 = to_int16b;
        to_int32 = to_int32b;
    } else if (x == 0x49 && y == 0x49) {
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
        fprintf(stderr, "%s: fseek failed for offset = %u\n", me, offset);
        exit(2);
    }

    for (;;) {
        NXT(&x);
        NXT(&y);
        count = to_int16(x, y);
        for (i = 0; i < count; i++) {
            NXT(&x);
            NXT(&y);
            tag = to_int16(x, y);
            NXT(&x);
            NXT(&y);
            type = to_int16(x, y);
            if (type2name(type, &name) != 0) {
                fprintf(stderr, "%s: unknown type: %d\n", me, type);
                exit(2);
            }
            NXT(&x);
            NXT(&y);
            NXT(&z);
            NXT(&w);
            entry_count = to_int32(x, y, z, w);
            NXT(&x);            /* offset */
            NXT(&y);
            NXT(&z);
            NXT(&w);
            if (tag2name(tag, &name) == 0) {
                printf("tag: %s\n", name);
                tag2type(tag, &tag_type);
                switch (tag_type) {
                case TAG_ASCII:
                    assert(type == ASCII);
                    break;
                case TAG_LONG:
                    assert(type == LONG);
                    break;
                case TAG_SHORT:
                    assert(type == SHORT);
                    break;
                case TAG_SHORT_OR_LONG:
                    assert(type == SHORT || type == LONG);
                    break;
                case TAG_RATIONAL:
                    assert(type == RATIONAL);
                    break;		    
                default:
                    fprintf(stderr, "%s: unknown tag_type '%d'\n", me,
                            tag_type);
                    exit(2);
                    break;
                }
            } else {
                fprintf(stderr, "unknown tag: %d\n", tag);
            }
        }
        NXT(&x);
        NXT(&y);
        NXT(&z);
        NXT(&w);
        next_offset = to_int32(x, y, z, w);
        if (next_offset == 0)
            break;
        if (fseek(f, next_offset, SEEK_SET) != 0) {
            fprintf(stderr, "%s: fseek failed for next_offset = %u\n", me,
                    offset);
            exit(2);
        }
    }
    free(image);
    fclose(f);
}

static uint16_t
to_int16l(int x, int y)
{
    return (x << 0) | (y << 8);
}

static uint16_t
to_int16b(int x, int y)
{
    return (y << 0) | (x << 8);
}

static uint32_t
to_int32l(int x, int y, int z, int w)
{
    return (x << 0) | (y << 8) | (z << 16) | (w << 24);
}

static uint32_t
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

static int
tag2name(int type, const char **p)
{
    int i;
    int n;

    n = sizeof(Tag) / sizeof(Tag[0]);
    for (i = 0; i < n; i++)
        if (type == Tag[i]) {
            *p = TagName[i];
            return 0;
        }
    return 1;
}

static int
tag2type(int type, int *p)
{
    int i;
    int n;

    n = sizeof(Tag) / sizeof(Tag[0]);
    for (i = 0; i < n; i++)
        if (type == Tag[i]) {
            *p = TagType[i];
            return 0;
        }
    return 1;
}

static long
file_end(FILE * f)
{
    long ans;
    long pos;

    pos = ftell(f);
    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "%s: fseek failed\n", me);
        exit(2);
    }
    ans = ftell(f);
    fseek(f, pos, SEEK_SET);
    return ans;
}
