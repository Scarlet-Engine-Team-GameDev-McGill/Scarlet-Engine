file (GLOB Tracy_SRC "*.cpp")
add_library(TracyProfiler STATIC ${Tracy_SRC})
target_include_directories(TracyProfiler SYSTEM PUBLIC ".")

find_package(Threads REQUIRED)

target_compile_options(TracyProfiler PUBLIC
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Wall
		-Wno-unused-parameter>
	$<$<CXX_COMPILER_ID:MSVC>:>)

target_link_libraries(TracyProfiler PRIVATE Threads::Threads)