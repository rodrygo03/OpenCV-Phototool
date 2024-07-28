cmake_minimum_required(VERSION 3.5.0)
project(phototool)

find_package(OpenCV 4.10.0)
message("openCV version : ${OpenCV_VERSION}")

include_directories(${OpenCV_VERSION})
link_directories(${OpenCV_LIB_DIR})


add_executable(phototool main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

