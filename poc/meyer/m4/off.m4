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
