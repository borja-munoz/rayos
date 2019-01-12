#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "primitive.h"
#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/util.h"


//-------------------------------------------------------------------------------

class Triangle : public Primitive
{
	Point3D *vertex[3];
	Vector3D *normal;              // Plane containing
	real distance;                 // the triangle
  
  public:
	Triangle();
	Triangle(Point3D *x, Point3D *y, Point3D *z, Material *mat);
	Triangle(const Triangle &t);
	Triangle *copy();
	~Triangle();
	real intersect(Ray *r, Vector3D &normal);
};


//-------------------------------------------------------------------------------

#endif
