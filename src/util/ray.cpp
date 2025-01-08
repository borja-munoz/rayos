#include "ray.h"

Ray::Ray()
{
	this->origin = std::make_shared<Point3D>();
	this->direction = std::make_shared<Vector3D>();
}

Ray::Ray(const std::shared_ptr<Point3D> p, std::shared_ptr<Vector3D> direction)
{
	this->origin = std::make_shared<Point3D>(*p);
	this->direction = direction;
}

Ray::~Ray()
{
}

std::shared_ptr<Point3D> Ray::getOrigin(void)
{
	return(this->origin);  
}

std::shared_ptr<Vector3D> Ray::getDirection(void)
{
	return(std::make_shared<Vector3D>(*(this->direction)));
}

void Ray::setOrigin(std::shared_ptr<Point3D> p)
{
	this->origin = p;
}

void Ray::setDirection(std::shared_ptr<Vector3D> v)
{
	this->direction = v;
}

// Calculates a point in the ray using the parametric equation
std::shared_ptr<Point3D> Ray::pointParametric(real t)
{
	std::shared_ptr<Vector3D> tD;
	std::shared_ptr<Point3D> p;
	
	tD = this->direction->product(t);
	
	p = this->origin->sum(tD);

	return(p);
}


