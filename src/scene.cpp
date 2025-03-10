#include "scene.h"
#include "pbrtParser.h"

// Default scene (Cornell Box)
Scene::Scene()
{
  this->createCornellBox();

  this->objects.push_back(std::make_shared<TriangleMesh>());
}

void Scene::createCornellBox()
{
  Point3D a, b, c, d;
  std::shared_ptr<TriangleMesh> lightObject;

  this->camera = Camera();

  // Light
  a = Point3D(-0.5, 2.0, -2.75);
  b = Point3D(-0.5, 2.0, -3.25);
  c = Point3D(0.5, 2.0, -3.25);
  d = Point3D(0.5, 2.0, -2.75);

  /*a = new Point3D(-0.5, 0.0, 3.00);
  b = new Point3D(-0.5, 0.0, 3.00);
  c = new Point3D(0.5, 0.0, 3.00);
  d = new Point3D(0.5, 0.0, 3.00);*/

  // Light object
  Material mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
           1.0, 1.0, 0.0, 1.0);
  // lightObject = std::make_shared<Quad>(a, b, c, d, mat); // Defaults to white color
  // ToDo: define vertices and indexes for the light object
  lightObject = std::make_shared<TriangleMesh>();

  // Light source
  std::shared_ptr<RectLight> lightTecho = std::make_shared<RectLight>();
  lightTecho->setLocation(*lightObject);
  this->lights.push_back(lightTecho);

  // Objects (including lights)
  this->objects.push_back(lightObject);

  // Cornell Box
  this->objects.push_back(this->cbLeftWall());
  this->objects.push_back(this->cbRightWall());
  this->objects.push_back(this->cbBackWall());
  this->objects.push_back(this->cbCeiling());
  this->objects.push_back(this->cbFloor());

  // Short box
  // std::vector<std::shared_ptr<Quad>> shortBox = this->cbShortBox();
  // this->object.insert(this->object.end(), shortBox.begin(), shortBox.end());

  // Tall box
  // std::vector<std::shared_ptr<Quad>> tallBox = this->cbTallBox();
  // this->object.insert(this->object.end(), tallBox.begin(), tallBox.end());

  // Sphere
  // std::shared_ptr<Point3D> center = std::make_shared<Point3D>(0, 0, -3.0);
  // mat = std::make_shared<Material>();
  // mat->set(std::make_shared<Color>(1.0, 0.2, 1.0),
  //          1.0, 0.5, 0.5, 0.0);
  // this->object.push_back(std::make_shared<Sphere>(center, 0.5, mat));

  // Triangle
  // a = new Point3D(0.0, 1.0, -2.5);
  // b = new Point3D(-0.5, -1.0, -3.0);
  // c = new Point3D(0.5, -1.0, -3.0);
  /*a = new Point3D(0.0, 1.0, -3.5);
  b = new Point3D(-0.5, -1.0, -3.0);
  c = new Point3D(0.5, -1.0, -3.0);
  Material *mat = new Material();
  real *colorMat = new real[3];
  colorMat[0] = 1.0f;  colorMat[1] = 1.0f;  colorMat[2] = 0.2f;
  mat->set(colorMat, 1.0, 0.5, 0.5);
  this->object->push_back(new Triangle(a, b, c, mat));*/

  // Box
  // vector<Triangle *> *t;
}

std::shared_ptr<Quad> Scene::cbLeftWall()
{
  Point3D a, b, c, d;
  Material mat;

  a = Point3D(-2.0, -2.0, -2.0);
  b = Point3D(-2.0, -2.0, -4.0);
  c = Point3D(-2.0, 2.0, -4.0);
  d = Point3D(-2.0, 2.0, -2.0);
  mat = Material();
  mat.set(Color(1.0, 0.2, 0.2),
          1.0, 0.5, 0.5, 0.0);

  // t = (new Quad(a, b, c, d, mat))->tessellate();
  // this->object->push_back((*t)[0]);
  // this->object->push_back((*t)[1]);

  return (std::make_shared<Quad>(a, b, c, d, mat));
}

std::shared_ptr<Quad> Scene::cbBackWall()
{
  Point3D a, b, c, d;
  Material mat;

  a = Point3D(-2.0, -2.0, -4.0);
  b = Point3D(2.0, -2.0, -4.0);
  c = Point3D(2.0, 2.0, -4.0);
  d = Point3D(-2.0, 2.0, -4.0);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);

  return (std::make_shared<Quad>(a, b, c, d, mat));
}

std::shared_ptr<Quad> Scene::cbRightWall()
{
  Point3D a, b, c, d;
  Material mat;

  a = Point3D(2.0, -2.0, -4.0);
  b = Point3D(2.0, -2.0, -2.0);
  c = Point3D(2.0, 2.0, -2.0);
  d = Point3D(2.0, 2.0, -4.0);
  mat = Material();
  mat.set(Color(0.1, 0.8, 0.1),
          1.0, 0.5, 0.5, 0.0);

  return (std::make_shared<Quad>(a, b, c, d, mat));
}

std::shared_ptr<Quad> Scene::cbCeiling()
{
  Point3D a, b, c, d;
  Material mat;

  a = Point3D(-2.0, 2.0, -2.0);
  b = Point3D(-2.0, 2.0, -4.0);
  c = Point3D(2.0, 2.0, -4.0);
  d = Point3D(2.0, 2.0, -2.0);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);

  return (std::make_shared<Quad>(a, b, c, d, mat));
}

std::shared_ptr<Quad> Scene::cbFloor()
{
  Point3D a, b, c, d;
  Material mat;

  a = Point3D(-2.0, -2.0, -2.0);
  b = Point3D(2.0, -2.0, -2.0);
  c = Point3D(2.0, -2.0, -4.0);
  d = Point3D(-2.0, -2.0, -4.0);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);

  return (std::make_shared<Quad>(a, b, c, d, mat));
}

std::vector<std::shared_ptr<Quad>> Scene::cbShortBox()
{
  std::vector<std::shared_ptr<Quad>> box;
  Point3D a, b, c, d;
  Material mat;

  // Front
  a = Point3D(0, -2.0, -2.5);
  b = Point3D(0, -1.0, -2.5);
  c = Point3D(1.5, -1.0, -2.25);
  d = Point3D(1.5, -2.0, -2.25);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);
  box.push_back(std::make_shared<Quad>(a, b, c, d, mat));

  // Right
  a = Point3D(1.5, -2.0, -2.25);
  b = Point3D(1.5, -1.0, -2.25);
  c = Point3D(1.6, -1.0, -2.75);
  d = Point3D(1.6, -2.0, -2.75);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);
  box.push_back(std::make_shared<Quad>(a, b, c, d, mat));

  // Top
  a = Point3D(0, -1.0, -2.5);
  b = Point3D(1.5, -1.0, -2.25);
  c = Point3D(1.6, -1.0, -2.75);
  d = Point3D(0.1, -1.0, -3.0);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);
  box.push_back(std::make_shared<Quad>(a, b, c, d, mat));

  return (box);
}

std::vector<std::shared_ptr<Quad>> Scene::cbTallBox()
{
  std::vector<std::shared_ptr<Quad>> box;
  Point3D a, b, c, d;
  Material mat;

  return (box);

  // Front
  a = Point3D(0, -2.0, -2.25);
  b = Point3D(0, -1.0, -2.25);
  c = Point3D(1.75, -1.0, -2.15);
  d = Point3D(1.75, -2.0, -2.15);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);
  box.push_back(std::make_shared<Quad>(a, b, c, d, mat));

  // Right
  a = Point3D(1.75, -2.0, -2.15);
  b = Point3D(1.75, -1.0, -2.15);
  c = Point3D(1.8, -1.0, -2.5);
  d = Point3D(1.8, -2.0, -2.5);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);
  box.push_back(std::make_shared<Quad>(a, b, c, d, mat));

  // Top
  a = Point3D(0, -1.0, -2.25);
  b = Point3D(1.75, -1.0, -2.15);
  c = Point3D(1.8, -1.0, -2.5);
  d = Point3D(0.15, -1.0, -2.65);
  mat = Material();
  mat.set(Color(1.0, 1.0, 1.0),
          1.0, 0.5, 0.5, 0.0);
  box.push_back(std::make_shared<Quad>(a, b, c, d, mat));

  return (box);
}

// Read scene information from a PBRT scene file
Scene::Scene(const std::string filename)
{
    PBRTParser parser(filename);
    std::shared_ptr<Scene> parsedScene = parser.parse();

    if (!parsedScene) {
        std::cerr << "Error parsing scene file: " << filename << std::endl;
        return;
    }

    // Copy lights
    for (const auto& light : parsedScene->lights) {
        lights.push_back(light);
        // We also add lights to the list of objects
        // Check if the light is a RectLight
        if (std::shared_ptr<RectLight> rectLight = std::dynamic_pointer_cast<RectLight>(light))
        {
            // Create an emissive material using the light's color
            Material emissiveMaterial = Material(
                rectLight->getColor(),  // Emission color
                0.0f,                   // ka (ambient)
                0.0f,                   // kd (diffuse)
                0.0f,                   // ks (specular)
                1.0f                    // ke (emission factor, making it a glowing object)
            );

            // Use the existing TriangleMesh that defines the light's geometry

            // Create an object using the TriangleMesh and emissive material
            std::shared_ptr<TriangleMesh> lightObject = 
              std::make_shared<TriangleMesh>(
                rectLight->getLocation(), 
                emissiveMaterial);

            // Add the light mesh to the scene objects so it gets rendered
            objects.push_back(lightObject);
        }
    }

    // Copy primitives
    for (const auto& object : parsedScene->objects) {
        objects.push_back(object);
    }

    // Copy camera
    camera = parsedScene->camera;
}

void Scene::addLight(const std::shared_ptr<Light>& light) 
{
    this->lights.push_back(light);
}

void Scene::addObject(const std::shared_ptr<Primitive>& object) 
{
    this->objects.push_back(object);
}

void Scene::setCamera(const Camera& cam) 
{
    camera = cam;
}

// void Scene::traversePBRT(pbrt::Object::SP object)
// {
//   const bool firstTime = (alreadyTraversed.find(object) == alreadyTraversed.end());
//   alreadyTraversed.insert(object);

//   numObjects.add(firstTime,1);
//   numLights.add(firstTime,object->lightSources.size());

//   for (auto light : object->lightSources) {
//     if (light->as<PointLightSource>())
//       numPointLights.add(firstTime,1);
//     if (light->as<SpotLightSource>())
//       numSpotLights.add(firstTime,1);
//     if (light->as<InfiniteLightSource>())
//       numInfiniteLights.add(firstTime,1);
//     if (light->as<DistantLightSource>())
//       numDistantLights.add(firstTime,1);
//   }
  
//   numShapes.add(firstTime,object->shapes.size());
  
//   for (auto shape : object->shapes) {
//     usedMaterials.insert(shape->material);
//     if (shape->areaLight) {
//       numAreaLights.add(firstTime,1);
//     }
//     if (TriangleMesh::SP mesh=std::dynamic_pointer_cast<TriangleMesh>(shape)){
//       numTriangles.add(firstTime,mesh->index.size());
//     } else if (QuadMesh::SP mesh=std::dynamic_pointer_cast<QuadMesh>(shape)){
//       numQuads.add(firstTime,mesh->index.size());
//     } else if (Sphere::SP sphere=std::dynamic_pointer_cast<Sphere>(shape)){
//       numSpheres.add(firstTime,1);
//     } else if (Disk::SP disk=std::dynamic_pointer_cast<Disk>(shape)){
//       numDisks.add(firstTime,1);
//     } else if (Curve::SP curves=std::dynamic_pointer_cast<Curve>(shape)){
//       numCurves.add(firstTime,1);
//     } else
//       std::cout << "un-handled geometry type : " << shape->toString() << std::endl;
//   }

//   numInstances.add(firstTime,object->instances.size());
//   for (auto inst : object->instances) {
//     traverse(inst->object);
//   }
// }


unsigned int Scene::getNumberObjects(void) const
{
  return (this->objects.size());
}

std::shared_ptr<Primitive> Scene::getObject(unsigned int objectIndex) const
{
  if ((objectIndex >= 0) && (objectIndex < this->objects.size()))
    return (this->objects[objectIndex]);

  return (0);
}

unsigned int Scene::getNumberLights(void) const
{
  return (this->lights.size());
}

std::shared_ptr<Light> Scene::getLight(unsigned int lightIndex) const
{
  if ((lightIndex >= 0) && (lightIndex < this->objects.size()))
    return (this->lights[lightIndex]);

  return (0);
}

Camera Scene::getCamera(void) const
{
  return (this->camera);
}

bool Scene::mutuallyVisible(Point3D p, Point3D q) const
{
  unsigned int numberObjects;
  real distance, intersection;
  bool visible = true;
  Ray r;
  Vector3D lineOfSight;
  Vector3D N;
  std::shared_ptr<Primitive> object;

  // We build the line of sight between the two points
  lineOfSight = q.substract(p);
  distance = lineOfSight.length();
  lineOfSight.normalize();

  // We create the ray with origin in the first point
  // and pointing towards the second point
  r = Ray(p, lineOfSight);

  numberObjects = this->objects.size();

  // Intersection with the different objects
  // We start at 1 to avoid light intersection
  // We must change it in the future
  for (unsigned int i = 1; i < numberObjects; i++)
  {
    object = this->objects[i];
    intersection = object->intersect(r, N, 0.001, distance);

    // If the hitpoint distance is smaller than the distance
    // between the two points, there is an object between them and
    // they are not visible
    if ((intersection > 0) && (!ZERO(intersection)))
      if (intersection < distance)
      {
        // cout << "Object Index = " << i << " --- Intersection = " << intersection << " --- Distance = " << distance << "\n";
        if (!ZERO(intersection - distance))
        {
          visible = false;
          break;
        }
      }
  }

  return (visible);
}

void Scene::buildBVH() 
{
    this->bvh.build(this->objects);
}

std::optional<HitPoint> Scene::intersect(const Ray& ray, real tMin, real tMax) const 
{
    return this->bvh.intersect(ray, tMin, tMax);
}
