#include "rectLight.h"

RectLight::RectLight()
{
}

RectLight::RectLight(std::shared_ptr<Quad> location, std::shared_ptr<Color> color, real intensity)
	: Light(color, intensity)
{	
	this->location = location;
}

std::shared_ptr<Quad> RectLight::getLocation(void)
{
	return(this->location);
}

void RectLight::setLocation(std::shared_ptr<Quad> q)
{
	this->location = q;
}

std::shared_ptr<Point3D> RectLight::getSamplePoint(void)
{
	std::shared_ptr<Point3D> samplePoint, p;
	real xi, eta;
	std::shared_ptr<Vector3D> aux1, aux2, aux3;
	vector<std::shared_ptr<Point3D>> coordinates;

    // To generate a random point within a quad,
    // we have used information from this URL:
	// http://www.magic-software.com/Resources/TechPosts/RandomPointInAQuad.Nov2001.txt

    // We could also divide the quad in two triangles and generate
    // a random point within one of the them

	xi  = getRandomNumberMT(-1, 1);
	eta = getRandomNumberMT(-1, 1);

	// samplePoint = {(1-xi)(1-eta)A + (1+xi)(1-eta)B + (1+xi)(1+eta)C + (1-xi)((1+eta)D} / 4

	coordinates = this->location->getCoordinates();

	p = coordinates[0];
	aux1 = std::make_shared<Vector3D>(p->x, p->y, p->z);
	aux2 = aux1->product((1 - xi) * (1 - eta));  // aux2 = (1-xi)(1-eta)A

	p = coordinates[1];
	aux1 = std::make_shared<Vector3D>(p->x, p->y, p->z);
	aux3 = aux1->product((1 + xi) * (1 - eta));
	aux1 = aux2->sum(aux3);					  // aux1 = (1-xi)(1-eta)A + (1+xi)(1-eta)B

	p = coordinates[2];
	aux2 = std::make_shared<Vector3D>(p->x, p->y, p->z);
	aux3 = aux2->product((1 + xi) * (1 + eta));
	aux2 = aux1->sum(aux3);					  // aux2 = (1-xi)(1-eta)A + (1+xi)(1-eta)B + (1+xi)(1+eta)C

	p = coordinates[3];
	aux1 = std::make_shared<Vector3D>(p->x, p->y, p->z);
	aux3 = aux1->product((1 - xi) * (1 + eta));
	aux1 = aux2->sum(aux3);				  

	samplePoint = std::make_shared<Point3D>(aux1->x / 4, aux1->y / 4, aux1->z / 4);

	return(samplePoint);
}

real RectLight::getArea(void)
{
	return(this->location->getArea());
}

std::shared_ptr<Vector3D> RectLight::getNormal(void)
{
	return(this->location->getNormal());
}




