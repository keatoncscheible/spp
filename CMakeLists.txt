cmake_minimum_required(VERSION 3.22.1)
project(image_processing_learning)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Set the path to OpenCVConfig.cmake
set(OpenCV_DIR "/usr/local/src/opencv/build")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your source files here
set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/diagnostics.cpp
    ${SOURCE_DIR}/video_capture.cpp
    ${SOURCE_DIR}/video_proc.cpp
    ${SOURCE_DIR}/task.cpp
)

add_executable(image_processing_learning_app ${SOURCES})
target_compile_definitions(image_processing_learning_app PRIVATE DIAGNOSTICS_ENABLED)
target_include_directories(image_processing_learning_app PRIVATE ${INCLUDE_DIR})
target_link_libraries(image_processing_learning_app PRIVATE ${OpenCV_LIBS})
