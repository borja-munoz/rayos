#include "pointLight.h"

PointLight::PointLight()
{
}

PointLight::PointLight(std::shared_ptr<Point3D> location, std::shared_ptr<Color> color, real intensity)
	: Light(color, intensity)
{
	this->location = location;
}

std::shared_ptr<Point3D> PointLight::getLocation(void)
{
	return(this->location);
}

void PointLight::setLocation(std::shared_ptr<Point3D> location)
{
	this->location = location;
}

std::shared_ptr<Point3D> PointLight::getSamplePoint(void)
{
	return(this->location);
}

// The area for a point light source is 1 because the probability
// of selecting a point within this light source is always 1
// (we always select the same one)
real PointLight::getArea(void)
{
	return(1.0);
}

// How do we define the normal to a point light source?
std::shared_ptr<Vector3D> PointLight::getNormal(void)
{
	return(std::make_shared<Vector3D>());
}
