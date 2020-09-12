# --- Set up CMake version. Require at least version 3.1 and use the latest version policy possible (up to 3.15) ---
cmake_minimum_required(VERSION 3.10)
project("ScarletEngine" VERSION 1.0 LANGUAGES CXX)

if (${CMAKE_VERSION} VERSION_LESS 3.15)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.15)
endif()

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
	# Disable RTTI in the future. Currently needed for CTIDs
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
	# Disable RTTI in the future. Currently needed for CTIDs
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
	# Disable RTTI in the future. Currently needed for CTIDs
	string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# Append the "CMake" directory to the module path so that Find*.cmake files can be used
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# Define a global DEBUG to determine if in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

# Compile the ScarletEngine library
add_subdirectory("ScarletEngine")

# Testing
option(TESTING "Compile and run test app" ON)
if (TESTING)
	add_subdirectory("TestGame")
endif()