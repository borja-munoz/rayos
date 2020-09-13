#include "whittedRayTracer.h"
#include <iostream>

WhittedRayTracer::WhittedRayTracer()
{

}


WhittedRayTracer::WhittedRayTracer(BRDFtype t)
	: Tracer(t)
{

}


Bitmap * WhittedRayTracer::trace(Scene *e)
{
	unsigned int rx, ry;
	real *radiance;
	Bitmap *im;
	Camera *cam;

	cam = e->getCamera();

	cam->getResolution(rx, ry);
	im = new Bitmap(rx, ry);

	// We loop through the entire viewplane
	for (unsigned int i = 0; i < ry; i++)
		for (unsigned int j = 0; j < rx; j++)
		{
			radiance = traceRay(cam->getEyeRay(j, i), e);

			im->setHDRPixel(i, j, radiance[0], radiance[1], radiance[2]);
		}

	return(im);
}


// Send a ray and returns radiance emitted
real * WhittedRayTracer::traceRay(Ray *r, Scene *e)
{
	real *radiance = new real[3];
	int numberObjects, i, j;
	Point3D *hitPoint;
	real *intersection;
	Light *light1;
	Vector3D *L, *V;
	Primitive *object;
	  
	numberObjects = e->getNumberObjects();
	intersection = new real[numberObjects];
	Vector3D *N[numberObjects];
	light1 = e->getLight(0);
	for (i = 0; i < numberObjects; i++)
	{
		N[i] = new Vector3D();    
		object = e->getObject(i);
		intersection[i] = object->intersect(r, *N[i]);
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
	hitPoint = r->getOrigin()->sum(r->getDirection()->product(intersection[nearest]));

	// If we have a hitpoint, we calculate emitted radiance
    // in the viewer direction using the nearest object BRDF
	if (nearest != -1)
	{
        if (nearest == 0)
        {
            // First element in the scene is the light source
            radiance[0] = radiance[1] = radiance[2] = 1;
        }
		else
        {
            // Vector to light source
            L = light1->getSamplePoint()->substract(hitPoint);
            L->normalize();

            // Vector to viewer
            V = e->getCamera()->getLocation()->substract(hitPoint);
            V->normalize();

            radiance = this->BRDF(e->getObject(nearest)->getMaterial(), *N[nearest], L, V);	  
        }
	}
	else
		radiance[0] = radiance[1] = radiance[2] = 0;

	for (unsigned int k = 0; k < numberObjects; k++)
		delete(N[k]);

	return(radiance);
}


/*---------------------------------------------------------*/
