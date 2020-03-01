#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>

#define FMT  CO_REAL_OUT
#define FMT3 FMT " " FMT " " FMT

static const char *me = "vtk/grid";
static void
usg(void)
{
    fprintf(stderr, "%s -o float[3] -s float[3] -n int[3]> vtk\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    enum {X, Y, Z};
    real origin[3];
    real spacing[3];
    int size[3];
    real *data;
    int i;
    int j;
    int k;
    int l;
    int n;
    int OFlag;
    int SFlag;
    int NFlag;

    OFlag = SFlag = NFlag = 0;
    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	case 'o':
	    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
		fprintf(stderr, "%s: -o needs three arguments\n", me);
		exit(2);
	    }
	    origin[X] = atof(*++argv);
	    origin[Y] = atof(*++argv);
	    origin[Z] = atof(*++argv);
	    OFlag = 1;
	    break;
	case 's':
	    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
		fprintf(stderr, "%s: -s needs three arguments\n", me);
		exit(2);
	    }
	    spacing[X] = atof(*++argv);
	    spacing[Y] = atof(*++argv);
	    spacing[Z] = atof(*++argv);
	    SFlag = 1;
	    break;
	case 'n':
	    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
		fprintf(stderr, "%s: -n needs three arguments\n", me);
		exit(2);
	    }
	    size[X] = atoi(*++argv);
	    size[Y] = atoi(*++argv);
	    size[Z] = atoi(*++argv);
	    NFlag = 1;
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(2);
	    break;
	}
    if (NFlag == 0) {
	fprintf(stderr, "%s: -n is not set\n", me);
	exit(2);
    }
    if (SFlag == 0) {
	fprintf(stderr, "%s: -s is not set\n", me);
	exit(2);
    }
    if (OFlag == 0) {
	fprintf(stderr, "%s: -o is not set\n", me);
	exit(2);
    }

    MSG(FMT3, origin[X], origin[Y], origin[Z]);
    MSG(FMT3, spacing[X], spacing[Y], spacing[Z]);    
    MSG("%d %d %d", size[X], size[Y], size[Z]);

    n = size[X] * size[Y] * size[Z];
    MALLOC(n, &data);
    l = 0;
    for (k = 0; k < size[Z]; k++)
	for (j = 0; j < size[Y]; j++)
	    for (i = 0; i < size[X]; i++) {
		data[l++] = j;
	    }
    if (vtk_grid_write(stdout, size, origin, spacing, data) != CO_OK) {
	fprintf(stderr, "%s: vtk_grid_write failed\n", me);
	exit(2);
    }
    FREE(data);
}
