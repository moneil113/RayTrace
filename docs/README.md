# RayTrace

This project is a physically based ray trace renderer implementing a subset of
the POV-ray scene description language. It provides supports for a number of
geometric primitives, including boxes, spheres, and planes, as well as more
complex triangle meshes.

---

# Features

The ray tracer supports the following features.

## Reflection and Refraction

* Physically-based refraction and reflection including approximations for
fresnel reflectance and energy conservation

![Refraction](img/Refraction.png)

![Reflection](img/Shiny.png)

## Antialiasing

* Antialiasing with variable number of subpixel samples

![No Antialiasing](img/NoAA.png)

![With Antialiasing](img/AA.png)

## Spatial Transformations

* Arbitrary spatial transformations of geometric objects

![Transformations](img/Transform.png)

## Bounding Volume Hierarchies

* Bounding volume hierarchies for fast ray intersection calculations for large
scenes

![Lots of Spheres](img/Gnarly.png)

## Monte Carlo Global illumination

* Monte Carlo global illumination with variable depth and samples

![Monte Carlo Global illumination](img/MonteCarlo.png)

## Soft Area Lights

* Soft lighting and shadows through use of multisample area lights

![Bunny](img/Bunny.png)

![Room](img/Room.png)
