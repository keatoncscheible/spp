cmake_minimum_required(VERSION 3.22.1)
project(image_processing_learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

# Set the path to OpenCVConfig.cmake
set(OpenCV_DIR "/usr/local/src/opencv/build")
find_package(OpenCV REQUIRED)
message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS})

# Find the spdlog package (you can customize this if needed)
set(spdlog_DIR "/usr/local/src/spdlog/build")  # Replace with the actual path to your spdlog cmake directory
find_package(spdlog REQUIRED)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(TASK_INCLUDE_DIR "${INCLUDE_DIR}/task")
set(UTIL_INCLUDE_DIR "${INCLUDE_DIR}/util")
set(VIDEO_INCLUDE_DIR "${INCLUDE_DIR}/video")
set(INCLUDE_DIRS 
    ${INCLUDE_DIR}
    ${TASK_INCLUDE_DIR}
    ${UTIL_INCLUDE_DIR}
    ${VIDEO_INCLUDE_DIR}
)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(TASK_SOURCE_DIR "${SOURCE_DIR}/task")
set(UTIL_SOURCE_DIR "${SOURCE_DIR}/util")
set(VIDEO_SOURCE_DIR "${SOURCE_DIR}/video")

set(TASK_SOURCES
    ${TASK_SOURCE_DIR}/task.cc
)

set(UTIL_SOURCES
    # ${UTIL_SOURCE_DIR}/diagnostics.cc
    ${UTIL_SOURCE_DIR}/logger.cc
)

set(VIDEO_SOURCES
    ${VIDEO_SOURCE_DIR}/grayscale_transformer.cc
    ${VIDEO_SOURCE_DIR}/video_file.cc
    ${VIDEO_SOURCE_DIR}/video_input.cc
    ${VIDEO_SOURCE_DIR}/video_output.cc
    ${VIDEO_SOURCE_DIR}/video_player.cc
    ${VIDEO_SOURCE_DIR}/video_processor.cc        
    ${VIDEO_SOURCE_DIR}/video_task.cc
    ${VIDEO_SOURCE_DIR}/webcam.cc
)

set(SOURCES
    ${SOURCE_DIR}/main.cc
    ${TASK_SOURCES}
    ${UTIL_SOURCES}
    ${VIDEO_SOURCES} 
)

add_executable(image_processing_learning_app ${SOURCES})
target_compile_definitions(image_processing_learning_app PRIVATE DIAGNOSTICS_ENABLED)
target_include_directories(image_processing_learning_app PRIVATE ${INCLUDE_DIRS})
target_link_libraries(image_processing_learning_app PRIVATE ${OpenCV_LIBS} spdlog::spdlog)
