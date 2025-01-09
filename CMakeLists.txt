cmake_minimum_required(VERSION 3.10...3.25)
project(rayos VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(external/pbrt-parser EXCLUDE_FROM_ALL) 

set(CMAKE_CXX_FLAGS "-std=c++17")
# set(CMAKE_CXX_FLAGS "-std=c++17 -fsanitize=thread")

# For MacOS, OpenMP should be installed with homebrew
# and the OpenMP_ROOT must be set to the location where
# it has been installed
find_package(OpenMP COMPONENTS CXX REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(SOURCES 
    src/bitmap.cpp
    src/rayos.cpp
    src/scene.cpp

    src/geom/primitive.cpp
    src/geom/quad.cpp
    src/geom/sphere.cpp
    src/geom/triangle.cpp
    src/geom/triangleMesh.cpp

    src/tracer/brdf.cpp
    src/tracer/stochasticRayTracer.cpp
    src/tracer/tracer.cpp
    src/tracer/whittedRayTracer.cpp

    src/util/camera.cpp
    src/util/chrono.cpp
    src/util/color.cpp
    src/util/light.cpp
    src/util/material.cpp
    src/util/mt19937ar.cpp
    src/util/point3D.cpp
    src/util/pointLight.cpp
    src/util/rand.cpp
    src/util/ray.cpp
    src/util/rectLight.cpp
    # src/util/rndGenSSE.cpp
    src/util/util.cpp
)

set(HEADERS 
    include/bitmap.h
    include/scene.h

    include/geom/primitive.h
    include/geom/quad.h
    include/geom/sphere.h
    include/geom/triangle.h
    include/geom/triangleMesh.h

    include/tracer/brdf.h
    include/tracer/stochasticRayTracer.h
    include/tracer/tracer.h
    include/tracer/whittedRayTracer.h

    include/util/camera.h
    include/util/chrono.h
    include/util/color.h
    include/util/light.h
    include/util/material.h
    include/util/point3D.h
    include/util/pointLight.h
    include/util/rand.h
    include/util/ray.h
    include/util/rectLight.h
    include/util/util.h
    include/util/vector3D.h
)

add_executable(rayos ${SOURCES} ${HEADERS})

# set(CMAKE_BUILD_TYPE Debug) 
# set(CMAKE_BUILD_TYPE Release) 
# set(CMAKE_BUILD_TYPE RelWithDebInfo) # Release mode with debug symbols

set_target_properties(rayos PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    # CXX_EXTENSIONS OFF
    # COMPILE_WARNING_AS_ERROR ON
)

target_include_directories(rayos PRIVATE 
  include
  include/geom
  include/tracer
  include/util
)
target_include_directories(rayos PRIVATE ${CMAKE_PROJECT_SOURCE_DIR}/external/pbrt-parser)

target_include_directories(rayos PRIVATE /opt/homebrew/opt/libomp/include)

target_link_libraries(rayos PRIVATE pbrtParser)

target_link_libraries(rayos PRIVATE OpenMP::OpenMP_CXX)