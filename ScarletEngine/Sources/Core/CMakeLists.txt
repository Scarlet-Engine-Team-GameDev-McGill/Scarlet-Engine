set(Core_PrivateDeps)

set(Core_PublicDeps
	"${CMAKE_DL_LIBS}"
	"TracyProfiler"
	"glm")

DECLARE_MODULE(
	MODULE_NAME "Core"
	PRIVATE_DEPS ${Core_PrivateDeps}
	PUBLIC_DEPS ${Core_PublicDeps})

if(${CMAKE_VERSION} VERSION_LESS "3.16.0") 
    message(STATUS "Please consider switching to CMake 3.16 or higher for PCH support!")
else()
	message(STATUS "Using Precompiled Headers")
	target_precompile_headers(Core PUBLIC "Public/CorePCH.h")
endif()