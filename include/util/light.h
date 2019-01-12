#ifndef _LUZ_H
#define _LUZ_H

#include "point3D.h"

class Light
{
  protected:

	real color[3];
	real intensity;

  public: 
	Light();
	Light(real *color, real intensity);
	Light(Light &l);
	virtual ~Light();
	virtual Light *copy(void) = 0;
	real *getColor(void);
	real getIntensity(void);
	void setColor(real *color);
	void setIntensity(real intensity);
	virtual Point3D *getSamplePoint(void) = 0;
	virtual real getArea(void) = 0;
	virtual Vector3D *getNormal(void) = 0;
};


//-------------------------------------------------------------------------------

#endif
