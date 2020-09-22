#ifndef _RAY_H
#define _RAY_H

#include "point3D.h"
#include "vector3D.h"
#include "util.h"

#include <memory>

//-----------------------------------------------

class Ray
{
	std::shared_ptr<Point3D> origin;
	std::shared_ptr<Vector3D> direction;
  
  public:
 
	Ray();
	Ray(std::shared_ptr<Point3D> p, std::shared_ptr<Vector3D> dir);
	~Ray();
	std::shared_ptr<Point3D> getOrigin(void);
	std::shared_ptr<Vector3D> getDirection(void);
	void setOrigin(std::shared_ptr<Point3D> p);
	void setDirection(std::shared_ptr<Vector3D> v);
	std::shared_ptr<Point3D> pointParametric(real t);
};


//-----------------------------------------------


#endif
