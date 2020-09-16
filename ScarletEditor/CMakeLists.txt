file(GLOB_RECURSE ScarletEditor_SRC "src/*.cpp")

add_executable(ScarletEditor ${ScarletEditor_SRC})

target_include_directories(ScarletEditor PUBLIC "include")
target_link_libraries(ScarletEditor PUBLIC ScarletEngine)
target_compile_features(ScarletEditor PUBLIC cxx_std_17)
