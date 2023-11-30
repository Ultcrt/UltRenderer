cmake_minimum_required(VERSION 3.26)
project(UltRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(UltRenderer src/main.cpp
        src/data/Image.cpp
        include/data/Image.h
        src/data/Image.cpp
        include/data/Image.h
        include/data/Image.h
        src/data/Image.cpp
        src/rasterization/Rasterization.cpp
        include/rendering/Rasterization.h
        src/data/Matrix.cpp
        include/data/Matrix.h)

include_directories(${PROJECT_SOURCE_DIR}/include)