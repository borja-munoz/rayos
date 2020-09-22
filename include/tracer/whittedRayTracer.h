#ifndef _WHITTED_RAY_TRACER_H
#define _WHITTED_RAY_TRACER_H

#include "tracer.h"

class WhittedRayTracer : public Tracer
{
	Color traceRay(std::shared_ptr<Ray> r, std::shared_ptr<Scene> s);

  public:
	WhittedRayTracer();
	WhittedRayTracer(BRDFtype t);
	std::shared_ptr<Bitmap> trace(std::shared_ptr<Scene> e);
};


#endif
