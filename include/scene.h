#ifndef _SCENE_H
#define _SCENE_H

#include "geom/primitive.h"
#include "geom/sphere.h"
#include "geom/triangle.h"
#include "geom/quad.h"
#include "util/light.h"
#include "util/rectLight.h"
#include "util/material.h"
#include "util/point3D.h"
#include "util/camera.h"
#include "util/util.h"

#include <vector>

using namespace std;

//-------------------------------------------------------------------------------

class Scene
{
	vector<Light *> *light;                
	Camera *camera;                    	

  public:

	vector<Primitive *> *object;       

	Scene();
	~Scene();
	unsigned int getNumberObjects(void);
	Primitive *getObject(unsigned int objectIndex);
	unsigned int getNumberLights(void);
	Light *getLight(unsigned int lightIndex);
	Camera *getCamera(void);
	bool mutuallyVisible(Point3D *p, Point3D *q);
};


//-------------------------------------------------------------------------------

#endif
