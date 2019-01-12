#include "ray.h"


//-----------------------------------------------------------

Ray::Ray()
{
	this->origin = new Point3D();
	this->direction = new Vector3D();
}


//-----------------------------------------------------------

Ray::Ray(Point3D *p, Vector3D *direction)
{
	this->origin = new Point3D(*p);
	this->direction = new Vector3D(*direction);
}


//-----------------------------------------------------------

Ray::~Ray()
{
	delete(this->origin);
	delete(this->direction);
}


//-----------------------------------------------------------

Point3D * Ray::getOrigin(void)
{
	return(new Point3D(*(this->origin)));  
}


//-----------------------------------------------------------

Vector3D * Ray::getDirection(void)
{
	return(new Vector3D(*(this->direction)));
}

//-----------------------------------------------------------

void Ray::setOrigin(Point3D *p)
{
	delete(this->origin);
	this->origin = new Point3D(*p);
}

//-----------------------------------------------------------

void Ray::setDirection(Vector3D *v)
{
	delete(this->direction);
	this->direction = new Vector3D(*v);
}


//-----------------------------------------------------------

// Calculates a point in the ray using the parametric equation
Point3D * Ray::pointParametric(real t)
{
	Vector3D *tD;
	Point3D *p;
	
	tD = this->direction->product(t);
	
	p = this->origin->sum(tD);

	delete(tD);

	return(p);
}


//-----------------------------------------------------------


