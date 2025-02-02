#ifndef _SCENE_H
#define _SCENE_H

#include <memory>
#include <set>
#include <vector>

#include "geom/BVH.h"
#include "geom/primitive.h"
#include "geom/quad.h"
#include "geom/sphere.h"
#include "geom/triangle.h"
#include "geom/triangleMesh.h"
#include "tracer/hitPoint.h"
#include "util/camera.h"
#include "util/light.h"
#include "util/material.h"
// #include "util/pbrtParser.h"
#include "util/point3D.h"
#include "util/pointLight.h"
#include "util/rectLight.h"
#include "util/util.h"

// #include "pbrtParser/Scene.h"

// Forward declaration to break dependency
class PBRTParser;

using namespace std;

class Scene
{    
    // std::set<pbrt::Object::SP> alreadyTraversed;
    // std::set<pbrt::Material::SP> usedMaterials;
    
    // void Scene::traversePBRT(pbrt::Object::SP object);

    void createCornellBox();
    std::shared_ptr<Quad> cbLeftWall();
    std::shared_ptr<Quad> cbRightWall();
    std::shared_ptr<Quad> cbBackWall();
    std::shared_ptr<Quad> cbCeiling();
    std::shared_ptr<Quad> cbFloor();
    std::vector<std::shared_ptr<Quad>> cbShortBox();
    std::vector<std::shared_ptr<Quad>> cbTallBox();

  public:

	  vector<std::shared_ptr<Light>> lights;                
	  Camera camera;                    	
	  vector<std::shared_ptr<Primitive>> objects;       
    BVH bvh;

	  Scene();
    Scene(const std::string);

	  unsigned int getNumberObjects(void);
	  std::shared_ptr<Primitive> getObject(unsigned int objectIndex);
    void addObject(const std::shared_ptr<Primitive>& object);

	  unsigned int getNumberLights(void);
	  std::shared_ptr<Light> getLight(unsigned int lightIndex);
    void addLight(const std::shared_ptr<Light>& light);

	  Camera getCamera(void) const; 
    void setCamera(const Camera& camera);

	  bool mutuallyVisible(Point3D p, Point3D q);

    void buildBVH();
    std::optional<HitPoint> intersect(const Ray& ray, real tMin, real tMax) const;
};


#endif
