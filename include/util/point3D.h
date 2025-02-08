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
    real operator[](size_t index) const;
    real& operator[](size_t index);
	Point3D operator+(Point3D otherPoint) const;
	Point3D& operator+=(Point3D otherPoint);
	Point3D operator*(real scalar) const;
	Point3D& operator*=(real scalar);
	Point3D sum(const Vector3D& v) const;
	Vector3D substract(const Point3D& p) const;

	static Point3D min(const Point3D& a, const Point3D& b) 
	{
		return { std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z) };
	}

	static Point3D max(const Point3D& a, const Point3D& b) 
	{
		return { std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z) };
	}

};

#endif
