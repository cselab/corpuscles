#include <stdio.h>

#include <real.h>
#include <co/memory.h>

#pragma	src	"$HOME/co/lib"

#define T Ilist

typedef struct T T;
struct T {
};

int ilist_ini(T**);
int ilist_fin(T*);
int ilist_push(T*, int);
int ilist_size(T*);
int ilist_head(T*, int*);

int main(void)
{
	printf("hello a_list\n");
}
