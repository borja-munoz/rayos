#include "tracer/stochasticRayTracer.h"
#include "util/chrono.h"

#include <sstream>

StochasticRayTracer::StochasticRayTracer()
{
  sampleRays = 4;
  shadowRays = 4;
  indirectRays = 16;
}

StochasticRayTracer::StochasticRayTracer(
  unsigned int sampleRays, 
  unsigned int shadowRays,
  unsigned int indirectRays,
  bool useBVH)
{
  this->sampleRays = sampleRays;
  this->shadowRays = shadowRays;
  this->indirectRays = indirectRays;
  this->useBVH = useBVH;
}

StochasticRayTracer::StochasticRayTracer(BRDFtype t)
    : Tracer(t)
{
  sampleRays = 4;
  shadowRays = 1;
}

std::shared_ptr<Bitmap> StochasticRayTracer::trace(std::shared_ptr<Scene> s)
{
  unsigned int rx, ry;
  Color rad, radiance;
  std::shared_ptr<Bitmap> im;
  Camera cam;
  Point3D viewer;
  vector<Ray> eyeRays;
  Ray eyeRay;
  vector<real> probLight;

  if (useBVH)
  {
    s->buildBVH();
  }

  cam = s->getCamera();

  viewer = cam.getLocation();

  cam.getResolution(rx, ry);
  im = std::make_shared<Bitmap>(rx, ry);

  // We pre-calculate the probability of points in each light source (uniform distribution)
  // prob(y) = prob(k) * prob(y|k)
  // y = Point within light source
  // k = k light source
  for (unsigned int i = 0; i < s->getNumberLights(); i++)
    probLight.push_back((1.0 / s->getNumberLights()) * (1.0 / s->getLight(i)->getArea()));
  probLight[0] = 1.0;

  // We loop through the entire viewplane
  // Parallel for used for an image of 640x640 pixels
  // with each iteration working on a 16x16 tile
  int tileSize = 16; 
  int tileRows = ry / tileSize;
  int tileColumns = rx / tileSize;
  int totalTiles = tileRows * tileColumns;

  std::atomic<int> completedTiles{0};  // Atomic counter for progress tracking

  // Timer for progress tracking
  auto startTime = std::chrono::high_resolution_clock::now();

  // Print three blank lines at the start so we can overwrite them later
  std::cout << "\n\n\n";

// Important to define thread shared and private variables
// to avoid data races.
// With collapse we are parallelizing the two outer loops.
// I've tried dynamic scheduling, that let threads pick up 
// new tiles dynamically as they finish their work, 
// but the execution time has increased. Static scheduling
// works better because in a simple scene, there is no 
// significant computing time differences among tiles.
// Increasing the tile size from 16x16 to 32x32 to reduce
// thread synchronization overhead has not improved the
// execution time either.
// #pragma omp parallel for \
//   shared(cam, s, viewer, probLight) \
//   private(eyeRay, eyeRays, rad, radiance) \
//   collapse(2)
  // schedule(dynamic, 1)   

  for (int tileRow = 0; tileRow < tileRows; tileRow++) 
  {
    for (int tileColumn = 0; tileColumn < tileColumns; tileColumn++) 
    {

      // Only the two outer loops are parallelized, these inner
      // loops are executed completely in the same thread

      for (int xTile = 0; xTile < tileSize; xTile++) 
      {
        for (int yTile = 0; yTile < tileSize; yTile++) 
        {
          int pixelX = xTile + tileRow * tileSize;
          int pixelY = yTile + tileColumn * tileSize;

          std::cout << "\n\nCalculating radiance for pixel: (" << pixelX << ", " << pixelY << "\n\n";

          // Chrono *c = new Chrono();
          // c->start();

          if (this->sampleRays == 1)
          {
            eyeRay = cam.getEyeRay(pixelX, pixelY);

            radiance = traceRay(eyeRay, s, probLight, viewer);

            this->sampleRaysTraced.fetch_add(1); // Single ray traced            
          }
          else
          {
            // We send several rays for each pixel to reduce
            // aliasing and improve image quality
            eyeRays = cam.getSampleEyeRays(pixelX, pixelY, this->sampleRays);
            radiance = Color(0, 0, 0);
            for (unsigned int k = 0; k < this->sampleRays; k++)
            {
              // _CrtMemState s1, s2, s3;
              // _CrtMemCheckpoint(&s1);

              cout << "\n\nCalculating radiance for sample ray " << k << "\n\n";

              rad = traceRay(eyeRays[k], s, probLight, viewer);

              this->sampleRaysTraced.fetch_add(1); // Count each traced ray

              for (unsigned int x = 0; x < 3; x++)
                radiance += rad;

              //_CrtMemCheckpoint(&s2);
              // if (_CrtMemDifference(&s3, &s1, &s2))
              //	_CrtMemDumpStatistics(&s3);
            }

            radiance /= this->sampleRays;
          }

          im->setHDRPixel(pixelY, pixelX, radiance);

          printProgressBar(
              0, 
              0,
              0,
              sampleRaysTraced.load(),
              shadowRaysTraced.load(),
              indirectRaysTraced.load(),
              0
          );
            
          return(im);
          // c->stop();
          // this->timeStats.timePixel += c->value() * 1000;
          // cout << "Pixel elapsed time = " << c->value() * 1000 << " milliseconds\n";
        }
      }

      // Update the progress counter after processing one tile
      int tilesProcessed = completedTiles.fetch_add(1) + 1;

      // Display progress every 5% or at the end
      if (tilesProcessed % (totalTiles / 20) == 0 || tilesProcessed == totalTiles) 
      {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - startTime;
        double avgTimePerTile = elapsed.count() / tilesProcessed;
        double remainingTime = avgTimePerTile * (totalTiles - tilesProcessed);

        // Calculate rays per second (millions of rays per second)
        double raysPerSec = (sampleRaysTraced.load() + shadowRaysTraced.load() + indirectRaysTraced.load()) / (elapsed.count() * 1e6);

        #pragma omp critical
        {
            printProgressBar(
                static_cast<double>(tilesProcessed) / totalTiles, 
                elapsed.count(),
                remainingTime,
                sampleRaysTraced.load(),
                shadowRaysTraced.load(),
                indirectRaysTraced.load(),
                raysPerSec
            );
        }
      }
    }
    // if (i % 10 == 0)
    //   cout << "Completed = " << i * 100 / ry << "%" << endl;
  }

  // cout << "Time Get Hit Point: " << this->timeStats.timeGetHitPoint << " ms\n";
  // cout << "Time Radiance: " << this->timeStats.timeCalculateRadiance << " ms\n";
  // cout << "Time Mutually Visible: " << this->timeStats.timeMutuallyVisible << " ms\n";
  // cout << "Time Pixel: " << this->timeStats.timePixel << " ms\n";

  return (im);
}

Color StochasticRayTracer::traceRay(const Ray& r,
                                    std::shared_ptr<Scene> s,
                                    std::vector<real> probLight,
                                    Point3D viewer)
{
  Color radiance;
  Vector3D radianceDirection;

  // The RayTrace must intersect the ray with all the objects
  // in the scene and calculate the nearest to the viewer.
  // Finally, we will evaluate a local BRDF in the hitpoint.

  auto h = this->getHitPoint(r, s);

  // If we have a hitpoint, we must calculate radiance emitted in the viewer direction
  if (h)
  {
    // Direction where we need to find the radiance
    radianceDirection = viewer.substract((*h).hitPoint);
    radianceDirection.normalize();

    radiance = this->calculateRadiance(*h, radianceDirection, s, probLight, 0);
  }
  else
    radiance = Color(0, 0, 0);

  return (radiance);
}

// Calculates reflected radiance from a given point in a given direction
// h          : Point
// dir        : Direction
// nearest    : Nearest intersected object index
// normal     : Nearest object normal in the hitpoint
Color StochasticRayTracer::calculateRadiance(HitPoint h,
                                             Vector3D dir,
                                             std::shared_ptr<Scene> s,
                                             std::vector<real> probLight,
                                             int depth)
{
  // Radiance is composed of object emitted radiance (if the object is a light source)
  // plus object reflected radiance.
  // Reflected radiance is the sum of direct lighting from light source plus
  // indirect light
  // Summary:
  // Radiance = Emitted Radiance + Reflected Radiance
  // Reflected Radiance = Direct Lighting + Indirect Lighting

  Color radiance;
  Color emittedRadiance;
  Color reflectedRadiance;
  Color directLighting;
  Color indirectLighting;

  if (depth >= MAX_DEPTH) 
  {
    return Color(0, 0, 0); // Stop recursion
  }

  emittedRadiance = this->emittedRadiance(s, h);

  directLighting = this->directLighting(s, h, probLight, dir);

  indirectLighting = this->indirectLighting(s, h, probLight, depth + 1);

  reflectedRadiance = directLighting + indirectLighting;
  // reflectedRadiance = this->indirectLightHeatmap(indirectLighting * 10.0);

  radiance = emittedRadiance + reflectedRadiance;

  return (radiance);
}

// Calculates emitted radiance
Color StochasticRayTracer::emittedRadiance(std::shared_ptr<Scene> s,
                                           HitPoint h)
{
  Color radiance;
  real ke;

  const Material &mat = h.object->getMaterial();
  ke = mat.getKe();
  const Color &matColor = mat.getColor();
  if (ke > 0)
  {
    radiance = matColor * ke;
  }
  else
  {
    radiance = Color(0, 0, 0);
  }

  return (radiance);
}

// Calculates direct lighting given a hitpoint
Color StochasticRayTracer::directLighting(std::shared_ptr<Scene> s,
                                          HitPoint h,
                                          std::vector<real> probLight,
                                          Vector3D dir)
{

  // In order to calculate direct lighting, we send rays to the light sources (shadow rays)
  // We use these rays to calculate the contribution from each of the light sources
  // to lighting in a given point.
  // Light source selection and point selection within the light source is probabilistic,
  // meaning than depending on the probability density function used, the results
  // will be different.
  // Initially, we are going to use uniform probability functions both for selecting
  // the light source and the point within the light source. It is the simplest one
  // and gives the worst results.

  Color radiance(0, 0, 0);
  Color rad;
  int lightIndex;
  Point3D lightPoint;
  real distancePointLight, geometryTerm;
  bool lightVisible;
  Vector3D L, lightNormal;
  std::shared_ptr<Light> selectedLight;

  // Use raw pointer or reference to avoid shared_ptr overhead
  const Scene &scene = *s;
  
  for (unsigned int i = 0; i < this->shadowRays; i++)
  {
    this->shadowRaysTraced.fetch_add(1);

    cout << "Calculating radiance for shadow ray " << i << "\n\n";

    // Light source selection
    lightIndex = (int)getRandomInt(0, scene.getNumberLights() - 1);

    // Light point selection
    const Light &selectedLight = *scene.getLight(lightIndex);
    lightPoint = selectedLight.getSamplePoint();

    // Check if we can see the light point from the hitpoint
    // Avoid shadow acne by displacing the hitpoint a bit in the direction of the normal
    lightVisible = scene.mutuallyVisible(
      h.hitPoint + h.normal * 1e-4, 
      lightPoint
    );

    // If it is visible, we calculate reflected radiance from the light source
    if (lightVisible)
    {
      // Ray from hitpoint to light point
      L = lightPoint - h.hitPoint;
      distancePointLight = L.length();
      L.normalize();

      lightNormal = selectedLight.getNormal();

      const Material &mat = h.object->getMaterial();
      rad = this->BRDF(mat, h.normal, selectedLight, L, dir);

      geometryTerm = (h.normal.dotProduct(L) * lightNormal.dotProduct(L)) /
                     (distancePointLight * distancePointLight);

      // for (unsigned int x = 0; x < 3; x++)
      // radiance[x] += (rad[x] * geometryTerm) / probLight[lightIndex];
      // radiance[x] += rad[x];
      radiance += rad;
    }
  }

  radiance /= this->shadowRays;

  return (radiance);
}

// Indirect light in a given object point
Color StochasticRayTracer::indirectLighting(std::shared_ptr<Scene> s,
                                            HitPoint h,
                                            std::vector<real> probLight,
                                            int depth)
{
  Color radiance, radIndirectRay, brdf;
  real alfa, P, random, factor;
  Vector3D psi, radianceDirection;
  Ray r;
  Material mat;
  int lightIndex;
  Point3D lightPoint;
  bool lightVisible;
  Vector3D L;
  real distancePointLight;

  // Initialize
  alfa = (real)0.5; // Alfa is the material absortion probability
  P = 1 - alfa;
  r = Ray();
  r.origin = h.hitPoint;

  // We send a ray from the hitpoint to the light point
  // We should select the light source probabilistically
  // L = lightPoint->substract(h->hitPoint);
  // L->normalize();

  // Russian roulette to stop recursion
  random = getRandomNumber(0, 1);
  //cout << "random = " << random << "\n";

  if (random < P)
  {
    std::cout << "Continuing recursion at depth: " << depth << std::endl;
    radiance = Color(0, 0, 0);
    for (unsigned int i = 0; i < this->indirectRays; i++)
    {
      this->indirectRaysTraced.fetch_add(1);

      cout << "Calculating radiance for indirect ray " << i << "\n";

      // We obtain a random direction by uniformly sampling the hemisphere
      // psi = this->getRandomDirection();

      // Instead of uniform hemisphere sampling, 
      // we can use cosine-weighted hemisphere sampling
      psi = this->getCosineWeightedDirection(h.normal);

      // We calculate the point seen from the hitpoint in direction "psi"
      r.direction = psi;
      auto hPsi = this->getHitPoint(r, s);

      // If we have a hitpoint, calculate radiance emitted in direction psi
      if (hPsi)
      {
        // Direction
        radianceDirection = h.hitPoint.substract((*hPsi).hitPoint);
        radianceDirection.normalize();

        radIndirectRay = this->calculateRadiance(*hPsi, radianceDirection, s, probLight, depth);

        // We should multiply radiance by the following factor:
        // BRDF(intersection) * cos(Nx, Psi) / pdf(Psi)
        // Nx and PSI have been normalized, so the dot product
        // is the cosine of the angle between the two vectors
        // As we uniformly sample the hemisphere, the probability density
        // function is 1 / (2 * Pi)

        // Light source selection
        lightIndex = (int)getRandomInt(0, s->getNumberLights() - 1);

        // Light point selection
        const Light &selectedLight = *s->getLight(lightIndex);
        lightPoint = selectedLight.getSamplePoint();

        // Check if we can see the light point from the hitpoint
        lightVisible = s->mutuallyVisible(hPsi->hitPoint, lightPoint);

        // If it is visible, we calculate reflected radiance from the light source
        if (lightVisible)
        {
          mat = h.object->getMaterial();

          // Ray from hitpoint to light point
          L = lightPoint.substract((*hPsi).hitPoint);
          distancePointLight = L.length();
          L.normalize();

          brdf = this->BRDF(mat, h.normal, selectedLight, L, radianceDirection);
          factor = h.normal.dotProduct(psi) / (2 * PI_RAYOS);
          radiance += radIndirectRay * brdf * factor;
        }
      }
    }

    radiance /= this->indirectRays;

    // radiance /= P;

    // cout << radiance << '\n';
  }
  else {
    radiance = Color(0, 0, 0);
    std::cout << "Russian Roulette terminated recursion at depth: " << depth << std::endl;
    //cout << "Stopping recursion\n";
  }

  return (radiance);
}

// Returns a random direction to send a ray
// From Flipcode - Code of the Day
// http://www.flipcode.com/cgi-bin/msg.cgi?showThread=COTD-RandomUnitVectors&forum=cotd&id=-1
Vector3D StochasticRayTracer::getRandomDirection(void)
{
  Vector3D dir;

  real z = getRandomNumber(0.0, 1.0); // We should use (-1, 1) for a sphere
  real a = getRandomNumber(0.0, 2 * PI_RAYOS);

  real r = sqrtf(1.0f - z * z);

  real x = r * cosf(a);
  real y = r * sinf(a);

  dir = Vector3D(x, y, z);

  return (dir);
}

Vector3D StochasticRayTracer::getCosineWeightedDirection(const Vector3D &normal)
{
    real r1 = getRandomNumber(0, 1);
    real r2 = getRandomNumber(0, 1);

    real theta = acos(sqrt(1.0 - r1));  // Cosine-weighted
    real phi = 2 * M_PI * r2;

    // Convert spherical to cartesian
    real x = cos(phi) * sin(theta);
    real y = sin(phi) * sin(theta);
    real z = cos(theta);

    // Transform from local to world space
    return localToWorld(normal, Vector3D(x, y, z));
}

Vector3D StochasticRayTracer::localToWorld(const Vector3D &N, const Vector3D &localDir) 
{
  Vector3D T, B, W;

  // Create an orthonormal basis (Tangent, Bitangent, Normal)
  
  // Choose a safe perpendicular vector
  if (fabs(N.x) > fabs(N.z)) 
  {
      T = Vector3D(-N.y, N.x, 0);  // Avoid (0,0,0)
  } 
  else 
  {
      T = Vector3D(0, -N.z, N.y);  // Alternate choice
  }

  if (ZERO(T.length())) 
  {  // Failsafe
      T = Vector3D(1, 0, 0);  // Default fallback
  }
  T.normalize();

  // Compute the bitangent
  B = N.crossProduct(T); 
  B.normalize();  

  // Transform the local direction to world space
  // W = T * localDir.x + B * localDir.y + N * localDir.z;
  W = T.product(localDir.x);
  W = W.sum(B.product(localDir.y));
  W = W.sum(N.product(localDir.z));
  W.normalize();

  // #pragma omp critical
  // {
  //   std::cout << "N: (" << N.x << ", " << N.y << ", " << N.z << ")\n";
  //   std::cout << "LocalDir: (" << localDir.x << ", " << localDir.y << ", " << localDir.z << ")\n";
  //   std::cout << "T (Before Normalization): (" << T.x << ", " << T.y << ", " << T.z << ")\n";
  //   std::cout << "B (Before Normalization): (" << B.x << ", " << B.y << ", " << B.z << ")\n";
  //   std::cout << "W (Before Normalization): (" << W.x << ", " << W.y << ", " << W.z << ")\n";
  // }

  // #pragma omp critical
  // {
  //   if (ZERO(T.length()) || ZERO(B.length())) 
  //   {
  //     std::cout << "T (Before Normalization): (" << T.x << ", " << T.y << ", " << T.z << ")\n";
  //     std::cout << "T length: " << T.length() << "\n";
  //     std::cout << "B (Before Normalization): (" << B.x << ", " << B.y << ", " << B.z << ")\n";
  //     std::cout << "B length: " << B.length() << "\n";
  //     std::cerr << "ERROR: One of the basis vectors is (0,0,0)!\n";
  //     exit(1);
  //   }
  // }

  // #pragma omp critical
  // {
  //   std::cout << "N: (" << N.x << ", " << N.y << ", " << N.z << ")\n";
  //   std::cout << "LocalDir: (" << localDir.x << ", " << localDir.y << ", " << localDir.z << ")\n";
  //   std::cout << "T (Before Normalization): (" << T.x << ", " << T.y << ", " << T.z << ")\n";
  //   std::cout << "B (Before Normalization): (" << B.x << ", " << B.y << ", " << B.z << ")\n";
  //   std::cout << "W (Before Normalization): (" << W.x << ", " << W.y << ", " << W.z << ")\n";
  // }

  if (std::isnan(W.x) || std::isnan(W.y) || std::isnan(W.z) ||
      std::isinf(W.x) || std::isinf(W.y) || std::isinf(W.z)) 
  {
    std::cerr << "ERROR: W has invalid values! (" << W.x << ", " << W.y << ", " << W.z << ")\n";
    exit(1);  // Forcefully stop execution to debug
  }
  // If I return a new fixed vector instead of W, the program does not crash,
  // even if I'm still doing the calculation for W
  // return Vector3D(0, 0.5, 0.5);
  return Vector3D(W.x, W.y, W.z);

  // return W;
}

// Method to render a heatmap of the indirect light
Color StochasticRayTracer::indirectLightHeatmap(Color indirect)
{
    // Get intensity from indirect radiance
    real intensity = (indirect.getR() + indirect.getG() + indirect.getB()) / 3.0;

    // Normalize intensity (clamp to [0, 1] for display)
    intensity = std::min(1.0, std::max(0.0, intensity));

    // Simple heatmap: blue → green → yellow → red
    if (intensity < 0.25) {
        // 0.00 to 0.25: Blue to Cyan
        return Color(0, intensity * 4, 1);
    } else if (intensity < 0.5) {
        // 0.25 to 0.50: Cyan to Green
        return Color(0, 1, 1 - (intensity - 0.25) * 4);
    } else if (intensity < 0.75) {
        // 0.50 to 0.75: Green to Yellow
        return Color((intensity - 0.5) * 4, 1, 0);
    } else {
        // 0.75 to 1.00: Yellow to Red
        return Color(1, 1 - (intensity - 0.75) * 4, 0);
    }
}