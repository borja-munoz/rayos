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

std::shared_ptr<Point3D> Point3D::sum(std::shared_ptr<Vector3D> v)
{
  return(std::make_shared<Point3D>(this->x + v->x,
                                   this->y + v->y,
                                   this->z + v->z));
}

std::shared_ptr<Vector3D> Point3D::substract(std::shared_ptr<Point3D> p)
{
  return(std::make_shared<Vector3D>(this->x - p->x,
                                    this->y - p->y,
                                    this->z - p->z));
}


