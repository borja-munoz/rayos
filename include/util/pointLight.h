#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "light.h"
#include "point3D.h"

#include <memory>


//-------------------------------------------------------------------------------

class PointLight : public Light
{
	std::shared_ptr<Point3D> location;

  public: 
	PointLight();
	PointLight(std::shared_ptr<Point3D> location, std::shared_ptr<Color> color, real intensity);
	std::shared_ptr<Point3D> getLocation(void);
	void setLocation(std::shared_ptr<Point3D> p);
	std::shared_ptr<Point3D> getSamplePoint(void);
	real getArea(void);
	std::shared_ptr<Vector3D> getNormal(void);
};


//-------------------------------------------------------------------------------

#endif
