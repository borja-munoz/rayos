#include "BVH.h"

void BVH::build(const std::vector<std::shared_ptr<Primitive>>& primitives) 
{
    std::vector<size_t> indices(primitives.size());
    std::iota(indices.begin(), indices.end(), 0);

    // Compute bounding boxes for all primitives
    std::vector<AABB> primitiveBounds(primitives.size());
    for (size_t i = 0; i < primitives.size(); ++i) {
        primitiveBounds[i] = primitives[i]->boundingBox();
    }

    root = buildNode(primitives, primitiveBounds, indices, 0, indices.size());
}

std::optional<HitPoint> BVH::intersect(const Ray& ray, 
                                       real tMin, 
                                       real tMax) const 
{
    if (!root) return std::nullopt; 
    return traverse(root.get(), ray, tMin, tMax);
}

std::unique_ptr<BVHNode> BVH::buildNode(const std::vector<std::shared_ptr<Primitive>>& primitives,
                                        const std::vector<AABB>& primitiveBounds,
                                        std::vector<size_t>& indices,
                                        size_t start, size_t end) 
{
    auto node = std::make_unique<BVHNode>();

    // Compute the bounding box for this node
    AABB bounds;
    for (size_t i = start; i < end; ++i) {
        bounds.expand(primitiveBounds[indices[i]]);
    }
    node->bounds = bounds;

    size_t numPrimitives = end - start;
    if (numPrimitives <= 2) {
        // Create a leaf node
        node->primitives.reserve(numPrimitives);
        for (size_t i = start; i < end; ++i) {
            node->primitives.push_back(primitives[indices[i]]);
        }
    } else {
        // Split the primitives and create child nodes
        int axis = bounds.longestAxis();
        float midpoint = (bounds.min[axis] + bounds.max[axis]) / 2.0f;

        // Patition the primitives based on the position of the primitive
        // longest axis centroid with respect to the axis midpoint
        // The primitive indices are rearranged in the indices vector
        // midIt points to the first element in the right partition
        auto midIt = std::partition(indices.begin() + start, indices.begin() + end, [&](int i) {
            float centroid = (primitiveBounds[i].min[axis] + primitiveBounds[i].max[axis]) / 2.0f;
            return centroid < midpoint;
        });

        // Ensure the split is valid to avoid degenerate cases
        size_t mid = std::distance(indices.begin(), midIt);
        if (mid == start || mid == end) {
            mid = start + numPrimitives / 2;
        }

        node->left = buildNode(primitives, primitiveBounds, indices, start, mid);
        node->right = buildNode(primitives, primitiveBounds, indices, mid, end);
    }

    return node;
}

std::optional<HitPoint> BVH::traverse(const BVHNode* node, 
                                      const Ray& ray, 
                                      real tMin, 
                                      real tMax) const 
{
    if (!node) return std::nullopt;

    if (!node->bounds.intersects(ray, tMin, tMax)) {
        return std::nullopt;
    }

    if (node->isLeaf()) {
        HitPoint hitPoint;
        float closestT = tMax;
        bool hitSomething = false;

        for (const auto& primitive : node->primitives) {
            float tHit;
            Vector3D normalHit;

            // Test intersection with the primitive
            tHit = primitive->intersect(ray, normalHit, tMin, closestT);
            if (tHit > 0.0f && tHit < closestT) {
                hitSomething = true;
                closestT = tHit;
                hitPoint = {
                    ray.pointParametric(tHit),
                    primitive,
                    normalHit,
                    tHit
                };
            }
        }

        if (hitSomething) {
            return hitPoint;
        }

        return std::nullopt;
    }

    // Otherwise, traverse child nodes
    Vector3D leftNormal, rightNormal;
    std::optional<HitPoint> leftHitPoint = traverse(node->left.get(), 
                                                    ray, tMin, tMax);
    std::optional<HitPoint> rightHitPoint = traverse(node->right.get(),  
                                                      ray, tMin, tMax);

    if (leftHitPoint && 
        (!rightHitPoint || 
         (*leftHitPoint).distance < (*rightHitPoint).distance)) {
        return leftHitPoint;
    }

    if (rightHitPoint) {
        return rightHitPoint;
    }

    return std::nullopt; // No intersection
}
