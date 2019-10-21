#!/usr/bin/awk -f

{
    i++
    x[i] = $1
    y[i] = $2
}

END {
    for (i = 2; i in x; i++)
        print (x[i] + x[i-1])/2, x[i] - x[i - 1]
}
