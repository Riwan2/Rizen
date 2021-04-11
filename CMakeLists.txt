cmake_minimum_required(VERSION 3.0)

# set project
project(app)

# basic info
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND})
message(STATUS "CMAKE_VERSION: " ${CMAKE_VERSION})
message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
message(STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE)

# set cmake vars
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")

# source files
set(SOURCE_FILES 
    main.cpp
)

message("")
message(STATUS "SOURCE entry point : " ${SOURCE_FILES})

# rizen files
set(RIZEN_SOURCE_FILES
    ../rizen/utils/rizen_time.cpp
    ../rizen/utils/rizen_math.cpp
    ../rizen/app/app.cpp
    ../rizen/app/input.cpp
    ../rizen/opengl/opengl_2d.cpp
    ../rizen/opengl/shader.cpp
    ../rizen/opengl/texture.cpp
    ../rizen/opengl/camera.cpp
    ../rizen/opengl/model.cpp
    ../rizen/opengl/renderer.cpp
    ../rizen/entity/entity.cpp
)

add_library(rizen STATIC ${RIZEN_SOURCE_FILES})

message(STATUS "LIBRARY RIZEN ADDED: " ${RIZEN_SOURCE_FILES})

# executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# # find sdl
find_package(SDL2 REQUIRED)
message("")
message(STATUS "FINDING SDL2")
if (${SDL2_FOUND})
    message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    message(STATUS "SDL2_INCLUDE_DIRS: " ${SDL2_INCLUDE_DIRS})
    message(STATUS "SDL2_LIBRARIES: " ${SDL2_LIBRARIES})
else()
    message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    message(FATAL_ERROR "SDL2 NOT FOUND")
endif()

# # find opengl
find_package(OpenGL REQUIRED)
message("")
message(STATUS "FINDING OPENGL")
if (${OPENGL_FOUND})
    message(STATUS "OPENGL_FOUND: " ${OPENGL_FOUND})
    message(STATUS "OPENGL_INCLUDE_DIR: " ${OPENGL_INCLUDE_DIR})
    message(STATUS "OPENGL_LIBRARY: " ${OPENGL_LIBRARY})
else()
    message(STATUS "OPENGL_FOUND: " ${OPENGL_FOUND})
    message(FATAL_ERROR "OPENGL NOT FOUND")
endif()

# find glew
find_package(GLEW 2.0 REQUIRED)
message("")
message(STATUS "FINDING GLEW")
if (${GLEW_FOUND})
    message(STATUS "GLEW_FOUND: " ${GLEW_FOUND})
    message(STATUS "GLEW_INCLUDE_DIRS: " ${GLEW_INCLUDE_DIRS})
    message(STATUS "GLEW_LIBRARIES: " ${GLEW_LIBRARIES})
else()
    message(STATUS "GLEW_FOUND: " ${GLEW_FOUND})
    message(FATAL_ERROR "GLEW NOT FOUND")
endif()

# find glm
find_package(GLM REQUIRED)
message("")
message(STATUS "FINDING GLM")
if (${GLM_FOUND})
    message(STATUS "GLM_FOUND: " ${GLM_FOUND})
    message(STATUS "GLM_INCLUDE_DIRS: " ${GLM_INCLUDE_DIRS})
else()
    message(STATUS "GLM_FOUND: " ${GLM_FOUND})
    message(FATAL_ERROR "GLM NOT FOUND")
endif()

# include and link
message("")
include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} rizen)