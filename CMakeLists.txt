cmake_minimum_required(VERSION 3.26)
project(UltRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(UltRenderer src/Main.cpp
        src/data/Image.cpp
        include/data/Image.h
        src/data/Image.cpp
        include/data/Image.h
        include/data/Image.h
        src/data/Image.cpp
        src/rendering/Rasterize.cpp
        include/rendering/Rasterize.h
        src/data/TriangleMesh.cpp
        include/data/TriangleMesh.h
        src/math/Matrix.cpp
        include/math/Matrix.h
        include/utils/String.h
        src/utils/String.cpp
        src/utils/Hash.cpp
        include/utils/Hash.h
        src/math/Geometry.cpp
        include/math/Geometry.h
        src/rendering/Camera.cpp
        include/rendering/Camera.h
        src/rendering/Scene.cpp
        include/rendering/Scene.h
        src/math/Transform.cpp
        include/math/Transform.h
        src/utils/Proxy.cpp
        include/utils/Proxy.h
        src/rendering/Light.cpp
        include/rendering/Light.h)

include_directories(${PROJECT_SOURCE_DIR}/include)