#include <stdio.h>
#include <stdlib.h>
#include <real.h>

#include <alg/special.h>

#define FMT CO_REAL_OUT

static char me[] = "special";
static void
usg()
{
    fprintf(stderr, "%s -x float -n int\n", me);
    exit(1);
}

int
main(int argc, char **argv)
{
    real x;
    real y;
    int n;
    int Nflag;
    int Xflag;

    Nflag = Xflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'x':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -x needs an argument\n", me);
                exit(2);
            }
            x = atof(*argv);
            Xflag = 1;
            break;
        case 'n':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -n needs an argument\n", me);
                exit(2);
            }
            n = atoi(*argv);
            Nflag = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    if (!Nflag) {
        fprintf(stderr, "%s: -n is not set\n", me);
        exit(1);
    }
    if (!Xflag) {
        fprintf(stderr, "%s: -x is not set\n", me);
        exit(1);
    }
    switch (n) {
    case 0:
        y = alg_special_bessel_J0(x);
        break;
    case 1:
        y = alg_special_bessel_J1(x);
        break;
    case 2:
        y = alg_special_bessel_J2(x);
        break;
    default:
        y = alg_special_bessel_Jn(n, x);
        break;
    }
    printf(FMT "\n", y);
}
