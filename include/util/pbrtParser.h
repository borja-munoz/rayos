#ifndef _PBRTPARSER_H
#define _PBRTPARSER_H

#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "../scene.h" 
#include "pointLight.h"
#include "rectLight.h"

class PBRTParser 
{
    std::string filename;
    std::unordered_map<std::string, Material> materialMap;

    void parsePixelFilter(std::istringstream& stream);
    void parseSampler(std::istringstream& stream);
    void parseFilm(std::istringstream& stream); 

    Camera parseLookAt(std::istringstream& stream);
    std::optional<real> parseCamera(std::istringstream& stream);
    std::shared_ptr<PointLight> parsePointLight(std::istringstream& stream);
    std::shared_ptr<RectLight> parseAreaLight(std::ifstream& file, std::istringstream& stream);
    std::shared_ptr<Primitive> parseShape(std::ifstream& file, std::istringstream& stream, Material currentMaterial);
    Material parseMaterial(std::ifstream& file, std::istringstream& stream);
    Material parseMatteMaterial(std::ifstream& file);
    Material parsePlasticMaterial(std::istringstream& stream);
    Color parseRGBColor(std::istringstream& stream);
    Color parseSpectrumColor(std::istringstream& stream);

  public:
    PBRTParser(const std::string& filename) : filename(filename) {}
    std::shared_ptr<Scene> parse();
};

#endif