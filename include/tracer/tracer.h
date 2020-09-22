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
	std::shared_ptr<Point3D> hitPoint;     	// Hit Point between Ray and Scene
	int nearestObject;		       		    // Index for nearest object intersected by the ray
	std::shared_ptr<Vector3D> normal;		// Object normal in the point intersected by the ray
} HitPoint;


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

  public:
	Tracer();
	Tracer(BRDFtype t);
	virtual std::shared_ptr<Bitmap> trace(std::shared_ptr<Scene> s) = 0;
};


#endif
