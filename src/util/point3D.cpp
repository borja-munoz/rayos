#include "point3D.h"

Point3D::Point3D()
{
  this->x = this->y = this->z = 0;
}

Point3D::Point3D(real x, real y, real z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

Point3D::Point3D(const Point3D &p)
{
  this->x = p.x;
  this->y = p.y;
  this->z = p.z;
}

// Access operator for x, y, z by index
real Point3D::operator[](size_t index) const 
{
  switch (index) 
  {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default: throw std::out_of_range("Point3D index out of range");
  }
}

// Non-const access operator for x, y, z
real& Point3D::operator[](size_t index) 
{
  switch (index) 
  {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default: throw std::out_of_range("Point3D index out of range");
  }
}

Point3D Point3D::sum(const Vector3D& v) const
{
  return(Point3D(this->x + v.x,
                 this->y + v.y,
                 this->z + v.z));
}

Vector3D Point3D::substract(const Point3D& p) const
{
  return(Vector3D(this->x - p.x,
                  this->y - p.y,
                  this->z - p.z));
}


