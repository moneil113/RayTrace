# RayTrace

## Soft Lighting

I chose to implement soft lighting using multi-sample area lights. I felt this
feature would do the most to help create interesting images without requiring an
extensive redesign of software components.

My research of soft area lights included the section on soft shadows in the
Shirley textbook and the [pov-ray specification for area lights](http://www.povray.org/documentation/view/3.6.0/313/).

Until now, lights have been represented as as single points. The result is that
shadows cast by the light have hard edges.

![Hard shadows](Examples/simple-hard.png)

In reality, lights are not a single point in space, but emit light in an area
around their centers. For example, a lightbulb emits light from the entire
surface of the bulb.

I model this behavior by treating area lights as an array of point lights and
blending the light contribution of each of these samples when calculating shadows
and local colors.

Some examples images rendered with soft lighting are shown here.

![Soft shadows](Examples/simple-area.png)

![Bunny](Examples/bunny-area.png)

![Room](Examples/room-area.png)

---

## Software Design

I designed my ray tracer to have relatively small functions that belong to relevant
classes. Because functions are smaller (the longest function in the `Renderer` class is 60
lines), they are easier to read and understand. This compartmentalization also allows for
the testing of smaller pieces of code responsible for only one thing.

The functions responsible for Monte Carlo global illumination in the `Renderer` class are a
good example of this practice. If a boolean flag has been set, then the ambient term of an
object's color is determined by a call to  `monteCarloAmbient()`. In turn,
`monteCarloAmbient()` produces a number of reflected rays that are created in the
`monteCarloRay()` function. These rays are themselves seeded using cosine-weighted sample
points generated in `monteCarloSample()`.
