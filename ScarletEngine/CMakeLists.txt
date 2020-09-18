file(GLOB_RECURSE ScarletEngine_SRC "src/*.cpp")

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

# Include the imgui sources in the main engine binary
file (GLOB ImGUI_SRC
	"thirdparty/imgui/*.cpp"
	"thirdparty/imgui/examples/imgui_impl_glfw.cpp"
	"thirdparty/imgui/examples/imgui_impl_vulkan.cpp"
	"thirdparty/imgui/examples/imgui_impl_opengl3.cpp"
	"thirdparty/imgui/misc/cpp/imgui_stdlib.cpp")

set (IMGUI_INCLUDE "thirdparty/imgui")

# Include GLAD headers and sources
file (GLOB Glad_SRC
	"thirdparty/glad/*.cpp")

set (GLAD_INCLUDE "thirdparty/glad")

# Add GLM to public headers
set (GLM_INCLUDE "thirdparty/glm")

# Add STB_Image to private headers
set(STBIMAGE_INCLUDE "thirdparty/stb_image")

set(GRAPHICS_API "OpenGL" CACHE STRING "Which Graphics API to use")
if (GRAPHICS_API STREQUAL "OpenGL")
	add_definitions(-DRAL_USE_OPENGL)
elseif(CMAKE_BUILD_TYPE STREQUAL "Vulkan")
	add_definitions(-DRAL_USE_VULKAN)
endif()

add_library(ScarletEngine STATIC ${ScarletEngine_SRC} ${ImGUI_SRC} ${Glad_SRC})

target_include_directories(ScarletEngine PUBLIC "include/" ${IMGUI_INCLUDE} ${GLFW3_INCLUDE} ${GLM_INCLUDE})
target_include_directories(ScarletEngine PRIVATE "src/" ${STBIMAGE_INCLUDE} ${GLAD_INCLUDE})

target_link_libraries(ScarletEngine PRIVATE ${GLFW3_LIBRARY} Vulkan::Vulkan OpenGL::GL ${CMAKE_DL_LIBS})

target_compile_features(ScarletEngine PUBLIC cxx_std_17)

configure_files("${CMAKE_CURRENT_SOURCE_DIR}/content" "${CMAKE_CURRENT_BINARY_DIR}/content")
