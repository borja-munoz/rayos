#include "sphere.h"

Sphere::Sphere()
{
    this->center = Point3D(0, 0, 0);
    this->radius = 1.0;
    this->material = Material();
    this->type = SPHERE;
}

Sphere::Sphere(Point3D center, real radius, Material material)
{
    this->center = center;
    this->radius = radius;
    this->material = material;
    this->type = SPHERE;
}

// Intersects the sphere with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)
real Sphere::intersect(const Ray& r, Vector3D &normal)
{
    real b, c, t, t1, t2, discriminant, squareRoot;
    Vector3D difRayCenter, n;
    Point3D intersection;  
  
    // To calculate the intersection, we substitute ray parameters
    // in the sphere equation
    // We need to resolve a second grade equation
    // at^2 + bt + c = 0

    difRayCenter = r.origin.substract(this->center);

    // If we consider the ray direction to be normalized,
    // we don't need to calculate a = Dir * Dir, because the value es 1
    b = difRayCenter.dotProduct(r.direction);
    c = difRayCenter.dotProduct(difRayCenter) - 
        this->radius * this->radius;

    // We calculate the discriminant
    discriminant = b * b - c;
    if (discriminant > 0.0)
    {
        squareRoot = sqrt(discriminant);
        t1 = - b - squareRoot;
        t2 = - b + squareRoot;
        if (t1 > 0.0)
        {
            if ((t2 > 0.0) && (t2 < t1))
                t = t2;
            else
                t = t1;
        }
        else
        {
            t = t2;
        }    

        Vector3D tD = r.direction.product(t);
        intersection = r.origin.sum(tD);

        n = intersection.substract(this->center);
        //n = this->center->substract(intersection);
        n = n.product(1.0 / this->radius);

        normal.x = n.x;
        normal.y = n.y;
        normal.z = n.z;
        normal.normalize();

        return(t);
    }

    return(0);
}


