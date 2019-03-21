#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include <co/list/c.h>

#define T Cell
typedef struct Cell Cell;
int cell_pp_ini(const real lo[2], const real hi[2], real size, T**);
int cell_head(const real[3],  int**);
int cell_fin();

int main(void)
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
