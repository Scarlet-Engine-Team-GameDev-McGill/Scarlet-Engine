# Include GLAD headers and sources
add_library(Glad STATIC "glad.cpp")
target_include_directories(Glad SYSTEM PUBLIC ".")