#include "ray.h"

Ray::Ray()
{
	this->origin = Point3D();
	this->direction = Vector3D();
}

Ray::Ray(const Point3D& p, const Vector3D& direction)
{
	this->origin = p;
	this->direction = direction;
}

Ray::~Ray()
{
}

Point3D Ray::getOrigin(void) const
{
	return(this->origin);  
}

Vector3D Ray::getDirection(void) const
{
	return(this->direction);
}

void Ray::setOrigin(const Point3D& p)
{
	this->origin = p;
}

void Ray::setDirection(const Vector3D& v)
{
	this->direction = v;
}

// Calculates a point in the ray using the parametric equation
Point3D Ray::pointParametric(real t) const
{
	Vector3D tD;
	Point3D p;
	
	tD = this->direction.product(t);
	
	p = this->origin.sum(tD);

	return(p);
}


