cmake_minimum_required(VERSION 3.5.0)
project(phototool VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV 4.0.0 REQUIRED)
message("openCV version : ${OpenCV_VERSION}")

include_directories(${OpenCV_VERSION})
link_directories(${OpenCV_LIB_DIR})


add_executable(phototool main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
