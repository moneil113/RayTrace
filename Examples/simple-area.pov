// csc473, assignment 1 sample file (RIGHT HANDED)

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {
   <0, 5, 0> color rgb <1.5, 1.5, 1.5>
   area_light
   <1, 0, 0>, <0, 0, 1>, 10, 10
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4}
}

plane {<0, 1, 0>, -4
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}

triangle {
   <-0.5, 5.1, -0.5>, <0.5, 5.1, -0.5>, <-0.5, 5.1, 0.5>
   pigment {color rgb <1, 1, 1>}
   finish {ambient 1 diffuse 0}
}

triangle {
   <0.5, 5.1, -0.5>, <0.5, 5.1, 0.5>, <-0.5, 5.1, 0.5>
   pigment {color rgb <1, 1, 1>}
   finish {ambient 1 diffuse 0}
}
