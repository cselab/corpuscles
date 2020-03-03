#include <stdlib.h>
#include <stdio.h>
#include <co/err.h>
#include <co/macro.h>

const char *me = "byte2";

static int16_t to_int16l(int, int);
static int16_t to_int16b(int, int);
static int32_t to_int32l(int, int, int, int);
static int32_t to_int32b(int, int, int, int);

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
    int16_t (*to_int16)(int, int);
    int32_t (*to_int32)(int, int, int, int);
    int x;
    int y;
    int z;
    int w;
    int16_t magic;
    int16_t count;    
    int32_t offset;
    
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
    NXT(&x); NXT(&y);
    if (x == 0x4D && y == 0x4D) {
      fprintf(stderr, "%s: little\n", me);
      to_int16 = to_int16b;
      to_int32 = to_int32b;
    } else if (x == 0x49 && y == 0x49) {
      fprintf(stderr, "%s: big\n", me);
      to_int16 = to_int16l;
      to_int32 = to_int32l;      
    } else {
      fprintf(stderr, "%s: unexpected bytes\n", me);
      fprintf(stderr, "%s: %X %X\n", me, x, y);
      exit(2);
    }
    NXT(&x); NXT(&y);
    magic = to_int16(x, y);
    if (magic != 42) {
      fprintf(stderr, "%s: no magic => not a tif file\n", me);
      exit(2);
    }
    NXT(&x); NXT(&y); NXT(&z); NXT(&w);
    offset = to_int32(x, y, z, w);
    if (fseek(f, offset, SEEK_SET) != 0) {
      fprintf(stderr, "%s: fseek failed for offset = %ld\n", me, offset);
      exit(2);
    }
    NXT(&x); NXT(&y);
    count = to_int16(x, y);
    fclose(f);
}

static int16_t
to_int16l(int x, int y)
{
  return (x << 0) | (y << 4);
}

static int16_t
to_int16b(int x, int y)
{
  return (y << 0) | (x << 4);
}

static int32_t
to_int32l(int x, int y, int z, int w)
{
  return (x << 0) | (y << 4) | (z << 8) | (w << 24) ;
}

static int32_t
to_int32b(int x, int y, int z, int w)
{
  return (w << 0) | (z << 4) | (y << 8) | (x << 24) ;
}
