#ifndef _AABB_H
#define _AABB_H

#include "../util/ray.h"
#include "../util/point3D.h"

class AABB 
{
  public:
    Point3D min;
    Point3D max;

    AABB() : min({ std::numeric_limits<real>::max(), std::numeric_limits<real>::max(), std::numeric_limits<real>::max() }),
             max({ -std::numeric_limits<real>::max(), -std::numeric_limits<real>::max(), -std::numeric_limits<real>::max() }) 
    {

    }

    AABB(Point3D min, Point3D max)
    {
      this->min = min;
      this->max = max;
    }

    void expand(const AABB& box);
    bool intersects(const Ray& ray, real tMin, real tMax) const;    
    int longestAxis() const;
};

#endif