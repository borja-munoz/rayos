#ifndef _STOCHASTIC_RAY_TRACER_H
#define _STOCHASTIC_RAY_TRACER_H

#include "tracer.h"
#include "../util/util.h"

#include <iostream>
#include <vector>

using namespace std;

class StochasticRayTracer : public Tracer
{
	unsigned int sampleRays;                     // Number of rays per pixel
	unsigned int shadowRays;					 // Number of shadow rays
	unsigned int indirectRays;                   // Number of indirect rays

	real *traceRay(Ray *r, Scene *s, real *probLight, Point3D *viewer);
	real *calculateRadiance(HitPoint *h, Vector3D *dir, Scene *e, real *probLight);
	real *emittedRadiance(Scene *s, HitPoint *h);
	real *directLighting(Scene *s, HitPoint *h, real *probLight, Vector3D *dir);
	real *indirectLighting(Scene *s, HitPoint *h, real *probLight);
	Vector3D *getRandomDirection(void);

  public:
	StochasticRayTracer();
	StochasticRayTracer(unsigned int sampleRays, unsigned int shadowRays);
	StochasticRayTracer(BRDFtype t);
	Bitmap *trace(Scene *e);
};


#endif
