cmake_minimum_required(VERSION 2.8.12)

project(RoadSignRecognition)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "image_utils.cpp" "math_utils.cpp" "ihlsandnhs.cpp" "post-processing.cpp" "define.cpp")

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
