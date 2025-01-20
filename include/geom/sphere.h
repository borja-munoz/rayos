#ifndef _sphere_H
#define _sphere_H

#include "primitive.h"
#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/util.h"

class Sphere : public Primitive
{
	Point3D center;
	real radius;
  
  public:
	Sphere();
	Sphere(Point3D center, real radius, Material material);
	real intersect(const Ray& r, Vector3D &normal);
};


#endif
