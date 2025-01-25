#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "primitive.h"
#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/util.h"


//-------------------------------------------------------------------------------

class Triangle : public Primitive
{
	Point3D vertex[3];
	Vector3D normal;              // Plane containing
	real distance;                // the triangle
  
  public:
	Triangle();
	Triangle(Point3D x, 
             Point3D y, 
             Point3D z, 
             Material mat);
	real intersect(const Ray& r, Vector3D &normal, real tMin, real tMax) const;
	AABB boundingBox() const;
};


//-------------------------------------------------------------------------------

#endif
