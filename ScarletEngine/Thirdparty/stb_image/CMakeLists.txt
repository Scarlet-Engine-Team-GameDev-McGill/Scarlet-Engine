add_library(STBImage INTERFACE)
target_include_directories(STBImage SYSTEM INTERFACE ".")
set_target_properties(STBImage PROPERTIES FOLDER "Thirdparty")