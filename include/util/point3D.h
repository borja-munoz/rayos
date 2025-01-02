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
	std::shared_ptr<Point3D> sum(std::shared_ptr<Vector3D> v);
	std::shared_ptr<Vector3D> substract(std::shared_ptr<Point3D> p);
};

#endif
