// csc473, assignment 4 file

camera {
  location  <0, 0, 18>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <-1.6, -2, 7>, 1.25
  pigment { color rgb <1.0, 1.0, 1.0> }
  finish {ambient 0.2 diffuse 0.4 specular 0.7 roughness 0.02 ior 1.6 reflection 0.8}
}

sphere { <1.6, -2, 8.5>, 1.25
  pigment { color rgb <1.0, 1.0, 1.0> }
  finish {ambient 0.2 diffuse 0.4 specular 0.7 roughness 0.02 ior 1.6 reflection 0.8}
}

sphere { <-2, -2, 10>, 1.25
  pigment { color rgb <1.0, 1.0, 1.0> }
  finish {ambient 0.2 diffuse 0.4 specular 0.7 roughness 0.02 ior 1.6 reflection 0.8}
}

sphere { <2, -2, 11.5>, 1.25
  pigment { color rgb <1.0, 1.0, 1.0> }
  finish {ambient 0.2 diffuse 0.4 specular 0.7 roughness 0.02 ior 1.6 reflection 0.8}
}

plane {<0, 1, 0>, -4
  pigment {color rgb <1, 0.65, 0.0>}
  finish {ambient 0.4 diffuse 0.8}
}

plane {<0, 0, 1>, -6
  pigment {color rgb <0.0, 1.0, 0.65>}
  finish {ambient 0.4 diffuse 0.8}
}
