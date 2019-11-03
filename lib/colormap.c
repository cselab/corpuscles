#include "real.h"

#include "co/err.h"
#include "co/colormap.h"

int
colormap(real v, real l, real h, /**/ float *pR, float *pG, float *pB)
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
