#include <stdio.h>
#include <co/err.h>
#include <co/list/a.h>
#include <co/list/c.h>

int
main(void)
{
    Clist *clist;

    clist_gen_ppn(3, 3, 3, &clist);

    clist_push(clist, 0, 100);
    clist_fwrite(stdout, clist);
    clist_fin(clist);

    return 0;
}
