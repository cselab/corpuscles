#!/usr/bin/python3
#--------------------------------------------------
#Example 17.4, Page 516
#<<Numerical optimization>>
# Nocedal and Wright 2006
#--------------------------------------------------
import math
import matplotlib.pyplot as plt
import numpy as np

def func(x1, x2):
    return x1+x2

def funcder(x1, x2):
    return 1, 1

def cons(x1, x2):
    return x1*x1+x2*x2-2

def consder(x1, x2):
    return 2*x1, 2*x2

def Qfunc(x1, x2, lam, mu):
    f = func(x1,x2)
    c = cons(x1,x2)
    
    return f - lam*c + mu*c*c/2.0

def Qfuncder(x1, x2, lam, mu):
    
    fd1, fd2 = funcder(x1,x2)
    c = cons(x1, x2)
    cd1, cd2 = consder(x1, x2)
    return fd1 - lam*cd1 + mu*c*cd1, fd2 - lam*cd2 + mu*c*cd2


x1=0
x2=0

toler0=1.0e-2
error=1000

k=0
k_max=500
lam0=-0.5
mu0=1

s=1.0e-2

toler = toler0
lam   = lam0
mu    = mu0

for k in range(0, k_max):
    
    Q   = Qfunc(x1, x2, lam, mu)
    Qd1, Qd2=Qfuncder(x1, x2, lam, mu)
    
    error=np.sqrt(Qd1*Qd1+Qd2*Qd2)

    c = cons(x1, x2)
    lam = lam - mu*c
    
    print("c, err, lam = ", c, error, lam)
    print("k, x1,  x2  = ", k, x1, x2)
    print("Q, Qd1, Qd2 = ", Q, Qd1, Qd2)
    print("")
    
    
    if (error < toler):
        break
    
    x1 -= Qd1*s
    x2 -= Qd2*s

    
    
    

