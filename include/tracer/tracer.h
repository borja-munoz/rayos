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


// Struct to store intersection information
typedef struct
{
	Point3D *hitPoint;     			// Hit Point between Ray and Scene
	int nearestObject;				// Index for nearest object intersected by the ray
	Vector3D *normal;				// Object normal in the point intersected by the ray
} HitPoint;


class Tracer
{
  protected:
	// Pointer to function calculating the BRDF
	real * (*BRDF)(Material *m, Vector3D N, Vector3D *L, Vector3D *V);
	// Pointer to random number generator
	real (*getRandomNumber)(real x, real y);
	HitPoint *getHitPoint(Ray *r, Scene *e);

  public:
	Tracer();
	Tracer(BRDFtype t);
	virtual Bitmap *trace(Scene *e) = 0;
};


#endif
