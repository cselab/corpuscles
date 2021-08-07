CC = gcc
FC = gfortran

FCFLAGS = -O2 -g
FXFLAGS = -fallow-argument-mismatch

GSL_CFLAGS = "`gsl-config --cflags`"
GSL_LDFLAGS = "`gsl-config --libs`"

CFLAGS = -O2 -g
LDFLAGS =
PREFIX = $(HOME)/.local
DATAPATH = $(HOME)/.co
MAXIMA_HOME = $(HOME)/.maxima

# prefix for commands
P = co
