#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include "util.h"

#include <math.h>

#include <memory>

class Vector3D
{
	real x;
	real y;
	real z;
  
  public:
 
    /*
	Vector3D();
	Vector3D(real x, real y, real z);
	Vector3D(const Vector3D &v);
	~Vector3D();
	real getX(void) const;
	real getY(void) const;
	real getZ(void) const;
	void setX(real x);
	void setY(real y);
	void setZ(real z);
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

    inline real getX(void) const
    {
        return(this->x);
    }

    inline real getY(void) const
    {
        return(this->y);
    }

    inline real getZ(void) const
    {
        return(this->z);
    }

    inline void setX(real x)
    {
        this->x = x;
    }

    inline void setY(real y)
    {
        this->y = y;
    }

    inline void setZ(real z)
    {
        this->z = z;
    }

    inline real dotProduct(std::shared_ptr<Vector3D> v) const
    {
        return(this->x * v->x + this->y * v->y + this->z * v->z);
    }

    inline std::shared_ptr<Vector3D> crossProduct(std::shared_ptr<Vector3D> v) const
    {
        std::shared_ptr<Vector3D> resultado = std::make_shared<Vector3D>();
    
        resultado->x = this->y * v->z - this->z * v->y;
        resultado->y = this->z * v->x - this->x * v->z;
        resultado->z = this->x * v->y - this->y * v->x;
    
        return(resultado);
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

    inline std::shared_ptr<Vector3D> sum(std::shared_ptr<Vector3D> v) const
    {
        return(std::make_shared<Vector3D>(this->x + v->x, 
                                          this->y + v->y, 
                                          this->z + v->z));
    }

    inline std::shared_ptr<Vector3D> substract(std::shared_ptr<Vector3D> v) const
    {
        return(std::make_shared<Vector3D>(this->x - v->x, 
                                          this->y - v->y, 
                                          this->z - v->z));
    }

    inline std::shared_ptr<Vector3D> product(real d) const
    {
        return(std::make_shared<Vector3D>(this->x * d,
                                          this->y * d,
                                          this->z * d));
    }

};

#endif
