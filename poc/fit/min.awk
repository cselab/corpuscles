#!/usr/bin/awk -f


{
    i++
    x[i] = $1
    y[i] = $2
}

END {
    for (i = 2; i in x; i++) {
        if (y[i] < y[i + 1] && y[i] < y[i - 1])
            print x[i], y[i]
    }
}
