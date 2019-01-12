#ifndef _Point3D_H
#define _Point3D_H

#include "vector3D.h"
#include "util.h"

// Declaration to avoid compiler error
// due to cyclic dependencies
class Quad;
class Triangle;

class Point3D
{
	friend Quad;
	friend Triangle;

	real x;
	real y;
	real z;
  
 public:
 
	Point3D();
	Point3D(real x, real y, real z);
	Point3D(const Point3D &p);
	~Point3D();
	real getX(void);
	real getY(void);
	real getZ(void);
	void setX(real x);
	void setY(real y);
	void setZ(real z);
	Point3D *sum(Vector3D *v);
	Vector3D *substract(Point3D *p);
};

#endif
