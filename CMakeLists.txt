cmake_minimum_required(VERSION 3.5.0)
set (CMAKE_CXX_STANDARD 11)
project(phototool)

IF(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
ENDIF()

set(OpenCV_DIR = /Users/rodrigoorozco/Source-libs/OpenCV/lib/cmake/opencv4)
    # build from source // path to OpenCVConfig.cmake 
    # address brew install qt compilation dependecy
    # build from source instead
find_package(OpenCV 4.10.0 PATHS ${OpenCV_DIR})
message("openCV version : ${OpenCV_VERSION}")

include_directories(${OpenCV_VERSION})
link_directories(${OpenCV_LIB_DIR})


add_executable(phototool main.cpp)
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OpenCV_LIBS})