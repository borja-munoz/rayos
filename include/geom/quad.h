#ifndef _QUAD_H
#define _QUAD_H

#include "primitive.h"
#include "triangle.h"
#include "../util/point3D.h"
#include "../util/vector3D.h"
#include "../util/ray.h"
#include "../util/util.h"

#include <vector>

using namespace std;

//-------------------------------------------------------------------------------

class Quad : public Primitive
{
	std::shared_ptr<Point3D> vertex[4];
	std::shared_ptr<Vector3D> normal;              
	char greaterNormalComponent;    // Greater normal component (in absolute value)	

 public:
	Quad();
	Quad(std::shared_ptr<Point3D> a, 
         std::shared_ptr<Point3D> b, 
         std::shared_ptr<Point3D> c, 
         std::shared_ptr<Point3D> d);
	Quad(std::shared_ptr<Point3D> a, 
         std::shared_ptr<Point3D> b, 
         std::shared_ptr<Point3D> c, 
         std::shared_ptr<Point3D> d, 
         std::shared_ptr<Material> mat);
	std::shared_ptr<Vector3D> getNormal(void);
	real intersect(std::shared_ptr<Ray> r, Vector3D &normal);
	vector<std::shared_ptr<Triangle>> tessellate(void);
	real getArea(void);
	vector<std::shared_ptr<Point3D>> getCoordinates(void); 
};


//-------------------------------------------------------------------------------

#endif
