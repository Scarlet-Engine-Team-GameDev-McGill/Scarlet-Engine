add_library(IconFontCppHeaders INTERFACE)
target_include_directories(IconFontCppHeaders SYSTEM INTERFACE ".")
set_target_properties(IconFontCppHeaders PROPERTIES FOLDER "Thirdparty")