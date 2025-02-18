#include "rectLight.h"

RectLight::RectLight()
{
}

RectLight::RectLight(TriangleMesh& location, Color color, real intensity)
	: Light(color, intensity)
{	
	this->location = location;
}

real RectLight::getArea(void) const
{
	// ToDo: Compute the area of the triangle mesh
	return(1.0);
}

const TriangleMesh& RectLight::getLocation(void) const
{
	return(this->location);
}

void RectLight::setLocation(TriangleMesh& q)
{
	this->location = q;
}

Vector3D RectLight::getNormal(void) const
{
	// We return the normal of the first triangle
	return this->location.getTriangle(0).getNormal();
}

Point3D RectLight::getSamplePoint(void) const
{
	return(this->location.getSamplePoint());
}