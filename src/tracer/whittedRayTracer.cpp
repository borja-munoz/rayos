#include "whittedRayTracer.h"

WhittedRayTracer::WhittedRayTracer()
{

}


WhittedRayTracer::WhittedRayTracer(BRDFtype t)
	: Tracer(t)
{

}


std::shared_ptr<Bitmap> WhittedRayTracer::trace(std::shared_ptr<Scene> s)
{
	unsigned int rx, ry;
	Color radiance;
	std::shared_ptr<Bitmap> im;
	std::shared_ptr<Camera> cam;

	cam = s->getCamera();

	cam->getResolution(rx, ry);
	im = std::make_shared<Bitmap>(rx, ry);

	// We loop through the entire viewplane
	for (unsigned int i = 0; i < ry; i++)
		for (unsigned int j = 0; j < rx; j++)
		{
			radiance = traceRay(cam->getEyeRay(j, i), s);

			im->setHDRPixel(i, j, radiance);
		}

	return(im);
}


// Send a ray and returns radiance emitted
Color WhittedRayTracer::traceRay(std::shared_ptr<Ray> r, std::shared_ptr<Scene> s)
{
	Color radiance;
	int numberObjects, i, j;
	std::shared_ptr<Point3D> hitPoint;
	std::vector<real> intersection;
	std::shared_ptr<Light> light1;
	std::shared_ptr<Vector3D> L, V;
	std::shared_ptr<Primitive> object;
	  
	numberObjects = s->getNumberObjects();
	std::shared_ptr<Vector3D> N[numberObjects];
	light1 = s->getLight(0);
	for (i = 0; i < numberObjects; i++)
	{
		N[i] = std::make_shared<Vector3D>();    
		object = s->getObject(i);
		intersection.push_back(object->intersect(r, *N[i]));
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
            radiance = Color(1, 1, 1);
        }
		else
        {
            // Vector to light source
            L = light1->getSamplePoint()->substract(hitPoint);
            L->normalize();

            // Vector to viewer
            V = s->getCamera()->getLocation()->substract(hitPoint);
            V->normalize();

            radiance = this->BRDF(s->getObject(nearest)->getMaterial(), *N[nearest], L, V);	  
        }
	}
	else
        radiance = Color(0, 0, 0);

	return(radiance);
}
