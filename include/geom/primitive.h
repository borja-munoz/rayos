#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

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
	std::shared_ptr<Material> material;
	ObjectType type;
  
  public:
 
	Primitive();
	std::shared_ptr<Material> getMaterial(void);
	ObjectType getType(void);
	virtual real intersect(std::shared_ptr<Ray> r, Vector3D &normal) = 0;
};


//-----------------------------------------------

#endif
