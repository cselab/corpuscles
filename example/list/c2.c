#include <stdio.h>
#include <co/err.h>
#include <co/list/a.h>
#include <co/list/c.h>

int
main(void)
{
    Clist *clist;
    int n;

    n = 3;
    clist_gen_np(n, n, &clist);

    clist_push(clist, 0, 100);
    clist_push(clist, 1, 101);
    clist_push(clist, 2, 102);
    clist_push(clist, 3, 103);
    clist_fwrite(stdout, clist);
    clist_fin(clist);

    return 0;
}
