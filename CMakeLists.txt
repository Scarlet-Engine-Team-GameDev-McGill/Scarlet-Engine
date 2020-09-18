# --- Set up CMake version. Require minimum version 3.1 but prefer 3.16 for precompiled headers ---
cmake_minimum_required(VERSION 3.8...3.16)
message(STATUS "CMake Version: ${CMAKE_VERSION}")
project("ScarletEngine" VERSION 1.0 LANGUAGES CXX)

# --- Set compiler warnings and disable RTTI/Exceptions
# https://github.com/izenecloud/cmake/blob/master/SetCompilerWarningAll.cmake
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	message("Detected MSVC Compiler")
	# Use the highest warning level for Visual Studio.
	set(CMAKE_CXX_WARNING_LEVEL 4)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	# Disable C++ exceptions.
	string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	# Disable RTTI in the future.
	string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message("Detected Clang")
	# Use -Wall for clang and gcc.
	if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
	# Use -Wextra for clang and gcc.
	if(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
	# Use -Werror for clang and gcc.
	if(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
	# Disable C++ exceptions.
	string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
	# Disable RTTI in the future.
	string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
	if(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message("Detected GNU-GCC")
	# Use -Wall for clang and gcc.
	if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
	# Use -Wextra for clang and gcc.
	if(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
	# Use -Werror for clang and gcc.
	if(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	endif(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
	# Disable C++ exceptions.
	string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
	# Disable RTTI in the future.
	string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# Add a helper macro to move files to the build directory
macro(configure_files srcDir destDir)
	message(STATUS "Configuring directory ${destDir}")
	make_directory(${destDir})

	file(GLOB templateFiles RELATIVE "${srcDir}" "${srcDir}/*")
	foreach(templateFile ${templateFiles})
		set(srcTemplatePath ${srcDir}/${templateFile})
		if(NOT IS_DIRECTORY ${srcTemplatePath})
			message(STATUS "Configuring file ${templateFile}")
			configure_file(
					${srcTemplatePath}
					${destDir}/${templateFile}
					COPYONLY)
		endif(NOT IS_DIRECTORY ${srcTemplatePath})
	endforeach(templateFile)
endmacro(configure_files)

# Append the "CMake" directory to the module path so that Find*.cmake files can be used
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# Define a global DEBUG to determine if in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG)
endif()

# Compile the ScarletEngine library
add_subdirectory("ScarletEngine")

# Editor
option(COMPILE_EDITOR "Should compile Editor" ON)
if (COMPILE_EDITOR)
	add_subdirectory("ScarletEditor")
endif()

# Testing
option(TESTING "Compile test app" ON)
if (TESTING)
	add_subdirectory("TestGame")
endif()
