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
- [x] Implement the TriangleMesh primitive, including intersection with rays [scratchapixel](https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-polygon-mesh)
- [x] Parallelize the code with OpenMP
- [x] Improve triangle mesh intersection avoid the creation of triangles in every intersection. 
- [x] Improve triangle intersection using the Moller-Trumbore algorithm
- [x] Improve performance using a space partitioning technique like Bounding Volume Hierarchies (tinybvh from Jacco Bikker)
- [ ] Read the scene data from PBRT format using https:/github.com/ingowald/pbrt-parser
- [ ] Improve performance using importance sampling or stratified sampling to reduce the number of sample rays traced
- [ ] Use [Assimp](http://assimp.org/) for reading 3D model file formats
- [ ] Test using more complex models
- [ ] Implement Jensen's Photon Mapping
- [ ] Implement additional BRDF (Blinn, Microfacet...)

## Performance

### Parallelization

Being able to use all the cores in the computer has a big impact (not linear but close). I'm dividing the image in small tiles so we can take advantage of locality and there's not so much thread synchronization when compared to parallelize at the pixel level. 

Tiles shouldn't be big because we don't want threads to spend too much time in each unit of work. I've tested with 16x16 and 32x32 tiles and the performance was better with the smaller tilesize.

### Precomputation

When possible we should precompute things. Generating the image for the scene with the polysphere (5 divisions) took 40 seconds (M3) without precomputation and 18 seconds when precomputing the triangles.

### Smart pointers vs objects

Working with smart pointers make the code less readable and also introduces overhead when needing to allocate memory and dereference variables. When possible we should just use objects because performance will be better.

We need to be careful when using stack objects because, if they are large, you can hit the stack limit. When profiling, we can find there are lots of calls to tiny_free_detach_region and tiny_free_reattach_region (memory movement) that slowdowns everything.

So we should keep the stack objects for small objects.

### Structs vs objects

When implementing the intersection for the triangle mesh I started with a simple struct with float arrays for the vertices and normal. Moving this code to the Triangle object adds a 15% overhead on execution time. It's significant and can have a big impact when rendering complex scenes. 

I'm not sure how the memory is allocated for objects within a vector but objects should be contiguous. It is true that memory layout with a vector of structs is probably more efficient.

### Float vs double

I have a typedef with a type called `real` to switch easily between single and double precision floating point. Using single precision (float) does not reduce the image generation time and adds some artifacts to the image.

### Sampling

Adapting the number of samples depending on the area of the image should be also an important aspect for improving performance. We could reduce the number of sample rays per pixel for those areas where there are not big variations in color and we could also adapt the number of shadow rays we trace. 

We need to investigate importance and stratified sampling.

### Bounding volume hierarchies

Adding some kind of hierarchy to the scene to avoid comparing each ray with each of the scene objects is key to be able to reduce the image generation time when working with complex scenes.

I've implemented a very simple bounding box (AABB) hierarchy that split the objects using the longest axis of the bounding box. I had around 25% improvement in execution time but it should be much higher with more complex scenes.

Further improvements can be obtained with different measures:
- Using a better heuristic for splitting the objects
- Reducing the BVHNode size so it is aligned with cache line sizes

### Ray Coherence