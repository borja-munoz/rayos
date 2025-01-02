//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>

#include <stdio.h>
#include <math.h>

#include "util/chrono.h"
#include "scene.h"
#include "bitmap.h"
#include "tracer/whittedRayTracer.h"
#include "tracer/stochasticRayTracer.h"
#include "tracer/brdf.h"

std::string getFilename(char *argv[])
{
  auto time = std::time(nullptr);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%F_%T"); // ISO 8601 without timezone information.
  auto strCurrentTime = ss.str();
  std::replace(strCurrentTime.begin(), strCurrentTime.end(), ':', '-');

  std::stringstream filename;
  filename << "output/" << argv[1] << "-" << argv[2] << "-" << argv[3] << "-" << strCurrentTime.c_str() << ".tga";

  return (filename.str());
}

int main(int argc, char *argv[])
{
  std::shared_ptr<Scene> s;
  std::shared_ptr<Bitmap> im;
  std::shared_ptr<Tracer> rt;

  // File3ds *f = new File3ds("knot.3ds");

  // Start chrono
  Chrono *c = new Chrono();
  c->start();

  // Instantiate raytracer
  if (!strcmp(argv[1], "w"))
  {
    // Whitted RayTracer
    rt = std::make_shared<WhittedRayTracer>(PHONG);
  }
  else
  {
    // Stochastic RayTracer
    rt = std::make_shared<StochasticRayTracer>(atoi(argv[2]), atoi(argv[3]));
  }

  // Create scene
  if (argc == 4)
  {
    s = std::make_shared<Scene>();
  }
  else
  {
    s = std::make_shared<Scene>(argv[4]);
    return (0);
  }

  // Raytrace the scene to create an image
  im = rt->trace(s);

  // Stop chrono
  c->stop();
  cout << "Elapsed time = " << c->value() << " seconds\n";
  delete (c);

  // Write the image to a file
  std::string strFilename = getFilename(argv);
  const char *filename = strFilename.c_str();
  im->writeTGA(filename);
  // im->writeTGA("imagen.tga");

  std::stringstream openFileCommand;

  // Windows
  // openFileCommand << "start " << filename;

  // Mac OS X
  openFileCommand << "open " << filename;

  system(openFileCommand.str().c_str());
  /*
    }
    else
    cout << endl << "Usage: rayos [w|s] <sampleRays> <shadowRays> <path_scene_file>" << endl;
    */

  // Write time stats to another file
  strFilename.replace(strFilename.find(".tga"), sizeof(".tga") - 1, ".txt");
  ofstream o;
  o.open(strFilename.c_str());
  o << "Algorithm: " << ((!strcmp(argv[1], "w")) ? "Whitted" : "Stochastic") << "\n";
  o << "Sample Rays: " << argv[2] << "\n";
  o << "Shadow Rays: " << argv[3] << "\n";
  o << "Time Get Hit Point: " << rt->getTimeStats().timeGetHitPoint << " ms\n";  
  o << "Time Radiance: " << rt->getTimeStats().timeCalculateRadiance << " ms\n";
  o << "Time Mutually Visible: " << rt->getTimeStats().timeMutuallyVisible << " ms\n";
  o << "Time Pixel: " << rt->getTimeStats().timePixel << " ms\n";
  o.close();

  return (0);
}
