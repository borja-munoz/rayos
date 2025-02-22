#ifndef _STOCHASTIC_RAY_TRACER_H
#define _STOCHASTIC_RAY_TRACER_H

#include "tracer.h"
#include "../util/util.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <vector>

#include "omp.h"

using namespace std;

const int MAX_DEPTH = 10;

class StochasticRayTracer : public Tracer
{
	unsigned int sampleRays;                     // Number of rays per pixel
	unsigned int shadowRays;					           // Number of shadow rays
	unsigned int indirectRays;                   // Number of indirect rays

  std::atomic<long long> sampleRaysTraced{0};
  std::atomic<long long> shadowRaysTraced{0};
  std::atomic<long long> indirectRaysTraced{0};

	Color traceRay(const Ray& r, 
                 std::shared_ptr<Scene> s, 
                 std::vector<real> probLight, 
                 Point3D viewer);
	Color calculateRadiance(HitPoint h, 
                          Vector3D dir, 
                          std::shared_ptr<Scene> s, 
                          std::vector<real> probLight,
                          int depth);
	Color emittedRadiance(std::shared_ptr<Scene> s, 
                        HitPoint h);
	Color directLighting(std::shared_ptr<Scene> s, 
                       HitPoint h, 
                       std::vector<real> probLight, 
                       Vector3D dir);
	Color indirectLighting(std::shared_ptr<Scene> s, 
                         HitPoint h, 
                         std::vector<real> probLight,
                         int depth);
	Vector3D getRandomDirection(void);
  Vector3D getCosineWeightedDirection(const Vector3D &normal);
  Vector3D localToWorld(const Vector3D &N, const Vector3D &localDir);
  
  Color indirectLightHeatmap(Color indirect);

  public:
	StochasticRayTracer();
	StochasticRayTracer(unsigned int sampleRays, 
                      unsigned int shadowRays, 
                      unsigned int indirectRays,
                      bool useBVH);
	StochasticRayTracer(BRDFtype t);
	std::shared_ptr<Bitmap> trace(std::shared_ptr<Scene> s);
};


#endif
