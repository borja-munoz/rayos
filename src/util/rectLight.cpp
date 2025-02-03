#include "rectLight.h"

RectLight::RectLight()
{
}

RectLight::RectLight(std::shared_ptr<Primitive> location, Color color, real intensity)
	: Light(color, intensity)
{	
	this->location = location;
}

std::shared_ptr<Primitive> RectLight::getLocation(void)
{
	return(this->location);
}

void RectLight::setLocation(std::shared_ptr<Primitive> q)
{
	this->location = q;
}

real RectLight::getArea(void) const
{
	if (std::dynamic_pointer_cast<Quad>(this->location)) 
	{
		std::shared_ptr<Quad> quadLight = 
			std::dynamic_pointer_cast<Quad>(this->location);
		return quadLight->getArea();
	}
	else if (std::dynamic_pointer_cast<TriangleMesh>(this->location))
	{
		// ToDo
		return(0);
	}
	else
	{
		return(0);
	}
}

Vector3D RectLight::getNormal(void) const
{
	if (std::dynamic_pointer_cast<Quad>(this->location)) 
	{
		std::shared_ptr<Quad> quadLight = 
			std::dynamic_pointer_cast<Quad>(this->location);
		return quadLight->getNormal();
	}
	else if (std::dynamic_pointer_cast<TriangleMesh>(this->location))
	{
		// ToDo
		return(Vector3D(0, 0, 0));
	}
	else
	{
		return(Vector3D(0, 0, 0));	
	}
}

Point3D RectLight::getSamplePoint(void) const
{
	if (std::dynamic_pointer_cast<Quad>(this->location)) 
	{
		return(this->getSamplePointQuad());
	}
	else if (std::dynamic_pointer_cast<TriangleMesh>(this->location))
	{
		return(this->getSamplePointMesh());
	}
	else
	{
		return(Point3D(0, 0, 0));
	}
}

Point3D RectLight::getSamplePointQuad(void) const
{
	std::shared_ptr<Quad> quad;

	quad = std::dynamic_pointer_cast<Quad>(this->location);

	return(quad->getSamplePoint());
}

Point3D RectLight::getSamplePointMesh(void) const
{
	std::shared_ptr<TriangleMesh> mesh;

	mesh = std::dynamic_pointer_cast<TriangleMesh>(this->location);

	return(mesh->getSamplePoint());
}