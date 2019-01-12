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
HitPoint * Tracer::getHitPoint(Ray *r, Scene *e)
{
	HitPoint *h = new HitPoint;
	unsigned int numberObjects, i, j;
	real *intersection;
	vector<Vector3D *> *normal;
	Vector3D N;
	Primitive *object;
	  
	// We intersect the ray with all the objects
    // and select the one with the nearest intersection

	numberObjects = e->getNumberObjects();
	intersection = new real[numberObjects];
	normal = new vector<Vector3D *>;
	for (i = 0; i < numberObjects; i++)
	{    
		object = (*(e->object))[i];
		intersection[i] = object->intersect(r, N);
		normal->push_back(new Vector3D(N));
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
		h->normal = new Vector3D(*((*normal)[nearest]));
		h->normal->normalize();
	}
	else
		h = 0;

	for (i = 0; i < numberObjects; i++)
		delete((*normal)[i]);
	delete(normal);
	delete[](intersection);

	return(h);
}


//-------------------------------------------------------------------------------
