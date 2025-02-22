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

enum Algorithm 
{ 
  WHITTED = 'w', 
  STOCHASTIC = 's' 
};

struct ProgramArgs 
{
    std::string sceneFilename = "";
    bool useBVH = true; // Default to true, set false if specified
    Algorithm algorithm = Algorithm::WHITTED;
    int sampleRays = 1;
    int shadowRays = 1;
    int indirectRays = 4;
};

std::string getFilename(ProgramArgs args)
{
  auto time = std::time(nullptr);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%F_%T"); // ISO 8601 without timezone information.
  auto strCurrentTime = ss.str();
  std::replace(strCurrentTime.begin(), strCurrentTime.end(), ':', '-');

  std::stringstream filename;
  filename << 
    "output/" << 
    static_cast<char>(args.algorithm) << "-" << 
    args.sampleRays << "-" << 
    args.shadowRays << "-" << 
    strCurrentTime.c_str() << ".tga";

  return (filename.str());
}

std::optional<ProgramArgs> parseArgs(int argc, char* argv[]) 
{
    ProgramArgs args;

    for (int i = 1; i < argc; ++i) 
    {
        std::string arg = argv[i];

        if (arg == "--no-bvh") 
        {
            args.useBVH = false;
        } 
        else if (arg == "--scene") 
        {
            if (++i < argc) 
            {
                args.sceneFilename = argv[i];
            } 
            else 
            {
                std::cerr << "Error: Missing scene filename after --scene\n";
                return std::nullopt;
            }
        } 
        else if (arg == "--algorithm") 
        {
            if (++i < argc) 
            {
                std::string algoStr = argv[i];
                std::transform(algoStr.begin(), algoStr.end(), algoStr.begin(), ::tolower);
                if (algoStr == "w") 
                {
                    args.algorithm = Algorithm::WHITTED;
                } 
                else if (algoStr == "s") 
                {
                    args.algorithm = Algorithm::STOCHASTIC;
                } 
                else 
                {
                    std::cerr << "Error: Unknown algorithm '" << algoStr << "'\n";
                    return std::nullopt;
                }
            } 
            else 
            {
                std::cerr << "Error: Missing algorithm after --algorithm\n";
                return std::nullopt;
            }
        } 
        else if (arg == "--sample-rays") 
        {
            if (++i < argc) 
            {
                args.sampleRays = std::stoi(argv[i]);
            } 
            else 
            {
                std::cerr << "Error: Missing value for --sample-rays\n";
                return std::nullopt;
            }
        } 
        else if (arg == "--shadow-rays") 
        {
            if (++i < argc) 
            {
                args.shadowRays = std::stoi(argv[i]);
            } 
            else 
            {
                std::cerr << "Error: Missing value for --shadow-rays\n";
                return std::nullopt;
            }
        } 
        else if (arg == "--indirect-rays") 
        {
            if (++i < argc) 
            {
                args.indirectRays = std::stoi(argv[i]);
            } 
            else 
            {
                std::cerr << "Error: Missing value for --indirect-rays\n";
                return std::nullopt;
            }
        } 
        else if (arg == "--help") 
        {
            std::cout << "Usage: rayos [options]\n"
                      << "Options:\n"
                      << "  --scene <filename>       Path to the scene file\n"
                      << "  --no-bvh                 Disable BVH acceleration\n"
                      << "  --algorithm <type>       Rendering algorithm: w -> Whitted, s -> stochastic\n"
                      << "  --sample-rays <count>    Number of sample rays per pixel\n"
                      << "  --shadow-rays <count>    Number of shadow rays per light\n"
                      << "  --indirect-rays <count>  Number of indirect rays\n"
                      << "  --help                   Show this help message\n";
            return std::nullopt;
        } 
        else 
        {
            std::cerr << "Error: Unknown argument '" << arg << "'\n";
            return std::nullopt;
        }
    }

    return args;
}

int main(int argc, char *argv[])
{
  std::shared_ptr<Scene> s;
  std::shared_ptr<Bitmap> im;
  std::shared_ptr<Tracer> rt;

  auto argsOpt = parseArgs(argc, argv);
  if (!argsOpt) return 0;
  ProgramArgs args = *argsOpt;

  // Start chrono
  Chrono *c = new Chrono();
  c->start();

  // Instantiate raytracer
  if (args.algorithm == Algorithm::WHITTED)
  {
    // Whitted RayTracer
    rt = std::make_shared<WhittedRayTracer>(PHONG);
  }
  else if (args.algorithm == Algorithm::STOCHASTIC)
  {
    // Stochastic RayTracer
    rt = std::make_shared<StochasticRayTracer>(
      args.sampleRays, 
      args.shadowRays,
      args.indirectRays,
      args.useBVH);
  }

  if (args.sceneFilename.empty())
  {
    s = std::make_shared<Scene>();
  }
  else
  {
    s = std::make_shared<Scene>(args.sceneFilename);
  }

  // Raytrace the scene to create an image
  im = rt->trace(s);

  // Stop chrono
  c->stop();
  cout << "\n\nElapsed time = " << c->value() << " seconds\n";

  // Write the image to a file
  std::string strFilename = getFilename(args);
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
  o << "Algorithm: " << ((args.algorithm == Algorithm::WHITTED) ? "Whitted" : "Stochastic") << "\n";
  o << "Scene: " << (!(args.sceneFilename.empty()) ? args.sceneFilename : "") << "\n";
  o << "Elapsed time: " << c->value() << " seconds\n";
  o << "Sample Rays: " << args.sampleRays << "\n";
  o << "Shadow Rays: " << args.shadowRays << "\n";
  o << "Time Get Hit Point: " << rt->getTimeStats().timeGetHitPoint << " ms\n";  
  o << "Time Radiance: " << rt->getTimeStats().timeCalculateRadiance << " ms\n";
  o << "Time Mutually Visible: " << rt->getTimeStats().timeMutuallyVisible << " ms\n";
  o << "Time Pixel: " << rt->getTimeStats().timePixel << " ms\n";
  o.close();

  delete (c);

  return (0);
}
