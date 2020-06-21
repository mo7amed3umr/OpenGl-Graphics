cmake_minimum_required(VERSION 3.15)
project(Project)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL)
find_package(GLUT)

set(Files main.cpp imageloader.h imageloader.cpp glm.cpp glm.h)
set(CMAKE_CURRENT_SOURCE_DIR "/home/abdallahdarwesh/CLionProjects/CG-Tasks/Graphics")
add_executable(${CMAKE_PROJECT_NAME} ${Files})
target_link_libraries(${CMAKE_PROJECT_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLM_LIBRARY})