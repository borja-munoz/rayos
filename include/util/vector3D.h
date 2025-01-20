#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include "util.h"

#include <math.h>

#include <memory>

class Vector3D
{
  public:

    real x;
    real y;
    real z;
  
    /*
	Vector3D();
	Vector3D(real x, real y, real z);
	Vector3D(const Vector3D &v);
	~Vector3D();
	real dotProduct(Vector3D *v) const;
	Vector3D *crossProduct(Vector3D *v) const;
	real length(void) const;
	void normalize(void);    
	Vector3D *sum(Vector3D *v) const;
	inline Vector3D *substract(Vector3D *v) const;
	Vector3D *product(real d) const;
    */

    inline Vector3D()
    {
        this->x = this->y = this->z = 0;
    }

    inline Vector3D(real x, real y, real z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline Vector3D(const Vector3D &v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    ~Vector3D()
    {

    }

    inline real dotProduct(const Vector3D& v) const
    {
        return(this->x * v.x + this->y * v.y + this->z * v.z);
    }

    inline Vector3D crossProduct(const Vector3D& v) const
    {
        return Vector3D(
            this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x
        );
    }

    inline real length(void) const
    {
        return(sqrt(this->x * this->x + this->y * this->y + this->z * this->z));
    }

    inline void normalize(void)
    {
        real length;
        
        length = this->length();
        
        this->x /= length;
        this->y /= length;
        this->z /= length;
    }

    inline Vector3D sum(const Vector3D& v) const
    {
        return(Vector3D(this->x + v.x, 
                        this->y + v.y, 
                        this->z + v.z));
    }

    inline Vector3D substract(const Vector3D& v) const
    {
        return(Vector3D(this->x - v.x, 
                        this->y - v.y, 
                        this->z - v.z));
    }

    inline Vector3D product(real d) const
    {
        return(Vector3D(this->x * d,
                        this->y * d,
                        this->z * d));
    }

};

#endif
