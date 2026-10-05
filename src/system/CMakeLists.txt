
file(GLOB files_library_system "./*.cpp" "./controller/*.cpp")
add_library(library_system STATIC ${files_library_system})

target_compile_features(library_system PRIVATE cxx_std_17)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(UsewxWidgets)

target_compile_definitions(library_system PRIVATE USE_LIBZIP)


target_link_libraries(library_system
  PRIVATE
	${wxWidgets_LIBRARIES}
	zip
)
