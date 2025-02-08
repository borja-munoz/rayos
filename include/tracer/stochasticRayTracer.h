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

	Color traceRay(const Ray& r, 
                 std::shared_ptr<Scene> s, 
                 std::vector<real> probLight, 
                 Point3D viewer);
	Color calculateRadiance(HitPoint h, 
                          Vector3D dir, 
                          std::shared_ptr<Scene> s, 
                          std::vector<real> probLight);
	Color emittedRadiance(std::shared_ptr<Scene> s, 
                        HitPoint h);
	Color directLighting(std::shared_ptr<Scene> s, 
                       HitPoint h, 
                       std::vector<real> probLight, 
                       Vector3D dir);
	Color indirectLighting(std::shared_ptr<Scene> s, 
                         HitPoint h, 
                         std::vector<real> probLight);
	Vector3D getRandomDirection(void);

  public:
	StochasticRayTracer();
	StochasticRayTracer(unsigned int sampleRays, unsigned int shadowRays, bool useBVH);
	StochasticRayTracer(BRDFtype t);
	std::shared_ptr<Bitmap> trace(std::shared_ptr<Scene> s);
};


#endif
