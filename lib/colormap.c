#include <stdio.h>
#include <stdlib.h>
#include "real.h"

#include "co/colormap.h"
#include "co/err.h"
#include "co/util.h"

typedef int (*MType)(real, real, real, /**/ float *, float *, float *);
typedef int (*const CType)(real, real, real, /**/ float *, float *, float *);

#define DEC(fun) \
    static int fun(real v, real l, real h, float *pR, float *pG, float *pB);
DEC(jet);
DEC(viridis);
DEC(gray);
DEC(cool);
DEC(winter);
DEC(summer);
static const CType Maps[] = { jet, viridis, gray, cool, winter, summer};
static const char *Names[] = {"jet", "viridis", "gray", "cool", "winter", "summer"};

int
colormap(real v, real l, real h, float *r, float *g, float *b)
{
    static MType map = NULL;
    const char *name;
    int i;

    if (map == NULL) {
	name = getenv("COLORMAP");
	if (name == NULL)
	    map = jet;
	else
	    for (i = 0; ; i++) {
		if (i == sizeof(Names)/sizeof(*Names))
		    ERR(CO_IO, "unknown colormap '%s'", name);
		if (util_eq(name, Names[i])) {
		    map = Maps[i];
		    break;
		}
	    }
    }
    return map(v, l, h, r, g, b);;
}

static int
jet(real v, real l, real h, float *pR, float *pG, float *pB)
{
    float R, G, B;

    if (v < l)
	v = l;
    if (v > h)
	v = h;

    if (l != h)
	v = 4 * (v - l) / (h - l);
    else
	v = 0;

    R = 0;
    G = B = 1;
    if (v < 1)
	G = v;
    else if (v < 2)
	B = 2 - v;
    else if (v < 3) {
	R = v - 2;
	B = 0;
    } else {
	R = 1;
	G = 4 - v;
	B = 0;
    }

    *pR = R;
    *pG = G;
    *pB = B;
    return CO_OK;
}

static int
viridis(real v, real l, real h, float *pR, float *pG, float *pB)
{
    float R, G, B;
    MSG("viridis");

    if (v < l)
	v = l;
    if (v > h)
	v = h;

    if (l != h)
	v = 4 * (v - l) / (h - l);
    else
	v = 0;

    R = 0;
    G = B = 1;
    if (v < 1)
	G = v;
    else if (v < 2)
	B = 2 - v;
    else if (v < 3) {
	R = v - 2;
	B = 0;
    } else {
	R = 1;
	G = 4 - v;
	B = 0;
    }

    *pR = R;
    *pG = G;
    *pB = B;
    return CO_OK;
}

static int
gray(real v, real l, real h, float *pR, float *pG, float *pB)
{
    float R, G, B;

    if (v < l)
	v = l;
    if (v > h)
	v = h;
    if (l != h)
	v = (v - l) / (h - l);
    else
	v = 0;
    *pR = v;
    *pG = v;
    *pB = v;
    return CO_OK;
}

static int
cool(real v, real l, real h, float *pR, float *pG, float *pB)
{
    float R, G, B;

    if (v < l)
	v = l;
    if (v > h)
	v = h;
    if (l != h)
	v = (v - l) / (h - l);
    else
	v = 0;
    *pR = v;
    *pG = 1 - v;
    *pB = 1;
    return CO_OK;
}

static int
winter(real v, real l, real h, float *pR, float *pG, float *pB)
{
    float R, G, B;

    if (v < l)
	v = l;
    if (v > h)
	v = h;
    if (l != h)
	v = (v - l) / (h - l);
    else
	v = 0;
    *pR = 0;
    *pG = v;
    *pB = 1 - v/2;
    return CO_OK;
}

static int
summer(real v, real l, real h, float *pR, float *pG, float *pB)
{
    float R, G, B;

    if (v < l)
	v = l;
    if (v > h)
	v = h;
    if (l != h)
	v = (v - l) / (h - l);
    else
	v = 0;
    *pR = v;
    *pG = 0.5 + v/2;
    *pB = 0.4;
    return CO_OK;
}
