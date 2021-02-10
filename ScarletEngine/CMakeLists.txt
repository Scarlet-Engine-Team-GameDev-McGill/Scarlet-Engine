find_package(Vulkan REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory("Thirdparty/glad")
add_subdirectory("Thirdparty/glfw")
add_subdirectory("Thirdparty/glm")
add_subdirectory("Thirdparty/imgui")
add_subdirectory("Thirdparty/imguizmo")
add_subdirectory("Thirdparty/tracy")
add_subdirectory("Thirdparty/stb_image")
add_subdirectory("Thirdparty/IconFontCppHeaders")

configure_files("${CMAKE_CURRENT_SOURCE_DIR}/Content" "${CMAKE_CURRENT_BINARY_DIR}/Content")
configure_files("${CMAKE_CURRENT_SOURCE_DIR}/Shaders" "${CMAKE_CURRENT_BINARY_DIR}/Shaders")

set(GRAPHICS_API "OpenGL" CACHE STRING "Which Graphics API to use")
if (GRAPHICS_API STREQUAL "OpenGL")
	add_definitions(-DRAL_USE_OPENGL)
elseif(CMAKE_BUILD_TYPE STREQUAL "Vulkan")
	add_definitions(-DRAL_USE_VULKAN)
endif()

define_property(GLOBAL PROPERTY ScarletEngineModulesProperty
    BRIEF_DOCS "Global list of ScarletEngine modules"
    FULL_DOCS "Global list of ScarletEngine modules")
set_property(GLOBAL PROPERTY ScarletEngineModulesProperty "")
# Macro for add values into the list
macro(register_module moduleName)
    set_property(GLOBAL APPEND PROPERTY ScarletEngineModulesProperty "${moduleName}")
endmacro(register_module)


include(CMakeParseArguments)
macro(declare_module)
	set(oneValueArgs MODULE_NAME)
	set(multiValueArgs PRIVATE_DEPS PUBLIC_DEPS)
	cmake_parse_arguments(
		DECLARE_MODULE 
		"${options}"
		"${oneValueArgs}"
		"${multiValueArgs}"
		${ARGN})

		
	if(NOT DECLARE_MODULE_MODULE_NAME)
		message(FATAL_ERROR "Missing module name!")
	endif()

	file(GLOB_RECURSE MODULE_SRC "*.cpp")
	file(GLOB_RECURSE MODULE_HDRS "*.h")
	add_library(${DECLARE_MODULE_MODULE_NAME} STATIC ${MODULE_SRC} ${MODULE_HDRS})
	target_include_directories(${DECLARE_MODULE_MODULE_NAME} PUBLIC "Public" PRIVATE "Private")
	target_link_libraries(${DECLARE_MODULE_MODULE_NAME} PUBLIC ${DECLARE_MODULE_PUBLIC_DEPS} PRIVATE ${DECLARE_MODULE_PRIVATE_DEPS})

	target_compile_features(${DECLARE_MODULE_MODULE_NAME} PUBLIC cxx_std_20)
	
	set_target_properties(${DECLARE_MODULE_MODULE_NAME} PROPERTIES FOLDER "ScarletEngine")

	# for now make a list of all modules which external things should link to if they want everything
	register_module(${DECLARE_MODULE_MODULE_NAME})

	if(PRINT_MODULE_DEPS)
		foreach(PrivateDep ${DECLARE_MODULE_PRIVATE_DEPS})
		message(STATUS "\"${DECLARE_MODULE_MODULE_NAME}\" -> \"${PrivateDep}\"")
		endforeach()

		foreach(PublicDep ${DECLARE_MODULE_PUBLIC_DEPS})
		message(STATUS "\"${DECLARE_MODULE_MODULE_NAME}\" -> \"${PublicDep}\"")
		endforeach()
	endif(PRINT_MODULE_DEPS)
endmacro(declare_module)

add_subdirectory("Sources/Core")
add_subdirectory("Sources/Engine")
add_subdirectory("Sources/Window")
add_subdirectory("Sources/AssetManager")
add_subdirectory("Sources/UI")
add_subdirectory("Sources/ECS")
add_subdirectory("Sources/OpenGLRAL")
add_subdirectory("Sources/Renderer")
add_subdirectory("Sources/RAL")
add_subdirectory("Sources/Gameplay")
add_subdirectory("Sources/Ac_Physics")