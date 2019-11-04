#!/usr/bin/env python3

import cv2 as cv
import math
import numpy as np

ALPHA = 3

d = cv.imread('img/0.png', cv.IMREAD_UNCHANGED)

alpha = d[:, :, ALPHA]

f1 = 3
f0 = math.exp(-f1) + 1

def f(x):
    return 1/(1 + math.exp(-f1*x))

def g(x):
    return f(x) - f(0)

def h(x):
    return g(x)/g(1)

def sigmoid(x):
    return h(2*x - 1)

def transform(e):
    e = (e - lo + 0.0)/(hi - lo)
    e = sigmoid(e)
    e *= 255
    e = int(e)
    if e < 0:
       e = 0
    elif e > 255:
       e = 255
    return e
transform = np.vectorize(transform)
lo = np.min(alpha)
hi = np.max(alpha)
alpha = transform(alpha)
d[:, :, ALPHA] = alpha

cv.imwrite("o.png", d)
