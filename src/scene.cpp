#include "scene.h"


//-------------------------------------------------------------------------------

// Default scene

Scene::Scene()
{
	Point3D *a, *b, *c, *d;
	Quad *lightObject;

	this->camera = new Camera();

	// Light
	a = new Point3D(-0.5, 2.0, -2.75);
	b = new Point3D(-0.5, 2.0, -3.25);
	c = new Point3D(0.5, 2.0, -3.25);
	d = new Point3D(0.5, 2.0, -2.75);
	
	/*a = new Point3D(-0.5, 0.0, 3.00);
	b = new Point3D(-0.5, 0.0, 3.00);
	c = new Point3D(0.5, 0.0, 3.00);
	d = new Point3D(0.5, 0.0, 3.00);*/

    // Lights
    this->light = new vector<Light *>;

    // Light object
    Material *mat = new Material();
	real *colorMat = new real[3];
	colorMat[0] = 1.0f;  colorMat[1] = 1.0f;  colorMat[2] = 1.0f;
	mat->set(colorMat, 1.0, 1.0, 0.0, 1.0);
	lightObject = new Quad(a, b, c, d, mat);  // Defaults to white color

    // Light source
	RectLight *lightTecho = new RectLight();
	lightTecho->setLocation(lightObject);
	this->light->push_back(lightTecho);

	// Objects (including lights)
	this->object = new vector<Primitive *>;

	this->object->push_back(lightObject);

	// Sphere
	Point3D *center = new Point3D(0, 0, -3.0);
    mat = new Material();
	colorMat[0] = 1.0f;  colorMat[1] = 0.2f;  colorMat[2] = 1.0f;
	mat->set(colorMat, 1.0, 0.5, 0.5, 0.0);
	this->object->push_back(new Sphere(center, 0.5, mat));

	// Tri�ngle
	//a = new Point3D(0.0, 1.0, -2.5);
	//b = new Point3D(-0.5, -1.0, -3.0);
	//c = new Point3D(0.5, -1.0, -3.0);
	/*a = new Point3D(0.0, 1.0, -3.5);
	b = new Point3D(-0.5, -1.0, -3.0);
	c = new Point3D(0.5, -1.0, -3.0);
	Material *mat = new Material();
	real *colorMat = new real[3];
	colorMat[0] = 1.0f;  colorMat[1] = 1.0f;  colorMat[2] = 0.2f;
	mat->set(colorMat, 1.0, 0.5, 0.5);
	this->object->push_back(new Triangle(a, b, c, mat));*/

	// Box
	//vector<Triangle *> *t;

	// Left wall
	a = new Point3D(-2.0, -2.0, -2.0);
	b = new Point3D(-2.0, -2.0, -4.0);
	c = new Point3D(-2.0, 2.0, -4.0);
	d = new Point3D(-2.0, 2.0, -2.0);
	mat = new Material();
	colorMat[0] = 1.0f;  colorMat[1] = 0.2f;  colorMat[2] = 0.2f;
	mat->set(colorMat, 1.0, 0.5, 0.5, 0.0);
	//t = (new Quad(a, b, c, d, mat))->tessellate();
	//this->object->push_back((*t)[0]);
	//this->object->push_back((*t)[1]);
	this->object->push_back(new Quad(a, b, c, d, mat));

	// Background wall
	a = new Point3D(-2.0, -2.0, -4.0);
	b = new Point3D(2.0, -2.0, -4.0);
	c = new Point3D(2.0, 2.0, -4.0);
	d = new Point3D(-2.0, 2.0, -4.0);
	mat = new Material();
	colorMat[0] = 0.1f;  colorMat[1] = 0.8f;  colorMat[2] = 0.1f;
	mat->set(colorMat, 1.0, 0.5, 0.5, 0.0);
	this->object->push_back(new Quad(a, b, c, d, mat));

	// Right wall
	a = new Point3D(2.0, -2.0, -4.0);
	b = new Point3D(2.0, -2.0, -2.0);
	c = new Point3D(2.0, 2.0, -2.0);
	d = new Point3D(2.0, 2.0, -4.0);
	mat = new Material();
	colorMat[0] = 0.2f;  colorMat[1] = 0.2f;  colorMat[2] = 1.0f;
	mat->set(colorMat, 1.0, 0.5, 0.5, 0.0);
	this->object->push_back(new Quad(a, b, c, d, mat));

	// Ceiling
	a = new Point3D(-2.0, 2.0, -2.0);
	b = new Point3D(-2.0, 2.0, -4.0);
	c = new Point3D(2.0, 2.0, -4.0);
	d = new Point3D(2.0, 2.0, -2.0);
	mat = new Material();
	colorMat[0] = 1.0f;  colorMat[1] = 1.0f;  colorMat[2] = 1.0f;
	mat->set(colorMat, 1.0, 0.5, 0.5, 0.0);
	this->object->push_back(new Quad(a, b, c, d, mat));

	// Floor
	a = new Point3D(-2.0, -2.0, -2.0);
	b = new Point3D(2.0, -2.0, -2.0);
	c = new Point3D(2.0, -2.0, -4.0);
	d = new Point3D(-2.0, -2.0, -4.0);
	mat = new Material();
	colorMat[0] = 0.2f;  colorMat[1] = 1.0f;  colorMat[2] = 0.2f;
	mat->set(colorMat, 1.0, 0.5, 0.5, 0.0);
	this->object->push_back(new Quad(a, b, c, d, mat));
		  
}


//-------------------------------------------------------------------------------

Scene::~Scene()
{
	unsigned int numberObjects, numeroLuces;

	numberObjects = this->object->size();
	for (unsigned int i = 0; i < numberObjects; i++)
		delete((*(this->object))[i]);
	delete(this->object);

	numeroLuces = this->light->size();
	for (unsigned int i = 0; i < numeroLuces; i++)
		delete((*(this->light))[i]);
	delete(this->light);

	delete(this->camera);
}


//-------------------------------------------------------------------------------

unsigned int Scene::getNumberObjects(void)
{
	return(this->object->size());
}


//-------------------------------------------------------------------------------

Primitive * Scene::getObject(unsigned int objectIndex)
{
	if ((objectIndex >= 0) && (objectIndex < this->object->size()))
		return((*(this->object))[objectIndex]->copy());
  
	return(0);
}


//-------------------------------------------------------------------------------

unsigned int Scene::getNumberLights(void)
{
	return(this->light->size());
}


//-------------------------------------------------------------------------------

Light * Scene::getLight(unsigned int lightIndex)
{
	if ((lightIndex >= 0) && (lightIndex < this->object->size()))
		return((*(this->light))[lightIndex]->copy());
  
	return(0);
}


//-------------------------------------------------------------------------------

Camera * Scene::getCamera(void)
{
	return(new Camera(*(this->camera)));
}


//-------------------------------------------------------------------------------

bool Scene::mutuallyVisible(Point3D *p, Point3D *q)
{
	unsigned int numberObjects;
	real distance, intersection;
	bool visible = true;
	Ray *r;
	Vector3D *lineOfSight, N;
	Primitive *object;

	// Construimos la l�nea de visi�n entre los dos puntos
	lineOfSight = q->substract(p);
	distance = lineOfSight->length();
	lineOfSight->normalize();

	// Creamos el rayo correspondiente
	r = new Ray(p, lineOfSight);

	// Intersecci�n con los diferentes object
	numberObjects = this->object->size();

    // We start at 1 to avoid light intersection
    // We must change it in the future
	for (unsigned int i = 1; i < numberObjects; i++)
	{    
		object = (*(this->object))[i];
		intersection = object->intersect(r, N);

        // If the hitpoint distance is smaller than the distance
        // between the two points, there is an object between them and
        // they are not visible
		if (intersection > 0)
			if (intersection < distance)
				if (!ZERO(intersection - distance))
				{
					visible = false;
					break;
				}
	}

	delete(lineOfSight);
	delete(r);

	return(visible);
}


//-------------------------------------------------------------------------------


