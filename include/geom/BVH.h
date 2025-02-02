#ifndef _BVH_H
#define _BVH_H

#include <numeric> // For std::iota

#include "AABB.h"
#include "BVHNode.h"
#include "primitive.h"
#include "../tracer/hitPoint.h"
#include "../util/ray.h"


class BVH {
  public:
    std::unique_ptr<BVHNode> root;

    void build(const std::vector<std::shared_ptr<Primitive>>& primitives);
    std::optional<HitPoint> intersect(const Ray& ray, real tMin, real tMax) const;

  private:
    std::unique_ptr<BVHNode> buildNode(const std::vector<std::shared_ptr<Primitive>>& primitives,
                                       const std::vector<AABB>& primitiveBounds,
                                       std::vector<size_t>& indices,
                                       size_t start, size_t end);
    std::optional<HitPoint>  traverse(const BVHNode* node, 
                                      const Ray& ray, 
                                      real tMin, 
                                      real tMax) const;
};

#endif