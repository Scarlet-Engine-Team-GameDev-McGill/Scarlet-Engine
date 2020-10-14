define_property(GLOBAL PROPERTY ScarletEditorModulesProperty
    BRIEF_DOCS "Global list of ScarletEditor modules"
    FULL_DOCS "Global list of ScarletEditor modules")
set_property(GLOBAL PROPERTY ScarletEditorModulesProperty "")
# Macro for add values into the list
macro(register_editor_module moduleName)
    set_property(GLOBAL APPEND PROPERTY ScarletEditorModulesProperty "${moduleName}")
endmacro(register_editor_module)

macro(declare_editor_module)
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
	add_library(${DECLARE_MODULE_MODULE_NAME} STATIC ${MODULE_SRC})
	target_include_directories(${DECLARE_MODULE_MODULE_NAME} PUBLIC "Public" PRIVATE "Private")
	target_link_libraries(${DECLARE_MODULE_MODULE_NAME} PUBLIC ${DECLARE_MODULE_PUBLIC_DEPS} PRIVATE ${DECLARE_MODULE_PRIVATE_DEPS})

	target_compile_features(${DECLARE_MODULE_MODULE_NAME} PUBLIC cxx_std_20)

	# for now make a list of all modules which external things should link to if they want everything
	register_editor_module(${DECLARE_MODULE_MODULE_NAME})
	
	if(PRINT_MODULE_DEPS)
		foreach(PrivateDep ${DECLARE_MODULE_PRIVATE_DEPS})
		message(STATUS "\"${DECLARE_MODULE_MODULE_NAME}\" -> \"${PrivateDep}\"")
		endforeach()

		foreach(PublicDep ${DECLARE_MODULE_PUBLIC_DEPS})
		message(STATUS "\"${DECLARE_MODULE_MODULE_NAME}\" -> \"${PublicDep}\"")
		endforeach()
	endif(PRINT_MODULE_DEPS)
endmacro(declare_editor_module)

add_subdirectory("Sources/Editor")
add_subdirectory("Sources/SceneGraph")
add_subdirectory("Sources/OutputLog")
add_subdirectory("Sources/PropertyEditor")

file(GLOB ScarletEditor_SRC "Sources/EditorMain.cpp")

add_executable(ScarletEditor ${ScarletEditor_SRC})

get_property(ScarletEditorModules GLOBAL PROPERTY ScarletEditorModulesProperty)
target_link_libraries(ScarletEditor PRIVATE "Core" ${ScarletEditorModules})

target_include_directories(ScarletEditor PUBLIC "include")
target_compile_features(ScarletEditor PUBLIC cxx_std_20)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/imgui.ini")
	message(STATUS "Configuring file imgui.ini")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config/default_imgui.ini" "${CMAKE_CURRENT_BINARY_DIR}/imgui.ini")
endif()