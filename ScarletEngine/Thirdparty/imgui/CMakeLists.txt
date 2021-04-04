file (GLOB ImGUI_SRC "*.cpp")
add_library(ImGUI STATIC ${ImGUI_SRC})

target_include_directories(ImGUI PUBLIC ".")
target_link_libraries(ImGUI PUBLIC glm PRIVATE Vulkan::Vulkan OpenGL::GL glfw glad)
set_target_properties(ImGUI PROPERTIES FOLDER "Thirdparty")