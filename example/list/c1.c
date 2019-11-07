#include <stdio.h>
#include <co/err.h>
#include <co/list/a.h>
#include <co/list/c.h>

int
main(void)
{
    int n;
    Alist *alist;
    Clist *clist;

    n = 5;
    alist_ini(n, &alist);
    alist_push(alist, 0, 1);
    alist_push(alist, 0, 2);
    alist_push(alist, 1, 2);
    alist_push(alist, 3, 4);

    clist_ini(n, &clist);
    clist_alist(clist, alist);

    clist_push(clist, 0, 100);
    clist_push(clist, 1, 101);
    clist_push(clist, 3, 103);
    clist_fwrite(stdout, clist);

    clist_fin(clist);
    alist_fin(alist);

    return 0;
}
