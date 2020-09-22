#ifndef _RECT_LIGHT_H
#define _RECT_LIGHT_H

#include "../geom/quad.h"
#include "point3D.h"
#include "util.h"
#include "light.h"

#include <memory>

class RectLight : public Light
{
	std::shared_ptr<Quad> location;

  public:
 
	RectLight();
	RectLight(std::shared_ptr<Quad> location, std::shared_ptr<Color> color, real intensity);
	std::shared_ptr<Quad> getLocation(void);
	void setLocation(std::shared_ptr<Quad> c);
	std::shared_ptr<Point3D> getSamplePoint(void);
	real getArea(void);
	std::shared_ptr<Vector3D> getNormal(void);
};


//-------------------------------------------------------------------------------

#endif
