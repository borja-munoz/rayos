#include "sphere.h"

Sphere::Sphere()
{
    this->center = new Point3D(0, 0, 0);
    this->radius = 1.0;
    this->material = new Material();
    this->type = SPHERE;
}

Sphere::Sphere(Point3D *center, real radius, Material *material)
{
    this->center = new Point3D(*center);
    this->radius = radius;
    this->material = new Material(*material);
    this->type = SPHERE;
}


//-----------------------------------------------

Sphere::Sphere(const Sphere &e)
{
    this->center = new Point3D(*(e.center));
    this->radius = e.radius;
    this->material = new Material(*(e.material));
    this->type = SPHERE;
}


//-----------------------------------------------

Sphere * Sphere::copy()
{
    return(new Sphere(*this));
}


//-----------------------------------------------

Sphere::~Sphere()
{
	delete(this->center);
}


//-----------------------------------------------

// Intersects the sphere with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)

real Sphere::intersect(Ray *r, Vector3D &normal)
{
    real b, c, t, t1, t2, discriminant, raiz;
    Vector3D *rayDirection, *difRayCenter, *n;
    Point3D *rayOrigin, *intersection;  
  
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
        raiz = sqrt(discriminant);
        t1 = - b - raiz;
        t2 = - b + raiz;
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

        Vector3D *tD = rayDirection->product(t);
        intersection = rayOrigin->sum(tD);

        n = intersection->substract(this->center);
        //n = this->center->substract(intersection);
        n = n->product(1.0 / this->radius);

        normal.setX(n->getX());
        normal.setY(n->getY());
        normal.setZ(n->getZ());
        normal.normalize();

        delete(tD);
        delete(n);
        delete(intersection);
        delete(rayDirection);
        delete(rayOrigin);
        delete(difRayCenter);

        return(t);
    }

    delete(rayDirection);
    delete(rayOrigin);
    delete(difRayCenter);

    return(0);
}


//-----------------------------------------------

