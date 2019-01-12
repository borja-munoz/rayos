#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include "util.h"

#include <math.h>


//-------------------------------------------------------------------------------

class Vector3D
{
	real x;
	real y;
	real z;
  
  public:
 
	Vector3D();
	Vector3D(real x, real y, real z);
	Vector3D(const Vector3D &v);
	~Vector3D();
	real getX(void);
	real getY(void);
	real getZ(void);
	void setX(real x);
	void setY(real y);
	void setZ(real z);
	real dotProduct(Vector3D *v);
	Vector3D *crossProduct(Vector3D *v);
	real length(void);
	void normalize(void);    
	Vector3D *sum(Vector3D *v);
	Vector3D *substract(Vector3D *v);
	Vector3D *product(real d);
};


//-------------------------------------------------------------------------------

#endif
