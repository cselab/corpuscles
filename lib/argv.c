#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "co/err.h"

#include "inc/def.h"
#include "co/argv.h"

int
argv_real(char ***pargv, real * p)
{
  char **argv;

  argv = *pargv;

  if (*argv == NULL)
    ERR(CO_IO, "not enough arguments");

  if (sscanf(*argv, CO_REAL_IN, p) != 1)
    ERR(CO_IO, "not a number '%s'", *argv);

  argv++;
  *pargv = argv;
  return CO_OK;
}

int
argv_int(char ***pargv, int *p)
{
  char **argv;

  argv = *pargv;

  if (*argv == NULL)
    ERR(CO_IO, "not enough arguments");

  if (sscanf(*argv, "%d", p) != 1)
    ERR(CO_IO, "not an integer '%s'", *argv);

  argv++;
  *pargv = argv;
  return CO_OK;
}

int
argv_str(char ***pargv, char *p)
{
  char **argv;

  argv = *pargv;

  if (*argv == NULL)
    ERR(CO_IO, "not enough arguments");
  strcpy(p, *argv);

  argv++;
  *pargv = argv;
  return CO_OK;
}
