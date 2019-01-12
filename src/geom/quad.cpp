#include "quad.h"

Quad::Quad()
{
	Vector3D *aux1, *aux2;

	this->vertex[0] = new Point3D(0, 0, 0);
	this->vertex[1] = new Point3D(1, 0, 0);
	this->vertex[2] = new Point3D(1, 1, 0);
	this->vertex[3] = new Point3D(0, 1, 0);

	aux1 = this->vertex[1]->substract(this->vertex[0]);
	aux2 = this->vertex[2]->substract(this->vertex[0]);
	this->normal = aux1->crossProduct(aux2);
	this->normal->normalize();

	this->material = new Material();
	  
	this->type = QUAD;
	  
	delete(aux1);
	delete(aux2);
}


//-------------------------------------------------------------------------------

Quad::Quad(Point3D *a, Point3D *b, Point3D *c, Point3D *d)
{
	Vector3D *aux1, *aux2;

	this->vertex[0] = new Point3D(*a);
	this->vertex[1] = new Point3D(*b);
	this->vertex[2] = new Point3D(*c);
	this->vertex[3] = new Point3D(*d);

	aux1 = this->vertex[1]->substract(this->vertex[0]);
	aux2 = this->vertex[2]->substract(this->vertex[0]);
	this->normal = aux1->crossProduct(aux2);
	this->normal->normalize();

	this->material = new Material();
	  
	this->type = QUAD;
	  
	// We calculate the greater normal vector component (absolute value)
	// to choose which plane we project to (the one that maximizes the projected region) 
	if (fabs(this->normal->getX()) > fabs(this->normal->getY()))
	{
		if (fabs(this->normal->getX()) > fabs(this->normal->getZ()))
			this->greaterNormalComponent = 'X';
		else
			this->greaterNormalComponent = 'Z';
	}
	else
	{
		if (fabs(this->normal->getY()) > fabs(this->normal->getZ()))
			this->greaterNormalComponent = 'Y';
		else
			this->greaterNormalComponent = 'Z';
	}

	delete(aux1);
	delete(aux2);
}


//-------------------------------------------------------------------------------

Quad::Quad(Point3D *a, Point3D *b, Point3D *c, Point3D *d, Material *mat)
{
	Vector3D *aux1, *aux2;

	this->vertex[0] = new Point3D(*a);
	this->vertex[1] = new Point3D(*b);
	this->vertex[2] = new Point3D(*c);
	this->vertex[3] = new Point3D(*d);

	aux1 = this->vertex[1]->substract(this->vertex[0]);
	aux2 = this->vertex[2]->substract(this->vertex[0]);
	this->normal = aux1->crossProduct(aux2);
	this->normal->normalize();

	this->material = new Material(*mat);
	  
	this->type = QUAD;
	  
	// We calculate the greater normal vector component (absolute value)
	// to choose which plane we project to (the one that maximizes the projected region) 
	if (fabs(this->normal->getX()) > fabs(this->normal->getY()))
	{
		if (fabs(this->normal->getX()) > fabs(this->normal->getZ()))
			this->greaterNormalComponent = 'X';
		else
			this->greaterNormalComponent = 'Z';
	}
	else
	{
		if (fabs(this->normal->getY()) > fabs(this->normal->getZ()))
			this->greaterNormalComponent = 'Y';
		else
			this->greaterNormalComponent = 'Z';
	}

	delete(aux1);
	delete(aux2);
}


//-------------------------------------------------------------------------------

Quad::Quad(const Quad &c)
{
	for (int i = 0; i < 4; i++)
		this->vertex[i] = new Point3D(*(c.vertex[i]));
	this->normal = new Vector3D(*(c.normal));
	this->material = new Material(*(c.material));
	this->type = QUAD;
	this->greaterNormalComponent = c.greaterNormalComponent;
}


//-------------------------------------------------------------------------------

Quad * Quad::copy()
{
	return(new Quad(*this));
}


//-------------------------------------------------------------------------------

Quad::~Quad()
{
	unsigned int i;

	for (i = 0; i < 4; i++)
		delete(this->vertex[i]);
	
	delete(this->normal);
}


//-------------------------------------------------------------------------------

Vector3D * Quad::getNormal(void)
{
	return(new Vector3D(*(this->normal)));
}


//-------------------------------------------------------------------------------

// Intersects the poligon with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)

real Quad::intersect(Ray *r, Vector3D &normal)
{
	Point3D *intersection;
	real t, a0, b0, a1, b1;
	unsigned int crosses;

    // If ray is parallel to polygon, there is no intersection 
	Vector3D *rayDirection = r->getDirection();
	t = rayDirection->dotProduct(this->normal);
    if (ZERO(t)) 
	{
		delete(rayDirection);
		return(0);
	}

    // We calculate intersection between the ray and the polygon plane 
	Point3D *rayOrigin = r->getOrigin();
	Vector3D *vec1 = this->vertex[0]->substract(rayOrigin);
	t = vec1->dotProduct(this->normal) / t;
	//Vector3D *tD = rayDirection->product(t);
	//intersection = rayOrigin->sum(tD);
	//delete(tD);
	delete(vec1);
	delete(rayOrigin);
	delete(rayDirection);
	intersection = r->pointParametric(t);

	// Now we need to find if the hit point is contained within the polygon
	// We project onto one plane and calculate the number of crosses
	
	crosses = 0;
	if (this->greaterNormalComponent == 'X')       // 'X' is the greater, we project over YZ plane
	{
		a0 = this->vertex[3]->z - intersection->z;
		b0 = this->vertex[3]->y - intersection->y;
		for (int i = 0; i < 4; i++)
		{
			a1 = this->vertex[i]->z - intersection->z;
			b1 = this->vertex[i]->y - intersection->y;
			if (b0 > 0)
			{
				if (b1 <= 0)
				if (a1 * b0 > a0 * b1)
					crosses++;
			}
			else 
			{
				if (b1 > 0)
				if (a0 * b1 > b0 * a1)
					crosses++;
			}  
			a0 = a1;
			b0 = b1;
		}
	}
	else if (this->greaterNormalComponent == 'Y')  // 'Y' is the greater, we project over XZ plane
	{
		a0 = this->vertex[3]->x - intersection->x;
		b0 = this->vertex[3]->z - intersection->z;
		for (int i = 0; i < 4; i++)
		{
			a1 = this->vertex[i]->x - intersection->x;
			b1 = this->vertex[i]->z - intersection->z;
			if (b0 > 0)
			{
				if (b1 <= 0)
				if (a1 * b0 > a0 * b1)
					crosses++;
			}
			else 
			{
				if (b1 > 0)
				if (a0 * b1 > b0 * a1)
					crosses++;
			}  
			a0 = a1;
			b0 = b1;
		}
	}
	else                    // 'Z' is the greater, we project over XY plane
	{
		a0 = this->vertex[3]->x - intersection->x;
		b0 = this->vertex[3]->y - intersection->y;
		for (int i = 0; i < 4; i++)
		{
			a1 = this->vertex[i]->x - intersection->x;
			b1 = this->vertex[i]->y - intersection->y;
			if (b0 > 0)
			{
				if (b1 <= 0)
				if (a1 * b0 > a0 * b1)
					crosses++;
			}
			else 
			{
				if (b1 > 0)
				if (a0 * b1 > b0 * a1)
					crosses++;
			}  
			a0 = a1;
			b0 = b1;
		}
	}

	// We have an intersection if the number of crosses is odd
	if (crosses & 1)      
    {
		normal.setX(this->normal->getX());
		normal.setY(this->normal->getY());
		normal.setZ(this->normal->getZ());

		delete(intersection);

		return(t); 
    }

	delete(intersection);

	return(0);
}


//-------------------------------------------------------------------------------

// Convert the four side polygon in two triangles

vector<Triangle *> * Quad::tessellate()
{
	vector<Triangle *> *t = new vector<Triangle *>;

	t->push_back(new Triangle(this->vertex[0], this->vertex[1], this->vertex[2], this->material));
	t->push_back(new Triangle(this->vertex[0], this->vertex[2], this->vertex[3], this->material));

	return(t);
}


//-------------------------------------------------------------------------------

real Quad::getArea(void)
{
	real base, height;
	Vector3D *vBase, *vHeight;

	vBase = this->vertex[1]->substract(this->vertex[0]);
	vHeight = this->vertex[3]->substract(this->vertex[0]);
	base = vBase->length();
	height = vHeight->length();

	delete(vBase);
	delete(vHeight);

	return(base * height);
}


//-------------------------------------------------------------------------------

vector<Point3D *> * Quad::getCoordinates(void)
{
	vector<Point3D *> *coordinates;

	coordinates = new vector<Point3D *>;
	for (int i = 0; i < 4; i++)
		coordinates->push_back(new Point3D(*(this->vertex[i])));

	return(coordinates);
}


//-------------------------------------------------------------------------------


