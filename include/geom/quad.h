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
	Point3D *vertex[4];
	Vector3D *normal;              
	char greaterNormalComponent;    // Greater normal component (in absolute value)	

 public:
	Quad();
	Quad(Point3D *a, Point3D *b, Point3D *c, Point3D *d);
	Quad(Point3D *a, Point3D *b, Point3D *c, Point3D *d, Material *mat);
	Quad(const Quad &c);
	Quad *copy(void);
	~Quad();
	Vector3D *getNormal(void);
	real intersect(Ray *r, Vector3D &normal);
	vector<Triangle *> *tessellate(void);
	real getArea(void);
	vector<Point3D *> *getCoordinates(void); 
};


//-------------------------------------------------------------------------------

#endif
