#ifndef _RECT_LIGHT_H
#define _RECT_LIGHT_H

#include "../geom/primitive.h"
#include "../geom/quad.h"
#include "../geom/triangleMesh.h"
#include "point3D.h"
#include "util.h"
#include "light.h"

#include <memory>
#include <vector>

class RectLight : public Light
{
	TriangleMesh location;

  public:
 
	RectLight();
	RectLight(TriangleMesh& location, Color color, real intensity);
	const TriangleMesh& getLocation(void) const;
	void setLocation(TriangleMesh& location);
	real getArea(void) const;
	Vector3D getNormal(void) const;
	Point3D getSamplePoint(void) const;
};


//-------------------------------------------------------------------------------

#endif
