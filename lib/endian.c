#include <co/endian.h>

static int
little_p0()
{
    int a;
    unsigned char *b;

    a = 1;
    b = (unsigned char *) &a;
    return (*b) != 0;
}

static int
little_p()
{
    static int done = 0;
    static int little = 0;

    if (done)
        return little;
    little = little_p0();
    done = 1;
    return little;
}

static void
big_endian_dbl0(double *px)
{
    const int n = sizeof(double);
    int i;
    unsigned char *c, b[n];

    if (!little_p())
        return;
    c = (unsigned char *) px;
    for (i = 0; i < n; i++)
        b[i] = c[i];
    for (i = 0; i < n; i++)
        c[i] = b[n - i - 1];
}

void
big_endian_dbl(int n, double *d)
{
    int i;

    for (i = 0; i < n; i++)
        big_endian_dbl0(&d[i]);
}

static void
big_endian_int0(int *px)
{
    const int n = sizeof(int);
    int i;
    unsigned char *c, b[n];

    if (!little_p())
        return;
    c = (unsigned char *) px;
    for (i = 0; i < n; i++)
        b[i] = c[i];
    for (i = 0; i < n; i++)
        c[i] = b[n - i - 1];
}

void
big_endian_int(int n, int *d)
{
    int i;

    for (i = 0; i < n; i++)
        big_endian_int0(&d[i]);
}

static void
big_endian_flt0(float *px)
{
    const int n = sizeof(float);
    int i;
    unsigned char *c, b[n];

    if (!little_p())
        return;
    c = (unsigned char *) px;
    for (i = 0; i < n; i++)
        b[i] = c[i];
    for (i = 0; i < n; i++)
        c[i] = b[n - i - 1];
}

void
big_endian_flt(int n, float *d)
{
    int i;

    for (i = 0; i < n; i++)
        big_endian_flt0(&d[i]);
}


static void
big_endian_short0(short *px)
{
    const int n = sizeof(short);
    int i;
    unsigned char *c, b[n];

    if (!little_p())
        return;
    c = (unsigned char *) px;
    for (i = 0; i < n; i++)
        b[i] = c[i];
    for (i = 0; i < n; i++)
        c[i] = b[n - i - 1];
}

void
big_endian_short(int n, short *d)
{
    int i;

    for (i = 0; i < n; i++)
        big_endian_short0(&d[i]);
}
