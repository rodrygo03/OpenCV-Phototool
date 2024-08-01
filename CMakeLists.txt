cmake_minimum_required(VERSION 3.5.0)
set (CMAKE_CXX_STANDARD 11)
project(phototool)

IF(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
ENDIF()

# included opencv binaries from source build within the directory; opencv is gitignored
set(OpenCV_DIR = ${CMAKE_CURRENT_SOURCE_DIR}/opencv-4.10.0/lib/cmake/opencv4)

find_package(OpenCV 4.10.0 PATHS ${OpenCV_DIR})
message("openCV version : ${OpenCV_VERSION}")

include_directories(${OpenCV_VERSION})
link_directories(${OpenCV_LIB_DIR})


add_executable(phototool main.cpp)
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OpenCV_LIBS})