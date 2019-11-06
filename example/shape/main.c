#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/x.h>
#include <co/vec.h>
#include <co/ten.h>
#include <co/tri.h>
#include <co/dtri.h>
#include <co/memory.h>
#include <co/punto.h>

#include "inc/util.h"
#include "inc/io.h"
#include "inc/mesh.h"
#include "inc/march.h"

int main() {
  
  ini("/dev/stdin");

  march_integrate_velocity_verlet();
  
  fin();
  
  return CO_OK;
  
}
