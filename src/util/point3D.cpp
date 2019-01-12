#include "point3D.h"

//-----------------------------------------------

Point3D::Point3D()
{
  this->x = this->y = this->z = 0;
}


//-----------------------------------------------

Point3D::Point3D(real x, real y, real z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}


//-----------------------------------------------

Point3D::Point3D(const Point3D &p)
{
  this->x = p.x;
  this->y = p.y;
  this->z = p.z;
}


//-----------------------------------------------

Point3D::~Point3D()
{

}


//-----------------------------------------------

real Point3D::getX(void)
{
  return(this->x);
}


//-----------------------------------------------

real Point3D::getY(void)
{
  return(this->y);
}


//-----------------------------------------------

real Point3D::getZ(void)
{
  return(this->z);
}


//-----------------------------------------------

void Point3D::setX(real x)
{
  this->x = x;
}


//-----------------------------------------------

void Point3D::setY(real y)
{
  this->y = y;
}


//-----------------------------------------------

void Point3D::setZ(real z)
{
  this->z = z;
}

//-----------------------------------------------

Point3D * Point3D::sum(Vector3D *v)
{
  return(new Point3D(this->x + v->getX(),
                     this->y + v->getY(),
                     this->z + v->getZ()));
}


//-----------------------------------------------

Vector3D * Point3D::substract(Point3D *p)
{
  return(new Vector3D(this->x - p->x,
                     this->y - p->y,
                     this->z - p->z));
}


//-----------------------------------------------

