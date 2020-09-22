#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "primitive.h"
#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/util.h"


//-------------------------------------------------------------------------------

class Triangle : public Primitive
{
	std::shared_ptr<Point3D> vertex[3];
	std::shared_ptr<Vector3D> normal;              // Plane containing
	real distance;                 // the triangle
  
  public:
	Triangle();
	Triangle(std::shared_ptr<Point3D> x, 
             std::shared_ptr<Point3D> y, 
             std::shared_ptr<Point3D> z, 
             std::shared_ptr<Material> mat);
	real intersect(std::shared_ptr<Ray> r, Vector3D &normal);
};


//-------------------------------------------------------------------------------

#endif
