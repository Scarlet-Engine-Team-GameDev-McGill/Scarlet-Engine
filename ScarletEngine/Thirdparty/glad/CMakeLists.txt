# Include GLAD headers and sources
add_library(glad STATIC "glad.cpp")
target_include_directories(glad SYSTEM PUBLIC ".")
set_target_properties(glad PROPERTIES FOLDER "Thirdparty")