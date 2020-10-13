file(GLOB_RECURSE ScarletEditor_SRC "src/*.cpp")

add_executable(ScarletEditor ${ScarletEditor_SRC})

get_property(ScarletEngineModules GLOBAL PROPERTY ScarletEngineModulesProperty)
target_link_libraries(ScarletEditor PRIVATE ${ScarletEngineModules})

target_include_directories(ScarletEditor PUBLIC "include")
target_compile_features(ScarletEditor PUBLIC cxx_std_17)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/imgui.ini")
	message(STATUS "Configuring file imgui.ini")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/default_imgui.ini" "${CMAKE_CURRENT_BINARY_DIR}/imgui.ini")
endif()
