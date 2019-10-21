#include <tgmath.h>
#include <stdio.h>

#include "real.h"

#include "co/argv.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/util.h"
#include "co/kernel.h"

#define pi (3.141592653589793)
#include "kernel/quintic.inc"
#include "kernel/wendland6.inc"
#include "kernel/yang.inc"
#include "kernel/gauss.inc"

#include "kernel/main.inc"
