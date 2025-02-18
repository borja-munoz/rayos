#include "triangle.h"

Triangle::Triangle()
{
  this->vertex[0] = Point3D(-1, 0, 0);
  this->vertex[1] = Point3D(0, 1, 0);
  this->vertex[2] = Point3D(1, 0, 0);

  this->edge1 = this->vertex[1].substract(this->vertex[0]);
  this->edge2 = this->vertex[2].substract(this->vertex[0]);

  this->normal = this->edge1.crossProduct(this->edge2);
  this->normal.normalize();
  this->distance = -(this->normal.x * this->vertex[0].x +
                     this->normal.y * this->vertex[0].y +
                     this->normal.z * this->vertex[0].z);

  this->material = Material();

  this->type = TRIANGLE;
}

Triangle::Triangle(Point3D x,
                   Point3D y,
                   Point3D z,
                   Material mat)
{
  this->vertex[0] = x;
  this->vertex[1] = y;
  this->vertex[2] = z;

  this->edge1 = this->vertex[1].substract(this->vertex[0]);
  this->edge2 = this->vertex[2].substract(this->vertex[0]);

  this->normal = this->edge1.crossProduct(this->edge2);
  this->normal.normalize();
  this->distance = -(this->normal.x * this->vertex[0].x +
                     this->normal.y * this->vertex[0].y +
                     this->normal.z * this->vertex[0].z);

  this->material = mat;

  this->type = TRIANGLE;
}

Point3D Triangle::getVertex(int index) const
{
  return this->vertex[index];
}

Vector3D Triangle::getNormal() const
{
  return this->normal;
}

// Intersects the polygon with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns 0
// Möller-Trumbore ray-triangle intersection
real Triangle::intersect(const Ray& ray, Vector3D& normal, real tMin, real tMax) const
{
    const Vector3D& edge1 = this->edge1;
    const Vector3D& edge2 = this->edge2;

    // Compute the determinant
    Vector3D h = ray.direction.crossProduct(edge2); // h = direction × edge2
    real a = edge1.dotProduct(h); // a = edge1 · h

    // Check if the ray is parallel to the triangle
    if (fabs(a) < tMin)
        return 0; // No intersection

    real f = 1.0f / a; // Inverse of determinant
    Vector3D s = ray.origin - this->vertex[0]; // s = origin - v0
    real u = f * s.dotProduct(h); // Compute barycentric coordinate u

    // Check if the intersection is outside the triangle
    if (u < 0.0f || u > 1.0f)
        return 0;

    Vector3D q = s.crossProduct(edge1); // q = s × edge1
    real v = f * ray.direction.dotProduct(q); // Compute barycentric coordinate v

    // Check if the intersection is outside the triangle
    if (v < 0.0f || (u + v) > 1.0f)
        return 0;

    // Compute the distance to the intersection point
    real t = f * edge2.dotProduct(q);

    // Check if the intersection is valid
    if (t >= tMin && t <= tMax) {
        normal = this->normal;
        return t; // Intersection found, return the distance
    }

    return 0; // No intersection
}

AABB Triangle::boundingBox() const
{
    // Initialize min and max points to the first vertex
    Point3D min = vertex[0];
    Point3D max = vertex[0];

    // Loop through the remaining vertices to find min and max for each axis
    for (int i = 1; i < 3; ++i) {
        min.x = std::min(min.x, vertex[i].x);
        min.y = std::min(min.y, vertex[i].y);
        min.z = std::min(min.z, vertex[i].z);

        max.x = std::max(max.x, vertex[i].x);
        max.y = std::max(max.y, vertex[i].y);
        max.z = std::max(max.z, vertex[i].z);
    }

    // Return the AABB constructed with min and max points
    return AABB(min, max);
};