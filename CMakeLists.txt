cmake_minimum_required(VERSION 3.22.1)
project(spp)

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
set(APP_INCLUDE_DIR "${INCLUDE_DIR}/app")
set(TASK_INCLUDE_DIR "${INCLUDE_DIR}/task")
set(UTIL_INCLUDE_DIR "${INCLUDE_DIR}/util")
set(VIDEO_INCLUDE_DIR "${INCLUDE_DIR}/video")
set(INCLUDE_DIRS 
    ${INCLUDE_DIR}
    ${APP_INCLUDE_DIR}
    ${TASK_INCLUDE_DIR}
    ${UTIL_INCLUDE_DIR}
    ${VIDEO_INCLUDE_DIR}
    ${VIDEO_INCLUDE_DIR}/input
    ${VIDEO_INCLUDE_DIR}/processing
    ${VIDEO_INCLUDE_DIR}/output
)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(APP_SOURCE_DIR "${SOURCE_DIR}/app")
set(TASK_SOURCE_DIR "${SOURCE_DIR}/task")
set(UTIL_SOURCE_DIR "${SOURCE_DIR}/util")
set(VIDEO_SOURCE_DIR "${SOURCE_DIR}/video")

set(APP_SOURCES
    ${APP_SOURCE_DIR}/app.cc
)

set(TASK_SOURCES
    ${TASK_SOURCE_DIR}/task.cc
)

set(UTIL_SOURCES
    ${UTIL_SOURCE_DIR}/diagnostics.cc
    ${UTIL_SOURCE_DIR}/logger.cc
)

set(VIDEO_SOURCES
    ${VIDEO_SOURCE_DIR}/video_task.cc
    # Inputs
    ${VIDEO_SOURCE_DIR}/input/video_file.cc
    ${VIDEO_SOURCE_DIR}/input/video_input.cc
    ${VIDEO_SOURCE_DIR}/input/webcam.cc
    # Processing
    ${VIDEO_SOURCE_DIR}/processing/bypass_transformer.cc
    ${VIDEO_SOURCE_DIR}/processing/grayscale_transformer.cc
    ${VIDEO_SOURCE_DIR}/processing/hsv_transformer.cc
    ${VIDEO_SOURCE_DIR}/processing/object_detector.cc  
    ${VIDEO_SOURCE_DIR}/processing/video_processor.cc  
    # Outputs
    ${VIDEO_SOURCE_DIR}/output/video_output.cc
    ${VIDEO_SOURCE_DIR}/output/video_player.cc
)

set(SOURCES
    ${SOURCE_DIR}/main.cc
    ${APP_SOURCES}
    ${TASK_SOURCES}
    ${UTIL_SOURCES}
    ${VIDEO_SOURCES} 
)

add_executable(spp_app ${SOURCES})
target_compile_definitions(spp_app PRIVATE DIAGNOSTICS_ENABLED)
target_include_directories(spp_app PRIVATE ${INCLUDE_DIRS})
target_link_libraries(spp_app PRIVATE ${OpenCV_LIBS} spdlog::spdlog)
