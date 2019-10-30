#include <stdio.h>
#include <string.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"

#define FMT_IN   CO_REAL_IN
#define FMT_OUT  CO_REAL_OUT

#define SIZE (MAX_STRING_SIZE)

int
punto_fwrite(int n, const real * queue[], /**/ FILE * f)
{
  int i, r;
  const real **q;

  for (i = 0; i < n; i++) {
    q = queue;
    for (;;) {
      r = fprintf(f, FMT_OUT, (*q)[i]);
      q++;
      if (r < 0)
        ERR(CO_IO, "fail to write");
      if (*q != NULL)
        fputc(' ', f);
      else {
        fputc('\n', f);
        break;
      }
    }
  }
  return CO_OK;
}

int
punto_write(int n, const real * queue[], /**/ const char *path)
{
  FILE *f;

  if ((f = fopen(path, "w")) == NULL)
    ERR(CO_IO, "fail to open '%s'", path);
  if (punto_fwrite(n, queue, f) != CO_OK)
    ERR(CO_IO, "fail to write to '%s", path);
  if (fclose(f) != 0)
    ERR(CO_IO, "fail to close '%s'", path);
  return CO_OK;
}

int
punto_fappend(int n, const real * queue[], /**/ FILE * f)
{
  int i, r;
  const real **q;

  for (i = 0; i < n; i++) {
    q = queue;
    for (;;) {
      r = fprintf(f, FMT_OUT, (*q)[i]);
      q++;
      if (r < 0)
        ERR(CO_IO, "fail to write");
      if (*q != NULL)
        fputc(' ', f);
      else {
        fputc('\n', f);
        break;
      }
    }
  }
  fputc('\n', f);
  return CO_OK;
}

int
punto_append(int n, const real * queue[], /**/ const char *path)
{
  FILE *f;

  if ((f = fopen(path, "a")) == NULL)
    ERR(CO_IO, "fail to open '%s'", path);
  if (punto_fappend(n, queue, f) != CO_OK)
    ERR(CO_IO, "fail to write to '%s", path);
  if (fclose(f) != 0)
    ERR(CO_IO, "fail to close '%s'", path);
  return CO_OK;
}
enum { OK, FAIL };
static int
read(char *s, int i, real * q[])
{
  const char delim[] = " \t";
  char *tok;

  for (;; s = NULL) {
    if (*q == NULL)
      break;
    if ((tok = strtok(s, delim)) == NULL)
      return FAIL;
    if (sscanf(tok, FMT_IN, &(*q)[i]) != 1)
      return FAIL;
    q++;
  }
  return OK;
}

static int
blank(const char *s)
{
  char c;

  while ((c = *s++) != '\0')
    if (c != ' ' && c != '\t' && c != '\n')
      return 0;
  return 1;
}

int
punto_read(const char *path, /**/ int *pn, real * queue[])
{
  FILE *f;
  int n;
  char s[SIZE];

  if ((f = fopen(path, "r")) == NULL)
    ERR(CO_IO, "fail to open '%s'", path);
  n = 0;
  while (fgets(s, SIZE, f) != NULL && !blank(s)) {
    if (read(s, n++, queue) != OK)
      ERR(CO_IO, "wrong line '%s' in file '%s'", s, path);
  }

  if (fclose(f) != 0)
    ERR(CO_IO, "fail to close '%s'", path);
  *pn = n;
  return CO_OK;
}
