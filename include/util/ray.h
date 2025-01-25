#ifndef _RAY_H
#define _RAY_H

#include "point3D.h"
#include "vector3D.h"
#include "util.h"

#include <memory>

//-----------------------------------------------

class Ray
{
  public:

	Point3D origin;
	Vector3D direction;
   
	Ray();
	Ray(const Point3D& p, const Vector3D& dir);
	~Ray();
	Point3D pointParametric(real t) const;
};


//-----------------------------------------------


#endif
