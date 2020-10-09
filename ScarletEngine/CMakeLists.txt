file(GLOB_RECURSE ScarletEngine_SRC "src/*.cpp")

find_package(Vulkan REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory("thirdparty/glad")
add_subdirectory("thirdparty/glfw")

set(GLM_TEST_ENABLE OFF CACHE BOOL "Compile GLM tests")
set(GLM_TEST_ENABLE_CXX_17 ON CACHE BOOL "")
add_subdirectory("thirdparty/glm")
add_subdirectory("thirdparty/imgui")
add_subdirectory("thirdparty/tracy")
add_subdirectory("thirdparty/stb_image")
add_subdirectory("thirdparty/IconFontCppHeaders")

add_library(ScarletEngine STATIC ${ScarletEngine_SRC})
target_compile_features(ScarletEngine PUBLIC cxx_std_17)

target_include_directories(ScarletEngine PUBLIC "include/"
										 PRIVATE "src/")

target_link_libraries(ScarletEngine	PUBLIC TracyProfiler ImGUI glm IconFontCppHeaders
									PRIVATE Vulkan::Vulkan OpenGL::GL ${CMAKE_DL_LIBS} Glad STBImage glfw)

if(${CMAKE_VERSION} VERSION_LESS "3.16.0") 
    message(STATUS "Please consider switching to CMake 3.16 or higher for PCH support!")
else()
	message(STATUS "Using Precompiled Headers")
	target_precompile_headers(ScarletEngine PUBLIC "include/Core/CorePCH.h")
endif()

configure_files("${CMAKE_CURRENT_SOURCE_DIR}/content" "${CMAKE_CURRENT_BINARY_DIR}/content")
configure_files("${CMAKE_CURRENT_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders")

set(GRAPHICS_API "OpenGL" CACHE STRING "Which Graphics API to use")
if (GRAPHICS_API STREQUAL "OpenGL")
	add_definitions(-DRAL_USE_OPENGL)
elseif(CMAKE_BUILD_TYPE STREQUAL "Vulkan")
	add_definitions(-DRAL_USE_VULKAN)
endif()
