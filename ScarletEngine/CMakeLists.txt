file(GLOB_RECURSE ScarletEngine_SRC "src/*.cpp")

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

# Include the imgui sources in the main engine binary
file (GLOB ImGUI_SRC
	"thirdparty/imgui/*.cpp"
	"thirdparty/imgui/examples/imgui_impl_glfw.cpp"
	"thirdparty/imgui/examples/imgui_impl_vulkan.cpp")

set (IMGUI_INCLUDE "thirdparty/imgui")

# Add GLM to public headers
set (GLM_INCLUDE "thirdparty/glm")

add_library(ScarletEngine STATIC ${ScarletEngine_SRC} ${ImGUI_SRC})

target_include_directories(ScarletEngine PUBLIC "include/" ${IMGUI_INCLUDE} ${GLFW3_INCLUDE} ${GLM_INCLUDE})
target_include_directories(ScarletEngine PRIVATE "src/")

target_link_libraries(ScarletEngine PRIVATE ${GLFW3_LIBRARY} Vulkan::Vulkan)

target_compile_features(ScarletEngine PUBLIC cxx_std_17)