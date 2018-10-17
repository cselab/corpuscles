#####################################################
#Given area and volume of the red blood cell,
#
#1)it calculates the radius, volume of a sphere with the
#same area.
#it further calculates the ratio of volumes between
#the rbc and sphere,
#which defines the reduced volume
#U. Seifert, Adv. Phys. 1997
#K. Tsubota, J. Comput. Phys. 2014
#
#2)it calculates the radius, area of a sphere with the
#same volume.
#it further calculates the ratio of areas between
#the rbc and sphere,
#which defines the excess surface area
#
#
#We also calculate sphericity index which is defined in
#P. B. Canham & A. C. Burton, Circulation, 1968
#P. B. Canham,  J. Theoret. Biol. 1970
#########################################################
import sys
import math

#print "Argument list: ", str(sys.argv)

area_rbc=float(sys.argv[1])
vol_rbc=float(sys.argv[2])

r1=area_rbc/4.0/math.pi
r1=math.sqrt(r1)
area_sph1=area_rbc
vol_sph1=4.0*math.pi*r1**3/3.0

ratio1=vol_rbc/vol_sph1

r2=vol_rbc*3.0/4.0/math.pi
r2=r2**(1.0/3.0)
area_sph2=4.0*math.pi*r2**2
vol_sph2=vol_rbc

ratio2=area_rbc/area_sph2

print "***equivalent area***"
print "rbc        area, volume: ", area_rbc, " ", vol_rbc
print "sphere radius, diameter: ", r1, " ", 2*r1
print "sphere     area, volume: ", area_sph1, " ", vol_sph1
print "reduced volume ratio   : ", ratio1

print "***equivalent volume***"
print "rbc        area, volume: ", area_rbc, " ", vol_rbc
print "sphere radius, diameter: ", r2, " ", 2*r2
print "sphere     area, volume: ", area_sph2, " ", vol_sph2
print "excess area ratio      : ", ratio2


const_norm=(1.0/36.0/math.pi)**(1.0/3.0)
const_norm=1.0/const_norm
#approximately 4.835976
#print "const_norm = ", 1./const_norm

sphericity_index_rbc=const_norm*vol_rbc**(2.0/3.0)/area_rbc
sphericity_index_sph1=const_norm*vol_sph1**(2.0/3.0)/area_sph1
sphericity_index_sph2=const_norm*vol_sph2**(2.0/3.0)/area_sph2

print "*** ***"
print "rbc sphericity index  : ", sphericity_index_rbc
print "sph1 sphericity index : ", sphericity_index_sph1
print "sph2 sphericity index : ", sphericity_index_sph2
