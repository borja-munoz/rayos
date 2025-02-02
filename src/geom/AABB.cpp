#include "AABB.h"

void AABB::expand(const AABB& box) 
{
    min = Point3D::min(min, box.min);
    max = Point3D::max(max, box.max);
}

bool AABB::intersects(const Ray& ray, real tMin, real tMax) const 
{
    for (int i = 0; i < 3; ++i) 
    {
        real invD = 1.0f / ray.direction[i];
        real t0 = (min[i] - ray.origin[i]) * invD;
        real t1 = (max[i] - ray.origin[i]) * invD;

        if (invD < 0.0f) 
            std::swap(t0, t1);

        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);

        if (tMax <= tMin) 
            return false;
    }

    return true;
}

int AABB::longestAxis() const 
{
    // Compute the extents along each axis
    float xExtent = max.x - min.x;
    float yExtent = max.y - min.y;
    float zExtent = max.z - min.z;

    // Return the axis with the largest extent
    if (xExtent >= yExtent && xExtent > zExtent) {
        return 0; // X-axis
    } else if (yExtent > zExtent) {
        return 1; // Y-axis
    } else {
        return 2; // Z-axis
    }
}