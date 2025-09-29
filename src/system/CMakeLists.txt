
file(GLOB files_library_system "./*.cpp" "./controller/*.cpp")
add_library(library_system STATIC ${files_library_system})

target_compile_features(library_system PRIVATE cxx_std_17)

find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

target_link_libraries(library_system
	${wxWidgets_LIBRARIES}
)
