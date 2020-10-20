file(GLOB_RECURSE TestGame_SRC "*.cpp")

add_executable(TestGame ${TestGame_SRC})
get_property(ScarletEngineModules GLOBAL PROPERTY ScarletEngineModulesProperty)
target_link_libraries(TestGame PUBLIC ${ScarletEngineModules})
target_compile_features(TestGame PUBLIC cxx_std_17)