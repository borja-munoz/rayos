#ifndef _LUZ_H
#define _LUZ_H

#include "color.h"
#include "point3D.h"

#include <memory>

class Light
{
  protected:

	std::shared_ptr<Color> color;
	real intensity;

  public: 
	Light();
	Light(std::shared_ptr<Color> color, real intensity);
	std::shared_ptr<Color> getColor(void);
	real getIntensity(void);
	void setColor(std::shared_ptr<Color> color);
	void setIntensity(real intensity);
	virtual std::shared_ptr<Point3D> getSamplePoint(void) = 0;
	virtual real getArea(void) = 0;
	virtual std::shared_ptr<Vector3D> getNormal(void) = 0;
};


//-------------------------------------------------------------------------------

#endif
