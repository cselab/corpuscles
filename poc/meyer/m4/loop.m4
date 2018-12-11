divert(-1)
define(`BEGIN_T', `
for (h = 0; h in ii; h++) {
    i = ii[h]; j = jj[h]; k = kk[h]
    get3(i, j, k, a, b, c)')
define(`END_T', }
)
define(`BEGIN_V', `
for (i = 0; i < nv; i++) {
    get(i, a)')
define(`END_V', }
)
divert`'dnl
function get3(i, j, k, a, b, c) {
    get(i, a); get(j, b); get(k, c)
}

function get(i, a) {
    a[X] = r[i, X]
    a[Y] = r[i, Y]
    a[Z] = r[i, Z]
}
