#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/viscosity/pairwise.h>
#include <co/y.h>

#define OUT  CO_REAL_OUT
#define IN  CO_REAL_IN
#define OUT3 OUT " " OUT " " OUT "\n"
enum { SIZE = 9999 };
enum { X, Y, Z };

static const char *me = "viscosity/test";
static char path_in[SIZE];
static char path_out[SIZE];
static void cleanup(int);
static void cleanup0(void);

static void
usg(void)
{
    fprintf(stderr, "%s < off\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    char command[4 * SIZE];
    char line[SIZE];
    const char *cmd;
    const char *delimeter = " \t";
    FILE *f;
    He *he;
    int i;
    int nv;
    int dim;
    real *x;
    real *y;
    real *z;
    real *vx;
    real *vy;
    real *vz;
    real *fx;
    real *fy;
    real *fz;
    real v[3];
    ViscosityPairwise *viscosity;

    err_set_ignore();
    USED(argc);
    cmd = NULL;
    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	case 'c':
	    argv++;
	    if (argv[0] == NULL) {
		fprintf(stderr, "%s: -c needs an argumen\n", me);
		exit(2);
	    }
	    cmd = argv[0];
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(2);
	    break;
	}
    if (argv[0] != NULL) {
	fprintf(stderr, "%s: unknown argument '%s'\n", me, argv[0]);
	exit(2);
    }
    if (y_inif(stdin, &he, &x, &y, &z) != 0) {
	fprintf(stderr, "%s: y_inif failed\n", me);
	exit(2);
    }
    if (cmd == NULL) {
	fprintf(stderr, "%s: option -c is not set\n", me);
	exit(2);
    }
    nv = he_nv(he);
    MALLOC3(nv, &vx, &vy, &vz);
    CALLOC3(nv, &fx, &fy, &fz);
    viscosity_pairwise_ini(1.0, he, &viscosity);

    snprintf(path_in, SIZE, "/tmp/pairwise.in.%d", getpid());
    snprintf(path_out, SIZE, "/tmp/pairwise.out.%d", getpid());
    if (signal(SIGINT, cleanup) == SIG_ERR) {
	fprintf(stderr, "%s: signal(3) failed", me);
	exit(2);
    }

    if ((f = fopen(path_in, "w")) == NULL) {
	fprintf(stderr, "%s: fail to open '%s'\n", me, path_in);
	exit(2);
    }
    for (i = 0; i < nv; i++)
	if (fprintf(f, OUT3, x[i], y[i], z[i]) < 0) {
	    fprintf(stderr, "%s: fprintf(3) failed for '%s'\n", me,
		    path_in);
	    exit(2);
	}
    if (fclose(f) != 0) {
	fprintf(stderr, "%s: fclose(3) failed for '%s'\n", me, path_in);
	exit(2);
    }
    snprintf(command, 4 * SIZE, "<%s %s >%s", path_in, cmd, path_out);
    if (system(command) != 0) {
	fprintf(stderr, "%s: command '%s' failed\n", me, command);
	exit(2);
    }
    if ((f = fopen(path_out, "r")) == NULL) {
	fprintf(stderr, "%s: fail to open '%s'\n", me, path_out);
	exit(2);
    }

    i = 0;
    while (fgets(line, SIZE, f) != NULL) {
      if (strtok(line, delimeter) == NULL) {
	fprintf(stderr, "%s: fail to parse '%s'\n", me, line);
	exit(2);
      }
      dim = 0;
      do {
	if (dim == 3) {
	  fprintf(stderr, "%s: fail to parse'%s'\n", me, line);
	  exit(2);
	}
	if (sscanf(line, IN, &v[dim]) != 1) {
	  fprintf(stderr, "%s: fail to parse'%s'\n", me, line);
	  exit(2);
	}
	dim++;
      } while (strtok(NULL, delimeter) != NULL);
      vx[i] = v[X];
      vy[i] = v[Y];
      vz[i] = v[Z];
      i++;
    }
    if (fclose(f) != 0) {
	fprintf(stderr, "%s: fclose(3) failed for '%s'\n", me, path_out);
	exit(2);
    }
    cleanup0();
    viscosity_pairwise_force(viscosity, he, x, y, z, vx, vy, vz, fx, fy, fz);
    y_fin(he, x, y, z);
    viscosity_pairwise_fin(viscosity);


    const real *queue[] = {x, y, z, vx, vy, vz, fx, fy, fz, NULL};
    printf("x y z vx vy vz fx fy fz\n");
    punto_fwrite(nv, queue, stdout);
    
    FREE3(vx, vy, vz);
    FREE3(fx, fy, fz);
}

static void
cleanup(int signo)
{
  cleanup0();
  exit(2);
}


static void
cleanup0(void)
{
    char command[4 * SIZE];
    snprintf(command, 4 * SIZE, "rm -f -- '%s', '%s'", path_out, path_in);
}
