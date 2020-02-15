#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/macro.h>
#include <co/err.h>
#include <co/vec.h>

#define FMT CO_REAL_OUT
static char me[] = "poc/tri_plane";
static const real eps = 1e-12;
static void
usg()
{
    fprintf(stderr, "%s a[xyz] b[xyz] c[xyz]\n", me);
    exit(2);
}

int
main(int argc, const char **argv)
{
    real a[3];
    real b[3];
    real c[3];
    real normal[3];
    real point[3];
    int Tflag;
    int Pflag;
    int Nflag;
    USED(argc);
    argv++;
    Tflag = Pflag = Nflag = 0;
    while (*argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
        case 'h':
            usg();
            break;
	case 't':
	    argv++;
	    vec_argv(&argv, a);
	    vec_argv(&argv, b);
	    vec_argv(&argv, c);
	    Tflag = 1;
	    break;
	case 'n':
	    argv++;
	    vec_argv(&argv, normal);
	    Nflag = 1;
	    break;
	case 'p':
	    argv++;
	    vec_argv(&argv, point);
	    Pflag = 1;
	    break;	    
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    if (*argv != NULL) {
	fprintf(stderr, "%s: too many arguments: '%s'\n", me, *argv);
	exit(2);	
    }
    if (!Nflag) {
	fprintf(stderr, "%s: normal (-n) is not set\n", me);
	exit(2);		
    }
    if (!Pflag) {
	fprintf(stderr, "%s: point (-p) is not set\n", me);
	exit(2);		
    }
    if (!Tflag) {
	fprintf(stderr, "%s: triangle (-t) is not set\n", me);
	exit(2);		
    }
    if (vec_abs(normal) < eps) {
	fprintf(stderr, "%s: abs(normal) < eps\n", me);
	exit(2);		
    }

    real p0[3];
    real b0[3];
    real pn;
    real bn;
    vec_minus(point, a, p0);
    vec_minus(b, a, b0);
    pn = vec_dot(normal, p0);
    bn = vec_dot(normal, b0);
}
