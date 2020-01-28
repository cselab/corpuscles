#include <math.h>
#include <stdio.h>
#include <real.h>

static real
tnn(real q, real z, real z0, real W)
{
    return \
	(cosh(q*z)*(q*(exp(-W*q)*sinh(W*q)+W*q)*z0*sinh(q*z0)
		    -(exp(-W*q)*sinh(W*q)+W*q*(W*q+1))*cosh(q*z0)))
	/(cosh(W*q)*sinh(W*q)+W*q)
	+(q*z*sinh(q*z)*((exp(-W*q)*sinh(W*q)+W*q)*cosh(q*z0)-q*z0*sinh(q*z0)))
	/(cosh(W*q)*sinh(W*q)+W*q)
	+(sinh(q*z)*(q*(exp(-W*q)*cosh(W*q)+W*q)*z0*cosh(q*z0)
		     -(exp(-W*q)*cosh(W*q)+W*q*(W*q+1))*sinh(q*z0)))
	/(cosh(W*q)*sinh(W*q)-W*q)
	+(q*z*cosh(q*z)*((exp(-W*q)*cosh(W*q)+W*q)*sinh(q*z0)-q*z0*cosh(q*z0)))
	/(cosh(W*q)*sinh(W*q)-W*q);
}

int
main()
{
    printf("%g\n", tnn(1.4, 1.2, 1.8, 1.0));
}
