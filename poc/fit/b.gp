file = "a.dat"; h0 = 23.1815427785777
#file = "b.dat"; h0 = 10.6093701837166

f(x) = f0 + f1*x + f2*x*x
g(x) = g0 + g1*x + g2*x*x
h(x) = g(x) + (f(x) - g(x))*(cos(2*pi*x/h0 + h1) + 1)/2

fit f(x) "<./max.awk " . file  via f0, f1, f2
fit g(x) "<./min.awk " . file  via g0, g1, g2

h1 = 1
set sample 100000, 100000
fit h(x) file via h0, h1

plot file w dots, h(x)
print h0
