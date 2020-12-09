cmake_minimum_required(VERSION 2.8.11)
project(untitled_3d)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)

find_package(glm REQUIRED)

find_package(GLEW REQUIRED)

find_package(glfw3 REQUIRED)

find_package(Freetype REQUIRED)

set(LIBS glfw GL freetype GLEW)

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/include
	${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${LIBS})
