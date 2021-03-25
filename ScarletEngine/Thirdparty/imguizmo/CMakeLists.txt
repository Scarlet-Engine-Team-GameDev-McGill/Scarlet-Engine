file (GLOB ImGuizmo_SRC "*.cpp")
add_library(ImGuizmo STATIC ${ImGuizmo_SRC})

target_include_directories(ImGuizmo PUBLIC ".")
target_link_libraries(ImGuizmo PUBLIC ImGUI)
