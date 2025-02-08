#include "triangle.h"

Triangle::Triangle()
{
  Vector3D aux1, aux2;

  this->vertex[0] = Point3D(-1, 0, 0);
  this->vertex[1] = Point3D(0, 1, 0);
  this->vertex[2] = Point3D(1, 0, 0);

  aux1 = this->vertex[1].substract(this->vertex[0]);
  aux2 = this->vertex[2].substract(this->vertex[0]);
  this->normal = aux1.crossProduct(aux2);
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
  Vector3D aux1, aux2;

  this->vertex[0] = x;
  this->vertex[1] = y;
  this->vertex[2] = z;

  aux1 = this->vertex[1].substract(this->vertex[0]);
  aux2 = this->vertex[2].substract(this->vertex[0]);
  this->normal = aux1.crossProduct(aux2);
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
    Vector3D edge1, edge2;
    Vector3D h, s, q;
    real a, f, u, v;

    // Compute edges of the triangle
    edge1 = this->vertex[1].substract(this->vertex[0]); // edge1 = v1 - v0
    edge2 = this->vertex[2].substract(this->vertex[0]); // edge2 = v2 - v0

    // Compute the determinant
    h = ray.direction.crossProduct(edge2); // h = direction × edge2
    a = edge1.dotProduct(h); // a = edge1 · h

    // Check if the ray is parallel to the triangle
    if (std::fabs(a) < tMin)
        return 0; // No intersection

    f = 1.0f / a; // Inverse of determinant
    s = ray.origin.substract(this->vertex[0]); // s = origin - v0
    u = f * s.dotProduct(h); // Compute barycentric coordinate u

    // Check if the intersection is outside the triangle
    if (u < 0.0f || u > 1.0f)
        return 0;

    q = s.crossProduct(edge1); // q = s × edge1
    v = f * ray.direction.dotProduct(q); // Compute barycentric coordinate v

    // Check if the intersection is outside the triangle
    if (v < 0.0f || u + v > 1.0f)
        return 0;

    // Compute the distance to the intersection point
    float t = f * edge2.dotProduct(q);

    // Check if the intersection is valid
    if (t >= tMin && t <= tMax) {
        normal = this->normal;
        return t; // Intersection found, return the distance
    }

    return 0; // No intersection
}

// Number of crosses algorithm
// real Triangle::intersect(const Ray& r, Vector3D &normal) const
// {
//   int crosses;
//   real denominator, t, x0, y0, x1, y1;
//   Point3D intersection, rayOrigin;
//   Vector3D rayDirection;

//   // We calculate intersection between the ray and the triangle plane

//   // Now we need to find if the hit point is contained within the polygon
//   // We project onto one plane and calculate the number of crosses

//   // We can express the plane with the following equation:
//   //        (P - P') * Normal = 0
//   // where P and P' are points beloging to the plane
//   // We can substitute the ray in that equation
//   rayDirection = r.getDirection();
//   denominator = rayDirection.dotProduct(this->normal);
//   if (ZERO(denominator))
//   {
//     return (0);
//   }

//   rayOrigin = r.getOrigin();
//   Vector3D oV0;
//   oV0 = rayOrigin.substract(this->vertex[0]);
//   t = -(oV0.dotProduct(this->normal)) / denominator;

//   Vector3D td;
//   td = rayDirection.product(t);
//   intersection = rayOrigin.sum(td);

//   // We calculate the greater normal vector component (absolute value)
//   // to select the plane where we will do the projection (the one that maximizes the projected region)
//   char greater;
//   if (fabs(this->normal.x) > fabs(this->normal.y))
//   {
//     if (fabs(this->normal.x) > fabs(this->normal.z))
//       greater = 'X';
//     else
//       greater = 'Z';
//   }
//   else
//   {
//     if (fabs(this->normal.y) > fabs(this->normal.z))
//       greater = 'Y';
//     else
//       greater = 'Z';
//   }

//   // We calculate the number of crosses with the selected plane
//   crosses = 0;
//   if (greater == 'X') // 'X' is the greater, we project over YZ plane
//   {
//     x0 = this->vertex[2].z - intersection.z;
//     y0 = this->vertex[2].y - intersection.y;
//     for (int i = 0; i < 3; i++)
//     {
//       x1 = this->vertex[i].z - intersection.z;
//       y1 = this->vertex[i].y - intersection.y;
//       if (y0 > 0)
//       {
//         if (y1 <= 0)
//           if (x1 * y0 > x0 * y1)
//             crosses++;
//       }
//       else
//       {
//         if (y1 > 0)
//           if (x0 * y1 > y0 * x1)
//             crosses++;
//       }
//       x0 = x1;
//       y0 = y1;
//     }
//   }
//   else if (greater == 'Y') // 'Y' is the greater, we project over XZ plane
//   {
//     x0 = this->vertex[2].x - intersection.x;
//     y0 = this->vertex[2].z - intersection.z;
//     for (int i = 0; i < 3; i++)
//     {
//       x1 = this->vertex[i].x - intersection.x;
//       y1 = this->vertex[i].z - intersection.z;
//       if (y0 > 0)
//       {
//         if (y1 <= 0)
//           if (x1 * y0 > x0 * y1)
//             crosses++;
//       }
//       else
//       {
//         if (y1 > 0)
//           if (x0 * y1 > y0 * x1)
//             crosses++;
//       }
//       x0 = x1;
//       y0 = y1;
//     }
//   }
//   else // 'Z' is the greater, we project over XY plane
//   {
//     x0 = this->vertex[2].x - intersection.x;
//     y0 = this->vertex[2].y - intersection.y;
//     for (int i = 0; i < 3; i++)
//     {
//       x1 = this->vertex[i].x - intersection.x;
//       y1 = this->vertex[i].y - intersection.y;
//       if (y0 > 0)
//       {
//         if (y1 <= 0)
//           if (x1 * y0 > x0 * y1)
//             crosses++;
//       }
//       else
//       {
//         if (y1 > 0)
//           if (x0 * y1 > y0 * x1)
//             crosses++;
//       }
//       x0 = x1;
//       y0 = y1;
//     }
//   }

//   // If there is an odd number of crosses, there is an intersection
//   if (crosses & 1)
//   {
//     normal.x = this->normal.x;
//     normal.y = this->normal.y;
//     normal.z = this->normal.z;
//   }
//   else
//     t = 0;

//   return (t);
// }

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