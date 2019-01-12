#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "light.h"
#include "point3D.h"


//-------------------------------------------------------------------------------

class PointLight : public Light
{
	Point3D *location;

  public: 
	PointLight();
	PointLight(Point3D *location, real *color, real intensity);
	PointLight(PointLight &l);
	PointLight *copy(void);
	~PointLight();
	Point3D *getLocation(void);
	void setLocation(Point3D *p);
	Point3D *getSamplePoint(void);
	real getArea(void);
	Vector3D *getNormal(void);
};


//-------------------------------------------------------------------------------

#endif
