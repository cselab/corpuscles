define(`BEGIN_L1', `dnl
for (t = 0; (h = 3*t) in ii; t++) {
    i = ii[h]; j = jj[h]; k = kk[h]
    get3(i, j, k, a, b, c)')dnl
define(`END_L1', }
)dnl
define(`BEGIN_L3', `dnl
for (h = 0; h in ii; h++) {
    t = int(h/3)
    i = ii[h]; j = jj[h]; k = kk[h]
    get3(i, j, k, a, b, c)')dnl
define(`END_L3', }
)dnl
"${AWK=awk}" '
BEGIN {
    ini()
    read()

    BEGIN_L1
    print i, j, k
    END_L1
}

function read(   v, t, h, i, j, k) {
    getline; getline
    nv = $1; nt = $2
    for (v = 0; v < nv; v++) {
	getline
	r[v,X] = $1; r[v,Y] = $2; r[v,Z] = $3
    }
    for (h = t = 0; t <  nt; t++) {
	getline
	i = $2; j = $3; k = $4
	ii[h] = i; jj[h] = j; kk[h++] = k
	jj[h] = i; kk[h] = j; ii[h++] = k
	kk[h] = i; ii[h] = j; jj[h++] = k
    }
}

function ini() { X = 0; Y = 1; Z = 2 }
function get3(i, j, k, a, b, c) {
    get(i, a); get(j, b); get(k, c)
}
function get(i, a) {
    a[X] = r[i, X]
    a[Y] = r[i, Y]
    a[Z] = r[i, Z]
}

'
