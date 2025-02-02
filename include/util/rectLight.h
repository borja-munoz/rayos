#ifndef _RECT_LIGHT_H
#define _RECT_LIGHT_H

#include "../geom/quad.h"
#include "point3D.h"
#include "util.h"
#include "light.h"

#include <memory>

class RectLight : public Light
{
	Quad location;

  public:
 
	RectLight();
	RectLight(Quad location, Color color, real intensity);
	Quad getLocation(void);
	void setLocation(Quad c);
	Point3D getSamplePoint(void) const;
	real getArea(void) const;
	Vector3D getNormal(void) const;
};


//-------------------------------------------------------------------------------

#endif
