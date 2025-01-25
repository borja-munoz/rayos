#ifndef _HITPOINT_H
#define _HITPOINT_H

#include "../geom/primitive.h"
#include "../util/point3D.h"

// Struct to store intersection information
struct HitPoint
{
  Point3D hitPoint; 	               // Hit Point between Ray and Scene
  std::shared_ptr<Primitive> object; // Object hit by the ray
  Vector3D normal;                   // Normal at the hit point
  real distance;                     // Distance between the ray origin and the hit point
};

#endif