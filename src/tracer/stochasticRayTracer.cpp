#include "stochasticRayTracer.h"
#include "util/chrono.h"

#include <sstream>

StochasticRayTracer::StochasticRayTracer()
{
  sampleRays = 4;
  shadowRays = 4;
  indirectRays = 4;
}

StochasticRayTracer::StochasticRayTracer(unsigned int sampleRays, unsigned int shadowRays)
{
  this->sampleRays = sampleRays;
  this->shadowRays = shadowRays;
  this->indirectRays = 4;
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

  s->buildBVH();

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
#pragma omp parallel for \
  shared(cam, s, viewer, probLight), \
  private(eyeRay, eyeRays, rad, radiance) \
  collapse(2)
  // schedule(dynamic, 1)   

  for (int tileRow = 0; tileRow < tileRows; tileRow++) {
    for (int tileColumn = 0; tileColumn < tileColumns; tileColumn++) {

      // Only the two outer loops are parallelized, these inner
      // loops are executed completely in the same thread

      for (int xTile = 0; xTile < tileSize; xTile++) {
        for (int yTile = 0; yTile < tileSize; yTile++) {

          int pixelX = xTile + tileRow * tileSize;
          int pixelY = yTile + tileColumn * tileSize;

          // Chrono *c = new Chrono();
          // c->start();

          if (this->sampleRays == 1)
          {
            eyeRay = cam.getEyeRay(pixelX, pixelY);

            radiance = traceRay(eyeRay, s, probLight, viewer);
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

              rad = traceRay(eyeRays[k], s, probLight, viewer);

              for (unsigned int x = 0; x < 3; x++)
                radiance += rad;

              //_CrtMemCheckpoint(&s2);
              // if (_CrtMemDifference(&s3, &s1, &s2))
              //	_CrtMemDumpStatistics(&s3);
            }

            radiance /= this->sampleRays;
          }

          im->setHDRPixel(pixelY, pixelX, radiance);

          // c->stop();
          // this->timeStats.timePixel += c->value() * 1000;
          // cout << "Pixel elapsed time = " << c->value() * 1000 << " milliseconds\n";
        }
      }
    }
    // if (i % 10 == 0)
    //   cout << "Completed = " << i * 100 / ry << "%" << endl;
  }

  cout << "Time Get Hit Point: " << this->timeStats.timeGetHitPoint << " ms\n";
  
  cout << "Time Radiance: " << this->timeStats.timeCalculateRadiance << " ms\n";
  cout << "Time Mutually Visible: " << this->timeStats.timeMutuallyVisible << " ms\n";
  
  cout << "Time Pixel: " << this->timeStats.timePixel << " ms\n";

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

  // Chrono *c1 = new Chrono();
  // c1->start();
  auto h = this->getHitPoint(r, s);
  // c1->stop();
  // this->timeStats.timeGetHitPoint += c1->value() * 1000;
  // cout << "\ngetHitPoint elapsed time = " << c1->value() * 1000 << " milliseconds\n";

  // If we have a hitpoint, we must calculate radiance emitted in the viewer direction
  if (h)
  {
    // Direction where we need to find the radiance
    // Chrono *c2 = new Chrono();
    // c2->start();
    radianceDirection = viewer.substract((*h).hitPoint);
    radianceDirection.normalize();

    radiance = this->calculateRadiance(*h, radianceDirection, s, probLight);
    // c2->stop();
    // this->timeStats.timeCalculateRadiance += c2->value() * 1000;
    // cout << "calculateRadiance elapsed time = " << c2->value() * 1000 << " milliseconds\n";
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
                                             std::vector<real> probLight)
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

  emittedRadiance = this->emittedRadiance(s, h);

  // Chrono *c1 = new Chrono();
  // c1->start();
  directLighting = this->directLighting(s, h, probLight, dir);
  // c1->stop();
  // this->timeStats.timeDirectLighting += c1->value() * 1000;
  //cout << "directLighting = " << c1->value() * 1000 << " milliseconds\n";

  // Chrono *c2 = new Chrono();
  // c2->start();
  //indirectLighting = this->indirectLighting(s, h, probLight);
  // c2->stop();
  // this->timeStats.timeIndirectLighting += c2->value() * 1000;
  // cout << "indirectLighting elapsed time = " << c2->value() * 1000 << " milliseconds\n";

  //reflectedRadiance = directLighting + indirectLighting;
  reflectedRadiance = directLighting;

  radiance = emittedRadiance + reflectedRadiance;

  return (radiance);
}

// Calculates emitted radiance
Color StochasticRayTracer::emittedRadiance(std::shared_ptr<Scene> s,
                                           HitPoint h)
{
  Color radiance;
  real ke;
  Material mat;
  Color matColor;

  std::shared_ptr<Primitive> object = h.object;
  mat = object->getMaterial();
  ke = mat.getKe();
  matColor = mat.getColor();
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

  Color radiance, rad;
  int lightIndex;
  Point3D lightPoint;
  real distancePointLight, geometryTerm;
  bool lightVisible;
  Vector3D L, lightNormal;
  Material mat;
  std::shared_ptr<Light> selectedLight;

  radiance = Color(0, 0, 0);
  for (unsigned int i = 0; i < this->shadowRays; i++)
  {
    // Light source selection
    lightIndex = (int)getRandomIntMT(0, s->getNumberLights() - 1);

    // Light point selection
    selectedLight = s->getLight(lightIndex);
    lightPoint = selectedLight->getSamplePoint();
    lightNormal = selectedLight->getNormal();

    // Ray from hitpoint to light point
    L = lightPoint.substract(h.hitPoint);
    distancePointLight = L.length();
    L.normalize();

    // Check if we can see the light point from the hitpoint
    // Chrono *c = new Chrono();
    // c->start();
    // lightVisible = s->mutuallyVisible(h->hitPoint, lightPoint);
    // Avoid shadow acne by displacing the hitpoint a bit in the direction of the normal
    lightVisible = s->mutuallyVisible(
      h.hitPoint.sum(h.normal.product(1e-4)), 
      lightPoint
    );

    // lightVisible = true;
    // c->stop();
    // this->timeStats.timeMutuallyVisible += c->value() * 1000;
    // cout << "mutuallyVisible elapsed time = " << c->value() * 1000 << " milliseconds\n";

    // If it is visible, we calculate reflected radiance from the light source
    if (lightVisible)
    {
      // - Each object must have an ID
      // - Light sources will be added to the scene always before the other objects,
      //   to have the lower IDs
      // - We will know if the nearest object is a light source by comparing the
      //   object ID with the number of light sources in the scene

      // object = e->getObject(nearestObject);
      mat = h.object->getMaterial();
      rad = this->BRDF(mat, h.normal, L, dir);

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
                                            std::vector<real> probLight)
{
  Color radiance, radIndirectRay, brdf;
  real alfa, P, random, factor;
  Vector3D psi, radianceDirection;
  Ray r;
  Material mat;
  int lightIndex;
  Point3D lightPoint;
  bool lightVisible;
  Vector3D L, lightNormal;
  std::shared_ptr<Light> selectedLight;
  real distancePointLight;

  // Initialize
  alfa = (real)0.8; // Alfa is the material absortion probability
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
    radiance = Color(0, 0, 0);
    for (unsigned int i = 0; i < this->indirectRays; i++)
    {
      // We obtain a random direction by uniformly sampling the hemisphere
      psi = this->getRandomDirection();

      // We calculate the point seen from the hitpoint in direction "psi"
      r.direction = psi;
      auto hPsi = this->getHitPoint(r, s);

      // If we have a hitpoint, calculate radiance emitted in direction psi
      if (hPsi)
      {
        // Direction
        radianceDirection = h.hitPoint.substract((*hPsi).hitPoint);
        radianceDirection.normalize();

        radIndirectRay = this->calculateRadiance(*hPsi, radianceDirection, s, probLight);

        // We should multiply radiance by the following factor:
        // BRDF(intersection) * cos(Nx, Psi) / pdf(Psi)
        // Nx and PSI have been normalized, so the dot product
        // is the cosine of the angle between the two vectors
        // As we uniformly sample the hemisphere, the probability density
        // function is 1 / (2 * Pi)

        // Light source selection
        lightIndex = (int)getRandomIntMT(0, s->getNumberLights() - 1);

        // Light point selection
        selectedLight = s->getLight(lightIndex);
        lightPoint = selectedLight->getSamplePoint();
        lightNormal = selectedLight->getNormal();

        // Ray from hitpoint to light point
        L = lightPoint.substract((*hPsi).hitPoint);
        distancePointLight = L.length();
        L.normalize();

        // Check if we can see the light point from the hitpoint
        lightVisible = s->mutuallyVisible(hPsi->hitPoint, lightPoint);

        // If it is visible, we calculate reflected radiance from the light source
        if (lightVisible)
        {
          mat = h.object->getMaterial();
          brdf = this->BRDF(mat, h.normal, L, radianceDirection);
          factor = h.normal.dotProduct(psi) / (2 * PI_RAYOS);
          radiance += radIndirectRay * brdf * factor;
        }
      }
    }

    radiance /= this->indirectRays;

    radiance /= P;

    // cout << radiance << '\n';
  }
  else {
    radiance = Color(0, 0, 0);
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
