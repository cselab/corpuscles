#!/usr/bin/python3
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


def Qfunc(x1,x2,mu):
    f = func(x1,x2)
    c = cons(x1,x2)
    return f+mu*c*c/2.0

def Qfuncder(x1,x2,mu):
    fd1, fd2 = funcder(x1,x2)
    c = cons(x1,x2)
    cd1, cd2 = consder(x1,x2)
    return fd1+mu*c*cd1, fd2+mu*c*cd2


x1=0
x2=0
error=1000
toler0=1.0e-3
k=0
kmax=1000
mu=1
coefmu=1.1
s=1.0e-2

for k in range(0, kmax):
    
    Q   = Qfunc(x1,x2,mu)
    Qd1, Qd2=Qfuncder(x1,x2,mu)
    error=np.sqrt(Qd1*Qd1+Qd2*Qd2)
    print("k, x1,x2    =", k, x1, x2)
    print("Q, Qd1, Qd2 =", Q, Qd1, Qd2)
    print("error       =", error)

    if(error<toler):
        break
    
    x1 -= Qd1*s
    x2 -= Qd2*s

    
    
    

