#include "triangleMesh.h"

struct sRay {
    float origin[3];
    float direction[3];
};

// Helper function to compute the cross product of two vectors
void cross(const float a[3], const float b[3], float result[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

// Helper function to compute the dot product of two vectors
float dot(const float a[3], const float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Helper function to subtract two vectors
void subtract(const float a[3], const float b[3], float result[3]) {
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

// Möller-Trumbore ray-triangle intersection
std::optional<float> intersectRayTriangle(const sRay& ray, const TriangleData& triangle, float epsilon = 1e-6) {
    float edge1[3], edge2[3], h[3], s[3], q[3];
    float a, f, u, v;

    // Compute edges of the triangle
    subtract(triangle.v1, triangle.v0, edge1); // edge1 = v1 - v0
    subtract(triangle.v2, triangle.v0, edge2); // edge2 = v2 - v0

    // Compute the determinant
    cross(ray.direction, edge2, h); // h = direction × edge2
    a = dot(edge1, h); // a = edge1 · h

    // Check if the ray is parallel to the triangle
    if (std::fabs(a) < epsilon)
        return std::nullopt; // No intersection

    f = 1.0f / a; // Inverse of determinant
    subtract(ray.origin, triangle.v0, s); // s = origin - v0
    u = f * dot(s, h); // Compute barycentric coordinate u

    // Check if the intersection is outside the triangle
    if (u < 0.0f || u > 1.0f)
        return std::nullopt;

    cross(s, edge1, q); // q = s × edge1
    v = f * dot(ray.direction, q); // Compute barycentric coordinate v

    // Check if the intersection is outside the triangle
    if (v < 0.0f || u + v > 1.0f)
        return std::nullopt;

    // Compute the distance to the intersection point
    float t = f * dot(edge2, q);

    // Check if the intersection is behind the ray
    if (t > epsilon)
        return t; // Intersection found, return the distance

    return std::nullopt; // No intersection
}

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
        TriangleData tData;

        // Triangle vertices
        tData.v0[0] = this->vertices[this->vertexIndexes[j]].x;
        tData.v0[1] = this->vertices[this->vertexIndexes[j]].y;
        tData.v0[2] = this->vertices[this->vertexIndexes[j]].z;
        tData.v1[0] = this->vertices[this->vertexIndexes[j+1]].x;
        tData.v1[1] = this->vertices[this->vertexIndexes[j+1]].y;
        tData.v1[2] = this->vertices[this->vertexIndexes[j+1]].z;
        tData.v2[0] = this->vertices[this->vertexIndexes[j+2]].x;
        tData.v2[1] = this->vertices[this->vertexIndexes[j+2]].y;
        tData.v2[2] = this->vertices[this->vertexIndexes[j+2]].z;

        // Triangle normal
        float aux1[3], aux2[3];
        aux1[0] = tData.v1[0] - tData.v0[0];
        aux1[1] = tData.v1[1] - tData.v0[1];
        aux1[2] = tData.v1[2] - tData.v0[2];
        aux2[0] = tData.v2[0] - tData.v0[0];
        aux2[1] = tData.v2[1] - tData.v0[1];
        aux2[2] = tData.v2[2] - tData.v0[2];
        tData.normal[0] = aux1[1] * aux2[2] - aux1[2] * aux2[1];
        tData.normal[1] = aux1[2] * aux2[0] - aux1[0] * aux2[2];
        tData.normal[2] = aux1[0] * aux2[1] - aux1[1] * aux2[0];

        float length = sqrt(
            tData.normal[0] * tData.normal[0] + 
            tData.normal[1] * tData.normal[1] + 
            tData.normal[2] * tData.normal[2]
        );
        tData.normal[0] /= length;
        tData.normal[1] /= length;
        tData.normal[2] /= length;

        this->triangleData.push_back(tData);
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
}

// Intersects the triangle mesh with a ray
// If there is a hit, returns:
// - Hit point (parametric parameter on the ray)
// - Normal on the hit point
// If not, returns NULL (0)
real TriangleMesh::intersect(const Ray& r, Vector3D &normal) const
{
    // Triangle tri;
    // int triIndex;
    // real t;
    // real tNear;
    // Vector3D auxNormal;

    // int j = 0;
    // tNear = std::numeric_limits<float>::infinity();
    // for (int i = 0; i < this->numberTriangles; i++)
    // {
    //     tri = Triangle(this->vertices[this->vertexIndexes[j]],
    //                    this->vertices[this->vertexIndexes[j+1]],
    //                    this->vertices[this->vertexIndexes[j+2]],
    //                    this->material);

    //     t = tri.intersect(r, auxNormal);
    //     if ((t > 0) && (t < tNear))
    //     {
    //         tNear = t;
    //         normal = auxNormal;
    //         triIndex = i;
    //     }

    //     j += 3;
    // }

	//   return(tNear);

    float tNear;

    // Convert ray representation
    sRay ray;
    Point3D rayOrigin = r.getOrigin();
    Vector3D rayDirection = r.getDirection();
    ray.origin[0] = rayOrigin.x;
    ray.origin[1] = rayOrigin.y;
    ray.origin[2] = rayOrigin.z;
    ray.direction[0] = rayDirection.x;
    ray.direction[1] = rayDirection.y;
    ray.direction[2] = rayDirection.z;

    tNear = std::numeric_limits<float>::infinity();
    for (TriangleData tri : this->triangleData) 
    {
        auto t = intersectRayTriangle(ray, tri);

        if (t) 
        {
            if ((*t > 0) && (*t < tNear))
            {
                tNear = *t;
                normal.x = tri.normal[0];
                normal.y = tri.normal[1];
                normal.z = tri.normal[2];
            }
        }
    }

	return(tNear);
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

