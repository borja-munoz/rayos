#ifndef _STOCHASTIC_RAY_TRACER_H
#define _STOCHASTIC_RAY_TRACER_H

#include "tracer.h"
#include "../util/util.h"

#include <iostream>
#include <vector>

#include "omp.h"

using namespace std;

class StochasticRayTracer : public Tracer
{
	unsigned int sampleRays;                     // Number of rays per pixel
	unsigned int shadowRays;					 // Number of shadow rays
	unsigned int indirectRays;                   // Number of indirect rays

	Color traceRay(std::shared_ptr<Ray> r, 
                   std::shared_ptr<Scene> s, 
                   std::vector<real> probLight, 
                   std::shared_ptr<Point3D> viewer);
	Color calculateRadiance(std::shared_ptr<HitPoint> h, 
                            std::shared_ptr<Vector3D> dir, 
                            std::shared_ptr<Scene> s, 
                            std::vector<real> probLight);
	Color emittedRadiance(std::shared_ptr<Scene> s, 
                          std::shared_ptr<HitPoint> h);
	Color directLighting(std::shared_ptr<Scene> s, 
                         std::shared_ptr<HitPoint> h, 
                         std::vector<real> probLight, 
                         std::shared_ptr<Vector3D> dir);
	Color indirectLighting(std::shared_ptr<Scene> s, 
                           std::shared_ptr<HitPoint> h, 
                           std::vector<real> probLight);
	std::shared_ptr<Vector3D> getRandomDirection(void);

  public:
	StochasticRayTracer();
	StochasticRayTracer(unsigned int sampleRays, unsigned int shadowRays);
	StochasticRayTracer(BRDFtype t);
	std::shared_ptr<Bitmap> trace(std::shared_ptr<Scene> s);
};


#endif
