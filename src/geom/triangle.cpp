#include "triangle.h"


//-----------------------------------------------

Triangle::Triangle()
{
    Vector3D *aux1, *aux2;

    this->vertex[0] = new Point3D(-1, 0, 0);
    this->vertex[1] = new Point3D(0, 1, 0);
    this->vertex[2] = new Point3D(1, 0, 0);

    aux1 = this->vertex[1]->substract(this->vertex[0]);
    aux2 = this->vertex[2]->substract(this->vertex[0]);
    this->normal = aux1->crossProduct(aux2);
    this->normal->normalize();
    this->distance = - (this->normal->getX() * this->vertex[0]->getX() +
                        this->normal->getY() * this->vertex[0]->getY() +
                        this->normal->getZ() * this->vertex[0]->getZ());

    this->material = new Material();
    
    this->type = TRIANGULO;
    
    delete(aux1);
    delete(aux2);
}


//-----------------------------------------------

Triangle::Triangle(Point3D *x, Point3D *y, Point3D *z, Material *mat)
{
    Vector3D *aux1, *aux2;

    this->vertex[0] = new Point3D(*x);
    this->vertex[1] = new Point3D(*y);
    this->vertex[2] = new Point3D(*z);

    aux1 = this->vertex[1]->substract(this->vertex[0]);
    aux2 = this->vertex[2]->substract(this->vertex[0]);
    this->normal = aux1->crossProduct(aux2);
    this->normal->normalize();
    this->distance = - (this->normal->getX() * this->vertex[0]->getX() +
                        this->normal->getY() * this->vertex[0]->getY() +
                        this->normal->getZ() * this->vertex[0]->getZ());

    this->material = new Material(*mat);
    
    this->type = TRIANGULO;
    
    delete(aux1);
    delete(aux2);
}


//-----------------------------------------------

Triangle::Triangle(const Triangle &t)
{
    for (int i = 0; i < 3; i++)
        this->vertex[i] = new Point3D(*(t.vertex[i]));
    this->normal = new Vector3D(*(t.normal));
    this->distance = t.distance;
    this->material = new Material(*(t.material));
    this->type = TRIANGULO;
}

//-----------------------------------------------

Triangle * Triangle::copy()
{
	return(new Triangle(*this));
}


//-----------------------------------------------

Triangle::~Triangle()
{
	unsigned int i;

	for (i = 0; i < 3; i++)
		delete(this->vertex[i]);
	
	delete(this->normal);
}


//-----------------------------------------------

// Intersects the poligon with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)

real Triangle::intersect(Ray *r, Vector3D &normal)
{
	int crosses; 
	real denominator, t, x0, y0, x1, y1;
	Point3D *intersection, *rayOrigin;
	Vector3D *rayDirection;

    // We calculate intersection between the ray and the triangle plane 

	// Now we need to find if the hit point is contained within the polygon
	// We project onto one plane and calculate the number of crosses

	// We can express the plane with the following equation:
	//        (P - P') * Normal = 0
	// where P and P' are points beloging to the plane
	// We can substitute the ray in that equation      
	rayDirection = r->getDirection();
	denominator = rayDirection->dotProduct(this->normal);
	if (ZERO(denominator))
	{
		delete(rayDirection);
		return(0);
	}

    rayOrigin = r->getOrigin();
	Vector3D *oV0;
	oV0 = rayOrigin->substract(this->vertex[0]);
    t = - (oV0->dotProduct(this->normal)) / denominator;
	delete(oV0);
    
	Vector3D *td;
	td = rayDirection->product(t);
    intersection = rayOrigin->sum(td);
	delete(td);
	
    // We calculate the greater normal vector component (absolute value)
    // to select the plane where we will do the projection (the one that maximizes the projected region)
	char greater;
	if (fabs(this->normal->getX()) > fabs(this->normal->getY()))
	{
		if (fabs(this->normal->getX()) > fabs(this->normal->getZ()))
			greater = 'X';
		else
			greater = 'Z';
	}
	else
	{
		if (fabs(this->normal->getY()) > fabs(this->normal->getZ()))
			greater = 'Y';
		else
			greater = 'Z';
	}
	
	// We calculate the number of crosses with the selected plane
	crosses = 0;
	if (greater == 'X')       // 'X' is the greater, we project over YZ plane 
	{
		x0 = this->vertex[2]->z - intersection->z;
		y0 = this->vertex[2]->y - intersection->y;
		for (int i = 0; i < 3; i++)
		{
			x1 = this->vertex[i]->z - intersection->z;
			y1 = this->vertex[i]->y - intersection->y;
			if (y0 > 0)
			{
				if (y1 <= 0)
				if (x1 * y0 > x0 * y1)
					crosses++;
			}
			else 
			{
				if (y1 > 0)
				if (x0 * y1 > y0 * x1)
					crosses++;
			}  
			x0 = x1;
			y0 = y1;
		}
	}
	else if (greater == 'Y')   // 'Y' is the greater, we project over XZ plane 
	{
		x0 = this->vertex[2]->x - intersection->x;
		y0 = this->vertex[2]->z - intersection->z;
		for (int i = 0; i < 3; i++)
		{
			x1 = this->vertex[i]->x - intersection->x;
			y1 = this->vertex[i]->z - intersection->z;
			if (y0 > 0)
			{
				if (y1 <= 0)
				if (x1 * y0 > x0 * y1)
					crosses++;
			}
			else 
			{
				if (y1 > 0)
				if (x0 * y1 > y0 * x1)
					crosses++;
			}  
			x0 = x1;
			y0 = y1;
		}
	}
	else                    // 'Z' is the greater, we project over XY plane 
	{
		x0 = this->vertex[2]->x - intersection->x;
		y0 = this->vertex[2]->y - intersection->y;
		for (int i = 0; i < 3; i++)
		{
			x1 = this->vertex[i]->x - intersection->x;
			y1 = this->vertex[i]->y - intersection->y;
			if (y0 > 0)
			{
				if (y1 <= 0)
				if (x1 * y0 > x0 * y1)
					crosses++;
			}
			else 
			{
				if (y1 > 0)
				if (x0 * y1 > y0 * x1)
					crosses++;
			}  
			x0 = x1;
			y0 = y1;
		}
	}

	// If there is an odd number of crosses, there is an intersection
	if (crosses & 1)      
    {
		normal.setX(this->normal->getX());
		normal.setY(this->normal->getY());
		normal.setZ(this->normal->getZ());
    }
	else
		t = 0;

	delete(rayDirection);
	delete(rayOrigin);
	delete(intersection);

	return(t);
}


//-----------------------------------------------

