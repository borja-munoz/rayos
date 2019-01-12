#include "pointLight.h"

//-------------------------------------------------------------------------------

PointLight::PointLight()
{
}


//-------------------------------------------------------------------------------

PointLight::PointLight(Point3D *location, real *color, real intensity)
	: Light(color, intensity)
{
	this->location = new Point3D(*location);
}


//-------------------------------------------------------------------------------

PointLight::PointLight(PointLight &l)
	: Light(l)
{
	this->location = new Point3D(*(l.location));
}


//-------------------------------------------------------------------------------

PointLight * PointLight::copy(void)
{
	return(new PointLight(*this));
}


//-------------------------------------------------------------------------------

PointLight::~PointLight()
{
	delete(this->location);
}


//-------------------------------------------------------------------------------

Point3D * PointLight::getLocation(void)
{
	return(new Point3D(*(this->location)));
}


//-------------------------------------------------------------------------------

void PointLight::setLocation(Point3D *location)
{
	this->location = new Point3D(*location);
}


//-------------------------------------------------------------------------------

Point3D * PointLight::getSamplePoint(void)
{
	return(new Point3D(*this->location));
}


// The area for a point light source is 1 because the probability
// of selecting a point within this light source is always 1
// (we always select the same one)
real PointLight::getArea(void)
{
	return(1.0);
}


// How do we define the normal to a point light source?
Vector3D * PointLight::getNormal(void)
{
	return(new Vector3D());
}
