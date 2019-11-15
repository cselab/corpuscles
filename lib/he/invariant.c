#include "co/err.h"
#include "co/he.h"
#include "co/he/invariant.h"

int
he_invariant_nxt(He *he)
{
    int h, nh, n, nn, nnn;
    nh = he_nh(he);

    for (h = 0; h < nh, h++) {
	n  = he_nxt(he, h);
	nn = he_nxt(he, n);
	nnn = he_nxt(he, nn);
	if (nnn != h)
	    ERR(CO_NUM, "nnn=%d != h=%d", nnn, h);
    }
    return CO_OK;
}
