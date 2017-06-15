// csc473, assignment 1 sample file (RIGHT HANDED)

camera {
    location  <-5, 0, 5>
    up        <0,  1,  0>
    right     <.94, 0, .94>
    look_at   <0, 0, 0>
}


light_source {
    <20, 0, -3> color rgb <1.5, 1.5, 1.5>
    area_light
    <0, 0, 2>, <0, 3, 0>, 10, 15
}

light_source {
    <0, 5, 0> color rgb <0.4, 0.4, 0.4>
    area_light
    <1, 0, 0>, <0, 0, 1>, 4, 4
}

// Floor
plane {<0, 1, 0>, -4
    pigment {color rgb <0.2, 0.3, 0.4>}
    finish {ambient 0.4 diffuse 0.8}
}

// Wall
plane {<0, 0, 1>, -8
    pigment {color rgb <0.95, 0.95, 0.85>}
    finish {ambient 0.4 diffuse 0.8}
}

// Ceiling
plane {<0, 1, 0>, 6
    pigment {color rgb <0.95, 0.95, 0.85>}
    finish {ambient 0.4 diffuse 0.8}
}

// Window
box {<-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
    pigment {color rgb <0.95, 0.95, 0.85>}
    finish {ambient 0.4 diffuse 0.8}
    scale <1, 20, 4>
    translate <8, 2, -6>
}

box {<-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
    pigment {color rgb <0.95, 0.95, 0.85>}
    finish {ambient 0.4 diffuse 0.8}
    scale <1, 20, 10>
    translate <8, 2, 3>
}

box {<-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
    pigment {color rgb <0.95, 0.95, 0.85>}
    finish {ambient 0.4 diffuse 0.8}
    scale <1, 4, 2>
    translate <8, 5.1, -3>
}

// Props

sphere {<0, 0, 0>, 1
    pigment {color rgb <0.1, 0.1, 1.0>}
    finish {ambient 0.2 diffuse 0.4 specular 1.0}
    scale <0.4, 0.4, 0.4>
    translate <5, -3.61, -2>
}

// Block
box {<-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
    pigment { color rgb <1.0, 0.0, 1.0>}
    finish {ambient 0.2 diffuse 0.4}
    scale <0.4, 0.4, 0.4>
    rotate <0, 6, 0>
    translate <4, -3.81, -3>
}

box {<-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
    pigment { color rgb <0.0, 1.0, 1.0>}
    finish {ambient 0.2 diffuse 0.4}
    scale <0.4, 0.4, 0.4>
    rotate <0, 32, 0>
    translate <3.3, -3.81, -2.4>
}

box {<-0.5, -0.5, -0.5>, <0.5, 0.5, 0.5>
    pigment { color rgb <1.0, 1.0, 0.0>}
    finish {ambient 0.2 diffuse 0.4}
    scale <0.4, 0.4, 0.4>
    rotate <0, 46, 0>
    translate <3.2, -3.81, -3.4>
}
