#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <real.h>

#include <co/err.h>
#include <co/util.h>
#include <alg/integration.h>

#define FMT CO_REAL_OUT

static char me[] = "integration";

static void
usg()
{
    fprintf(stderr,
            "%s [-t type] [-a real] [-b real] [-k real] [-f sin|cos|sq]\n",
            me);
    exit(1);
}

static real
fsin(real x, void *p)
{
    real k;

    k = *(real *) p;
    return sin(k * x);
}

static real
fcos(real x, void *p)
{
    real k;

    k = *(real *) p;
    return cos(k * x);
}

static real
fsq(real x, void *p)
{
    real k;

    k = *(real *) p;
    return k * x * x;
}

int
main(int argc, char **argv)
{
    int type, status;
    AlgIntegration *integ;
    real a, b, result, k;
    const char *Type;
    const char *Fun;

    real(*f) (real, void *);

    a = 0;
    b = 1;
    k = 1;
    Type = "gauss15";
    f = fsin;
    while (*++argv != NULL && argv[0][0] == '-') {
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'i':
            argv++;
            if ((Type = *argv) == NULL) {
                fprintf(stderr, "%s: -t needs an argument\n", me);
                exit(2);
            }
            break;
        case 'a':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -a needs an argument\n", me);
                exit(2);
            }
            a = atof(*argv);
            break;
        case 'b':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -b needs an argument\n", me);
                exit(2);
            }
            b = atof(*argv);
            break;
        case 'k':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -b needs an argument\n", me);
                exit(2);
            }
            k = atof(*argv);
            break;
        case 'f':
            argv++;
            if ((Fun = *argv) == NULL) {
                fprintf(stderr, "%s: -f needs an argument\n", me);
                exit(2);
            }
            if (util_eq(Fun, "cos")) {
                f = *fcos;
            } else if (util_eq(Fun, "cos")) {
                f = *fsin;
            } else if (util_eq(Fun, "sq")) {
                f = *fsq;
            }
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    }
    status = alg_integration_str2enum(Type, &type);
    if (status != CO_OK)
        ER("alg_integration_str2enum failed");
    alg_integration_ini(QAGS, &integ);
    alg_integration_apply(integ, a, b, f, &k, &result);
    printf(FMT "\n", result);
    alg_integration_fin(integ);
}
