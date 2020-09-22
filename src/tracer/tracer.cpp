#include "tracer.h"


Tracer::Tracer()
{
	this->BRDF = PhongBRDF;

	//initGenSSE(12345678);
	//this->getRandomNumber = getRandomNumberSSE;
	initGenMT(12345678);
	this->getRandomNumber = getRandomNumberMT;
}

Tracer::Tracer(BRDFtype t)
{
	switch(t)
	{
		case PHONG:
			this->BRDF = PhongBRDF;
			break;
		case BLINN:
			this->BRDF = BlinnBRDF;
			break;
		default:
			this->BRDF = PhongBRDF;
	}
}

// Trace a ray in the scene and returns information about the hitpoint:
// Point, object and normal
std::shared_ptr<HitPoint> Tracer::getHitPoint(std::shared_ptr<Ray> r, 
                                              std::shared_ptr<Scene> s)
{
	std::shared_ptr<HitPoint> h = std::make_shared<HitPoint>();
	unsigned int numberObjects, i, j;
	vector<real> intersection;
	vector<std::shared_ptr<Vector3D>> normal;
	Vector3D N;
	std::shared_ptr<Primitive> object;
	  
	// We intersect the ray with all the objects
    // and select the one with the nearest intersection

	numberObjects = s->getNumberObjects();
	for (i = 0; i < numberObjects; i++)
	{    
		object = s->object[i];
		intersection.push_back(object->intersect(r, N));
		normal.push_back(std::make_shared<Vector3D>(N.getX(), N.getY(), N.getZ()));
	}

	int nearest = -1;
	for (i = 0; i < numberObjects; i++)
		if (intersection[i] > 0)
		{
			nearest = i;
			break;
		}
	for (j = i + 1; j < numberObjects; j++)
		if ((intersection[j] > 0) && (intersection[j] < intersection[nearest]))
			nearest = j;

	if (nearest != -1)
	{
		// Calculate hitpoint, object and normal
		h->hitPoint = r->pointParametric(intersection[nearest]);
		h->nearestObject = nearest;
		h->normal = normal[nearest];
		h->normal->normalize();
	}
	else
		h = 0;

	return(h);
}

