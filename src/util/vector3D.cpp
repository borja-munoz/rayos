#include "vector3D.h"

//-------------------------------------------------------------------------------

Vector3D::Vector3D()
{
	this->x = this->y = this->z = 0;
}


//-------------------------------------------------------------------------------

Vector3D::Vector3D(real x, real y, real z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


//-------------------------------------------------------------------------------

Vector3D::Vector3D(const Vector3D &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}


//-------------------------------------------------------------------------------

Vector3D::~Vector3D()
{

}


//-------------------------------------------------------------------------------

real Vector3D::getX(void)
{
	return(this->x);
}


//-----------------------------------------------

real Vector3D::getY(void)
{
	return(this->y);
}


//-----------------------------------------------

real Vector3D::getZ(void)
{
	return(this->z);
}


//-----------------------------------------------

void Vector3D::setX(real x)
{
	this->x = x;
}


//-----------------------------------------------

void Vector3D::setY(real y)
{
	this->y = y;
}


//-----------------------------------------------

void Vector3D::setZ(real z)
{
	this->z = z;
}

//-----------------------------------------------

real Vector3D::dotProduct(Vector3D *v)
{
	return(this->x * v->x + this->y * v->y + this->z * v->z);
}


//-----------------------------------------------

Vector3D * Vector3D::crossProduct(Vector3D *v)
{
	Vector3D *resultado = new Vector3D();
  
	resultado->x = this->y * v->z - this->z * v->y;
	resultado->y = this->z * v->x - this->x * v->z;
	resultado->z = this->x * v->y - this->y * v->x;
  
	return(resultado);
}


//-----------------------------------------------

real Vector3D::length(void)
{
	return(sqrt(this->x * this->x + this->y * this->y + this->z * this->z));
}

//-----------------------------------------------

void Vector3D::normalize(void)
{
	real length;
	  
	length = this->length();
	  
	this->x /= length;
	this->y /= length;
	this->z /= length;
}


//-----------------------------------------------

Vector3D * Vector3D::sum(Vector3D *v)
{
	return(new Vector3D(this->x + v->x, 
		                this->y + v->y, 
                        this->z + v->z));
}


//-----------------------------------------------

Vector3D * Vector3D::substract(Vector3D *v)
{
	return(new Vector3D(this->x - v->x, 
						this->y - v->y, 
						this->z - v->z));
}


//-----------------------------------------------

Vector3D * Vector3D::product(real d)
{
	return(new Vector3D(this->x * d,
						this->y * d,
						this->z * d));
}


//-----------------------------------------------

