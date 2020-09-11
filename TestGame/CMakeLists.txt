file(GLOB_RECURSE TestGame_SRC "*.cpp")

add_executable(TestGame ${TestGame_SRC})

target_link_libraries(TestGame PUBLIC ScarletEngine)
target_compile_features(TestGame PUBLIC cxx_std_17)