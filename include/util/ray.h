#ifndef _RAY_H
#define _RAY_H

#include "point3D.h"
#include "vector3D.h"
#include "util.h"

#include <memory>

//-----------------------------------------------

class Ray
{
	Point3D origin;
	Vector3D direction;
  
  public:
 
	Ray();
	Ray(const Point3D& p, const Vector3D& dir);
	~Ray();
	Point3D getOrigin(void) const;
	Vector3D getDirection(void) const;
	void setOrigin(const Point3D& p);
	void setDirection(const Vector3D& v);
	Point3D pointParametric(real t) const;
};


//-----------------------------------------------


#endif
