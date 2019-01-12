#ifndef _RAY_H
#define _RAY_H

#include "point3D.h"
#include "vector3D.h"
#include "util.h"


//-----------------------------------------------

class Ray
{
	Point3D *origin;
	Vector3D *direction;
  
  public:
 
	Ray();
	Ray(Point3D *p, Vector3D *dir);
	~Ray();
	Point3D *getOrigin(void);
	Vector3D *getDirection(void);
	void setOrigin(Point3D *p);
	void setDirection(Vector3D *v);
	Point3D *pointParametric(real t);
};


//-----------------------------------------------


#endif
