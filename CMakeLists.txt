cmake_minimum_required(VERSION 3.5)

project(gstreamer-dynamic-media)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

option(ENABLE_CJSON_TEST "" OFF)
option(BUILD_SHARED_AND_STATIC_LIBS "" ON)
add_subdirectory(cJSON)

option(EVENT__DISABLE_BENCHMARK"" OFF)
option(EVENT__DISABLE_TESTS "" OFF)
option(EVENT__DISABLE_REGRESS "" OFF)
option(EVENT__DISABLE_SAMPLES "" OFF)
add_subdirectory(libevent)

add_subdirectory(sigfn)

if(WIN32)
find_package(SDL REQUIRED)
set(SDL_LIBS SDL::SDL)
else()
find_package(SDL2 REQUIRED)
set(SDL_LIBS SDL2::SDL)
endif()

find_package(GStreamer)

add_executable(client client.c)
target_link_libraries(client ${SDL2_LIBRARIES} GStreamer::GLib GStreamer::GStreamer GStreamer::GStreamerApp cjson-static)