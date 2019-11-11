#include <stdio.h>
#include <co/err.h>
#include <co/list/c.h>

int
main(void)
{
    Clist *clist;
    int n;

    n = 3;
    clist_gen_nn(n, n, &clist);
    clist_fin(clist);

    return 0;
}
