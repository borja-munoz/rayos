#ifndef _LUZ_H
#define _LUZ_H

#include "color.h"
#include "point3D.h"

#include <memory>

class Light
{
  protected:

	Color color;
	real intensity;

  public: 
	Light();
	Light(Color color, real intensity);
	const Color& getColor(void) const;
	real getIntensity(void) const;
	void setColor(Color color);
	void setIntensity(real intensity);
	virtual Point3D getSamplePoint(void) const = 0;
	virtual real getArea(void) const = 0;
	virtual Vector3D getNormal(void) const = 0;
};


//-------------------------------------------------------------------------------

#endif
