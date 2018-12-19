#####################################################
##########################################################
import sys
import math

#print "Argument list: ", str(sys.argv)

#area_rbc=float(sys.argv[1])
#vol_rbc=float(sys.argv[2])

Kb=1.0
Kad=2*Kb/math.pi
Da0=0.002
C0=0.0
H0=C0/2
#A=8.72
A=12.56
D=3/3.91/1000
DA0=(Da0-H0*D)*A
DA0D=DA0/D

print "Kb, Kad", Kb, Kad
print "Da0   ", Da0
print "C0,H0 ", C0, H0
print "D, A  ", D, A
print "DA0, DA0D", DA0, DA0D

print "Input parameters for minimization:"
print "Kb, C0, Kad, DA0D: ", Kb, C0, Kad, DA0D
