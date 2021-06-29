CC = gcc
FC = gfortran

FCFLAGS = -Ofast  -g
FXFLAGS = -fallow-argument-mismatch

GSL_CFLAGS = "`gsl-config --cflags`"
GSL_LDFLAGS = "`gsl-config --libs`"

CFLAGS = -Ofast  -g
LDFLAGS =
PREFIX = $(HOME)/.local
DATAPATH = $(HOME)/.co
MAXIMA_HOME = $(HOME)/.maxima

# prefix
P = co
