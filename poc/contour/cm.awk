#!/usr/bin/awk -f

BEGIN {
    i = 0
}

NF {
    x[i] = $1; y[i] = $2; i++
}

END {

    u = v = 0
    for (n = i = 0; i in x; i++) {
        u += x[i]
        v += y[i]
        n++
    }
    u /= n
    v /= n

    for (i = 0; i in x; i++) {
        x[i] -= u
        y[i] -= v
    }

    for (i = 0; i in x; i++) {
        t = x[i]; x[i] = y[i]; y[i] = t
    }

    sc = 0.00475
    for (i = 0; i in x; i++) {
        x[i] *= sc
        y[i] *= sc
    }

    for (i = 0; i in x; i++) {
        if (y[i] > 0) {
            print x[i], y[i] | "sort -g -k 1"
        }
    }
    close("sort -g -k 1")

    for (i = 0; i in x; i++) {
        if (y[i] < 0) {
            print x[i], y[i] | "sort -g -k 1 -r"
        }
    }
}
