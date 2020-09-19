#include "stochasticRayTracer.h"
#include "util/chrono.h"

StochasticRayTracer::StochasticRayTracer()
{
	sampleRays = 4;
	shadowRays = 4;
	indirectRays = 4;
}


StochasticRayTracer::StochasticRayTracer(unsigned int sampleRays, unsigned int shadowRays)
{
	this->sampleRays = sampleRays;
	this->shadowRays = shadowRays;
	this->indirectRays = 4;
}


StochasticRayTracer::StochasticRayTracer(BRDFtype t)
	: Tracer(t)
{
	sampleRays = 4;
	shadowRays = 1;
}


Bitmap * StochasticRayTracer::trace(Scene *e)
{
	unsigned int rx, ry;
	real *rad, radiance[3];
	Bitmap *im;
	Camera *cam;
	Point3D *viewer;
	vector<Ray *> *eyeRays;
    Ray *eyeRay;
	real *probLight;

	cam = e->getCamera();

	viewer = cam->getLocation();

	cam->getResolution(rx, ry);
	im = new Bitmap(rx, ry);

    // We pre-calculate the probability of points in each light source (uniform distribution)
	// prob(y) = prob(k) * prob(y|k)
	// y = Point within light source
	// k = k light source
	probLight = new real[e->getNumberLights()];
	for (unsigned int i = 0; i < e->getNumberLights(); i++)
		probLight[i] = (1.0 / e->getNumberLights()) * (1.0 / e->getLight(i)->getArea());
	probLight[0] = 1.0;

	// We loop through the entire viewplane
	for (unsigned int i = 0; i < ry; i++)
	{
		for (unsigned int j = 0; j < rx; j++)
		{
            Chrono *c = new Chrono();
            c->start();

			radiance[0] = radiance[1] = radiance[2] = 0;
            if (this->sampleRays == 1)
            {
                eyeRay = cam->getEyeRay(j, i);
                rad = traceRay(eyeRay, e, probLight, viewer);
                for (unsigned int x = 0; x < 3; x++)
                    radiance[x] = rad[x];
                delete[](rad);			
                delete(eyeRay);
            }
			else
            {
                // We send several rays for each pixel to reduce 
                // aliasing and improve image quality
                eyeRays = cam->getSampleEyeRays(j, i, this->sampleRays);
                for (unsigned int k = 0; k < this->sampleRays; k++)
                {
                    
                    // _CrtMemState s1, s2, s3;
                    // _CrtMemCheckpoint(&s1);

                    rad = traceRay((*eyeRays)[k], e, probLight, viewer);

                    for (unsigned int x = 0; x < 3; x++)
                        radiance[x] += rad[x];
                    delete[](rad);			

                    //_CrtMemCheckpoint(&s2);
                    //if (_CrtMemDifference(&s3, &s1, &s2))
                    //	_CrtMemDumpStatistics(&s3);

                }

                for (unsigned int k = 0; k < this->sampleRays; k++)
                    delete((*eyeRays)[k]);
                delete(eyeRays);

                for (unsigned int x = 0; x < 3; x++)
                    radiance[x] /= this->sampleRays;
            }

			im->setHDRPixel(i, j, radiance[0], radiance[1], radiance[2]);

            c->stop();
        	//cout << "\nPixel elapsed time = " << c->value()*1000 << " milliseconds\n";

		}
		if (i % 10 == 0)
			cout << "Completed = " << i * 100 / ry << "%" << endl;
	}

	delete[](probLight);
	delete(cam);
	delete(viewer);

	return(im);
}


real * StochasticRayTracer::traceRay(Ray *r, Scene *e, real *probLight, Point3D *viewer)
{
	real *radiance = new real[3];
	Vector3D *radianceDirection;
	HitPoint *h;

    // The RayTrace must intersect the ray with all the objects
    // in the scene and calculate the nearest to the viewer.
    // Finally, we will evaluate a local BRDF in the hitpoint.

    Chrono *c1 = new Chrono();
    c1->start();
	h = this->getHitPoint(r, e);
    c1->stop();
	//cout << "getHitPoint elapsed time = " << c1->value()*1000 << " milliseconds\n";

    // If we have a hitpoint, we must calculate radiance emitted in the viewer direction
	if (h != 0)
	{
		// Direction where we need to find the radiance
        Chrono *c2 = new Chrono();
        c2->start();
		radianceDirection = viewer->substract(h->hitPoint);
		radianceDirection->normalize();

		radiance = this->calculateRadiance(h, radianceDirection, e, probLight);
        c2->stop();
    	//cout << "calculateRadiance elapsed time = " << c2->value() * 1000 << " milliseconds\n";

		delete(radianceDirection);
		delete(h->hitPoint);
		delete(h->normal);
		delete(h);
	}
	else
		radiance[0] = radiance[1] = radiance[2] = 0;

	return(radiance);
}


//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Calculates reflected radiance from a given point in a given direction
// h          : Point 
// dir        : Direction
// nearest    : Nearest intersected object index
// normal     : Nearest object normal in the hitpoint
//-------------------------------------------------------------------------------

real * StochasticRayTracer::calculateRadiance(HitPoint *h, Vector3D *dir, Scene *s, real *probLight)
{
	// Radiance is composed of object emitted radiance (if the object is a light source)
    // plus object reflected radiance.
    // Reflected radiance is the sum of direct lighting from light source plus 
    // indirect light
    // Summary:
    // Radiance = Emitted Radiance + Reflected Radiance
    // Reflected Radiance = Direct Lighting + Indirect Lighting
	
	real *radiance = new real[3];
    real *emittedRadiance;
    real *reflectedRadiance = new real[3];
    real *directLighting;
    real *indirectLighting;

    emittedRadiance = this->emittedRadiance(s, h);

	directLighting = this->directLighting(s, h, probLight, dir);
	indirectLighting = this->indirectLighting(s, h, probLight);
	for (int i = 0; i < 3; i++)
		reflectedRadiance[i] = directLighting[i] + indirectLighting[i];

	for (int i = 0; i < 3; i++)
		radiance[i] = emittedRadiance[i] + reflectedRadiance[i];

	delete[](emittedRadiance);
	delete[](directLighting);
	delete[](indirectLighting);

	return(radiance);
}

// Calculates emitted radiance 
real * StochasticRayTracer::emittedRadiance(Scene *s, HitPoint *h)
{
    real *radiance = new real[3];
    real ke;
    Material *mat;
    real *color;

    Primitive *object = (*(s->object))[h->nearestObject];
    mat = object->getMaterial();
    ke = mat->getKe();
    color = mat->getColor();
    if (ke > 0)
    {
        for (int i = 0; i < 3; i++)
            radiance[i] = color[i] * ke;
    }   
    else
    {
        radiance[0] = radiance[1] = radiance[2] = 0;
    }       

    return(radiance);
}

// Calculates direct lighting given a hitpoint
real * StochasticRayTracer::directLighting(Scene *s, HitPoint *h, real *probLight, Vector3D *dir)
{

    // In order to calculate direct lighting, we send rays to the light sources (shadow rays)
    // We use these rays to calculate the contribution from each of the light sources
    // to lighting in a given point.
    // Light source selection and point selection within the light source is probabilistic,
    // meaning than depending on the probability density function used, the results
    // will be different.
    // Initially, we are going to use uniform probability functions both for selecting
    // the light source and the point within the light source. It is the simplest one
    // and gives the worst results.

	real *radiance = new real[3], *rad;
	int lightIndex;
	Point3D *lightPoint;
	real distancePuntoLight, geometryTerm;
	bool lightVisible;
	Vector3D *L, *lightNormal;
	Primitive *object;
	Material *mat;
	Light *selectedLight;

	radiance[0] = radiance[1] = radiance[2] = 0;

	for (unsigned int i = 0; i < this->shadowRays; i++)
	{
		// Light source selection
		lightIndex = (int) getRandomIntMT(0, s->getNumberLights() - 1);

		// Light point selection
		selectedLight = s->getLight(lightIndex);
		lightPoint = selectedLight->getSamplePoint();
		lightNormal = selectedLight->getNormal();

		// Ray from hitpoint to light point
		L = lightPoint->substract(h->hitPoint);
		distancePuntoLight = L->length();
		L->normalize();

		// Check if we can see the light point from the hitpoint
		lightVisible = s->mutuallyVisible(h->hitPoint, lightPoint);

        // If it is visible, we calculate reflected radiance from the light source
		if (lightVisible)
		{
            // - Each object must have an ID
            // - Light sources will be added to the scene always before the other objects,
            //   to have the lower IDs
            // - We will know if the nearest object is a light source by comparing the 
            //   object ID with the number of light sources in the scene

			//object = e->getObject(nearestObject);
			object = (*(s->object))[h->nearestObject];
			mat = object->getMaterial();
			rad = this->BRDF(mat, *(h->normal), L, dir);	  

			geometryTerm = (h->normal->dotProduct(L) * lightNormal->dotProduct(L)) /
							(distancePuntoLight * distancePuntoLight);

			//for (unsigned int x = 0; x < 3; x++)
				//radiance[x] += (rad[x] * geometryTerm) / probLight[lightIndex];
				//radiance[x] += rad[x];
			radiance[0] += rad[0];
			radiance[1] += rad[1];
			radiance[2] += rad[2];

			delete(mat);
			//delete(object);
			delete[](rad);
		}

		delete(L);
		delete(selectedLight);
		delete(lightNormal);
		delete(lightPoint);
	}

    radiance[0] /= this->shadowRays;
    radiance[1] /= this->shadowRays;
    radiance[2] /= this->shadowRays;

	return(radiance);
}


// Indirect light in a given object point
real * StochasticRayTracer::indirectLighting(Scene *s, HitPoint *h, real *probLight)
{
	real *radiance, *radIndirectRay, alfa, P, random, factor, *brdf;
	Vector3D *psi, *radianceDirection;
	Ray *r;
	HitPoint *hPsi;
	Primitive *object;
    Material *mat;
	int lightIndex;
	Point3D *lightPoint;
	bool lightVisible;
	Vector3D *L, *lightNormal;
	Light *selectedLight;
    real distancePointLight;

	// Initialize
	radiance = new real[3];
	radiance[0] = radiance[1] = radiance[2] = 0;
	alfa = (real) 0.8;                 // Alfa is the material absortion probability
	P = 1 - alfa;
	r = new Ray();
	r->setOrigin(h->hitPoint);

    // We send a ray from the hitpoint to the light point
    // We should select the light source probabilistically
	// L = lightPoint->substract(h->hitPoint);
	// L->normalize();

	// Russian roulette to stop recursion
	random = getRandomNumber(0, 1);
	if (random < P)
	{
        radiance[0] = radiance[1] = radiance[2] = 0;

		for (unsigned int i = 0; i < this->indirectRays; i++)
		{
            // We obtain a random direction by uniformly sampling the hemisphere
			psi = this->getRandomDirection();

            // We calculate the point seen from the hitpoint in direction "psi"			
		    r->setDirection(psi);
			hPsi = this->getHitPoint(r, s);

			// If we have a hitpoint, calculate radiance emitted in direction psi
			if (hPsi != 0)
			{
				// Direction
				radianceDirection = h->hitPoint->substract(hPsi->hitPoint);
				radianceDirection->normalize();

				radIndirectRay = this->calculateRadiance(hPsi, radianceDirection, s, probLight);

                // We should multiply radiance by the following factor:
                // BRDF(intersection) * cos(Nx, Psi) / pdf(Psi)
                // Nx and PSI have been normalized, so the dot product
                // is the cosine of the angle between the two vectors
                // As we uniformly sample the hemisphere, the probability density
                // function is 1 / (2 * Pi)    

                // Light source selection
                lightIndex = (int) getRandomIntMT(0, s->getNumberLights() - 1);

                // Light point selection
                selectedLight = s->getLight(lightIndex);
                lightPoint = selectedLight->getSamplePoint();
                lightNormal = selectedLight->getNormal();

                // Ray from hitpoint to light point
                L = lightPoint->substract(hPsi->hitPoint);
                distancePointLight = L->length();
                L->normalize();

                // Check if we can see the light point from the hitpoint
                lightVisible = s->mutuallyVisible(hPsi->hitPoint, lightPoint);

                // If it is visible, we calculate reflected radiance from the light source
                if (lightVisible)
                {
                    object = (*(s->object))[h->nearestObject];
                    mat = object->getMaterial();
                    brdf = this->BRDF(mat, *(h->normal), L, radianceDirection);
                    factor = h->normal->dotProduct(psi) / (2 * PI_RAYOS);
                    for (int k = 0; k < 3; k++)
                        radiance[k] += radIndirectRay[k] * brdf[k] * factor;		
                    delete(mat);
                }

				delete(radianceDirection);
				delete(hPsi->hitPoint);
				delete(hPsi->normal);
				delete(hPsi);
			}

			delete(psi);
		}

		for (unsigned int i = 0; i < 3; i++)
			radiance[i] /= this->indirectRays;
	}
	else
		radiance[0] = radiance[1] = radiance[2] = 0;

    for (unsigned int i = 0; i < 3; i++)
        radiance[i] /= P;

	delete(r);

	return(radiance);
}


//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// Returns a random direction to send a ray
// From Flipcode - Code of the Day
// http://www.flipcode.com/cgi-bin/msg.cgi?showThread=COTD-RandomUnitVectors&forum=cotd&id=-1
//-------------------------------------------------------------------------------------------

Vector3D * StochasticRayTracer::getRandomDirection(void)
{
	Vector3D *dir;

	real z = getRandomNumber(0.0, 1.0);            // We should use (-1, 1) for a sphere
    real a = getRandomNumber(0.0, 2 * PI_RAYOS);

    real r = sqrtf(1.0f - z * z);

    real x = r * cosf(a);
    real y = r * sinf(a);

	dir = new Vector3D(x, y, z);

	return(dir);
}


//-------------------------------------------------------------------------------

