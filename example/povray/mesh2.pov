#include "colors.inc"
background { color Black }

mesh2 {
  #include "q.inc"
  pigment { color Red }
}

camera {
    location <0, 0, -3>
    look_at  <0, 0,  0>
}

light_source { <2, 4, -3> color Red}