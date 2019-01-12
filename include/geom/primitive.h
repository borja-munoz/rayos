#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "../util/material.h"
#include "../util/ray.h"
#include "../util/util.h"


//-----------------------------------------------

typedef enum
{
	SPHERE,
	TRIANGULO,
	QUAD
} ObjectType;


//-----------------------------------------------

class Primitive
{
  protected:
	Material *material;
	ObjectType type;
  
  public:
 
	Primitive();
	virtual ~Primitive();
	virtual Primitive *copy(void) = 0;
	Material *getMaterial(void);
	ObjectType getType(void);
	virtual real intersect(Ray *r, Vector3D &normal) = 0;
};


//-----------------------------------------------

#endif
