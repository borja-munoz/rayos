#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "AABB.h"
#include "../util/material.h"
#include "../util/ray.h"
#include "../util/util.h"

#include <memory>

//-----------------------------------------------

typedef enum
{
	SPHERE,
	TRIANGLE,
	QUAD,
    TRIANGLE_MESH
} ObjectType;


//-----------------------------------------------

class Primitive
{
  protected:
	Material material;
	ObjectType type;
  
  public:
 
	Primitive();
	Material getMaterial(void);
	ObjectType getType(void);
	virtual real intersect(const Ray& r, Vector3D& normal, real tMin, real tMax) const = 0;
	virtual AABB boundingBox() const = 0;
};


//-----------------------------------------------

#endif
