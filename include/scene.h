#ifndef _SCENE_H
#define _SCENE_H

#include "geom/BVH.h"
#include "geom/primitive.h"
#include "geom/quad.h"
#include "geom/sphere.h"
#include "geom/triangle.h"
#include "geom/triangleMesh.h"
#include "tracer/hitPoint.h"
#include "util/light.h"
#include "util/pointLight.h"
#include "util/rectLight.h"
#include "util/material.h"
#include "util/point3D.h"
#include "util/camera.h"
#include "util/util.h"

#include "pbrtParser/Scene.h"

#include <memory>
#include <set>
#include <vector>

using namespace std;

class Scene
{
	  vector<std::shared_ptr<Light>> light;                
	  Camera camera;                    	
    
    // std::set<std::shared_ptr<pbrt::Object>> alreadyTraversed;
    // std::set<std::shared_ptr<pbrt::Material>> usedMaterials;
    
    // void traversePBRT(std::shared_ptr<pbrt::Object>); 

    void createCornellBox();
    std::shared_ptr<Quad> cbLeftWall();
    std::shared_ptr<Quad> cbRightWall();
    std::shared_ptr<Quad> cbBackWall();
    std::shared_ptr<Quad> cbCeiling();
    std::shared_ptr<Quad> cbFloor();
    std::vector<std::shared_ptr<Quad>> cbShortBox();
    std::vector<std::shared_ptr<Quad>> cbTallBox();

  public:

	  vector<std::shared_ptr<Primitive>> object;       
    BVH bvh;

	  Scene();
    Scene(const std::string);
	  unsigned int getNumberObjects(void);
	  std::shared_ptr<Primitive> getObject(unsigned int objectIndex);
	  unsigned int getNumberLights(void);
	  std::shared_ptr<Light> getLight(unsigned int lightIndex);
	  Camera getCamera(void);
	  bool mutuallyVisible(Point3D p, Point3D q);

    void buildBVH();
    std::optional<HitPoint> intersect(const Ray& ray, real tMin, real tMax) const;
};


#endif
