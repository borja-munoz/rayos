#include "BVHNode.h"

bool BVHNode::isLeaf() const 
{ 
    return !primitives.empty(); 
}
