#include "pointLight.h"

PointLight::PointLight()
{
}

PointLight::PointLight(Point3D location, Color color, real intensity)
	: Light(color, intensity)
{
	this->location = location;
}

Point3D PointLight::getLocation(void) const
{
	return(this->location);
}

void PointLight::setLocation(Point3D location)
{
	this->location = location;
}

Point3D PointLight::getSamplePoint(void) const
{
	return(this->location);
}

// The area for a point light source is 1 because the probability
// of selecting a point within this light source is always 1
// (we always select the same one)
real PointLight::getArea(void) const
{
	return(1.0);
}

// How do we define the normal to a point light source?
Vector3D PointLight::getNormal(void) const
{
	return(Vector3D());
}
