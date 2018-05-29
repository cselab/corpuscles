#include <stdio.h>
#include <he/err.h>
#include <he/he.h>

int main() {
    He *he;
    he_file_ini("/dev/stdin", &he);

    MSG("%d %d %d %d", he_nv(he), he_nt(he), he_ne(he), he_nh(he));
    
    he_fin(he);
}
