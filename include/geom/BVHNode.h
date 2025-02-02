#ifndef _BVHNODE_H
#define _BVHNODE_H

#include <memory>
#include <vector>

#include "AABB.h"
#include "primitive.h"

class BVHNode {
  public:
    AABB bounds;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<std::shared_ptr<Primitive>> primitives;

    bool isLeaf() const;
};

#endif