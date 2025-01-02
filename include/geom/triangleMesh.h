#ifndef _TRIANGLE_MESH_H
#define _TRIANGLE_MESH_H

#include "primitive.h"
#include "triangle.h"

#include "../util/point3D.h"
#include "../util/ray.h"
#include "../util/util.h"

#include <limits>
#include <memory>
#include <vector>

// We store the triangle mesh using a collection of vertices and
// a collection of indexes for storing the indexes of the vertices
// for each triangle. The last collection has numberTriangles * 3 elements
// Example:
// 4 vertices (v0, v1, v2, v3)
// 2 triangles (t0 and t1) sharing 2 vertices (v1, v2)
// t0 = v0, v1, v2
// t1 = v1, v2, v3
// vertexIndex = [0, 1, 2, 1, 2, 3]

class TriangleMesh : public Primitive
{
	std::vector<std::shared_ptr<Point3D>> vertices;       // All the vertices in the mesh
    std::vector<int> vertexIndexes;      
    int numberTriangles;
  
  public:
	TriangleMesh();
	TriangleMesh(std::vector<std::shared_ptr<Point3D>> vertices, 
                 std::vector<int> vertexIndexes, 
                 std::shared_ptr<Material> mat);
	real intersect(std::shared_ptr<Ray> r, Vector3D &normal);
    int getNumberTriangles();
    int getNumberVertices();
  
  private:
    void generatePolySphere(float radius, int divisions);
};


#endif
