#include "rectLight.h"

//-------------------------------------------------------------------------------

RectLight::RectLight()
{
}


//-------------------------------------------------------------------------------

RectLight::RectLight(Quad *location, real *color, real intensity)
	: Light(color, intensity)
{	
	this->location = new Quad(*location);
}


//-------------------------------------------------------------------------------

RectLight::RectLight(RectLight &l)
	: Light(l)
{
	this->location = new Quad(*(l.location));
}


//-------------------------------------------------------------------------------

RectLight * RectLight::copy(void)
{
	return(new RectLight(*this));
}


//-------------------------------------------------------------------------------

RectLight::~RectLight()
{
	delete(this->location);
}


//-------------------------------------------------------------------------------

Quad * RectLight::getLocation(void)
{
	return(new Quad(*(this->location)));
}


//-------------------------------------------------------------------------------

void RectLight::setLocation(Quad *c)
{
	this->location = new Quad(*c);
}


//-------------------------------------------------------------------------------

Point3D * RectLight::getSamplePoint(void)
{
	Point3D *samplePoint, *p;
	real xi, eta;
	Vector3D *aux1, *aux2, *aux3;
	vector<Point3D *> *coordinates;

    // To generate a random point within a quad,
    // we have used information from this URL:
	// http://www.magic-software.com/Resources/TechPosts/RandomPointInAQuad.Nov2001.txt

    // We could also divide the quad in two triangles and generate
    // a random point within one of the them

	xi  = getRandomNumberMT(-1, 1);
	eta = getRandomNumberMT(-1, 1);

	// samplePoint = {(1-xi)(1-eta)A + (1+xi)(1-eta)B + (1+xi)(1+eta)C + (1-xi)((1+eta)D} / 4

	coordinates = this->location->getCoordinates();

	p = (*coordinates)[0];
	aux1 = new Vector3D(p->getX(), p->getY(), p->getZ());
	aux2 = aux1->product((1 - xi) * (1 - eta));  // aux2 = (1-xi)(1-eta)A
	delete(aux1);

	p = (*coordinates)[1];
	aux1 = new Vector3D(p->getX(), p->getY(), p->getZ());
	aux3 = aux1->product((1 + xi) * (1 - eta));
	delete(aux1);
	aux1 = aux2->sum(aux3);					  // aux1 = (1-xi)(1-eta)A + (1+xi)(1-eta)B
	delete(aux2);
	delete(aux3);

	p = (*coordinates)[2];
	aux2 = new Vector3D(p->getX(), p->getY(), p->getZ());
	aux3 = aux2->product((1 + xi) * (1 + eta));
	delete(aux2);
	aux2 = aux1->sum(aux3);					  // aux2 = (1-xi)(1-eta)A + (1+xi)(1-eta)B + (1+xi)(1+eta)C
	delete(aux1);
	delete(aux3);

	p = (*coordinates)[3];
	aux1 = new Vector3D(p->getX(), p->getY(), p->getZ());
	aux3 = aux1->product((1 - xi) * (1 + eta));
	delete(aux1); 
	aux1 = aux2->sum(aux3);				  

	samplePoint = new Point3D(aux1->getX() / 4, aux1->getY() / 4, aux1->getZ() / 4);

	delete(aux1);
	delete(aux2);
	delete(aux3);
	for (unsigned int i = 0; i < coordinates->size(); i++)
		delete ((*coordinates)[i]);
	delete(coordinates);

	return(samplePoint);
}


//-------------------------------------------------------------------------------

real RectLight::getArea(void)
{
	return(this->location->getArea());
}


//-------------------------------------------------------------------------------

Vector3D * RectLight::getNormal(void)
{
	return(this->location->getNormal());
}


//-------------------------------------------------------------------------------




