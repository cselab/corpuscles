A_{al,be,j} = diff(A_al, R_{be, j})
K_{be, j} = -F_{be, j} - sigma_ga A_{ga,be,j}
v_{al, i} = v0_{al, i} + O_{al,be,i,j} K_{be,j}
A_{al,be,j} v_{be, j} = 0

A*v0 + A*O*K

1. Build A: R
2. Build K: F, sigma, A
3. Build O: R
4. Compute v: O, K, v0(r)
5. Compute A * v

