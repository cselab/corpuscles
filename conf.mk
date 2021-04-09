CC = gcc
FC = gfortran

FCFLAGS = -Ofast  -g
FXFLAGS =  -fallow-argument-mismatch

CFLAGS = -Ofast  -g -fopenmp
LDFLAGS =
PREFIX = $(HOME)/.local
DATAPATH = $(HOME)/.co
MAXIMA_HOME = $(HOME)/.maxima

# prefix
P = co
