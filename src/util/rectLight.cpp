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

Point3D RectLight::getSamplePoint(void) const
{
	Point3D samplePoint, p;
	real xi, eta;
	Vector3D aux1, aux2, aux3;
	std::vector<Point3D> coordinates;

    // To generate a random point within a quad,
    // we have used information from this URL:
	// http://www.magic-software.com/Resources/TechPosts/RandomPointInAQuad.Nov2001.txt

    // We could also divide the quad in two triangles and generate
    // a random point within one of the them

	xi  = getRandomNumberMT(-1, 1);
	eta = getRandomNumberMT(-1, 1);

	// samplePoint = {(1-xi)(1-eta)A + (1+xi)(1-eta)B + (1+xi)(1+eta)C + (1-xi)((1+eta)D} / 4

  std::shared_ptr<Quad> quadLight = std::dynamic_pointer_cast<Quad>(this->location);
	coordinates = quadLight->getCoordinates();

	p = coordinates[0];
	aux1 = Vector3D(p.x, p.y, p.z);
	aux2 = aux1.product((1 - xi) * (1 - eta));  // aux2 = (1-xi)(1-eta)A

	p = coordinates[1];
	aux1 = Vector3D(p.x, p.y, p.z);
	aux3 = aux1.product((1 + xi) * (1 - eta));
	aux1 = aux2.sum(aux3);					  // aux1 = (1-xi)(1-eta)A + (1+xi)(1-eta)B

	p = coordinates[2];
	aux2 = Vector3D(p.x, p.y, p.z);
	aux3 = aux2.product((1 + xi) * (1 + eta));
	aux2 = aux1.sum(aux3);					  // aux2 = (1-xi)(1-eta)A + (1+xi)(1-eta)B + (1+xi)(1+eta)C

	p = coordinates[3];
	aux1 = Vector3D(p.x, p.y, p.z);
	aux3 = aux1.product((1 - xi) * (1 + eta));
	aux1 = aux2.sum(aux3);				  

	samplePoint = Point3D(aux1.x / 4, aux1.y / 4, aux1.z / 4);

	return(samplePoint);
}

real RectLight::getArea(void) const
{
  std::shared_ptr<Quad> quadLight = std::dynamic_pointer_cast<Quad>(this->location);
	return quadLight->getArea();
}

Vector3D RectLight::getNormal(void) const
{
  std::shared_ptr<Quad> quadLight = std::dynamic_pointer_cast<Quad>(this->location);
	return quadLight->getNormal();
}




