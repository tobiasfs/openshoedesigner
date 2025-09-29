
file(GLOB_RECURSE files_project "*.cpp")
add_library(project STATIC ${files_project})

target_compile_features(project PRIVATE cxx_std_17)

find_package (Eigen3 REQUIRED NO_MODULE)
target_compile_definitions(project PRIVATE USE_EIGEN)

find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

target_link_libraries(project
	${wxWidgets_LIBRARIES}
	Eigen3::Eigen
	library_3d
	library_system
)
