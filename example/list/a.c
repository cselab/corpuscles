#include <stdio.h>
#include <stdlib.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/list/a.h>

int
main(void)
{
    int n;
    Alist *alist;
    int *a;

    n = 4;
    alist_ini(n, &alist);
    alist_push(alist, 0, 1);
    alist_push(alist, 0, 2);
    alist_push(alist, 1, 2);

    alist_fwrite(stdout, alist);

    alist_fin(alist);

    return 0;
}
