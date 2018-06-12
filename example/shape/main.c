#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/ten.h>
#include <he/tri.h>
#include <he/dtri.h>
#include <he/memory.h>
#include <he/punto.h>

#include "inc/util.h"
#include "inc/io.h"
#include "inc/mesh.h"
#include "inc/march.h"

int main() {
  
  ini("/dev/stdin");

  march_integrate_velocity_verlet();
  
  fin();
  
  return HE_OK;
  
}
