#ifndef _sphere_H
#define _sphere_H

#include "primitive.h"
#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/util.h"

class Sphere : public Primitive
{
	std::shared_ptr<Point3D> center;
	real radius;
  
  public:
	Sphere();
	Sphere(std::shared_ptr<Point3D> center, real radius, std::shared_ptr<Material> material);
	real intersect(std::shared_ptr<Ray> r, Vector3D &normal);
};


#endif
