#include "tracer.h"
#include "util/chrono.h"


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
std::optional<HitPoint> Tracer::getHitPoint(const Ray& r, 
                                            std::shared_ptr<Scene> s)
{
	// HitPoint h = HitPoint();
	// unsigned int numberObjects, i, j;
	// vector<real> intersection;
	// vector<Vector3D> normal;
	// Vector3D N;
	// std::shared_ptr<Primitive> object;
	  
	// // We intersect the ray with all the objects
    // // and select the one with the nearest intersection

	// numberObjects = s->getNumberObjects();
	// for (i = 0; i < numberObjects; i++)
	// {    
	// 	object = s->object[i];
	// 	intersection.push_back(object->intersect(r, N, 0.001, __FLT_MAX__));
	// 	normal.push_back(Vector3D(N.x, N.y, N.z));
	// }

	// int nearest = -1;
	// for (i = 0; i < numberObjects; i++)
	// 	if (intersection[i] > 0)
	// 	{
	// 		nearest = i;
	// 		break;
	// 	}
	// for (j = i + 1; j < numberObjects; j++)
	// 	if ((intersection[j] > 0) && (intersection[j] < intersection[nearest]))
	// 		nearest = j;

	// if (nearest != -1)
	// {
	// 	// Calculate hitpoint, object and normal
	// 	h.hitPoint = r.pointParametric(intersection[nearest]);
	// 	h.nearestObject = nearest;
	// 	h.normal = normal[nearest];
	// 	h.normal.normalize();
	// }
	// else
	// 	return std::nullopt;

	// return h;

    std::optional<HitPoint> result;

    real tMin = 0.001f;   // Minimum distance to avoid self-intersections
    real tMax = FLT_MAX;  // Maximum allowable distance
    real closestT = tMax; // Track the closest intersection distance

	// Intersection using the BVH structure
	result = s->intersect(r, tMin, closestT);

	if (result) { // If an intersection was found
		// Store the hit point and normal
		// result.hitPoint = r.origin.sum(r.direction.product(t));
		// result.normal = normal;

		// Find the nearest object (assuming BVH returns the closest object)
		// result.nearestObject = s.bvh->getNearestObjectIndex(ray, t);

		if ((*result).distance > tMin && (*result).distance < tMax) {
			// Return the hit point information
			return *result;
		}
	}

    // No intersection, return std::nullopt
    return std::nullopt;	
}


TimeStats Tracer::getTimeStats()
{
  return(this->timeStats);
}