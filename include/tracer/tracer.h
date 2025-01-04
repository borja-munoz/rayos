#ifndef _TRACER_H
#define _TRACER_H

#include "../scene.h"
#include "../bitmap.h"
#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/vector3D.h"
#include "../util/light.h"
#include "../util/material.h"
#include "../geom/primitive.h"
#include "brdf.h"
#include "../util/camera.h"
#include "../util/util.h"

#include <memory>

// Struct to store intersection information
typedef struct
{
	std::shared_ptr<Point3D> hitPoint; 	// Hit Point between Ray and Scene
	int nearestObject;		       		    // Index for nearest object intersected by the ray
	std::shared_ptr<Vector3D> normal;		// Object normal in the point intersected by the ray
} HitPoint;

// Struct to store render stats
typedef struct RS {
  unsigned int numberObjects = 0;
  unsigned int numberTriangles = 0;
  unsigned int numSampleRays = 0;
  unsigned int numShadowRays = 0;
  unsigned int numRayTriangleTests = 0;
  unsigned int numRayTriangleIntersections = 0;
} RenderStats;

// Struct to store execution time measurements
typedef struct TS {
  double timeGetHitPoint = 0;
  double timeDirectLighting = 0;
  double timeIndirectLighting = 0;
  double timeMutuallyVisible = 0;
  double timeCalculateRadiance = 0;
  double timePixel = 0;
} TimeStats;

class Tracer
{
  protected:
	// Pointer to function calculating the BRDF
	Color (*BRDF)(std::shared_ptr<Material> m, 
                  Vector3D N, 
                  std::shared_ptr<Vector3D> L, 
                  std::shared_ptr<Vector3D> V);
	// Pointer to random number generator
	real (*getRandomNumber)(real x, real y);
	std::shared_ptr<HitPoint> getHitPoint(std::shared_ptr<Ray> r, std::shared_ptr<Scene> s);

  protected:
  RenderStats renderStats;
  TimeStats timeStats;

  public:
	Tracer();
	Tracer(BRDFtype t);
	virtual std::shared_ptr<Bitmap> trace(std::shared_ptr<Scene> s) = 0;
  RenderStats getRenderStats();
  TimeStats getTimeStats();
};


#endif
