#include <stdio.h>
#include <stdlib.h>
#include <co/err.h>

#include <co/list/i.h>

int
main(void)
{
    int i;
    Ilist *a;

    ilist_ini(&a);

    for (i = 0; i < 20; i++)
        ilist_push(a, i);
    ilist_push(a, 10);
    ilist_fwrite(stdout, a);
    ilist_fin(a);
    return 0;
}
