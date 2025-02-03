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
	std::shared_ptr<Primitive> location;
	Point3D getSamplePointQuad(void) const;
	Point3D getSamplePointMesh(void) const;

  public:
 
	RectLight();
	RectLight(std::shared_ptr<Primitive> location, Color color, real intensity);
	std::shared_ptr<Primitive> getLocation(void);
	void setLocation(std::shared_ptr<Primitive> c);
	real getArea(void) const;
	Vector3D getNormal(void) const;
	Point3D getSamplePoint(void) const;
};


//-------------------------------------------------------------------------------

#endif
