#include "pbrtParser.h"

std::shared_ptr<Scene> PBRTParser::parse() 
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    // Remove lights and objects created by the default constructor
    // Another option would be to create a constructor that takes
    // a camera and a list of objects and lights
    scene->lights.clear();
    scene->objects.clear();

    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open PBRT file: " << filename << std::endl;
        return nullptr;
    }

    std::string line;
    bool inWorldBlock = false;

    Material currentMaterial;

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
                scene->setCamera(camera);
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
            } 
            else if (keyword == "AttributeEnd") 
            {
            } 
            else if (keyword == "Material") 
            {
                currentMaterial = parseMaterial(file, iss);
            } 
            else if (keyword == "Shape") 
            {
                scene->addObject(parseShape(file, iss, currentMaterial));
            } 
            else if (keyword == "AreaLightSource") 
            {
                scene->addLight(parseAreaLight(file, iss));
            } 
            else if (keyword == "PointLightSource") 
            {
                scene->addLight(parsePointLight(iss));
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

    if (spectrumKey == "\"spectrum") 
    {
        std::string spectrumType;
        stream >> spectrumType;
        
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
        if (line != "") 
        {
            std::istringstream newLineStream(line);
            newLineStream >> keyword;
            if (keyword == "Material") 
            {
                material = parseMaterial(file, newLineStream);
            } 
            else if (keyword == "Shape") 
            {
                shape = parseShape(file, newLineStream, material);
            } 
            else if (keyword == "AttributeEnd") 
            {
                break;
            }
        }
    }

    return std::make_shared<RectLight>(shape, lightColor, 1.0);
}

std::shared_ptr<Primitive> PBRTParser::parseShape(
    std::ifstream& file,
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

    std::string line;
    bool finishedReadingVertices = false;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string token;
        stream >> token;

        if (token == "\"point") {
            // Parse vertices
            std::string attributeName;
            stream >> attributeName;  // Expect "P"
            if (attributeName != "P\"") continue;

            char bracket;
            stream >> bracket;  // Consume opening bracket '['
            if (bracket != '[') continue;

            float x, y, z;
            while (std::getline(file, line)) {
                std::istringstream vertexStream(line);
                if (vertexStream >> x >> y >> z) {
                    vertices.emplace_back(x, y, z);
                }

                // Keep reading characters in the current line
                // to see if we encounter the closing bracket
                vertexStream >> bracket;
                if (bracket == ']') 
                {
                    finishedReadingVertices = true;
                    break;
                }
            }
        } else if (token == "\"integer") {
            std::string attributeName;
            stream >> attributeName;  // Expect "indices"
            if (attributeName != "indices\"") continue;

            char bracket;
            stream >> bracket;  // Consume opening bracket '['
            if (bracket != '[') continue;

            int index;
            while (true) {
                while (stream >> index) {
                    vertexIndexes.push_back(index);
                }

                // Break if we encounter the closing bracket
                if (line.find(']') != std::string::npos) {
                    break;
                }

                // Read the next line if not finished
                if (!std::getline(file, line)) {
                    break;
                }
                stream.clear();
                stream.str(line);
            }
        }
        if (finishedReadingVertices) 
        {
            break;
        }
    }

    // Create the TriangleMesh with the current material
    std::shared_ptr<TriangleMesh> mesh;
    mesh = std::make_shared<TriangleMesh>(vertices, vertexIndexes, currentMaterial);

    return mesh;
}

Material PBRTParser::parseMaterial(
    std::ifstream& file, 
    std::istringstream& stream) 
{
    std::string materialType;
    stream >> materialType;

    Material material(Color(0, 0, 0), 0.1f, 0.6f, 0.0f, 0.0f);  // Default material values

    // Parse specific material properties
    if (materialType == "\"matte\"") {
        material = parseMatteMaterial(file);
    } else if (materialType == "\"plastic\"") {
        material = parsePlasticMaterial(stream);
    } else {
        std::cerr << "Unsupported material type: " << materialType << std::endl;
    }

    return material;
}

Material PBRTParser::parseMatteMaterial(std::ifstream& file)
{
    std::map<float, float> spectrumData;
    std::string line;

    bool readingSpectrum = false;
    float pendingWavelength = -1.0f;

    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string token;

        while (lineStream >> token)
        {
            // Look for spectrum Kd definition
            if (token == "\"spectrum")
            {
                lineStream >> token;
                if (token == "Kd\"")
                {
                    lineStream >> token; // Expect '[' after Kd
                    if (!token.empty() && token[0] == '[')  // Handle both "[" and "[400"
                    {
                        readingSpectrum = true;

                        // Handle case where there's a number after the bracket
                        std::istringstream dataStream(token.substr(1));
                        if (dataStream >> pendingWavelength) {
                            continue;  // Wavelength read; wait for intensity
                        }
                    }
                }
            }

            if (readingSpectrum)
            {
                float value;
                if (pendingWavelength) 
                {
                    std::istringstream iss(token);
                    iss >> value;
                    spectrumData[pendingWavelength] = value;
                    pendingWavelength = -1.0f;
                }
                while (lineStream >> value)
                {
                    if (pendingWavelength < 0)  // Expecting wavelength
                    {
                        pendingWavelength = value;
                    }
                    else  // Expecting intensity
                    {
                        spectrumData[pendingWavelength] = value;
                        pendingWavelength = -1.0f;  // Reset for next pair
                    }

                    // Break if we find the closing bracket
                    if (lineStream.peek() == ']')
                    {
                        lineStream.get();  // Consume the ']'
                        readingSpectrum = false;
                        break;
                    }
                }
            }
        }

        if (!readingSpectrum)
        {
            break;  // Exit the loop once spectrum parsing is complete
        }
    }

    // Create the spectrum-based Color
    Color kdColor(spectrumData);

    // Default material parameters for a matte material
    real ka = 0.0f;  // No ambient by default
    real kd = 1.0f;  // Full diffuse reflection
    real ks = 0.0f;  // No specular for matte
    real ke = 0.0f;  // No emissive properties

    return Material(kdColor, ka, kd, ks, ke);
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