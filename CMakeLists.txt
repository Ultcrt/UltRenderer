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
        src/rasterization/Rasterization.cpp
        include/rendering/Rasterization.h
        src/data/TriangleMesh.cpp
        include/data/TriangleMesh.h
        src/data/Matrix.cpp
        include/data/Matrix.h
        include/utils/String.h
        src/utils/String.cpp
        src/utils/Hash.cpp
        include/utils/Hash.h)

include_directories(${PROJECT_SOURCE_DIR}/include)