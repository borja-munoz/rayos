#include "quad.h"

Quad::Quad()
{
	Vector3D aux1, aux2;

	this->vertex[0] = Point3D(0, 0, 0);
	this->vertex[1] = Point3D(1, 0, 0);
	this->vertex[2] = Point3D(1, 1, 0);
	this->vertex[3] = Point3D(0, 1, 0);

	aux1 = this->vertex[1].substract(this->vertex[0]);
	aux2 = this->vertex[2].substract(this->vertex[0]);
	this->normal = aux1.crossProduct(aux2);
	this->normal.normalize();

	this->material = Material();
	  
	this->type = QUAD;
}


//-------------------------------------------------------------------------------

Quad::Quad(Point3D a, 
           Point3D b, 
           Point3D c, 
           Point3D d)
{
	Vector3D aux1, aux2;

	this->vertex[0] = a;
	this->vertex[1] = b;
	this->vertex[2] = c;
	this->vertex[3] = d;

	aux1 = this->vertex[1].substract(this->vertex[0]);
	aux2 = this->vertex[2].substract(this->vertex[0]);
	this->normal = aux1.crossProduct(aux2);
	this->normal.normalize();

	this->material = Material();
	  
	this->type = QUAD;
	  
	// We calculate the greater normal vector component (absolute value)
	// to choose which plane we project to (the one that maximizes the projected region) 
	if (fabs(this->normal.x) > fabs(this->normal.y))
	{
		if (fabs(this->normal.x) > fabs(this->normal.z))
			this->greaterNormalComponent = 'X';
		else
			this->greaterNormalComponent = 'Z';
	}
	else
	{
		if (fabs(this->normal.y) > fabs(this->normal.z))
			this->greaterNormalComponent = 'Y';
		else
			this->greaterNormalComponent = 'Z';
	}
}

Quad::Quad(Point3D a, 
           Point3D b, 
           Point3D c, 
           Point3D d,
           Material mat)
{
	Vector3D aux1, aux2;

	this->vertex[0] = a;
	this->vertex[1] = b;
	this->vertex[2] = c;
	this->vertex[3] = d;

	aux1 = this->vertex[1].substract(this->vertex[0]);
	aux2 = this->vertex[2].substract(this->vertex[0]);
	this->normal = aux1.crossProduct(aux2);
	this->normal.normalize();

	this->material = mat;
	  
	this->type = QUAD;
	  
	// We calculate the greater normal vector component (absolute value)
	// to choose which plane we project to (the one that maximizes the projected region) 
	if (fabs(this->normal.x) > fabs(this->normal.y))
	{
		if (fabs(this->normal.x) > fabs(this->normal.z))
			this->greaterNormalComponent = 'X';
		else
			this->greaterNormalComponent = 'Z';
	}
	else
	{
		if (fabs(this->normal.y) > fabs(this->normal.z))
			this->greaterNormalComponent = 'Y';
		else
			this->greaterNormalComponent = 'Z';
	}

}

Vector3D Quad::getNormal(void) const
{
	return(this->normal);
}

// Intersects the poligon with a ray
// If there is a hit, returns:
// - Hit point
// - Normal on the hit point
// If not, returns NULL (0)
real Quad::intersect(const Ray& r, Vector3D& normal, real tMin, real tMax) const
{
	Point3D intersection;
	real t, a0, b0, a1, b1;
	unsigned int crosses;

    // If ray is parallel to polygon, there is no intersection 
	t = r.direction.dotProduct(this->normal);
    if (ZERO(t)) 
	{
		return(0);
	}

    // We calculate intersection between the ray and the polygon plane 
	Vector3D vec1 = this->vertex[0].substract(r.origin);
	t = vec1.dotProduct(this->normal) / t;
	intersection = r.pointParametric(t);

	// Now we need to find if the hit point is contained within the polygon
	// We project onto one plane and calculate the number of crosses
	
	crosses = 0;
	if (this->greaterNormalComponent == 'X')       // 'X' is the greater, we project over YZ plane
	{
		a0 = this->vertex[3].z - intersection.z;
		b0 = this->vertex[3].y - intersection.y;
		for (int i = 0; i < 4; i++)
		{
			a1 = this->vertex[i].z - intersection.z;
			b1 = this->vertex[i].y - intersection.y;
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
		a0 = this->vertex[3].x - intersection.x;
		b0 = this->vertex[3].z - intersection.z;
		for (int i = 0; i < 4; i++)
		{
			a1 = this->vertex[i].x - intersection.x;
			b1 = this->vertex[i].z - intersection.z;
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
		a0 = this->vertex[3].x - intersection.x;
		b0 = this->vertex[3].y - intersection.y;
		for (int i = 0; i < 4; i++)
		{
			a1 = this->vertex[i].x - intersection.x;
			b1 = this->vertex[i].y - intersection.y;
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
		normal.x = this->normal.x;
		normal.y = this->normal.y;
		normal.z = this->normal.z;

		return(t); 
    }

	return(0);
}

// Convert the four side polygon in two triangles
vector<Triangle> Quad::tessellate() const
{
	vector<Triangle> t;

	t.push_back(Triangle(this->vertex[0], this->vertex[1], this->vertex[2], this->material));
	t.push_back(Triangle(this->vertex[0], this->vertex[2], this->vertex[3], this->material));

	return(t);
}

real Quad::getArea(void) const
{
	real base, height;
	Vector3D vBase, vHeight;

	vBase = this->vertex[1].substract(this->vertex[0]);
	vHeight = this->vertex[3].substract(this->vertex[0]);
	base = vBase.length();
	height = vHeight.length();

	return(base * height);
}

vector<Point3D> Quad::getCoordinates(void) const
{
	vector<Point3D> coordinates;

	for (int i = 0; i < 4; i++)
		coordinates.push_back(this->vertex[i]);

	return(coordinates);
}

AABB Quad::boundingBox() const
{
    // Initialize min and max points to the first vertex
    Point3D min = vertex[0];
    Point3D max = vertex[0];

    // Loop through the remaining vertices to find min and max for each axis
    for (int i = 1; i < 4; ++i) { // Iterate over the remaining 3 vertices
        min.x = std::min(min.x, vertex[i].x);
        min.y = std::min(min.y, vertex[i].y);
        min.z = std::min(min.z, vertex[i].z);

        max.x = std::max(max.x, vertex[i].x);
        max.y = std::max(max.y, vertex[i].y);
        max.z = std::max(max.z, vertex[i].z);
    }

    // Return the AABB constructed with min and max points
    return AABB(min, max);    
};