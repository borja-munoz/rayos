#ifndef _RECT_LIGHT_H
#define _RECT_LIGHT_H

#include "../geom/quad.h"
#include "point3D.h"
#include "util.h"
#include "light.h"


//-------------------------------------------------------------------------------

class RectLight : public Light
{
	Quad *location;

  public:
 
	RectLight();
	RectLight(Quad *location, real *color, real intensity);
	RectLight(RectLight &l);
	RectLight *copy(void);
	~RectLight();
	Quad *getLocation(void);
	void setLocation(Quad *c);
	Point3D *getSamplePoint(void);
	real getArea(void);
	Vector3D *getNormal(void);
};


//-------------------------------------------------------------------------------

#endif
