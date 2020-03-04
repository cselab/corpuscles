#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/vtk.h>
#include "def.h"

const char *me = "tag";
static const real spacing0[] = { 1.6, 1.6, 1.0 };
static void
usg(void)
{
    fprintf(stderr, "%s -r int[6] -s int[3] file\n", me);
    exit(2);
}

static uint16_t to_int16l(int, int);
static uint16_t to_int16b(int, int);
static uint32_t to_int32l(int, int, int, int);
static uint32_t to_int32b(int, int, int, int);
static int type2name(int, const char **);
static int type2size(int, int *);
static int tag2name(int, const char **);
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

static int Size[] = {
    1,
    1,
    2,
    4,
    8,
    1,
    1,
    2,
    4,
    8,
    4,
    8,
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

int
main(int argc, char **argv)
{
    enum { X, Y, Z };
    const char *name;
    FILE *f;
    int entry_size;
    int hi[3];
    int i;
    int j;
    int k;
    int l;
    int lo[3];
    int Rflag;
    int roi[3];
    int stride[3];
    int size[3];
    int w;
    int x;
    int y;
    int z;
    long end_offset;    
    long n;
    real *data;
    real origin[3];
    real spacing[3];
    uint16_t count;
    uint16_t magic;
    uint16_t tag;
    uint16_t(*to_int16) (int, int);
    uint16_t type;
    uint32_t entry_count;
    uint32_t offset;
    uint32_t strip_byte_counts;    
    uint32_t strip_offsets;
    uint32_t(*to_int32) (int, int, int, int);
    uint32_t value;

    USED(argc);
    Rflag = 0;
    stride[X] = stride[Y] = stride[Z] = 1;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'r':
            if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL ||
                argv[4] == NULL || argv[5] == NULL || argv[6] == NULL) {
                fprintf(stderr, "%s: -r needs six arguments\n", me);
                exit(2);
            }
            lo[X] = atoi(*++argv);
            lo[Y] = atoi(*++argv);
            lo[Z] = atoi(*++argv);
            hi[X] = atoi(*++argv);
            hi[Y] = atoi(*++argv);
            hi[Z] = atoi(*++argv);
            Rflag = 1;
            break;
        case 's':
            if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
                fprintf(stderr, "%s: -r needs six arguments\n", me);
                exit(2);
            }
            stride[X] = atoi(*++argv);
            stride[Y] = atoi(*++argv);
            stride[Z] = atoi(*++argv);
            break;	    
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    if (argv[0] == NULL) {
        fprintf(stderr, "%s: missing an argument\n", me);
        exit(2);
    }
    if (Rflag == 0) {
        fprintf(stderr, "%s: -r is not set\n", me);
        exit(2);
    }
    if (lo[X] >= hi[X]) {
        fprintf(stderr, "%s: lo[X]=%d >= hi[X]=%d\n", me, lo[X], hi[X]);
        exit(2);
    }
    if (lo[Y] >= hi[Y]) {
        fprintf(stderr, "%s: lo[Y]=%d >= hi[Y]=%d\n", me, lo[Y], hi[Y]);
        exit(2);
    }
    if (lo[Z] >= hi[Z]) {
        fprintf(stderr, "%s: lo[Z]=%d >= hi[Z]=%d\n", me, lo[Z], hi[Z]);
        exit(2);
    }
    if ((f = fopen(argv[0], "r")) == NULL) {
        fprintf(stderr, "%s: fail to open %s\n", me, argv[0]);
        exit(2);
    }
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
        NXT(&x);                /* offset or value */
        NXT(&y);
        NXT(&z);
        NXT(&w);
        type2size(type, &entry_size);
        if (tag2name(tag, &name) == 0) {
            if (entry_size * entry_count <= 4) {
                if (type == SHORT)
                    value = to_int16(x, y);
                else
                    value = to_int32(x, y, z, w);
            }
        } else {
            fprintf(stderr, "unknown tag: %d\n", tag);
        }
	switch (tag) {
	case ImageWidth:
	    size[X] = value;
	    break;
	case ImageLength:
	    size[Y] = value;
	    break;
	case StripOffsets:
	    strip_offsets = value;
	    break;
	case StripByteCounts:
	    strip_byte_counts = value;
	    break;
	case BitsPerSample:
	    assert(value == 16);
	    break;
	case NewSubfileType:
	    assert(value == 0);
	    break;
	case PhotometricInterpretation:
	    assert(value == 1);
	    break;
	}
    }
    end_offset = file_end(f);
    size[Z] = (end_offset - strip_offsets) / strip_byte_counts;
    roi[X] = (hi[X] - lo[X])/stride[X];
    roi[Y] = (hi[Y] - lo[Y])/stride[Y];
    roi[Z] = (hi[Z] - lo[Z])/stride[Z];
    spacing[X] = spacing0[X] * stride[X];
    spacing[Y] = spacing0[Y] * stride[Y];
    spacing[Z] = spacing0[Z] * stride[Z];    
    origin[X] = lo[X] * spacing0[X];
    origin[Y] = lo[Y] * spacing0[Y];
    origin[Z] = lo[Z] * spacing0[Z];
    n = roi[X] * roi[Y] * roi[Z];
    MALLOC(n, &data);
    if (fseek(f, strip_offsets + lo[Z] * strip_byte_counts, SEEK_SET) != 0) {
        fprintf(stderr, "%s: fseek failed\n", me);
        exit(2);
    }
    l = 0;
    for (k = lo[Z]; k < hi[Z]; k++)
	for (j = 0; j < size[Y]; j++)
	    for (i = 0; i < size[X]; i++) {
	       NXT(&x);
	       NXT(&y);
               if (lo[X] > i || i >= hi[X])
                    continue;
	       if (lo[Y] > j || j >= hi[Y])
		   continue;
	       if (i % stride[X] != 0) continue;
	       if (j % stride[Y] != 0) continue;
	       if (k % stride[Z] != 0) continue;	       
	       data[l++] = to_int16(x, y);
	    }
    assert(l == n);
    fprintf(stderr, "%d %d %d\n", size[X], size[Y], size[Z]);
    if (vtk_grid_write(stdout, roi, origin, spacing, data) != CO_OK) {
        fprintf(stderr, "%s: vtk_grid_write failed\n", me);
        exit(2);
    }
    FREE(data);    
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
type2size(int type, int *p)
{
    int i;
    int n;

    n = sizeof(Type) / sizeof(Type[0]);
    for (i = 0; i < n; i++)
        if (type == Type[i]) {
            *p = Size[i];
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
