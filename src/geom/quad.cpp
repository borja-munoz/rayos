#include "quad.h"

Quad::Quad()
{
	std::shared_ptr<Vector3D> aux1, aux2;

	this->vertex[0] = std::make_shared<Point3D>(0, 0, 0);
	this->vertex[1] = std::make_shared<Point3D>(1, 0, 0);
	this->vertex[2] = std::make_shared<Point3D>(1, 1, 0);
	this->vertex[3] = std::make_shared<Point3D>(0, 1, 0);

	aux1 = this->vertex[1]->substract(this->vertex[0]);
	aux2 = this->vertex[2]->substract(this->vertex[0]);
	this->normal = aux1->crossProduct(aux2);
	this->normal->normalize();

	this->material = std::make_shared<Material>();
	  
	this->type = QUAD;
}


//-------------------------------------------------------------------------------

Quad::Quad(std::shared_ptr<Point3D> a, 
           std::shared_ptr<Point3D> b, 
           std::shared_ptr<Point3D> c, 
           std::shared_ptr<Point3D> d)
{
	std::shared_ptr<Vector3D> aux1, aux2;

	this->vertex[0] = a;
	this->vertex[1] = b;
	this->vertex[2] = c;
	this->vertex[3] = d;

	aux1 = this->vertex[1]->substract(this->vertex[0]);
	aux2 = this->vertex[2]->substract(this->vertex[0]);
	this->normal = aux1->crossProduct(aux2);
	this->normal->normalize();

	this->material = std::make_shared<Material>();
	  
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
}

Quad::Quad(std::shared_ptr<Point3D> a, 
           std::shared_ptr<Point3D> b, 
           std::shared_ptr<Point3D> c, 
           std::shared_ptr<Point3D> d,
           std::shared_ptr<Material> mat)
{
	std::shared_ptr<Vector3D> aux1, aux2;

	this->vertex[0] = a;
	this->vertex[1] = b;
	this->vertex[2] = c;
	this->vertex[3] = d;

	aux1 = this->vertex[1]->substract(this->vertex[0]);
	aux2 = this->vertex[2]->substract(this->vertex[0]);
	this->normal = aux1->crossProduct(aux2);
	this->normal->normalize();

	this->material = mat;
	  
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

}

std::shared_ptr<Vector3D> Quad::getNormal(void)
{
	return(this->normal);
}

// Intersects the poligon with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)
real Quad::intersect(std::shared_ptr<Ray> r, Vector3D &normal)
{
	std::shared_ptr<Point3D> intersection;
	real t, a0, b0, a1, b1;
	unsigned int crosses;

    // If ray is parallel to polygon, there is no intersection 
	std::shared_ptr<Vector3D> rayDirection = r->getDirection();
	t = rayDirection->dotProduct(this->normal);
    if (ZERO(t)) 
	{
		return(0);
	}

    // We calculate intersection between the ray and the polygon plane 
	std::shared_ptr<Point3D> rayOrigin = r->getOrigin();
	std::shared_ptr<Vector3D> vec1 = this->vertex[0]->substract(rayOrigin);
	t = vec1->dotProduct(this->normal) / t;
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

		return(t); 
    }

	return(0);
}

// Convert the four side polygon in two triangles
vector<std::shared_ptr<Triangle>> Quad::tessellate()
{
	vector<std::shared_ptr<Triangle>> t;

	t.push_back(std::make_shared<Triangle>(this->vertex[0], this->vertex[1], this->vertex[2], this->material));
	t.push_back(std::make_shared<Triangle>(this->vertex[0], this->vertex[2], this->vertex[3], this->material));

	return(t);
}

real Quad::getArea(void)
{
	real base, height;
	std::shared_ptr<Vector3D> vBase, vHeight;

	vBase = this->vertex[1]->substract(this->vertex[0]);
	vHeight = this->vertex[3]->substract(this->vertex[0]);
	base = vBase->length();
	height = vHeight->length();

	return(base * height);
}

vector<std::shared_ptr<Point3D>> Quad::getCoordinates(void)
{
	vector<std::shared_ptr<Point3D>> coordinates;

	for (int i = 0; i < 4; i++)
		coordinates.push_back(this->vertex[i]);

	return(coordinates);
}


