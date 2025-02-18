#include "triangleMesh.h"

TriangleMesh::TriangleMesh()
{
    // Two triangles
    /*
    this->vertices.push_back(std::make_shared<Point3D>(0, 0, -3));
    this->vertices.push_back(std::make_shared<Point3D>(0, 1, -3));
    this->vertices.push_back(std::make_shared<Point3D>(-1, 0, -3));
    this->vertices.push_back(std::make_shared<Point3D>(1, 0, -3));

    this->vertexIndexes.push_back(0);
    this->vertexIndexes.push_back(1);
    this->vertexIndexes.push_back(2);
    this->vertexIndexes.push_back(0);
    this->vertexIndexes.push_back(3);
    this->vertexIndexes.push_back(1);
    */

    this->generatePolySphere(1, 5);

    this->numberTriangles = this->vertexIndexes.size() / 3;

    int j = 0;
    for (int i = 0; i < this->numberTriangles; i++)
    {
        this->triangles.push_back(
            Triangle(
                this->vertices[this->vertexIndexes[j]],
                this->vertices[this->vertexIndexes[j+1]],
                this->vertices[this->vertexIndexes[j+2]],
                this->material
            )
        );

        j += 3;
    }

    // for (int i = 0; i < this->vertices.size(); i++) 
    // {
    //   std::cout << this->vertices[i]->x << "," << this->vertices[i]->y << "," << this->vertices[i]->z << "\n";
    // }

    this->material = Material();
    this->material.set(
        Color(1.0, 0.2, 1.0),
        1.0, 0.5, 0.5, 0.0
    );

    this->type = TRIANGLE_MESH;
}

TriangleMesh::TriangleMesh(std::vector<Point3D> vertices, 
                           std::vector<int> vertexIndexes, 
                           Material mat)
{
    this->vertices = vertices;
    this->vertexIndexes = vertexIndexes;

    this->numberTriangles = this->vertexIndexes.size() / 3;

    this->material = mat;
    
    this->type = TRIANGLE_MESH;

    int j = 0;
    for (int i = 0; i < this->numberTriangles; i++)
    {
        this->triangles.push_back(
            Triangle(
                this->vertices[this->vertexIndexes[j]],
                this->vertices[this->vertexIndexes[j+1]],
                this->vertices[this->vertexIndexes[j+2]],
                this->material
            )
        );

        j += 3;
    }
}

TriangleMesh::TriangleMesh(const TriangleMesh& mesh, const Material& mat)
{
    this->vertices = mesh.vertices;
    this->vertexIndexes = mesh.vertexIndexes;
    this->numberTriangles = mesh.numberTriangles;
    this->material = mat;
    this->type = TRIANGLE_MESH;

    int j = 0;
    for (int i = 0; i < this->numberTriangles; i++)
    {
        this->triangles.push_back(
            Triangle(
                this->vertices[this->vertexIndexes[j]],
                this->vertices[this->vertexIndexes[j+1]],
                this->vertices[this->vertexIndexes[j+2]],
                this->material
            )
        );

        j += 3;
    }
}

// Returns a random point from a random triangle in the mesh
const Point3D& TriangleMesh::getSamplePoint() const
{
    // Select a random triangle
    int triangleIndex = getRandomNumber(0, this->triangles.size() - 1);
    const Triangle& triangle = triangles[triangleIndex];

    // Get the vertices of the selected triangle
    const Point3D& A = triangle.getVertex(0);
    const Point3D& B = triangle.getVertex(1);
    const Point3D& C = triangle.getVertex(2);

    // Generate random barycentric coordinates
    float u = getRandomNumber(0.0f, 1.0f);
    float v = getRandomNumber(0.0f, 1.0f);

    // Ensure the point is inside the triangle
    if (u + v > 1.0f) {
        u = 1.0f - u;
        v = 1.0f - v;
    }

    // Compute the sample point using barycentric coordinates
    return A * (1.0f - u - v) + B * u + C * v;
}

const Triangle& TriangleMesh::getTriangle(int index) const
{
    return this->triangles[index];
}

// Intersects the triangle mesh with a ray
// If there is a hit, returns:
// - Hit point (parametric parameter on the ray)
// - Normal on the hit point
// If not, returns NULL (0)
real TriangleMesh::intersect(const Ray& r, 
                             Vector3D &normal, 
                             real tMin, 
                             real tMax) const
{
    float tNear = tMax;
    Vector3D auxNormal;

    tNear = tMax;
    // Use a const reference to avoid copying
    for (const Triangle& tri : this->triangles) 
    {
        real t = tri.intersect(r, auxNormal, 0.001f, tNear);

        if ((t > tMin) && (t < tNear))
        {
            tNear = t;
            normal = auxNormal;
        }
    }

	return tNear;
}

// Adapted from code from Scratch a Pixel
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-polygon-mesh/Ray-Tracing%20a%20Polygon%20Mesh-part-1
void TriangleMesh::generatePolySphere(float radius, int divisions)
{
    int numVertices = (divisions - 1) * divisions + 2; 
 
    float u = -M_PI_2; 
    float v = -M_PI; 
    float du = M_PI / divisions; 
    float dv = 2 * M_PI / divisions; 
 
    // Create the vertices
    this->vertices.push_back(Point3D(0, -radius, -3)); 
    for (int i = 0; i < divisions - 1; i++) 
    { 
        u += du; 
        v = -M_PI; 
        for (int j = 0; j < divisions; j++) 
        { 
            float x = radius * cos(u) * cos(v); 
            float y = radius * sin(u); 
            float z = radius * cos(u) * sin(v); 
            this->vertices.push_back(Point3D(x, y, z - 3)); 
            v += dv; 
        } 
    } 
    this->vertices.push_back(Point3D(0, radius, -3)); 
 
    // Create the connectivity lists                                                                                                                                                                        
    int vid = 1, numV = 0; 
    for (int i = 0; i < divisions; i++) 
    { 
        for (int j = 0; j < divisions; j++) 
        { 
            if (i == 0) 
            { 
                this->vertexIndexes.push_back(0); 
                this->vertexIndexes.push_back(j + vid); 
                this->vertexIndexes.push_back((j == (divisions - 1)) ? vid : j + vid + 1); 
            } 
            else if (i == (divisions - 1)) 
            { 
                this->vertexIndexes.push_back(j + vid + 1 - divisions); 
                this->vertexIndexes.push_back(vid + 1); 
                this->vertexIndexes.push_back((j == (divisions - 1)) ? vid + 1 - divisions : j + vid + 2 - divisions); 
            } 
            else 
            { 
                // In the original code, this is a quad with the following vertices
                // this->vertexIndexes.push_back(j + vid + 1 - divisions); 
                // this->vertexIndexes.push_back(j + vid + 1);              
                // this->vertexIndexes.push_back((j == (divisions - 1)) ? vid + 1 : j + vid + 2); 
                // this->vertexIndexes.push_back((j == (divisions - 1)) ? vid + 1 - divisions : j + vid + 2 - divisions); 

                // We are adding here two triangles instead, repeating the first
                // and third vertex in the second triangle
                this->vertexIndexes.push_back(j + vid + 1 - divisions); 
                this->vertexIndexes.push_back(j + vid + 1);              
                this->vertexIndexes.push_back((j == (divisions - 1)) ? vid + 1 : j + vid + 2); 

                this->vertexIndexes.push_back(j + vid + 1 - divisions); 
                this->vertexIndexes.push_back((j == (divisions - 1)) ? vid + 1 : j + vid + 2); 
                this->vertexIndexes.push_back((j == (divisions - 1)) ? vid + 1 - divisions : j + vid + 2 - divisions); 
            } 
            numV++; 
        } 
        vid = numV; 
    } 

}

AABB TriangleMesh::boundingBox() const
{
    AABB bounds;

    // Iterate over all triangles in the mesh and expand the bounding box
    for (const Triangle& triangle : triangles) {
        AABB triangleBounds = triangle.boundingBox();
        bounds.expand(triangleBounds);
    }

    return bounds;    
}
