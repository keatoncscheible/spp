cmake_minimum_required(VERSION 3.22.1)
project(image_processing_learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Set the path to OpenCVConfig.cmake
set(OpenCV_DIR "/usr/local/src/opencv/build")

find_package(OpenCV REQUIRED)
message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your source files here
set(SOURCES
    ${SOURCE_DIR}/main.cc
    ${SOURCE_DIR}/diagnostics.cc
    ${SOURCE_DIR}/video_capture.cc
    ${SOURCE_DIR}/video_display.cc
    ${SOURCE_DIR}/video_process.cc
    ${SOURCE_DIR}/task.cc
)

add_executable(image_processing_learning_app ${SOURCES})
target_compile_definitions(image_processing_learning_app PRIVATE DIAGNOSTICS_ENABLED)
target_include_directories(image_processing_learning_app PRIVATE ${INCLUDE_DIR})
target_link_libraries(image_processing_learning_app PRIVATE ${OpenCV_LIBS})
