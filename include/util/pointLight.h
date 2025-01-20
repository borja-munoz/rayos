#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "light.h"
#include "point3D.h"

#include <memory>


//-------------------------------------------------------------------------------

class PointLight : public Light
{
	Point3D location;

  public: 
	PointLight();
	PointLight(Point3D location, Color color, real intensity);
	Point3D getLocation(void) const;
	void setLocation(Point3D p);
	Point3D getSamplePoint(void) const;
	real getArea(void) const;
	Vector3D getNormal(void) const;
};


//-------------------------------------------------------------------------------

#endif
