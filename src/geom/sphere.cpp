#include "sphere.h"

Sphere::Sphere()
{
    this->center = std::make_shared<Point3D>(0, 0, 0);
    this->radius = 1.0;
    this->material = std::make_shared<Material>();
    this->type = SPHERE;
}

Sphere::Sphere(std::shared_ptr<Point3D> center, real radius, std::shared_ptr<Material> material)
{
    this->center = std::make_shared<Point3D>(*center);
    this->radius = radius;
    this->material = std::make_shared<Material>(*material);
    this->type = SPHERE;
}

// Intersects the sphere with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)
real Sphere::intersect(std::shared_ptr<Ray> r, Vector3D &normal)
{
    real b, c, t, t1, t2, discriminant, squareRoot;
    std::shared_ptr<Vector3D> rayDirection, difRayCenter, n;
    std::shared_ptr<Point3D> rayOrigin, intersection;  
  
    // To calculate the intersection, we substitute ray parameters
    // in the sphere equation
    // We need to resolve a second grade equation
    // at^2 + bt + c = 0

    rayDirection = r->getDirection();
    rayOrigin = r->getOrigin();  

    difRayCenter = rayOrigin->substract(this->center);

    // If we consider the ray direction to be normalized,
    // we don't need to calculate a = Dir * Dir, because the value es 1
    b = difRayCenter->dotProduct(rayDirection);
    c = difRayCenter->dotProduct(difRayCenter) - 
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

        std::shared_ptr<Vector3D> tD = rayDirection->product(t);
        intersection = rayOrigin->sum(tD);

        n = intersection->substract(this->center);
        //n = this->center->substract(intersection);
        n = n->product(1.0 / this->radius);

        normal.x = n->x;
        normal.y = n->y;
        normal.z = n->z;
        normal.normalize();

        return(t);
    }

    return(0);
}


