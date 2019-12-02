#include "colors.inc"

background { color Cyan }

camera {
  location <3,3,-3>
  look_at  <0,0,0>
}

light_source { <3,3,3> color White}

sphere {
  <0,1,0>, 1
  texture { pigment { color Yellow} }
}

plane { <0,1,0>
  , 0
  pigment {
    checker color White, color Red
  }
}