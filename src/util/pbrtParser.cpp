#include "pbrtParser.h"

std::shared_ptr<Scene> PBRTParser::parse() 
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open PBRT file: " << filename << std::endl;
        return nullptr;
    }

    std::string line;
    bool inWorldBlock = false;

    Material currentMaterial;
    std::shared_ptr<Light> currentLight = nullptr;

    Camera camera;
    real fov;

    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "WorldBegin") 
        {
            inWorldBlock = true;
            continue;
        }

        if (!inWorldBlock) 
        {
            // Handle pre-WorldBegin directives
            if (keyword == "LookAt") 
            {
                camera = parseLookAt(iss);
            } 
            else if (keyword == "Camera") 
            {
                auto fov = parseCamera(iss);
                if (fov) 
                {
                    camera.setFOV(*fov);
                }
            } 
            else if (keyword == "PixelFilter") 
            {
                parsePixelFilter(iss);
            } 
            else if (keyword == "Sampler") 
            {
                parseSampler(iss);
            } 
            else if (keyword == "Film") 
            {
                parseFilm(iss);
            }
        } 
        else 
        {
            // Handle scene elements inside WorldBegin/WorldEnd
            if (keyword == "AttributeBegin") 
            {
                currentMaterial = Material();
                currentLight = nullptr;
            } 
            else if (keyword == "AttributeEnd") 
            {
                currentMaterial = Material();
            } 
            else if (keyword == "Material") 
            {
                currentMaterial = parseMaterial(iss);
            } 
            else if (keyword == "Shape") 
            {
                auto primitive = parseShape(iss, currentMaterial);
                if (primitive) 
                {
                    scene->addObject(primitive);
                }
            } 
            else if (keyword == "AreaLightSource") 
            {
                currentLight = parseAreaLight(file, iss);
                scene->addLight(currentLight);
            } 
            else if (keyword == "PointLightSource") 
            {
                currentLight = parsePointLight(iss);
                scene->addLight(currentLight);
            }
        }
    }

    file.close();
    return scene;
}


void PBRTParser::parsePixelFilter(std::istringstream& stream) {
    // std::cout << "PixelFilter configuration: " << line << std::endl;
}

void PBRTParser::parseSampler(std::istringstream& stream) 
{
    // std::cout << "Sampler configuration: " << line << std::endl;
}

void PBRTParser::parseFilm(std::istringstream& stream) 
{
    // std::cout << "Film configuration: " << line << std::endl;
}


Camera PBRTParser::parseLookAt(std::istringstream& stream) 
{
    float ex, ey, ez, lx, ly, lz, ux, uy, uz;
    stream >> ex >> ey >> ez >> lx >> ly >> lz >> ux >> uy >> uz;
    return(Camera(
        Point3D(ex, ey, ez),
        Point3D(lx, ly, lz),
        Vector3D(ux, uy, uz)
    ));
}

// The camera has already been created in the previous LookAt line
// Here we just parse the FOV
std::optional<real> PBRTParser::parseCamera(std::istringstream& stream) 
{
    std::string cameraType;
    real fov;

    stream >> cameraType;
    if (cameraType == "\"perspective\"") {
        std::string paramKey;

        // Parse parameters
        while (stream >> paramKey) {
            if (paramKey == "\"float") {
                std::string paramName;
                stream >> paramName;

                if (paramName == "fov\"") {
                    char bracket;
                    stream >> bracket;  // Should be '['
                    if (bracket != '[') {
                        std::cerr << "Expected '[' for fov value." << std::endl;
                        return std::nullopt;
                    }
                    stream >> fov;
                    stream.ignore(std::numeric_limits<std::streamsize>::max(), ']');  // Ignore until ']'
                }
            }
        }

        return fov;
    }

    return 0;
}

std::shared_ptr<PointLight> PBRTParser::parsePointLight(std::istringstream& stream) 
{
    std::string keyword;
    stream >> keyword; // Expecting "[intensity]"
    float r, g, b, x, y, z;
    stream >> x >> y >> z >> r >> g >> b;

    std::shared_ptr<PointLight> light;
    // light.position = Point3D(x, y, z);
    // light.intensity = Vector3D(r, g, b);

    return light;
}

std::shared_ptr<RectLight> PBRTParser::parseAreaLight(
  std::ifstream& file,
  std::istringstream& stream) 
{
    Material material;
    std::shared_ptr<Primitive> shape;
    Color lightColor;
    std::string line;
    std::string keyword;

    // The current position of the string is right after the AreaLightSource token
    std::string lightType;
    std::string spectrumKey;
    stream >> lightType >> spectrumKey;

    if (spectrumKey == "\"spectrum\"") 
    {
        char bracket;
        stream >> bracket;

        std::vector<float> spectrumData;
        float value;
        while (stream >> value) spectrumData.push_back(value);

        if (!spectrumData.empty() && spectrumData.back() == ']') {
            spectrumData.pop_back();
        }

        std::map<float, float> spectrum;
        for (size_t i = 0; i < spectrumData.size(); i += 2) {
            float wavelength = spectrumData[i];
            float intensity = spectrumData[i + 1];
            spectrum[wavelength] = intensity;
        }

        lightColor = Color(spectrum);
    }

    // Then we keep reading lines to read the material and
    // the shape for the light source until we find the AttributeEnd token
    while (std::getline(file, line)) 
    {
        std::istringstream newLineStream(line);
        newLineStream >> keyword;

        if (keyword == "Material") 
        {
            material = parseMaterial(newLineStream);
        } 
        else if (keyword == "Shape") 
        {
            shape = parseShape(newLineStream, material);
        } 
        else if (keyword == "AttributeEnd") 
        {
            break;
        }
    }

    return std::make_shared<RectLight>(shape, lightColor, 1.0);
}

std::shared_ptr<Primitive> PBRTParser::parseShape(
    std::istringstream& stream,
    Material currentMaterial
) 
{
    std::string shapeType;
    stream >> shapeType;

    if (shapeType != "\"trianglemesh\"") {
        return nullptr;  // We only handle triangle meshes here
    }

    // Variables to store parsed data
    std::vector<Point3D> vertices;
    std::vector<int> vertexIndexes;

    std::string token;
    while (stream >> token) {
        if (token == "\"point\"") {
            // Parse vertices
            std::string attributeName;
            stream >> attributeName;  // Expect "P"
            if (attributeName != "P") continue;

            char bracket;
            stream >> bracket;  // Consume opening bracket '['

            float x, y, z;
            while (stream >> x >> y >> z) {
                vertices.emplace_back(x, y, z);

                // Break if we encounter the closing bracket
                if (stream.peek() == ']') {
                    stream.ignore();  // Ignore the closing bracket ']'
                    break;
                }
            }
        } else if (token == "\"integer\"") {
            std::string attributeName;
            stream >> attributeName;  // Expect "indices"
            if (attributeName != "indices") continue;

            char bracket;
            stream >> bracket;  // Consume opening bracket '['

            int index;
            while (stream >> index) {
                vertexIndexes.push_back(index);

                // Break if we encounter the closing bracket
                if (stream.peek() == ']') {
                    stream.ignore();  // Ignore the closing bracket ']'
                    break;
                }
            }
        }
    }

    // Create the TriangleMesh with the current material
    std::shared_ptr<TriangleMesh> mesh;
    mesh = std::make_shared<TriangleMesh>(vertices, vertexIndexes, currentMaterial);

    return(mesh);
}

Material PBRTParser::parseMaterial(std::istringstream& stream) {
    std::string materialType;
    stream >> materialType;

    Material material(Color(0, 0, 0), 0.1f, 0.6f, 0.0f, 0.0f);  // Default material values

    // Parse specific material properties
    if (materialType == "\"matte\"") {
        material = parseMatteMaterial(stream);
    } else if (materialType == "\"plastic\"") {
        material = parsePlasticMaterial(stream);
    } else {
        std::cerr << "Unsupported material type: " << materialType << std::endl;
    }

    return material;
}

Material PBRTParser::parseMatteMaterial(std::istringstream& stream) {
    Color kdColor(0.5f, 0.5f, 0.5f);  // Default diffuse color
    real ka = 0.1f, kd = 0.6f;

    std::string token;
    while (stream >> token) {
        if (token == "\"rgb\"") {
            std::string paramName;
            stream >> paramName;
            if (paramName == "Kd") {
                kdColor = parseRGBColor(stream);
            }
        } else if (token == "\"spectrum\"") {
            std::string paramName;
            stream >> paramName;
            if (paramName == "Kd") {
                kdColor = parseSpectrumColor(stream);
            }
        }
    }

    return Material(kdColor, ka, kd, 0.0f, 0.0f);  // Matte materials are not specular/emissive
}

Material PBRTParser::parsePlasticMaterial(std::istringstream& stream) {
    Color kd(0.5f, 0.5f, 0.5f);
    Color ks(0.2f, 0.2f, 0.2f);
    real roughness = 0.1f;

    std::string token;
    while (stream >> token) {
        if (token == "\"rgb\"") {
            std::string paramName;
            stream >> paramName;
            if (paramName == "Kd") {
                kd = parseRGBColor(stream);
            } else if (paramName == "Ks") {
                ks = parseRGBColor(stream);
            }
        } else if (token == "\"float\"") {
            std::string paramName;
            stream >> paramName;
            if (paramName == "roughness") {
                stream.ignore();  // Skip '['
                stream >> roughness;
                stream.ignore();  // Skip ']'
            }
        }
    }

    return Material(kd, 0.1f, 0.6f, 0.8f, 0.0f);
}

Color PBRTParser::parseRGBColor(std::istringstream& stream) {
    char bracket;
    stream >> bracket;  // Consume opening bracket '['

    float r, g, b;
    stream >> r >> g >> b;
    stream.ignore();  // Skip closing bracket ']'

    return Color(r, g, b);
}

Color PBRTParser::parseSpectrumColor(std::istringstream& stream) {
    std::map<float, float> spectrumData;
    char bracket;
    stream >> bracket;  // Consume the opening bracket '['

    float wavelength, intensity;
    while (stream >> wavelength >> intensity) {
        spectrumData[wavelength] = intensity;

        // Break if we encounter the closing bracket ']'
        if (stream.peek() == ']') {
            stream.ignore();  // Ignore the closing bracket
            break;
        }
    }

    return Color(spectrumData);
}

Material PBRTParser::parseNamedMaterial(std::istringstream& stream) 
{
    std::string materialName;
    stream >> materialName;

    Material material = parseMatteMaterial(stream);  // Default to matte material parsing
    materialMap[materialName] = material;

    return material;
}
