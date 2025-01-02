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
- Number of sample rays (ignored if Whitted raytracer is used)
- Number of shadow rays (ignored if Whitted raytracer is used)

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

## ToDo

- [x] Include the light as a visible object (emitted radiance)
- [x] Improve performance by making the Vector3D member functions inline
- [x] Implement indirect lighting for the `StochasticRayTracer`
- [x] Switch all the regular pointers to smart pointers (shared_ptr)
- [x] Fix mutuallyVisible method on the Scene class
- [x] Optimize the code by using stack objects instead of heap objects where appropriate
- [x] Profile the code using Valgrind https://web.stanford.edu/class/archive/cs/cs107/cs107.1194/resources/callgrind
- [ ] Implement the TriangleMesh primitive, including intersection with rays [scratchapixel](https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-polygon-mesh)
- [ ] Improve performance using a space partitioning technique like Bounding Volume Hierarchies
- [ ] Use [Assimp](http://assimp.org/) for reading 3D model file formats
- [ ] Test using more complex models
- [ ] Implement Jensen's Photon Mapping
- [ ] Read the scene data from PBRT format using https:/github.com/ingowald/pbrt-parser
- [ ] Read the scene data from glTF format using https://github.com/syoyo/tinygltf 
- [ ] Implement Blinn BRDF

## Smart Pointers vs Stack Objects

Using mostly stack objects we observe this behaviour:

- For output images up to 300x300, the generation is quite fast (< 1 sec)
- For output images from 400x400, there is a big slowdown (> 5 sec) and, when profiling, we find there are lots of calls to tiny_free_detach_region and tiny_free_reattach_region. It seems we are hitting some stack limit and then there is lot of memory movement that slowdowns everything.