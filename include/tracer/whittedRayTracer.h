#ifndef _WHITTED_RAY_TRACER_H
#define _WHITTED_RAY_TRACER_H

#include "tracer.h"

class WhittedRayTracer : public Tracer
{
	real *traceRay(Ray *r, Scene *e);

  public:
	WhittedRayTracer();
	WhittedRayTracer(BRDFtype t);
	Bitmap *trace(Scene *e);
};


#endif
