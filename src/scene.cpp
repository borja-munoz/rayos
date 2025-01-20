#include "scene.h"

// Traverse the PBRT scene recursively
// void Scene::traversePBRT(std::shared_ptr<pbrt::Object> object)
// {
//   const bool firstTime = (alreadyTraversed.find(object) == alreadyTraversed.end());
//   alreadyTraversed.insert(object);

//   // numObjects.add(firstTime,1);
//   // numLights.add(firstTime,object->lightSources.size());

//   for (auto light : object->lightSources)
//   {
//     if (light->as<pbrt::PointLightSource>())
//     {
//       this->light.push_back(std::make_shared<PointLight>());
//       // numPointLights.add(firstTime,1);
//     }
//     if (light->as<pbrt::SpotLightSource>())
//     {
//       // numSpotLights.add(firstTime,1);
//     }
//     if (light->as<pbrt::InfiniteLightSource>())
//     {
//       // numInfiniteLights.add(firstTime,1);
//     }
//     if (light->as<pbrt::DistantLightSource>())
//     {
//       // numDistantLights.add(firstTime,1);
//     }
//   }

//   // numShapes.add(firstTime,object->shapes.size());

//   for (auto shape : object->shapes)
//   {
//     usedMaterials.insert(shape->material);
//     if (shape->areaLight)
//     {
//       // numAreaLights.add(firstTime,1);
//       this->light.push_back(std::make_shared<RectLight>());
//     }
//     if (std::shared_ptr<pbrt::TriangleMesh> mesh = std::dynamic_pointer_cast<pbrt::TriangleMesh>(shape))
//     {
//       this->object.push_back(std::make_shared<TriangleMesh>());
//       // numTriangles.add(firstTime,mesh->index.size());
//     }
//     else if (std::shared_ptr<pbrt::QuadMesh> mesh = std::dynamic_pointer_cast<pbrt::QuadMesh>(shape))
//     {
//       // numQuads.add(firstTime,mesh->index.size());
//     }
//     else if (std::shared_ptr<pbrt::Sphere> sphere = std::dynamic_pointer_cast<pbrt::Sphere>(shape))
//     {
//       this->object.push_back(std::make_shared<Sphere>());
//       // numSpheres.add(firstTime,1);
//     }
//     else if (std::shared_ptr<pbrt::Disk> disk = std::dynamic_pointer_cast<pbrt::Disk>(shape))
//     {
//       // numDisks.add(firstTime,1);
//     }
//     else if (std::shared_ptr<pbrt::Curve> curves = std::dynamic_pointer_cast<pbrt::Curve>(shape))
//     {
//       // numCurves.add(firstTime,1);
//     }
//     else
//       std::cout << "un-handled geometry type : " << shape->toString() << std::endl;
//   }

//   // numInstances.add(firstTime,object->instances.size());
//   for (auto inst : object->instances)
//   {
//     this->traversePBRT(inst->object);
//   }
// }

// Default scene (Cornell Box)
Scene::Scene()
{
  this->createCornellBox();

  this->object.push_back(std::make_shared<TriangleMesh>());
}

void Scene::createCornellBox()
{
  Point3D a, b, c, d;
  std::shared_ptr<Quad> lightObject;

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
  lightObject = std::make_shared<Quad>(a, b, c, d, mat); // Defaults to white color

  // Light source
  std::shared_ptr<RectLight> lightTecho = std::make_shared<RectLight>();
  lightTecho->setLocation(*lightObject);
  this->light.push_back(lightTecho);

  // Objects (including lights)
  this->object.push_back(lightObject);

  // Cornell Box
  this->object.push_back(this->cbLeftWall());
  this->object.push_back(this->cbRightWall());
  this->object.push_back(this->cbBackWall());
  this->object.push_back(this->cbCeiling());
  this->object.push_back(this->cbFloor());

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

// Read scene information from a file
// Currently only PBRT files are supported through the pbrt-parser library
// https://github.com/ingowald/pbrt-parser
Scene::Scene(const std::string filename)
{
  // std::shared_ptr<pbrt::Scene> scene;

  // scene = pbrt::importPBRT(filename);

  // scene->makeSingleLevel();

  // this->traversePBRT(scene->world);

  cout << "This scene has " << this->light.size() << " lights\n";
  cout << "This scene has " << this->object.size() << " objects\n";
  /*
  Scene::SP scene = SemanticParser(pbrt).result;
  createFilm(scene,pbrt);
  for (auto cam : pbrt->cameras)
    scene->cameras.push_back(createCamera(cam));
  //return scene;
  */

  // Camera
  this->camera = Camera();
}

unsigned int Scene::getNumberObjects(void)
{
  return (this->object.size());
}

std::shared_ptr<Primitive> Scene::getObject(unsigned int objectIndex)
{
  if ((objectIndex >= 0) && (objectIndex < this->object.size()))
    return (this->object[objectIndex]);

  return (0);
}

unsigned int Scene::getNumberLights(void)
{
  return (this->light.size());
}

std::shared_ptr<Light> Scene::getLight(unsigned int lightIndex)
{
  if ((lightIndex >= 0) && (lightIndex < this->object.size()))
    return (this->light[lightIndex]);

  return (0);
}

Camera Scene::getCamera(void)
{
  return (this->camera);
}

bool Scene::mutuallyVisible(Point3D p, Point3D q)
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

  numberObjects = this->object.size();

  // Intersection with the different objects
  // We start at 1 to avoid light intersection
  // We must change it in the future
  for (unsigned int i = 1; i < numberObjects; i++)
  {
    object = this->object[i];
    intersection = object->intersect(r, N);

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
