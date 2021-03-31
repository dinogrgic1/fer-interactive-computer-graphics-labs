cmake_minimum_required(VERSION 3.0)
project(irgLab)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(dependencies/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(dependencies/assimp)


add_subdirectory(primjerGLM)
add_subdirectory(primjerASSIMP)
add_subdirectory(primjerOpenGL)


add_subdirectory(vjezba1)
add_subdirectory(vjezba2)
add_subdirectory(vjezba3)
add_subdirectory(vjezba4)
add_subdirectory(vjezba5a)
add_subdirectory(vjezba5b)
add_subdirectory(vjezba6)
add_subdirectory(vjezba7)
add_subdirectory(vjezba8a)
add_subdirectory(vjezba8b)
add_subdirectory(vjezba8c)
add_subdirectory(vjezba9)
add_subdirectory(vjezba10)
