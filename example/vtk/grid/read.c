#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/macro.h>
#include <co/memory.h>

#define FMT  CO_REAL_OUT
#define FMT3 FMT " " FMT " " FMT "\n"

static const char *me = "vtk/read";
static void
usg(void)
{
    fprintf(stderr, "%s < vtk\n", me);
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
    int n;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(2);
	    break;
	}
    
    if (vtk_grid_read(stdin, size, origin, spacing, &data) != CO_OK) {
	fprintf(stderr, "%s: vtk_grid_read failed\n", me);
	exit(2);
    }

    printf("DIMENSIONS %d %d %d\n", size[X], size[Y], size[Z]);
    printf("ORIGIN " FMT3, origin[X], origin[Y], origin[Z]);
    printf("SPACING " FMT3, spacing[X], spacing[Y], spacing[Z]);
    n = size[X] * size[Y] * size[Z];
    for (i = 0; i < n; i++)
	printf(FMT "\n", data[i]);
    FREE(data);
}
