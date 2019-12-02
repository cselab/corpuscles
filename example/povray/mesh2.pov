#include "colors.inc"
#include "textures.inc"
global_settings { assumed_gamma 1.0 }
background { color Black }

mesh2 {
  #include "q.inc"
  texture {
    finish {
      diffuse 0.9
    }
    normal  { agate 0.1 scale 1/2 }
  }
}

camera {
    location <0, 0, 3>
    look_at  <0, 0, 0>
    right  <-1.33,0,0>
    focal_point <0, 0, 0.75>
    aperture 0.1     // 0.05 ~ 1.5
    blur_samples 100 // 4 ~ 100
    confidence 0.9   // 0 ~ 1
    variance 1/128   // 1/64 ~ 1/1024 ~
}

light_source { <1, 5, 0> color White }

plane { 
  <0,0,1>,
  -2
  pigment {
    checker color Black, color White
  }
}
