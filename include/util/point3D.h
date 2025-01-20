#ifndef _Point3D_H
#define _Point3D_H

#include "vector3D.h"
#include "util.h"

#include <memory>

// Declaration to avoid compiler error
// due to cyclic dependencies
class Quad;
class Triangle;
class TriangleMesh;

class Point3D
{
	friend Quad;
	friend Triangle;
  friend TriangleMesh;
  
 public:
 
	real x;
	real y;
	real z;

	Point3D();
	Point3D(real x, real y, real z);
	Point3D(const Point3D &p);
	Point3D sum(const Vector3D& v) const;
	Vector3D substract(const Point3D& p) const;
};

#endif
