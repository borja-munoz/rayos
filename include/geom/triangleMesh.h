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

// Instead of creating each triangle from the vertices and indexes, we
// will create the triangles and their normals in the constructor
struct TriangleData {
    float v0[3]; // Vertex 0
    float v1[3]; // Vertex 1
    float v2[3]; // Vertex 2
    float normal[3]; // Precomputed normal
};

class TriangleMesh : public Primitive
{
	std::vector<Point3D> vertices;       // All the vertices in the mesh
  std::vector<int> vertexIndexes;      
  int numberTriangles;
  std::vector<TriangleData> triangleData;
  
  public:
	TriangleMesh();
	TriangleMesh(std::vector<Point3D> vertices, 
               std::vector<int> vertexIndexes, 
               Material mat);
	real intersect(const Ray& r, Vector3D &normal) const;
  int getNumberTriangles();
  int getNumberVertices();
  
  private:
    void generatePolySphere(float radius, int divisions);
};


#endif
