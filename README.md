# rayos
Implementation of several raytracing algorithms

## Project structure
The project uses the file structure described in the following web page and the Makefile is a standard Unix Makefile:
[https://hiltmon.com/blog/2015/09/28/the-simple-c-plus-plus-makefile-executable-edition/](https://hiltmon.com/blog/2015/09/28/the-simple-c-plus-plus-makefile-executable-edition/)

## Executable
The build process generates an executable called "rayos" in the "bin" folder. This executable is called with the following arguments:

- Tracing Algorithm
  - w: Turner Whitted Raytracer
  - s: Stochastic Raytracer

The program instantiates a `Scene` and the calls the raytracer `trace` method passing the scene as the parameter.

The `trace` method returns a `Bitmap` object that is then written to a TGA file. Finally the file is opened using the operating system tools.

## Raytracing algorithms

The raytracing algorithms `WhittedRayTracer` and `StochasticRayTracer` extends the abstract `Tracer`class. This class only implement the base constructors and the `getHitPoint` method.

> ToDo: Implement indirect lighting for the `StochasticRayTracer`

The `Tracer` class initializes the BRDF object and the random number generator. There are several random number generators in the `util` folder. 

## Materials & BRDF

A `Material` is defined by:

- Color
- Ambient component (ka)
- Diffuse component (kd)
- Specular component (ks)

The BRDF (Bidirectional Reflectance Distribution Functions) calculates the radiance for each pixel in the scene by using the normal to the object in the hitpoint, the vector to the light, the vector to the viewer, and the material properties.

Currently the application supports only the `PhongBRDF`.

> ToDo: Implement Blinn BRDF

## Primitives

A `Primitive` has an `objectType` and a material. All the primitives extend the abstract `Primitive` class and need to implement the copy constructor, the destructor and the intersect methods.

Currently the application supports spheres, triangles and quads.

## Scenes

An `Scene` includes the following elements:

- A collection of lights
- A collection of objects: lights and primitives
- A camera

> ToDo: Read a scene from a 3D file format

## Lights

A `Light` has a color and an intensity. It can be sampled to get a point using the `getSamplePoint` method. The application supports two different types of lights, that extend the abstract `Light` class:

- `PointLight`. Returns always the same sample point.
- `RectLight`. Area light.

## Camera

A `Camera` is defined using the following parameters:

- `location`. Camera location
- `view`. Look-At Point
- `up`. Orientation
- `distance`. Distance to viewplane
- `hFov`, `vFov`. Horizontal and Vertical Field of View
- `xRes`, `yRes`. Viewplane resolution (image size)
- `xCam`, `yCam`, `zCam`. Camera coordinate system
- `oViewport`. Viewplane viewport origin
- `uViewport`, `vViewport`. Viewplane viewport axes
- `pixelWidth`, `pixelHeight`. Pixel dimensions

There is a `getEyeRay` method for getting an eye ray for non-stochastic algorithms and a `getSampleEyeRays` method for stochastic algorithms.