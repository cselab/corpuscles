#include "colors.inc"
#include "textures.inc"
global_settings { assumed_gamma 1.0 }
background { color Black }

plane {
  <0,0,1>, -2
  pigment {
    checker color Black, color White
  }
}

#macro CoTexture(A, B, C)
  texture {
    pigment {
      rgb <A,B,C> transmit 0.3
    }
    finish {
      ambient 0.1 diffuse 0.9
    }
    normal {
      agate 0.1 scale 1/2
    }
  }
#end
mesh2 {
  #include "q.inc"
}

camera {
    location <0, 0, 3>
    look_at  <0, 0, 0>
    right  <-1.33,0,0>
    focal_point <0, 0, 0>
    aperture 0.05     // 0.05 ~ 1.5
    blur_samples 100 // 4 ~ 100
    confidence 0.9   // 0 ~ 1
    variance 1/128   // 1/64 ~ 1/1024 ~
}

light_source { <1, 5, 0> color White }
