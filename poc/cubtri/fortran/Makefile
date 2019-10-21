.POSIX:
.SUFFIXES:  .f .o

FC = gfortran
FCFLAGS = -Og  -g
LINK = $(FC)

M = \
main\

all: $M
.o:
	$(LINK) $< $(LDFLAGS) -L../lib -lcubtri -o $@

.f:
%: %.f

.f.o:; $(FC) $(FCFLAGS) $< -c

.PHONY: clean test
clean:; rm -f $M $(M:=.o)
